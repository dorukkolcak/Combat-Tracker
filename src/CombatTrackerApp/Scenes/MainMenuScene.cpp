#include "MainMenuScene.h"
#include "EncounterCreationScene.h"
#include "../CombatTrackerLib/Utilities/UIUtils.h"

#include "imgui.h"

namespace CombatTrackerApp {

    MainMenuScene::MainMenuScene(CombatTrackerLib::App* app) :
        m_titleTexture{ "assets/Title.png" },
        CombatTrackerLib::Scene{ app },
        m_isMainMenuDockspaceInitialized{ false },
        m_UIThemeParameters{ -1, -1, false },
        m_chosenUIPreset{ m_applicationReference->getUITheme().preset },
        m_selectedOptionsCategory{}
	{   
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

        // Render the host window.
        ImGui::Begin("Host Window", nullptr, MAIN_MENU_HOST_WINDOW_FLAGS);

        // Render the dockspace of the host window.
        ImGui::DockSpace(m_mainMenuDockspaceID, ImGui::GetWindowSize(), MAIN_MENU_DOCKSPACE_FLAGS);
        ImGui::End();

        // Title window.
        ImGui::SetNextWindowClass(&m_mainMenuWindowClass);
        ImGui::Begin("CombatTracker");
        if (CombatTrackerLib::alignedButton("Options", 0.96f)) {
            ImGui::OpenPopup("Options Popup");
        }
        ImGui::SameLine();
        if (ImGui::Button("Exit")) {

            // This signal should be deferred and handled by the application instance at the start of the frame.
            m_applicationReference->signalApplicationClose();
        }

        // Tinting with a style colour makes the logo follow the active UI preset.
        // So we don't need a title asset for every UI Preset.
        CombatTrackerLib::centeredImage(m_titleTexture, ImGui::GetStyle().Colors[ImGuiCol_Text]);

        // Options popup for application-wide settings.
        // TODO: One thing to do is actually save these settings somewhere so they are preserved between runs.
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(640.0f, 420.0f), ImGuiCond_Appearing);

        // The title bar is hidden so the panel can draw its own header.
        if (ImGui::BeginPopupModal("Options Popup", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove)) {

            ImGui::PushFont(nullptr, ImGui::GetStyle().FontSizeBase * 1.35f);
            ImGui::TextUnformatted("Options");
            ImGui::PopFont();
            ImGui::Separator();

            const auto footerHeight{ ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y };
            const auto bodyHeight{ ImGui::GetContentRegionAvail().y - footerHeight };

            // Category list.
            ImGui::BeginChild("Options Categories", ImVec2(150.0f, bodyHeight), ImGuiChildFlags_Borders);
            for (const auto& optionsCategory : OPTIONS_CATEGORY_NAMES) {
                if (ImGui::Selectable(optionsCategory.second.data(), m_selectedOptionsCategory == optionsCategory.first)) {
                    m_selectedOptionsCategory = optionsCategory.first;
                }
            }
            ImGui::EndChild();

            ImGui::SameLine();

            // Controls belonging to the selected category.
            ImGui::BeginChild("Options Content", ImVec2(0.0f, bodyHeight), ImGuiChildFlags_Borders);
            if (m_selectedOptionsCategory == OptionsCategory::Appearance) {
                ImGui::SeparatorText("Theme");
                auto& applicationUITheme{ m_applicationReference->getUITheme() };
                CombatTrackerLib::renderListBox(m_UIThemeParameters, m_chosenUIPreset, "Choose Theme", CombatTrackerLib::PRESET_NAMES);
                applicationUITheme.setUITheme(m_chosenUIPreset); // This function early returns already so this is fine.
            }
            // TODO: Update this when more categories get added.

            ImGui::EndChild();

            ImGui::Separator();

            // A modal ignores both outside clicks and Escape, so this is the only way out.
            if (CombatTrackerLib::alignedButton("Close", 1.0f)) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

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
        
        // TODO: Complete the scene transition.
        if (CombatTrackerLib::alignedButton("Select Encounter", 0.5f)) {
           
        }
        ImGui::End();

        // Completed Encounters window.
        ImGui::Begin("View Completed Encounters");
        ImGui::Bullet(); ImGui::TextWrapped("Browse the list of your completed encounters and analyze their results.");
        ImGui::Bullet(); ImGui::TextWrapped("Remember what happened exactly in each turn.");

        // TODO: Complete the scene transition.
        if (CombatTrackerLib::alignedButton("Browse Encounters", 0.5f)) {
        
        }
        ImGui::End();

    }
	void MainMenuScene::renderScene() {}
}
