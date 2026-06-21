#include "App.h"
#include "Utils.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui.h"

#include <cassert>
#include <ranges>

namespace CombatTrackerLib {
	App::App(const AppSpecification& specification) : applicationSpecification{ specification }
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
		ImGuiIO& imguiIO{ ImGui::GetIO() };
		imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	   // Enable Gamepad Controls
		imguiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		   // Enable Docking

		// Set the reset color value used when glClear is called. Currently this is tan.
		glClearColor(0.858824f, 0.576471f, 0.439216f, 1.0f);

		double lastFrameTime{ glfwGetTime() };
		// Main application loop
		while (!App::mainWindow->getWindowShouldClose()) {

			// Get the delta time between frames, in seconds.
			double currentTime{ glfwGetTime() };
			double dt{ currentTime - lastFrameTime };
			lastFrameTime = currentTime;

			// This specifies the order in which we traverse the scenes.
			std::ranges::reverse_view reverseView{ sceneStack };

			// Scene event handling occurs here.
			glfwPollEvents();
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
			for (const auto& scene : sceneStack) {
				scene->renderScene();
			}

			// Draw UI on top of OpenGL render data.
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Any window updates go here, after all operations are done.
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
		sceneTransitionQueue.insert({ transitionedFrom, std::move(transitionedTo) });
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
}