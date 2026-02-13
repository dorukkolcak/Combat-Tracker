#include "App.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <cassert>


namespace CombatTrackerLib {
	App::App(const AppSpecification& applicationSpecification) {
		this->applicationSpecification = applicationSpecification;
		glfwInit();
		mainWindow = std::make_unique<Window>(applicationSpecification.mainWindowSpecification);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
			assert(false);
		}
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(mainWindow->getGLFWwindow(), true);
		const char* glsl_version = "#version 130";
		ImGui_ImplOpenGL3_Init(glsl_version);
	}
	// Destructor should be empty as we can explicitly call app.stop() to free all resources.
	App::~App() {}
	void App::run() {
		// Main application loop
		while (!App::mainWindow->getWindowShouldClose()) {
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			// TODO: Duration based scene updates go here.
			// Modify this function once all the scene functionality gets implemented.
			double ts = glfwGetTime();
			ts = glfwGetTime() - ts;
			for (auto& scene : sceneStack) {
				//scene->updateScene(ts);
			}
			// TODO: Scene event handling occurs here. 
			// Modify this section when event handling is done.
			for (auto& scene : sceneStack) {
				//scene->notifyScene();
			}
			// TODO: Rendering work goes here.
			// Modify this section when rendering is done.
			for (auto& scene : sceneStack) {
				//scene->renderScene();
			}
			/*
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			*/
			// TODO: Move these calls to the bottom scene (dubbed PresenceScene) so that we can render all the widgets of the top scenes properly.
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			mainWindow->update();

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