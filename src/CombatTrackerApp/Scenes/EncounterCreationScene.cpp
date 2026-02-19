#include "EncounterCreationScene.h"
#include "MainMenuScene.h"
#include "imgui_internal.h"

namespace CombatTrackerApp {
	EncounterCreationScene::EncounterCreationScene(CombatTrackerLib::App* app) : 
		CombatTrackerLib::Scene(app),
		isEncounterManagerDockspaceInitialized(false),
		isEntityBrowserDockspaceInitialized(false)
	{
		encounterCreationDockFlags = 
			ImGuiDockNodeFlags_NoResize |
			ImGuiDockNodeFlags_NoSplit;
		encounterCreationWindowFlags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;
	}
	EncounterCreationScene::~EncounterCreationScene() {}

	void EncounterCreationScene::notifyScene(ImGuiIO& imguiIO) {}
	void EncounterCreationScene::updateScene(double timeInterval) {}
	void EncounterCreationScene::renderUI() {
	
		// Set configurations of the host window of the scene on first render pass.
		if (!isEncounterManagerDockspaceInitialized) {
			mainViewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(mainViewport->WorkPos);
			ImGui::SetNextWindowSize(mainViewport->WorkSize);
			ImGui::SetNextWindowViewport(mainViewport->ID);
		}
		// Render host window.
		ImGui::Begin("Encounter Creator", nullptr, encounterCreationWindowFlags);

		// Setup dockspace layout on first render pass.
		if (!isEncounterManagerDockspaceInitialized) {

			encounterManagerDockspaceID = ImGui::GetID("Encounter Creator Dockspace");
			ImGui::DockBuilderRemoveNode(encounterManagerDockspaceID);

			// Add dockspace nodes.
			ImGui::DockBuilderAddNode(encounterManagerDockspaceID, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(encounterManagerDockspaceID, mainViewport->WorkSize);

			// Configure dockspace nodes.
			ImGuiID leftID;
			ImGui::DockBuilderSplitNode(encounterManagerDockspaceID, ImGuiDir_Left, 0.10f, &buttonsWindowID, &leftID);
			ImGui::DockBuilderSplitNode(leftID, ImGuiDir_Left, 0.33f, &entityBrowserWindowID, &encounterManagerWindowID);

			// Dock windows.
			ImGui::DockBuilderDockWindow("Entity Browser", entityBrowserWindowID);
			ImGui::DockBuilderDockWindow("Encounter Manager", encounterManagerWindowID);
			ImGui::DockBuilderDockWindow("Menu", buttonsWindowID);

			// Finish dockspace creation.
			ImGui::DockBuilderFinish(encounterManagerDockspaceID);
			isEncounterManagerDockspaceInitialized = true;
		}
		// Render dockspace of the host window.
		ImGui::DockSpace(encounterManagerDockspaceID, ImGui::GetWindowSize(), encounterCreationDockFlags);
		ImGui::End();

		// Submit windows.
		ImGui::Begin("Menu");
		if (ImGui::Button("Return to Main Menu")) {
			applicationReference->queueSceneTransition(this, std::make_unique<MainMenuScene>(applicationReference));
		}
		ImGui::End();

		/*
		Here is the overall layout of this window:
		The window will have 3 tabs: Monsters, Player Characters and Encounters.
		Each tab is further subdivided into two parts: User Created and Compendium.
		*/
		ImGui::Begin("Entity Browser");
		if (!isEntityBrowserDockspaceInitialized) {

			entityBrowserDockspaceID = ImGui::GetID("Entity Browser Dockspace");
			ImGui::DockBuilderRemoveNode(entityBrowserDockspaceID);

			// Add dockspace nodes.
			ImGui::DockBuilderAddNode(entityBrowserDockspaceID, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(entityBrowserDockspaceID, ImGui::GetWindowSize());

			// Dock windows.
			ImGui::DockBuilderDockWindow("Monsters", entityBrowserDockspaceID);
			ImGui::DockBuilderDockWindow("Player Characters", entityBrowserDockspaceID);
			ImGui::DockBuilderDockWindow("Encounters", entityBrowserDockspaceID);

			// Finish dockspace creation.
			ImGui::DockBuilderFinish(entityBrowserDockspaceID);
			isEntityBrowserDockspaceInitialized = true;
		}
		// Render dockspace of the entity browser window.
		ImGui::DockSpace(entityBrowserDockspaceID, ImGui::GetWindowSize());
		ImGui::End();
		
		ImGui::Begin("Monsters");
		ImGui::End();

		ImGui::Begin("Player Characters");
		ImGui::End();

		ImGui::Begin("Encounters");
		ImGui::End();

		ImGui::Begin("Encounter Manager");
		ImGui::End();
	}
	void EncounterCreationScene::renderScene() {}
}