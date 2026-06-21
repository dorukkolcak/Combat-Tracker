#include <iostream>

#include "OverlayScene.h"
#include "Utils.h"

#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

namespace CombatTrackerApp {
	OverlayScene::OverlayScene(CombatTrackerLib::App* app) : CombatTrackerLib::Scene{ app }, m_isOverlayDockspaceInitialized{ false }
	{
	}
	OverlayScene::~OverlayScene() {}

	void OverlayScene::notifyScene(const ImGuiIO& imguiIO) {}
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
		if (!m_isOverlayDockspaceInitialized) {
			m_mainViewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowSize(ImVec2{ m_mainViewport->Size.x / 3.0f, m_mainViewport->Size.y / 3.0f });
		}

		// Render the host window.
		ImGui::Begin("Options");

		// Configure the dockspace if it isn't configured previously or has received changes.
		if (!m_isOverlayDockspaceInitialized) {

			m_overlayDockspaceID = ImGui::GetID("Overlay Dockspace");

			// Setup dockspace node.
			ImGui::DockBuilderRemoveNode(m_overlayDockspaceID);
			ImGui::DockBuilderAddNode(m_overlayDockspaceID, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(m_overlayDockspaceID, ImVec2{ m_mainViewport->Size.x / 3, m_mainViewport->Size.y / 3 } );

			// Configure dockspace elements.
			ImGui::DockBuilderSplitNode(m_overlayDockspaceID, ImGuiDir_Left, 0.50f, &m_generalID, &m_viewID);
			ImGui::DockBuilderDockWindow("General", m_generalID);
			ImGui::DockBuilderDockWindow("View", m_viewID);

			// Finish dockspace creation.
			ImGui::DockBuilderFinish(m_overlayDockspaceID);
			m_isOverlayDockspaceInitialized = true;
		}

		// Render the dockspace.
		ImGui::DockSpace(m_overlayDockspaceID, ImGui::GetWindowSize(), 0);
		ImGui::End();

		// Submit windows in dockspace.
		ImGui::Begin("General");

		ImGui::End();

		ImGui::Begin("View");
		static int buttonCounter{};
		if (ImGui::RadioButton("Dark Mode", &buttonCounter, 0)) {
			CombatTrackerLib::createImguiDarkTheme();
			CombatTrackerLib::initializeCommonColors();
		}
		if (ImGui::RadioButton("Light Mode", &buttonCounter, 1)) {
			CombatTrackerLib::createImguiLightTheme();
			CombatTrackerLib::initializeCommonColors();
		}
		ImGui::End();

	}
	void OverlayScene::renderScene() {}
}