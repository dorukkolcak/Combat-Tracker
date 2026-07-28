#pragma once

#include <algorithm>
#include <array>
#include <string>
#include <unordered_map>

#include "../../CombatTrackerLib/Utilities/UIUtils.h"
#include "../../CombatTrackerLib/Scene.h"

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
		};
		enum class EntitySource {
			ALL,
			USER_CREATED,
			COMPENDIUM
		};

		/*
		Constant UI labels. These should be static as they shouldn't be instantiated per object/scene.
		The reason the type is string not string_view for most of the UI labels in this class is that I frequently do concatenation with them.
		So I need an underlying string type to work with, which is annoying to access with a string_view.
		Probably can make it work at some point and be more efficient but this is fine for now.
		*/
		inline static const std::unordered_map<WindowType, std::string> MAPPED_WINDOW_NAMES{
			{WindowType::ENCOUNTER_CREATOR, "Encounter Creator"},
			{WindowType::ENCOUNTER_MANAGER, "Encounter Manager"},
			{WindowType::MENU, "Menu"},
			{WindowType::ENTITY_BROWSER, "Entity Browser"},
			{WindowType::MONSTER_BROWSER, "Monster Browser"},
			{WindowType::PLAYER_CHARACTER_BROWSER, "Player Character Browser"},
			{WindowType::ENCOUNTER_BROWSER, "Encounter Browser"}
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
		inline static constexpr ImGuiDockNodeFlags ENCOUNTER_CREATION_DOCKSPACE_FLAGS{
			ImGuiDockNodeFlags_NoResize |
			ImGuiDockNodeFlags_NoSplit
		};

		inline static const std::array<std::string, 2> MONSTER_BROWSER_COLUMN_LABELS {
			"CR",
			"XP"
		};
		inline static const std::array<std::string, 3> PLAYER_CHARACTER_BROWSER_COLUMN_LABELS {
			"Level",
			"Species",
			"Origin"
		};

		ImGuiID m_encounterCreatorDockspaceID{};
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

		bool m_isEncounterCreatorDockspaceInitialized{};

		void renderHostWindow();

		/*
		Renders the filter row above a listing. Shared by all three browser tabs.
		The distinction between browser tabs is made via EntitySource parameter.
		Should be passed a non-const lvalue reference as we change it inside the function.
		*/
		void renderBrowserFilters(EntitySource& source, std::string& searchQuery, const char* label);
		void renderEntityBrowserHostWindow();

		template<size_t N, typename S>
		void renderEntityBrowserWindow(
			const std::string& typeLabel,
			const std::array<std::string, N>& columnLabels,
			EntitySource& source,
			std::string& searchQuery,
			int& selectionIndex
		);

		void renderEncounterManagerWindow();

	};

	// TODO: Finish this function when JSON functionality is implemented.
	template<size_t N, typename S>
	void EncounterCreationScene::renderEntityBrowserWindow(
		const std::string& typeLabel,
		const std::array<std::string, N>& columnLabels,
		EntitySource& source,
		std::string& searchQuery,
		int& selectionIndex
	) {
		renderBrowserFilters(source, searchQuery, typeLabel.c_str());

		/*
		Split what is left of the tab between the entity listings and the details panel.
		A scrolling table with no explicit height occupies every remaining pixel, which pushes the "functionality" buttons
		out of the tab as soon as the window gets shorter. So both regions get a computed height.
		*/
		const auto functionalityButtonsHeight{ CombatTrackerLib::reservedFooterHeight(1) };
		const auto createRowHeight{ ImGui::GetFrameHeightWithSpacing() };
		const auto bodyHeight{ ImGui::GetContentRegionAvail().y - createRowHeight - functionalityButtonsHeight };
		const auto listingHeight{ bodyHeight * 0.55f }; // Cpnsider playing around with the multiplier.

		if (ImGui::BeginTable(
			(typeLabel + " Listing").c_str(),
			static_cast<int>(N) + 2,
			BROWSER_TABLE_FLAGS,
			ImVec2{ 0.0f, listingHeight }
		)) {
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, 0.30f);

			for (size_t i{}; i < N; ++i) {
				ImGui::TableSetupColumn(columnLabels[i].c_str(), ImGuiTableColumnFlags_WidthStretch, 0.6f / static_cast<float>(N));
			}
			ImGui::TableSetupColumn("Source", ImGuiTableColumnFlags_WidthStretch, 0.10f);

			ImGui::TableSetupScrollFreeze(0, 1);
			ImGui::TableHeadersRow();

			ImGui::EndTable();
		}

		if (CombatTrackerLib::alignedButton(("Create " + typeLabel).c_str(), 0.5f)) {
			m_applicationReference->queueSceneTransition(this, std::make_unique<S>(m_applicationReference));
		}

		/*
		The negative height keeps the action row below the panel visible at any window size.
		The calculation is done by subtracting the absolute value of the negative height from the available height.
		So these buttons span the remaining height, which is exactly what we want since they are the last UI elements to be rendered.
		*/
		ImGui::BeginChild((typeLabel + " Details").c_str(), ImVec2{ 0.0f, -functionalityButtonsHeight });
		if (selectionIndex < 0) {
			ImGui::TextWrapped("Select a %s to see its details.", typeLabel.c_str());
		}
		ImGui::EndChild();

		ImGui::BeginDisabled(selectionIndex < 0);
		if (ImGui::Button("Add to Encounter")) {

		}
		ImGui::SameLine();
		if (ImGui::Button(("Edit " + typeLabel).c_str())) {

		}
		ImGui::SameLine();
		if (ImGui::Button(("Delete " + typeLabel).c_str())) {

		}
		ImGui::EndDisabled();
	}
}
