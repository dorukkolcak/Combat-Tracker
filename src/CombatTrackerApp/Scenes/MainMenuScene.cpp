#include "MainMenuScene.h"
#include "EncounterCreationScene.h"
#include "Utils.h"

#include "imgui.h"
#include "imgui_internal.h"

namespace CombatTrackerApp {
    MainMenuScene::MainMenuScene(CombatTrackerLib::App* app) :
        m_titleTexture{"../assets/Title.png"},
        CombatTrackerLib::Scene{ app },
        m_isMainMenuDockspaceInitialized{ false }
	{   
        m_mainMenuDockspaceFlags =
            ImGuiDockNodeFlags_NoResize |
            ImGuiDockNodeFlags_NoSplit | 
            ImGuiDockNodeFlags_NoWindowMenuButton;
        m_hostWindowFlags =
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus |
            ImGuiWindowFlags_NoScrollbar | 
            ImGuiWindowFlags_NoScrollWithMouse;
        m_mainMenuWindowClass.DockNodeFlagsOverrideSet =
            ImGuiDockNodeFlags_NoResize |
            ImGuiDockNodeFlags_NoSplit |
            ImGuiDockNodeFlags_NoTabBar;
	}
    void MainMenuScene::notifyScene(const ImGuiIO& imguiIO) {}
    void MainMenuScene::updateScene(double timeInterval) {}

    void MainMenuScene::renderUI() {

        // Set configurations of the host window of the scene on initial render pass.
        if (!m_isMainMenuDockspaceInitialized) {
            m_mainViewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(m_mainViewport->WorkPos);
            ImGui::SetNextWindowSize(m_mainViewport->WorkSize);
            ImGui::SetNextWindowViewport(m_mainViewport->ID);
        }

        // Render the host window.
        ImGui::Begin("Host Window", nullptr, m_hostWindowFlags);

        // Create the dockplace settings and layout on initial render pass.
        if (!m_isMainMenuDockspaceInitialized) {

            m_mainMenuDockspaceID = ImGui::GetID("Main Menu Dockspace");
            ImGui::DockBuilderRemoveNode(m_mainMenuDockspaceID);

            // Add dockspace nodes.
            ImGui::DockBuilderAddNode(m_mainMenuDockspaceID, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(m_mainMenuDockspaceID, m_mainViewport->WorkSize);

            // Configure dockspace layout.
            ImGuiID bottom{ m_mainMenuDockspaceID };
            ImGui::DockBuilderSplitNode(m_mainMenuDockspaceID, ImGuiDir_Up, 0.60f, &m_mainMenuTitleWindowID, &bottom);
            ImGui::DockBuilderSplitNode(bottom, ImGuiDir_Left, 0.33f, &m_encounterCreationWindowID, &m_encounterRunningWindowID);
            ImGui::DockBuilderSplitNode(m_encounterRunningWindowID, ImGuiDir_Right, 0.50f, &m_completedEncountersWindowID, &m_encounterRunningWindowID);
       
            // Dock windows.
            ImGui::DockBuilderDockWindow("CombatTracker", m_mainMenuTitleWindowID);
            ImGui::DockBuilderDockWindow("Encounter Creation", m_encounterCreationWindowID);
            ImGui::DockBuilderDockWindow("Run Encounter", m_encounterRunningWindowID);
            ImGui::DockBuilderDockWindow("View Completed Encounters", m_completedEncountersWindowID);
       
            // Finish dockspace creation.
            ImGui::DockBuilderFinish(m_mainMenuDockspaceID);
            m_isMainMenuDockspaceInitialized = true;

        }
        // Render the dockspace of the host window.
        ImGui::DockSpace(m_mainMenuDockspaceID, ImGui::GetWindowSize(), m_mainMenuDockspaceFlags);
        ImGui::End();

        // Title window.
        ImGui::SetNextWindowClass(&m_mainMenuWindowClass);
        ImGui::Begin("CombatTracker");
        ImGui::Image(static_cast<ImTextureID>(m_titleTexture.getTextureID()), ImVec2(m_titleTexture.getTextureWidth(), m_titleTexture.getTextureHeight()));
        ImGui::End();

        // Encounter Creation window.
        ImGui::Begin("Encounter Creation");
        ImGui::TextWrapped("Creature Management");
        ImGui::Bullet(); ImGui::TextWrapped("Create and save monsters and player characters.");
        ImGui::Bullet(); ImGui::TextWrapped("Browse and modify your saved monsters and player characters through the compendium.");
        ImGui::Bullet(); ImGui::TextWrapped("In addition, browse premade monsters and player characters through the compendium.");
        ImGui::TextWrapped("Encounter Management");
        ImGui::Bullet(); ImGui::TextWrapped("Create and save encounters from the giant list of creatures.");
        ImGui::Bullet(); ImGui::TextWrapped("Browse and modify your saved encounters through the compendium.");
        ImGui::Bullet(); ImGui::TextWrapped("In addition, browse premade encounters using premade creatures through the compendium.");
        ImGui::Bullet(); ImGui::TextWrapped("Attach a battlemap texture to created encounters for better visualization.");
        if (CombatTrackerLib::alignedButton("Create Encounter", 0.5f)) {
            m_applicationReference->queueSceneTransition(this, std::make_unique<EncounterCreationScene>(m_applicationReference));
        }
        ImGui::End();

        // Run Encounter window.
        ImGui::Begin("Run Encounter");
        ImGui::TextWrapped("Preperation Phase");
        ImGui::Bullet(); ImGui::TextWrapped("After selecting an encounter, roll or set initiative.");
        ImGui::Bullet(); ImGui::TextWrapped("Place creatures in their initial positions in either the associated battlemap or an empty-premade battlemap.");
        ImGui::TextWrapped("Running Encounters");
        ImGui::Bullet(); ImGui::TextWrapped("Progress encounters in a turn-by-turn basis.");
        ImGui::Bullet(); ImGui::TextWrapped("Select which actions each creature plays in a turn.");
        ImGui::Bullet(); ImGui::TextWrapped("Move each creature in the battlemap with both 2D or 3D visualization options.");
        ImGui::TextWrapped("Finishing Encounters");
        ImGui::Bullet(); ImGui::TextWrapped("Finish the current encounter and save the results.");
        if (CombatTrackerLib::alignedButton("Select Encounter", 0.5f)) {
        
        }
        ImGui::End();

        // Completed Encounters window.
        ImGui::Begin("View Completed Encounters");
        ImGui::Bullet(); ImGui::TextWrapped("Browse the list of your completed encounters and analyze their results.");
        ImGui::Bullet(); ImGui::TextWrapped("Remember what happened exactly in each turn.");
        if (CombatTrackerLib::alignedButton("Browse Encounters", 0.5f)) {
        
        }
        ImGui::End();
    }
	void MainMenuScene::renderScene() {}
}
