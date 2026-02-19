#include <iostream>

#include "OverlayScene.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

namespace CombatTrackerApp {
	OverlayScene::OverlayScene(CombatTrackerLib::App* app) : CombatTrackerLib::Scene(app), isOverlayDockspaceInitialized(false)
	{
	}
	OverlayScene::~OverlayScene() {}

	void OverlayScene::notifyScene(ImGuiIO& imguiIO) {}
	void OverlayScene::updateScene(double timeInterval) {}
	void OverlayScene::renderUI() {

		/*
		DEBUG AND DEVELOPMENT ONLY, remove these later.
		It is pretty cumbersome to progmatically dock these windows. 
		As these windows are submitted before dockspace creation.
		So we essentially need to create identical copies that are registered after dockspace creation. 
		I can't be bothered to do this. Instead we submit them during program execution.
		This change is saved in our .ini file so we only need to do this once.
		*/
		ImGui::ShowDebugLogWindow();
		ImGui::ShowMetricsWindow();
		ImGui::ShowDemoWindow();

		// Create the window that will host the dockspace.
		// Set the initial window size based on the main viewport.
		if (!isOverlayDockspaceInitialized) {
			mainViewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowSize(ImVec2(mainViewport->Size.x / 3.0f, mainViewport->Size.y / 3.0f));
		}

		// Render the host window.
		ImGui::Begin("Options");

		// Configure the dockspace if it isn't configured previously or has received changes.
		if (!isOverlayDockspaceInitialized) {

			overlayDockspaceID = ImGui::GetID("Overlay Dockspace");

			// Setup dockspace node.
			ImGui::DockBuilderRemoveNode(overlayDockspaceID);
			ImGui::DockBuilderAddNode(overlayDockspaceID, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(overlayDockspaceID, ImVec2(mainViewport->Size.x / 3, mainViewport->Size.y / 3));

			// Configure dockspace elements.
			ImGui::DockBuilderSplitNode(overlayDockspaceID, ImGuiDir_Left, 0.50f, &generalID, &viewID);
			ImGui::DockBuilderDockWindow("General", generalID);
			ImGui::DockBuilderDockWindow("View", viewID);

			// Finish dockspace creation.
			ImGui::DockBuilderFinish(overlayDockspaceID);
			isOverlayDockspaceInitialized = true;
		}
		// Render the dockspace.
		ImGui::DockSpace(overlayDockspaceID, ImGui::GetWindowSize(), 0);
		ImGui::End();

		// Submit windows in dockspace.
		ImGui::Begin("General");

		ImGui::End();

		ImGui::Begin("View");
		static int buttonCounter = 0;
		if (ImGui::RadioButton("Dark Mode", &buttonCounter, 0)) {
			CombatTrackerLib::App::createImguiDarkTheme();
			CombatTrackerLib::App::initializeCommonColors();
		}
		if (ImGui::RadioButton("Light Mode", &buttonCounter, 1)) {
			CombatTrackerLib::App::createImguiLightTheme();
			CombatTrackerLib::App::initializeCommonColors();
		}
		ImGui::End();

	}
	void OverlayScene::renderScene() {}
}