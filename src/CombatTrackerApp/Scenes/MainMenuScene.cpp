#include "CombatTrackerApp/Scenes/MainMenuScene.h"
#include "CombatTrackerApp/Scenes/EncounterCreationScene.h"
#include "CombatTrackerLib/Utilities/UIUtils.h"

#include <imgui.h>
#include <algorithm>
#include <glad/glad.h>

namespace CombatTrackerApp {

    MainMenuScene::MainMenuScene(CombatTrackerLib::App* app) :
        CombatTrackerLib::Scene{ app },
        m_titleTexture{ "assets/Title.png", 4, false, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR},
        m_UIThemeParameters{ -1, -1, false },
        m_chosenUIPreset{ m_applicationPointer->getUIPreset()},
        m_selectedOptionsCategory{},
        m_isDockspaceInitialized{ false }
	{   
        m_titleWindowClass.DockNodeFlagsOverrideSet =
            ImGuiDockNodeFlags_NoResize |
            ImGuiDockNodeFlags_NoSplit |
            ImGuiDockNodeFlags_NoTabBar;
	}
    void MainMenuScene::notifyScene(const ImGuiIO&) {}
    void MainMenuScene::updateScene(double) {}

    void MainMenuScene::renderUI() {

        // Set configurations of the host window of the scene.
        // This has to happen on every pass and not just the initial one, otherwise the host stays at its old size.
        const auto viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        // Create the dockplace settings and layout on initial render pass.
        if (!m_isDockspaceInitialized) {

            m_dockspaceID = ImGui::GetID("Main Menu Dockspace");
            ImGui::DockBuilderRemoveNode(m_dockspaceID);

            // Add dockspace nodes.
            ImGui::DockBuilderAddNode(m_dockspaceID, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(m_dockspaceID, viewport->WorkSize);

            // Configure dockspace layout.
            ImGuiID bottom{ m_dockspaceID };
            ImGui::DockBuilderSplitNode(m_dockspaceID, ImGuiDir_Up, 0.60f, &m_titleWindowID, &bottom);
            ImGui::DockBuilderSplitNode(bottom, ImGuiDir_Left, 0.33f, &m_encounterCreationWindowID, &m_encounterRunningWindowID);
            ImGui::DockBuilderSplitNode(m_encounterRunningWindowID, ImGuiDir_Right, 0.50f, &m_completedEncountersWindowID, &m_encounterRunningWindowID);

            // Dock windows.
            ImGui::DockBuilderDockWindow("CombatTracker", m_titleWindowID);
            ImGui::DockBuilderDockWindow("Encounter Creation", m_encounterCreationWindowID);
            ImGui::DockBuilderDockWindow("Run Encounter", m_encounterRunningWindowID);
            ImGui::DockBuilderDockWindow("View Completed Encounters", m_completedEncountersWindowID);

            // Finish dockspace creation.
            ImGui::DockBuilderFinish(m_dockspaceID);
            m_isDockspaceInitialized = true;

        }

        // Render the host window. The host window is pure scaffolding, so its padding and border are removed completely.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Host Window", nullptr, HOST_WINDOW_FLAGS);
        ImGui::PopStyleVar(3);

        // Render the dockspace of the host window.
        ImGui::DockSpace(m_dockspaceID, ImGui::GetContentRegionAvail(), DOCKSPACE_FLAGS);
        ImGui::End();

        // Title window.
        ImGui::SetNextWindowClass(&m_titleWindowClass);
        ImGui::Begin("CombatTracker");

        /*
        The two buttons are aligned as one row instead of individually.
        Aligning only the first one pushes the second one off the right edge once the window gets narrow.
        I thought initially this should be a constexpr static variable as the width is set.
        But the ImGui style can change during run time through changing the UI preset so this has to be calculated every render.
        */
        const auto buttonRowWidth{
            CombatTrackerLib::buttonWidth("Options") +
            CombatTrackerLib::buttonWidth("Exit") +
            ImGui::GetStyle().ItemSpacing.x
        };
        CombatTrackerLib::alignCursorForWidth(buttonRowWidth, 1.0f);
        if (ImGui::Button("Options")) {
            ImGui::OpenPopup("Options Popup");
        }
        ImGui::SameLine();
        if (ImGui::Button("Exit")) {

            // This signal should be deferred and handled by the application instance at the start of the frame.
            m_applicationPointer->signalApplicationClose();
        }

        // Tinting with a style colour makes the logo follow the active UI preset.
        // So we don't need a title asset for every UI Preset.
        CombatTrackerLib::centeredImage(m_titleTexture, ImGui::GetStyle().Colors[ImGuiCol_Text]);

        /*
        Options popup for application-wide settings.
        TODO: One thing to do is actually save these settings somewhere so they are preserved between runs.
        */
        const auto viewportSize{ ImGui::GetMainViewport()->Size };
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // Also default to a sensible minimum popup size.
        ImGui::SetNextWindowSize(
            ImVec2(std::min(640.0f, viewportSize.x * 0.80f), std::min(420.0f, viewportSize.y * 0.80f)),
            ImGuiCond_Appearing
        );
        ImGui::SetNextWindowSizeConstraints(ImVec2(std::min(360.0f, viewportSize.x), std::min(240.0f, viewportSize.y)), viewportSize);

        // The title bar is hidden so the panel can draw its own header.
        if (ImGui::BeginPopupModal("Options Popup", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove)) {

            ImGui::PushFont(nullptr, ImGui::GetStyle().FontSizeBase * 1.35f);
            ImGui::TextUnformatted("Options");
            ImGui::PopFont();
            ImGui::Separator();

            // The footer holds a separator and the close button, the rest of the popup contains the two panels.
            const auto footerHeight{ CombatTrackerLib::reservedFooterHeight(1) };
            const auto bodyHeight{ ImGui::GetContentRegionAvail().y - footerHeight };

            // Category list.
            const auto categoryWidth{ std::min(ImGui::GetFontSize() * 9.0f, ImGui::GetContentRegionAvail().x * 0.35f) };
            ImGui::BeginChild("Options Categories", ImVec2(categoryWidth, bodyHeight), ImGuiChildFlags_Borders);
            for (size_t i{}; i < OPTIONS_CATEGORY_NAMES.size(); ++i) {
                const auto category{ static_cast<OptionsCategory>(i) }; // Cast is needed here so that we actually store the enumerator type.
                if (ImGui::Selectable(OPTIONS_CATEGORY_NAMES[i].data(), m_selectedOptionsCategory == category)) {
                    m_selectedOptionsCategory = category;
                }
            }
            ImGui::EndChild();

            ImGui::SameLine();

            // Controls belonging to the selected category.
            ImGui::BeginChild("Options Content", ImVec2(0.0f, bodyHeight), ImGuiChildFlags_Borders);
            if (m_selectedOptionsCategory == OptionsCategory::APPEARANCE) {
                ImGui::SeparatorText("Theme");

                // Hide the label of the listbox and put it above the widget, so the listbox can take the full panel width.
                ImGui::TextUnformatted("Choose Theme");
                ImGui::SetNextItemWidth(-1.0f); // -1.0f passed to the function means that the item spans the entire width.
                CombatTrackerLib::renderListBox(m_UIThemeParameters, m_chosenUIPreset, "##ChooseTheme", CombatTrackerLib::PRESET_NAMES);
                m_applicationPointer->setUIPreset(m_chosenUIPreset); // This function early returns already so this is fine.
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
            m_applicationPointer->queueSceneTransition(this, std::make_unique<EncounterCreationScene>(m_applicationPointer));
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
