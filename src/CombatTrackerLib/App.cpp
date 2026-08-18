#include "CombatTrackerLib/App.h"
#include "CombatTrackerLib/Themes.h"

#if defined(_WIN32)

	// Windows.h defines min and max as macros, which collides with the std::min/std::max calls elsewhere in the library.
	// NOMINMAX suppresses them and WIN32_LEAN_AND_MEAN drops the parts of the header we have no use for.
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
	#include <Windows.h>
	#include <string>
#elif defined(__APPLE__)
	#include <mach-o/dyld.h>
	#include <limits.h>
	#include <cstdint>
	#include <vector>
#elif defined(__linux__)
	// read_symlink needs nothing other than <filesystem>. So no extra headers needed.
#else
	#error "Unsupported platform"
#endif

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>
#include <cassert>
#include <ranges>
#include <cstdlib>
#include <iostream>
#include <string_view>
#include <system_error>


namespace CombatTrackerLib {
	App::App(Specification specification) :
		m_specification{ std::move(specification) }, // Specification should be owned by the application so move it.
		m_optionsHandler{
			m_specification.name,
			std::move(m_specification.optionCategoryKeys),
			std::move(m_specification.defaultOptions)
		},
		m_localeProvider{
			m_optionsHandler.getOption<std::string>(OptionsCategory::GENERAL, "language"),
			&getResourceDirectory()
		},
		m_isCloseSignalled{ false },
		m_isUIPresetApplied{ false }
	{
		glfwInit();
		m_windowPointer = std::make_unique<Window>(
			m_specification.initialWindowWidth,
			m_specification.initialWindowHeight,
			m_specification.name
		);
		m_windowPointer->setFullscreen(m_optionsHandler.getOption<bool>(OptionsCategory::APPEARANCE, "fullscreenEnabled")); 
		m_windowPointer->setResizable(m_optionsHandler.getOption<bool>(OptionsCategory::APPEARANCE, "isResizable"));
		m_windowPointer->setVsync(m_optionsHandler.getOption<bool>(OptionsCategory::APPEARANCE, "vsyncEnabled"));

		/*
		Use reinterpret_cast here to treat expression as if it had the target type.
		We are casting a function pointer here to another function pointer that returns a void pointer.
		This is possibly an unsafe conversion so use reinterpret_cast.
		See: https://computergraphics.stackexchange.com/questions/8639/this-function-syntax-is-confusing-me
		*/
		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) { 
			glfwTerminate();
			assert(false);
		}
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
		const char* glsl_version = "#version 150";
		ImGui_ImplOpenGL3_Init(glsl_version);

