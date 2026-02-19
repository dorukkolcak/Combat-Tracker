#pragma once
#include "App.h"
#include "imgui.h"

namespace CombatTrackerLib {
	class App;
	class Scene {
	public:
		Scene(App* app);
		virtual ~Scene() = default;
		/*
		The imguiIO parameter is used for handling event logic.
		When imguiIO.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		When imguiIO.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		*/
		virtual void notifyScene(ImGuiIO& imguiIO) = 0;
		virtual void updateScene(double timeInterval) = 0;
		virtual void renderUI() = 0;
		virtual void renderScene() = 0;

	protected:
		// We keep this to queue scene events.
		App* applicationReference;

		ImGuiViewport* mainViewport = nullptr;
		
		static bool centeredButton(const char* buttonLabel);
	};
}