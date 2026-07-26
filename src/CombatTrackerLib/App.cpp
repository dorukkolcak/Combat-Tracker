#include "App.h"
#include "Utils.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui.h"

#include <algorithm>
#include <cassert>
#include <ranges>

namespace CombatTrackerLib {
	App::App(const Specification& specification) : m_applicationSpecification{ specification }, m_theme{}
	{
		glfwInit();
		m_mainWindow = std::make_unique<Window>(m_applicationSpecification.mainWindowSpecification);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
			assert(false);
		}
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(m_mainWindow->getGLFWwindow(), true);
		const char* glsl_version = "#version 150";
		ImGui_ImplOpenGL3_Init(glsl_version);
		m_theme.setUITheme(specification.initialPreset);
	}
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
		while (!App::m_mainWindow->getWindowShouldClose()) {

			// Get the delta time between frames, in seconds.
			double currentTime{ glfwGetTime() };
			double dt{ currentTime - lastFrameTime };
			lastFrameTime = currentTime;

			// This specifies the order in which we traverse the scenes.
			std::ranges::reverse_view reverseView{ m_sceneStack };

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
			for (const auto& scene : m_sceneStack) {
				scene->renderScene();
			}

			// Draw UI on top of OpenGL render data.
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Any window updates go here, after all operations are done.
			m_mainWindow->update();

			// Finally, after everything is done, we handle scene requests.
			dequeueSceneOperations();

		}
		App::stop();
	}
	void App::stop() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		m_mainWindow->destroy();
		glfwTerminate();

	}
	void App::queueAddScene(std::unique_ptr<Scene> scene) {
		m_sceneAdditionQueue.push_back(std::move(scene));
	}
	void App::queueDeleteScene(Scene* sceneToBeDeleted) {
		m_sceneDeletionQueue.push_back(sceneToBeDeleted);
	}
	void App::queueSceneTransition(Scene* transitionedFrom, std::unique_ptr<Scene> transitionedTo) {
		m_sceneTransitionQueue.insert({ transitionedFrom, std::move(transitionedTo) });
	}
	void App::dequeueSceneOperations() {

		// Handle scene transition.
		for (auto& transitionedTo : m_sceneTransitionQueue) {
			for (auto& transitionedFrom : m_sceneStack) {
				if (transitionedFrom.get() == transitionedTo.first) {
					transitionedFrom = std::move(transitionedTo.second);
					break;
				}
			}
		}
		m_sceneTransitionQueue.clear();

		// Handle scene deletion.
		for (auto& deletedScene : m_sceneDeletionQueue) {
			for (auto& scene : m_sceneStack) {
				if (scene.get() == deletedScene) {

					/*
					This is kinda dumb. The main reason we are using smart pointers in the first place is that they automatically manage memory.
					But I explicitly "delete" the memory associated with the smart pointer here.
					The thing is I don't like transferring the ownership of a new object to the "deleted" smart pointer either.
					As after deletion we might have less scenes than the previous state, aka less smart pointers.
					So regardless some smart pointers will just be left null with this design and I need to explicitly remove the null ones.
					*/
					scene.reset();
					break;
				}
			}
		}

		// Remove the deleted scene unique_ptrs from the sceneStack.
		m_sceneStack.erase(
			std::remove_if(m_sceneStack.begin(), m_sceneStack.end(), [](const std::unique_ptr<Scene>& scenePointer) {
				return scenePointer == nullptr;
			}),
			m_sceneStack.end()
		);
		m_sceneDeletionQueue.clear();

		// Handle sceneAddition
		for (auto& addedScene : m_sceneAdditionQueue) {
			m_sceneStack.push_back(std::move(addedScene));
		}
		m_sceneAdditionQueue.clear();
	}
}