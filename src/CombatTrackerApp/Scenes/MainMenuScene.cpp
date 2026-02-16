#include "MainMenuScene.h"
#include "./EncounterManagement/EncounterCreationScene.h"
#include "./EncounterManagement/EncounterRunningScene.h"
#include "./EncounterManagement/CompletedEncountersScene.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace CombatTrackerApp {
	MainMenuScene::MainMenuScene(CombatTrackerLib::App* app) : 
		CombatTrackerLib::Scene(app)
	{
        mainMenuDockspaceFlags =
            ImGuiDockNodeFlags_NoResize |
            ImGuiDockNodeFlags_NoSplit |
            ImGuiDockNodeFlags_PassthruCentralNode;
        hostWindowFlags = 
        ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus |
            ImGuiWindowFlags_MenuBar;

	}
	MainMenuScene::~MainMenuScene() {}
    void MainMenuScene::notifyScene(ImGuiIO& imguiIO) {}
    void MainMenuScene::updateScene(double timeInterval) {}
    void MainMenuScene::renderUI()  {

        mainMenuDockspaceID = ImGui::GetID("Main Menu Dockspace");
        mainMenuViewport = ImGui::GetMainViewport();

        // Set configurations of the host window of the scene.
        ImGui::SetNextWindowPos(mainMenuViewport->WorkPos);
        ImGui::SetNextWindowSize(mainMenuViewport->WorkSize);
        ImGui::SetNextWindowViewport(mainMenuViewport->ID);
        // Create the host window.
        ImGui::Begin("Host Window", nullptr, hostWindowFlags);

        // Create the dockplace settings and layout.
        ImGui::DockBuilderAddNode(mainMenuDockspaceID, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(mainMenuDockspaceID, mainMenuViewport->WorkSize);

        ImGui::DockBuilderSplitNode(mainMenuDockspaceID, ImGuiDir_Left, 0.33f, &encounterCreationID, &encounterRunningID);
        ImGui::DockBuilderSplitNode(encounterRunningID, ImGuiDir_Right, 0.50f, &completedEncountersViewID, &encounterRunningID);

        ImGui::DockBuilderDockWindow("Encounter Creation", encounterCreationID);
        ImGui::DockBuilderDockWindow("Run Encounter", encounterRunningID);
        ImGui::DockBuilderDockWindow("View Completed Encounters", completedEncountersViewID);

        ImGui::DockBuilderFinish(mainMenuDockspaceID);

        // Create the actual dockspace inside the host window.
        ImGui::DockSpace(mainMenuDockspaceID, ImVec2(0, 0), mainMenuDockspaceFlags);
        ImGui::End();

        // TODO: Create the Encounter Creation window. Finalize UI designs.
        if (ImGui::Begin("Encounter Creation")) {
            if (ImGui::Button("Create Encounter")) {
                applicationReference->queueSceneTransition(this,std::make_unique<EncounterCreationScene>(applicationReference));
            }
        }
        ImGui::End();

        // TODO: Create the Run Encounter window. Finalize UI designs.
        if (ImGui::Begin("Run Encounter")) {
            if (ImGui::Button("Select Encounter")) {
                applicationReference->queueSceneTransition(this, std::make_unique<EncounterRunningScene>(applicationReference));
            }
        }
        ImGui::End();

        // TODO: Create the Completed Encounters window. Finalize UI designs.
        if (ImGui::Begin("View Completed Encounters")) {
            if (ImGui::Button("Browse Encounters")) {
                applicationReference->queueSceneTransition(this, std::make_unique<CompletedEncountersScene>(applicationReference));
            }
        }
        ImGui::End();
    }

	void MainMenuScene::renderScene() {}
}
