#pragma once

#include "CombatTrackerLib/Utilities/UIUtils.h"
#include "CombatTrackerLib/Scene.h"

#include <span>
#include <string>
#include <string_view>

namespace CombatTrackerApp {
	class EncounterCreationScene : public CombatTrackerLib::Scene {
	public:

		explicit EncounterCreationScene(CombatTrackerLib::App* app);

		void notifyScene(const ImGuiIO& imguiIO) override;
		void updateScene(double timeInterval) override;
		void renderUI() override;
		void renderScene() override;
	private:

		enum class WindowType {
			ENCOUNTER_CREATOR,
			MENU,
			ENTITY_BROWSER,
			ENCOUNTER_MANAGER,
			MONSTER_BROWSER,
			PLAYER_CHARACTER_BROWSER,
			ENCOUNTER_BROWSER,
			COUNT,
		};
		enum class EntitySource {
			ALL,
			USER_CREATED,
			COMPENDIUM
		};

		struct BrowserLabels {
			std::string_view create{};
			std::string_view edit{};
			std::string_view remove{};
			std::string_view emptyDetails{};
			std::string_view stableID{};
		};

		// These flags are shared between browser tabs.
		inline static constexpr ImGuiTableFlags BROWSER_TABLE_FLAGS{
			ImGuiTableFlags_RowBg |
			ImGuiTableFlags_BordersInnerV |
			ImGuiTableFlags_ScrollY |
			ImGuiTableFlags_Sortable |
			ImGuiTableFlags_SizingStretchProp
		};
		inline static constexpr ImGuiWindowFlags ENCOUNTER_CREATION_WINDOW_FLAGS{
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_MenuBar // This flag allows us to append a menu bar to this window by immediately calling the appropriate ImGui function.
		};
		inline static constexpr ImGuiDockNodeFlags DOCKSPACE_FLAGS{
			ImGuiDockNodeFlags_NoResize |
			ImGuiDockNodeFlags_NoSplit
		};

		ImGuiID m_dockspaceID{};
		ImGuiID m_entityBrowserWindowID{};
		ImGuiID m_encounterManagerWindowID{};

		// Filter, search and selection state of the three browser tabs.
		EntitySource m_monsterSource{};
		EntitySource m_playerCharacterSource{};
		std::string m_monsterSearchQuery{};
		std::string m_playerCharacterSearchQuery{};

		// Negative means nothing is selected, so the details panel stays empty.
		int m_selectedMonsterIndex{-1};
		int m_selectedPlayerCharacterIndex{-1};

		bool m_isdockspaceInitialized{};

		std::string_view getWindowName(WindowType windowType) const;

		void renderHostWindow();

		/*
		Renders the filter row above a listing. Shared by all three browser tabs.
		The distinction between browser tabs is made via EntitySource parameter.
		Should be passed a non-const lvalue reference as we change it inside the function.
		*/
		void renderBrowserFilters(EntitySource& source, std::string& searchQuery, const char* label);
		void renderEntityBrowserHostWindow();

		template<typename S>
		void renderEntityBrowserWindow(
			const BrowserLabels& labels,
			std::span<const std::string_view> columnLabels,
			EntitySource& source,
			std::string& searchQuery,
			int& selectionIndex
		);
		void renderEncounterManagerWindow();
	};

	// TODO: Finish this function when JSON functionality is implemented.
	template<typename S>
	void EncounterCreationScene::renderEntityBrowserWindow(
		const BrowserLabels& labels,
		std::span<const std::string_view> columnLabels,
		EntitySource& source,
		std::string& searchQuery,
		int& selectionIndex
	) {
		ImGui::PushID(labels.stableID.data());
		renderBrowserFilters(source, searchQuery, labels.stableID.data());

		/*
		Split what is left of the tab between the entity listings and the details panel.
		A scrolling table with no explicit height occupies every remaining pixel, which pushes the "functionality" buttons
		out of the tab as soon as the window gets shorter. So both regions get a computed height.
		*/
		const auto functionalityButtonsHeight{ CombatTrackerLib::reservedFooterHeight(1) };
		const auto createRowHeight{ ImGui::GetFrameHeightWithSpacing() };
		const auto bodyHeight{ ImGui::GetContentRegionAvail().y - createRowHeight - functionalityButtonsHeight };
		const auto listingHeight{ bodyHeight * 0.55f }; // Consider playing around with the multiplier.

		const auto columnCount{ columnLabels.size() };
		if (ImGui::BeginTable(
			"Listing",
			static_cast<int>(columnCount) + 2,
			BROWSER_TABLE_FLAGS,
			ImVec2{ 0.0f, listingHeight }
		)) {

			// Column setup.
			ImGui::TableSetupColumn(
				m_localeProviderRef.getText("/scenes/encounterCreation/browser/nameColumn").data(),
				ImGuiTableColumnFlags_WidthStretch, 
				0.30f
			);
			for (const auto& columnLabel : columnLabels) {
				ImGui::TableSetupColumn(
					columnLabel.data(),
					ImGuiTableColumnFlags_WidthStretch,
					0.6f / static_cast<float>(columnCount)
				);
			}
			ImGui::TableSetupColumn(
				m_localeProviderRef.getText("/scenes/encounterCreation/browser/sourceColumn").data(), 
				ImGuiTableColumnFlags_WidthStretch, 
				0.10f
			);
			ImGui::TableSetupScrollFreeze(0, 1);
			ImGui::TableHeadersRow();
			ImGui::EndTable();
		}

		if (CombatTrackerLib::alignedButton(labels.create.data(), 0.5f)) {
			m_applicationPointer->queueSceneTransition(this, std::make_unique<S>(m_applicationPointer));
		}

		/*
		The negative height keeps the action row below the panel visible at any window size.
		The calculation is done by subtracting the absolute value of the negative height from the available height.
		So these buttons span the remaining height, which is exactly what we want since they are the last UI elements to be rendered.
		*/
		ImGui::BeginChild("Details", ImVec2{ 0.0f, -functionalityButtonsHeight });
		if (selectionIndex < 0) {
			ImGui::TextWrapped("%s", labels.emptyDetails.data());
		}
		ImGui::EndChild();

		ImGui::BeginDisabled(selectionIndex < 0);
		if (ImGui::Button(m_localeProviderRef.getText("/scenes/encounterCreation/browser/addToEncounter").data())) {

		}
		ImGui::SameLine();
		if (ImGui::Button(labels.edit.data())) {

		}
		ImGui::SameLine();
		if (ImGui::Button(labels.remove.data())) {

		}
		ImGui::EndDisabled();
		ImGui::PopID();
	}
}
