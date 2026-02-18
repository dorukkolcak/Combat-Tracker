#include "OverlayScene.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

namespace CombatTrackerApp {
	OverlayScene::OverlayScene(CombatTrackerLib::App* app) : CombatTrackerLib::Scene(app)
	{
		dockedWindowFlags = ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;
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

		// Create the window that will host the dockspace.
		ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowSize(ImVec2(mainViewport->Size.x / 3, mainViewport->Size.y / 3));
		ImGui::Begin("Options", nullptr, 0);

		// Create the dockspace.
		overlayDockspaceID = ImGui::GetID("Overlay Dockspace");
		ImGui::DockBuilderAddNode(overlayDockspaceID, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(overlayDockspaceID, ImVec2(mainViewport->Size.x / 3, mainViewport->Size.y / 3));

		// TODO: Configure dockspace elements. Add more options as the app gets developed.
		ImGuiID leftID = overlayDockspaceID;
		ImGui::DockBuilderSplitNode(overlayDockspaceID, ImGuiDir_Left, 0.25f, &viewID, &leftID);
		ImGui::DockBuilderDockWindow("View", viewID);

		// Finish dockspace creation.
		ImGui::DockBuilderFinish(overlayDockspaceID);
		ImGui::DockSpace(overlayDockspaceID, ImVec2(0, 0), 0);
		ImGui::End();

		// Submit windows in dockspace.
		ImGui::Begin("View", nullptr, dockedWindowFlags);
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