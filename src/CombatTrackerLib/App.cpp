#include "App.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui.h"
#include "Styling.h"

#include <cassert>
#include <ranges>

namespace CombatTrackerLib {
	App::App(const AppSpecification& specification) : applicationSpecification(specification)
	{
		glfwInit();
		mainWindow = std::make_unique<Window>(applicationSpecification.mainWindowSpecification);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
			assert(false);
		}
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		/*
		Pay attention to the UI ordering here: 
		Because we set the Imgui::Style in either dark or light theme, if the common colors are initialized before them, the common colors get overridden.
		So initialize the common colors after dark or light theme initialization.
		Ideally we should have an options section that dynamically changes the color scheme.
		With the way I create the themes this should already be possible, just needs UI support.
		*/
		setupLayout();
		if (applicationSpecification.isDarkModeEnabled) {
			createImguiDarkTheme();
		}
		else {
			createImguiLightTheme();
		}
		initializeCommonColors();
		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(mainWindow->getGLFWwindow(), true);
		const char* glsl_version = "#version 130";
		ImGui_ImplOpenGL3_Init(glsl_version);
	}
	// Destructor should be empty as we can explicitly call app.stop() to free all resources.
	App::~App() {}
	void App::run() {

		// Setup IO.
		ImGuiIO& imguiIO = ImGui::GetIO();
		imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	   // Enable Gamepad Controls
		imguiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		   // Enable Docking
		// Set the reset color value used when glClear is called. Currently this is tan.
		glClearColor(0.858824f, 0.576471f, 0.439216f, 1.0f);

		// Main application loop
		while (!App::mainWindow->getWindowShouldClose()) {

			// This specifies the order in which we traverse the scenes.
			std::ranges::reverse_view reverseView{ sceneStack };

			// TODO: Scene event handling occurs here.
			glfwPollEvents();
			for (auto& scene : reverseView) {
				scene->notifyScene(imguiIO);
			}

			// TODO: Duration based scene updates goes here.
			double ts = glfwGetTime();
			ts = glfwGetTime() - ts;
			for (auto& scene : reverseView) {
				scene->updateScene(ts);
			}

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Clear frame buffers before rendering next frame.
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// TODO: UI layouting and rendering goes here.
			for (auto& scene : reverseView) {
				scene->renderUI();
			}
			ImGui::Render();

			// TODO: OpenGL rendering work goes here. We do the rendering from bottom to top. .
			for (auto& scene : sceneStack) {
				scene->renderScene();
			}

			// Draw UI on top of OpenGL render data.
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Any window updates go here, after all operations are done.
			// Currently this just swaps frame buffers so that the next frame can be rendered.
			mainWindow->update();

			// Finally, after everything is done, we handle scene requests.
			dequeueSceneOperations();

		}
		App::stop();
	}
	void App::stop() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		mainWindow->destroy();
		glfwTerminate();

	}
	void App::queueAddScene(std::unique_ptr<Scene> scene) {
		sceneAdditionQueue.push_back(std::move(scene));
	}
	void App::queueDeleteScene(Scene* sceneToBeDeleted) {
		sceneDeletionQueue.push_back(sceneToBeDeleted);
	}
	void App::queueSceneTransition(Scene* transitionedFrom, std::unique_ptr<Scene> transitionedTo) {
		sceneTransitionQueue.insert({ transitionedFrom, std::move(transitionedTo) } );
	}
	void App::dequeueSceneOperations() {
		// Handle sceneTransition
		for (auto& transitionedTo : sceneTransitionQueue) {
			for (auto& transitionedFrom : sceneStack) {
				if (transitionedFrom.get() == transitionedTo.first) {
					transitionedFrom = std::move(transitionedTo.second);
					break;
				}
			}
		}
		sceneTransitionQueue.clear();
		// Handle sceneDeletion
		for (auto& deletedScene : sceneDeletionQueue) {
			for (auto& scene : sceneStack) {
				if (scene.get() == deletedScene) {
					/*
					This is kinda dumb. The main reason we are using smart pointers in the first place is that they automatically manage memory.
					But I explicitly "delete" the memory associated with the smart pointer here.
					The thing is I don't like transfering the ownership of a new object to the "deleted" smart pointer either.
					As after deletion we might have less scenes than the previous state, aka less smart pointers.
					So regardless some smart pointers will just be left null with this design and I need to explicitly remove the null ones.
					Which I do with the functor.
					*/
					scene.reset();
					break;
				}
			}
		}
		// Remove the deleted scene unique_ptrs from the sceneStack.
		sceneStack.erase(
			std::remove_if(sceneStack.begin(), sceneStack.end(), isSceneEmpty()), 
			sceneStack.end()
		);
		sceneDeletionQueue.clear();
		// Handle sceneAddition
		for (auto& addedScene : sceneAdditionQueue) {
			sceneStack.push_back(std::move(addedScene));
		}
		sceneAdditionQueue.clear();
	}
	void App::createImguiLightTheme() {

		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::StyleColorsLight();

		ImVec4* colors = style.Colors;

		// Text coloring.
		colors[ImGuiCol_Text] = LightModeColors::TEXT_COLOR;
		colors[ImGuiCol_TextDisabled] = LightModeColors::TEXT_DISABLED_COLOR;

		// Window coloring.
		colors[ImGuiCol_WindowBg] = LightModeColors::WINDOW_BACKGROUND_COLOR;
		colors[ImGuiCol_DockingEmptyBg] = LightModeColors::WINDOW_BACKGROUND_COLOR;
		colors[ImGuiCol_DockingPreview] = LightModeColors::DOCKING_PREVIEW_COLOR;
		colors[ImGuiCol_ChildBg] = LightModeColors::CHILD_WINDOW_BACKGROUND_COLOR;
		colors[ImGuiCol_PopupBg] = LightModeColors::CHILD_WINDOW_BACKGROUND_COLOR;

		// Border coloring.
		colors[ImGuiCol_Border] = LightModeColors::BORDER_COLOR;

		// Bar coloring.
		colors[ImGuiCol_TitleBg] = LightModeColors::BAR_COLOR;
		colors[ImGuiCol_TitleBgActive] = LightModeColors::BAR_COLOR;
		colors[ImGuiCol_MenuBarBg] = LightModeColors::BAR_COLOR;
		colors[ImGuiCol_ScrollbarBg] = LightModeColors::BAR_COLOR;
		colors[ImGuiCol_ScrollbarGrab] = LightModeColors::SCROLL_BAR_GRAB_COLOR;
		colors[ImGuiCol_ScrollbarGrabHovered] = LightModeColors::BORDER_COLOR;
		colors[ImGuiCol_ScrollbarGrabActive] = LightModeColors::SCROLL_BAR_GRAB_ACTIVE_COLOR;

		// Frame coloring.
		colors[ImGuiCol_FrameBg] = LightModeColors::FRAME_COLOR;
		colors[ImGuiCol_FrameBgHovered] = LightModeColors::FRAME_BACKGROUND_HOVERED_COLOR;
		colors[ImGuiCol_FrameBgActive] = LightModeColors::FRAME_BACKGROUND_ACTIVE_COLOR;

		// Separator coloring.
		colors[ImGuiCol_Separator] = LightModeColors::SEPARATOR_COLOR;

		// Tab coloring.
		colors[ImGuiCol_Tab] = LightModeColors::TAB_COLOR;
		colors[ImGuiCol_TabActive] = LightModeColors::TAB_ACTIVE_COLOR;
		colors[ImGuiCol_TabUnfocused] = LightModeColors::TAB_UNFOCUSED_COLOR;
		colors[ImGuiCol_TabUnfocusedActive] = LightModeColors::TAB_UNFOCUSED_ACTIVE_COLOR;

	}
	void App::createImguiDarkTheme() {

		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::StyleColorsDark();

		ImVec4* colors = style.Colors;

		// Text coloring.
		colors[ImGuiCol_Text] = DarkModeColors::TEXT_COLOR;
		colors[ImGuiCol_TextDisabled] = DarkModeColors::TEXT_DISABLED_COLOR;

		// Window coloring.
		colors[ImGuiCol_WindowBg] = DarkModeColors::WINDOW_BACKGROUND_COLOR;
		colors[ImGuiCol_DockingEmptyBg] = DarkModeColors::WINDOW_BACKGROUND_COLOR;
		colors[ImGuiCol_DockingPreview] = DarkModeColors::DOCKING_PREVIEW_COLOR;
		colors[ImGuiCol_ChildBg] = DarkModeColors::CHILD_WINDOW_BACKGROUND_COLOR;
		colors[ImGuiCol_PopupBg] = DarkModeColors::CHILD_WINDOW_BACKGROUND_COLOR;

		// Border coloring.
		colors[ImGuiCol_Border] = DarkModeColors::BORDER_COLOR;

		// Bar coloring.
		colors[ImGuiCol_TitleBg] = DarkModeColors::BAR_COLOR;
		colors[ImGuiCol_TitleBgActive] = DarkModeColors::BAR_COLOR;
		colors[ImGuiCol_MenuBarBg] = DarkModeColors::BAR_COLOR;
		colors[ImGuiCol_ScrollbarBg] = DarkModeColors::BAR_COLOR;
		colors[ImGuiCol_ScrollbarGrab] = DarkModeColors::SCROLL_BAR_GRAB_COLOR;
		colors[ImGuiCol_ScrollbarGrabHovered] = DarkModeColors::BORDER_COLOR;
		colors[ImGuiCol_ScrollbarGrabActive] = DarkModeColors::SCROLL_BAR_GRAB_ACTIVE_COLOR;

		// Frame coloring.
		colors[ImGuiCol_FrameBg] = DarkModeColors::FRAME_COLOR;
		colors[ImGuiCol_FrameBgHovered] = DarkModeColors::FRAME_BACKGROUND_HOVERED_COLOR;
		colors[ImGuiCol_FrameBgActive] = DarkModeColors::FRAME_BACKGROUND_ACTIVE_COLOR;

		// Separator coloring.
		colors[ImGuiCol_Separator] = DarkModeColors::SEPARATOR_COLOR;

		// Tab coloring.
		colors[ImGuiCol_Tab] = DarkModeColors::TAB_COLOR;
		colors[ImGuiCol_TabActive] = DarkModeColors::TAB_ACTIVE_COLOR;
		colors[ImGuiCol_TabUnfocused] = DarkModeColors::TAB_UNFOCUSED_COLOR;
		colors[ImGuiCol_TabUnfocusedActive] = DarkModeColors::TAB_UNFOCUSED_ACTIVE_COLOR;

	}
	void App::setupLayout() {

		ImGuiStyle& style = ImGui::GetStyle();

		// Padding, spacing, rounding and sizing.
		style.WindowPadding = WINDOW_PADDING;
		style.FramePadding = FRAME_PADDING;
		style.ItemSpacing = ITEM_SPACING;
		style.ItemInnerSpacing = ITEM_INNER_SPACING;

		style.WindowBorderSize = WINDOW_BORDER_SIZE;
		style.FrameBorderSize = FRAME_BORDER_SIZE;
		style.TabBorderSize = TAB_BORDER_SIZE;
		style.DockingSeparatorSize = DOCKING_SEPARATOR_SIZE;
	}
	void App::initializeCommonColors() {

		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;
		// Checkmark and slider coloring.
		colors[ImGuiCol_CheckMark] = CommonColors::BRICK_RED_COLOR;
		colors[ImGuiCol_SliderGrab] = CommonColors::GOLD_COLOR;
		colors[ImGuiCol_SliderGrabActive] = CommonColors::GOLD_COLOR;

		// Selectable and tree coloring.
		colors[ImGuiCol_Header] = CommonColors::HEADER_COLOR;
		colors[ImGuiCol_HeaderHovered] = CommonColors::HEADER_HOVERED_COLOR;
		colors[ImGuiCol_HeaderActive] = CommonColors::HEADER_ACTIVE_COLOR;

		colors[ImGuiCol_TabHovered] = CommonColors::TAB_HOVERED_COLOR;
		colors[ImGuiCol_SeparatorHovered] = CommonColors::SEPARATOR_HOVERED_COLOR;
		colors[ImGuiCol_SeparatorActive] = CommonColors::BRICK_RED_COLOR;
		colors[ImGuiCol_TextSelectedBg] = CommonColors::TEXT_SELECTED_BACKGROUND_COLOR;
		// Button coloring.
		colors[ImGuiCol_Button] = CommonColors::BRICK_RED_COLOR;
		colors[ImGuiCol_ButtonHovered] = CommonColors::BRICK_RED_HOVER_COLOR;
		colors[ImGuiCol_ButtonActive] = CommonColors::BRICK_RED_ACTIVE_COLOR;
	}
}