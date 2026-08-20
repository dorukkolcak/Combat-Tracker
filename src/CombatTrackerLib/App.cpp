#include "CombatTrackerLib/App.h"

#include <cassert>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <ranges>
#include <string_view>
#include <system_error>

namespace CombatTrackerLib {
	App::App(Specification specification)
		: Unique(),
		  m_specification{ std::move(specification) }, // Specification should be owned by the application so move it.
		  m_directoryProvider{ m_specification.name }, m_optionsHandler{ m_directoryProvider.getUserDataDirectory(),
																		 std::move(m_specification.optionCategoryKeys),
																		 std::move(m_specification.defaultOptions) },
		  m_localeProvider{ m_directoryProvider,
							m_optionsHandler.getOption<std::string>(OptionsCategory::GENERAL, "language") },
		  m_themeProvider{ m_directoryProvider }, m_isCloseSignalled{ false }, m_isUIPresetApplied{ false } {
		glfwInit();
		m_windowPointer = std::make_unique<Window>(
			m_specification.initialWindowWidth, m_specification.initialWindowHeight, m_specification.name
		);
		m_windowPointer->setFullscreen(
			m_optionsHandler.getOption<bool>(OptionsCategory::APPEARANCE, "fullscreenEnabled")
		);
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
		const std::filesystem::path fontPath{ m_directoryProvider.getResourceDirectory() /
											  "assets/Fonts/Roboto-Medium.ttf" };
		std::error_code fontErrorCode{};
		if (std::filesystem::exists(fontPath, fontErrorCode)) {
			ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath.string().c_str(), 18.0f);
		}
		else {
			std::cerr << "Font file couldn't be read, falling back to the "
						 "default font."
					  << std::endl;
		}

		setTheme(m_optionsHandler.getOption<std::string>(OptionsCategory::APPEARANCE, "theme"));
	}
	void App::run() {

		// Setup IO.
		ImGuiIO& imguiIO{ ImGui::GetIO() };
		imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
		imguiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	   // Enable Docking

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

			//OptionsHandlerProxy::invokeDeferredCallbacks(m_optionsHandler);

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
	void App::signalApplicationClose() { m_isCloseSignalled = true; }
	void App::setTheme(const std::string& themeName) {
		const auto currentPreset{ m_optionsHandler.getOption<std::string>(OptionsCategory::APPEARANCE, "theme") };
		if (m_isUIPresetApplied && themeName == currentPreset) {
			return;
		}
		m_isUIPresetApplied = true;
		if (m_optionsHandler.setOption<std::string>(OptionsCategory::APPEARANCE, "theme", themeName)) {
			const auto* style{ m_themeProvider.getStyle(themeName) };
			if (style == nullptr) {
				ImGui::StyleColorsDark();
			}
			else {
				ImGui::GetStyle() = *style;
			}
		}
	}
	const DirectoryProvider& App::getDirectoryProvider() const { return m_directoryProvider; }
	const LocaleProvider& App::getLocaleProvider() const { return m_localeProvider; }
	SceneManager& App::getSceneManager() { return m_sceneManager; }
	OptionsHandler& App::getOptionsHandler() { return m_optionsHandler; }

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
	const ThemeProvider& App::getThemeProvider() const { return m_themeProvider; }
}