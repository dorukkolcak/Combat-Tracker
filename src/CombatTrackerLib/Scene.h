#pragma once

#include "CombatTrackerLib/App.h"
#include "CombatTrackerLib/LocaleProvider.h"

#include <imgui.h>

namespace CombatTrackerLib {

	// Forward declaration to prevent linker errors.
	class App;
	class Scene {
	public:

		// A scene needs an application reference for construction so we shouldn't be able to construct a scene with a default constructor.
		Scene() = delete; 
		explicit Scene(App* app);

		virtual ~Scene() = default;

		/*
		The imguiIO parameter is used for handling event logic.
		When imguiIO.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		When imguiIO.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		This function is specifically for non-Imgui event handling.
		I expect this function to be empty for most scenes, as they purely consist of UI widgets.
		*/
		virtual void notifyScene(const ImGuiIO& imguiIO) = 0; 
		
		/*
		This function is for duration-based scene updates. Think something like a car moving across a road.
		I might actually end up never using this function as the application will probably be purely input based.
		Maybe I can do something in battlemap visualization? Some sort of simple animation system?
		*/
		virtual void updateScene(double timeInterval) = 0;

		/*
		This function is for rendering the ImGui side of the application.
		The override of this function will probably be quite large for most scenes.
		As most ImGui widgets return whether they receive any events or not, 
		this function is also responsible for handling application logic based on UI events.
		So think things like saving an entity based on a button press. This should be handled here.
		*/
		virtual void renderUI() = 0;

		/*
		This function is for rendering the OpenGL side of the application (VBO, VAO, EBO, any buffer object pretty much).
		One edge case to be considered here is rendering an OpenGL frame buffer to an ImGui window.
		The framebuffer itself should be configured within this function.
		But actual rendering of the buffer should be done in renderUI as the render call needs to be within ImGui::Begin/End() calls.
		*/
		virtual void renderScene() = 0;

	protected:
		App* m_applicationPointer{};
		const CombatTrackerLib::LocaleProvider& m_localeProviderRef;
	};
}