		// Load the application chosen font. This is needed for loading fonts that support some unconventional characters.
		// I though of creating a font loading system but I think it is completely unnecessary.
		const std::filesystem::path fontPath{ getResourceDirectory() / "assets/Fonts/Roboto-Medium.ttf" };
		std::error_code fontErrorCode{};
		if (std::filesystem::exists(fontPath, fontErrorCode)) {
			ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath.string().c_str(), 18.0f);
		}
		else {
			std::cerr << "Font file couldn't be read, falling back to the default font." << std::endl;
		}

		setUIPreset(m_optionsHandler.getOption<UIPreset>(OptionsCategory::APPEARANCE, "theme"));
	}
	void App::run() {

		// Setup IO.
		ImGuiIO& imguiIO{ ImGui::GetIO() };
		imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	   // Enable Gamepad Controls
		imguiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		   // Enable Docking

		double lastFrameTime{ glfwGetTime() };

		// Main application loop
		while (!glfwWindowShouldClose(glfwGetCurrentContext()) && !m_isCloseSignalled) {

			// Get the delta time between frames, in seconds.
			double currentTime{ glfwGetTime() };
			double dt{ currentTime - lastFrameTime };
			lastFrameTime = currentTime;

			// This specifies the order in which we traverse the scenes.
			const auto& scenes{ m_sceneManager.getScenes() };
			std::ranges::reverse_view reverseView{ scenes };

			// Scene event handling occurs here.
			for (const auto& scene : reverseView) {
				scene->notifyScene(imguiIO);
			}

			// Duration based scene updates goes here.
			for (const auto& scene : reverseView) {
				scene->updateScene(dt);
			}

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Clear frame buffers before rendering next frame.
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// UI layouting and rendering goes here.
			for (const auto& scene : reverseView) {
				scene->renderUI();
			}
			ImGui::Render();

			// OpenGL rendering work goes here. We do the rendering from bottom to top.
			for (const auto& scene : scenes) {
				scene->renderScene();
			}

			// Draw UI on top of OpenGL render data.
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Any window updates go here, after all operations are done.
			m_windowPointer->update();

			// Finally, after everything is done, we handle scene requests.
			m_sceneManager.dequeueSceneOperations();
		}
	}
	App::~App() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
	}
	void App::signalApplicationClose() {
		m_isCloseSignalled = true;
	}
	std::filesystem::path App::findResourceDirectory() {
		std::error_code errorCode{};
		#if defined(_WIN32)

			/*
			Read the module filename to a wide character string buffer.
			GetModuleFileNameW is a Windows API function that retrieves the fully qualified path for the file
			containing the specified module loaded by the current process, using wide-character (Unicode) strings.
			https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamew
			*/
			std::wstring buffer(MAX_PATH, L'\0'); //wstring has a initializer list constructor so we use direct brace initialization.
			auto written{ GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size())) };

			// According to the API specification, this implies that the string buffer is too small to hold the path.
			// So we double the size of the buffer and try to read the path again.
			while (written != 0 && written == buffer.size()) {
				buffer.resize(buffer.size() * 2);
				written = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
			}

			// This implies that the function has failed so we return a fallback which is just the current working directory.
			if (written == 0) {
				return std::filesystem::current_path(errorCode); // Again like JSON reading functionality, we use an error code to not throw an exception.
			}

			// Return the parent path so that the application using the library can place resources in the appropriate, high-level directory.
			buffer.resize(written);
			return std::filesystem::path{ buffer }.parent_path();
		#elif defined(__APPLE__)

			// _NSGetExecutablePath is a low-level C function used on Apple platforms (macOS, iOS) 
			// to retrieve the file system path of the currently running executable.
			// See: https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/dyld.3.html.
			std::vector<char> buffer(PATH_MAX);

			/*
			_NSGetExecutablePath requires us to pass a uint32_t to its argument.
			This function also returns 0 if it is successful, -1 if the buffer is not large enough.
			As a side effect, this function also set the bufsize pointer to the required size.
			So we now the required buffer size on failure immediately.
			*/
			auto size{ static_cast<std::uint32_t>(buffer.size()) };
			if (_NSGetExecutablePath(buffer.data(), &size) == -1) {

				// Resize the buffer to the appropriate final size and retry.
				// If this fails, there is some other issue so we return a default.
				buffer.resize(size);
				if (_NSGetExecutablePath(buffer.data(), &size) != 0) {
					return std::filesystem::current_path(errorCode);
				}
			}

			// According to _NSGetExecutablePath specification, the returned path can still contain symlinks and relative segments. 
			// This is resolved by calling std::filesystem::canonical which converts the path to the canonical absolute path.
			const auto executablePath{ std::filesystem::canonical(buffer.data(), errorCode) };
			if (errorCode) {
				return std::filesystem::path{ buffer.data() }.parent_path();
			}
			return executablePath.parent_path();
		#elif defined(__linux__)

			// On Linux, the OS maintains a symbolic link to the currently running executable file.
			// We read this symbolic link to acquire te resource directory.
			const auto executableDirectory{ std::filesystem::read_symlink("/proc/self/exe", errorCode) };
			if (errorCode) {
				return std::filesystem::current_path(errorCode);
			}
			return executableDirectory.parent_path();
		#endif
	}
	std::filesystem::path App::findUserDataDirectory(std::string_view applicationName) {
		if (applicationName.empty()) {
			return {};
		}
		#if defined(_WIN32)

			// Get the value of the "APPDATA" environment variable.
			// The reason we don't use std::getenv() here is because it doesn't work with wide characters, particularly a windows issue.
			const auto required{ GetEnvironmentVariableW(L"APPDATA", nullptr, 0) };
			if (required == 0) {
				return {};
			}
			std::wstring applicationData(required, L'\0');
			const auto written{ GetEnvironmentVariableW(L"APPDATA", applicationData.data(), required) };
			if (written == 0 || written >= required) {
				return {};
			}
			applicationData.resize(written);
			return std::filesystem::path{ applicationData } / applicationName;
		#elif defined(__APPLE__)
			const auto* home{ std::getenv("HOME") };
			if (home == nullptr || home[0] == '\0') {
				return {};
			}
			return std::filesystem::path{ home } / "Library" / "Application Support" / applicationName;
		#elif defined(__linux__)

			// The XDG base directory specification says to use the default whenever the variable is unset or empty.
			// See https://specifications.freedesktop.org/basedir/latest/.
			const auto* dataHome{ std::getenv("XDG_DATA_HOME") };

			// XDG_DATA_HOME is set, use its value.
			if (dataHome != nullptr && dataHome[0] != '\0') {
				return std::filesystem::path{ dataHome } / applicationName;
			}

			// The default is equal to $HOME/.local/share
			// See https://specifications.freedesktop.org/basedir/latest/.
			const char* home{ std::getenv("HOME") };
			if (home == nullptr || home[0] == '\0') {
				return {};
			}
			return std::filesystem::path{ home } / ".local" / "share" / applicationName;
		#endif
	}
	void App::setUIPreset(UIPreset newPreset) {
		const auto currentPreset{ m_optionsHandler.getOption<UIPreset>(OptionsCategory::APPEARANCE, "theme") };
		if (m_isUIPresetApplied && newPreset == currentPreset) {
			return;
		}
		m_isUIPresetApplied = true;
		if(m_optionsHandler.setOption<UIPreset>(OptionsCategory::APPEARANCE, "theme", newPreset)) {
			ImGui::GetStyle() = PRESET_TO_CONFIG.at(newPreset);
		}
	}
	const std::filesystem::path& App::getResourceDirectory() {
		static const std::filesystem::path s_resourceDirectory{ findResourceDirectory() };
		return s_resourceDirectory;
	}
	const std::filesystem::path& App::getUserDataDirectory(std::string_view applicationName) {
		static std::filesystem::path s_userDataDirectory{ findUserDataDirectory(applicationName) };
		return s_userDataDirectory;
	}
	const LocaleProvider& App::getLocaleProvider() const {
		return m_localeProvider;
	}
	SceneManager& App::getSceneManager() {
		return m_sceneManager;
	}
	OptionsHandler& App::getOptionsHandler() {
		return m_optionsHandler;
	}

	// IMPORTANT TODO: These should be written to utilize the observer pattern. See header file.
	void App::setWindowVsync(bool enabled) {
		m_optionsHandler.setOption<bool>(OptionsCategory::APPEARANCE, "vsyncEnabled", enabled);
		m_windowPointer->setVsync(enabled);
	}
	void App::setWindowFullscreen(bool enabled) {
		m_optionsHandler.setOption<bool>(OptionsCategory::APPEARANCE, "fullscreenEnabled", enabled);
		m_windowPointer->setFullscreen(enabled);
	}
	void App::setWindowResize(bool enabled) {
		m_optionsHandler.setOption<bool>(OptionsCategory::APPEARANCE, "isResizable", enabled);
		m_windowPointer->setResizable(enabled);
	}
	void App::setLanguage(std::string_view localeCode) {
		m_optionsHandler.setOption<std::string>(OptionsCategory::GENERAL, "language", std::string{ localeCode });
		m_localeProvider.setLocale(localeCode);
	}
}