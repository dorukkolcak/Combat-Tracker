#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <utility>

#include "../../CombatTrackerLib/Utils.h"
#include "../../CombatTrackerLib/Scene.h"
#include "../Entities/Creature/CreatureEnums.h"
#include "../Entities/Creature/CreatureTypes.h"
#include "../Entities/Creature/Creature.h"
#include "../Entities/Monster/Monster.h"
#include "../Entities/PlayerCharacter/PlayerCharacter.h"

#include "misc/cpp/imgui_stdlib.h" // std::string support

namespace CombatTrackerApp {
	class EncounterCreationScene : public CombatTrackerLib::Scene {
	public:

		explicit EncounterCreationScene(CombatTrackerLib::App* app);
		virtual ~EncounterCreationScene();

		virtual void notifyScene(const ImGuiIO& imguiIO) override;
		virtual void updateScene(double timeInterval) override;
		virtual void renderUI() override;
		virtual void renderScene() override;
	private:
		
		// Ordered from high level windows to low level windows. 
		enum class Subwindow {
			ENCOUNTER_CREATOR,
			MENU,
			ENTITY_BROWSER,
			ENCOUNTER_MANAGER,
			MONSTER_BROWSER,
			PLAYER_CHARACTER_BROWSER,
			ENCOUNTER_BROWSER,
		};

		// Consider changing this struct to be a namespace-wide type if other UI components need these parameters.
		struct ListBoxParameters {
			int chosenIndex{};
			int highlightedIndex{};
			bool isHighlighted{};
		};

		ImGuiWindowFlags m_encounterCreationWindowFlags{};
		ImGuiDockNodeFlags m_encounterCreationDockFlags{};
		ImGuiWindowFlags m_popupWindowFlags{};

		std::unordered_map<Subwindow, ImGuiID> m_dockspaceIDs{};
		std::unordered_map<Subwindow, bool> m_dockspaceInitializations {
			{Subwindow::ENCOUNTER_CREATOR, false},
			{Subwindow::ENTITY_BROWSER, false},
			{Subwindow::MONSTER_BROWSER, false},
			{Subwindow::PLAYER_CHARACTER_BROWSER, false},
			{Subwindow::ENCOUNTER_BROWSER, false}
		};

		std::string m_currentPopupName{};
		MonsterBuilder m_monsterBuilder{};
		PlayerCharacterBuilder m_playerCharacterBuilder{};

		CreatureStatBlock& m_currentCreatureStatBlock{ m_monsterBuilder.getCreatureBlock() };
		PlayerStatBlock& m_currentPlayerStatBlock{ m_playerCharacterBuilder.getPlayerCharacterBlock() };

		double m_currentMonsterCR{};
		int m_currentMonsterXP{};

		ListBoxParameters m_monsterCRParameters{};
		ListBoxParameters m_sizeListBoxParameters{};
		ListBoxParameters m_creatureTypeParameters{};
		ListBoxParameters m_alignmentParameters{};
		ListBoxParameters m_speciesParameters{};
		ListBoxParameters m_originParameters{};

		bool m_isInitiativeOverridden{};
		bool m_isPassivePerceptionOverridden{};
		bool m_isXPOverridden{};
		bool m_isPBOverridden{};

		std::unordered_map<Ability, bool> m_abilityModifiersOverride{};
		std::unordered_map<Ability, bool> m_savingThrowsOverride{};
		std::unordered_map<Skill, bool> m_skillModifiersOverride{};

		std::unordered_map<PlayerSubclass, bool> m_chosenSubclasses{
			{PlayerSubclass::PATH_OF_THE_BERSERKER, false},
			{PlayerSubclass::PATH_OF_THE_WILD_HEART, false},
			{PlayerSubclass::PATH_OF_THE_WORLD_TREE, false},
			{PlayerSubclass::PATH_OF_THE_ZEALOT, false},
			{PlayerSubclass::COLLEGE_OF_DANCE, false},
			{PlayerSubclass::COLLEGE_OF_GLAMOUR, false},
			{PlayerSubclass::COLLEGE_OF_LORE, false},
			{PlayerSubclass::COLLEGE_OF_VALOR, false},
			{PlayerSubclass::LIFE_DOMAIN, false},
			{PlayerSubclass::LIGHT_DOMAIN, false},
			{PlayerSubclass::TRICKERY_DOMAIN, false},
			{PlayerSubclass::WAR_DOMAIN, false},
			{PlayerSubclass::CIRCLE_OF_THE_LAND, false},
			{PlayerSubclass::CIRCLE_OF_THE_MOON, false},
			{PlayerSubclass::CIRCLE_OF_THE_SEA, false},
			{PlayerSubclass::CIRCLE_OF_THE_STARS, false},
			{PlayerSubclass::BATTLE_MASTER, false},
			{PlayerSubclass::CHAMPION, false},
			{PlayerSubclass::ELDRITCH_KNIGHT, false},
			{PlayerSubclass::PSI_WARRIOR, false},
			{PlayerSubclass::WARRIOR_OF_MERCY, false},
			{PlayerSubclass::WARRIOR_OF_SHADOW, false},
			{PlayerSubclass::WARRIOR_OF_THE_ELEMENTS, false},
			{PlayerSubclass::WARRIOR_OF_THE_OPEN_HAND, false},
			{PlayerSubclass::OATH_OF_DEVOTION, false},
			{PlayerSubclass::OATH_OF_GLORY, false},
			{PlayerSubclass::OATH_OF_THE_ANCIENTS, false},
			{PlayerSubclass::OATH_OF_VENGEANCE, false},
			{PlayerSubclass::BEAST_MASTER, false},
			{PlayerSubclass::FEY_WANDERER, false},
			{PlayerSubclass::GLOOM_STALKER, false},
			{PlayerSubclass::HUNTER, false},
			{PlayerSubclass::ARCANE_TRICKSTER, false},
			{PlayerSubclass::ASSASSIN, false},
			{PlayerSubclass::SOULKNIFE, false},
			{PlayerSubclass::THIEF, false},
			{PlayerSubclass::ABERRANT_SORCERY, false},
			{PlayerSubclass::CLOCKWORK_SORCERY, false},
			{PlayerSubclass::DRACONIC_SORCERY, false},
			{PlayerSubclass::WILD_MAGIC_SORCERY, false},
			{PlayerSubclass::ARCHFEY_PATRON, false},
			{PlayerSubclass::CELESTIAL_PATRON, false},
			{PlayerSubclass::FIEND_PATRON, false},
			{PlayerSubclass::GREAT_OLD_ONE_PATRON, false},
			{PlayerSubclass::ABJURER, false},
			{PlayerSubclass::DIVINER, false},
			{PlayerSubclass::EVOKER, false},
			{PlayerSubclass::ILLUSIONIST, false}
		};
		std::unordered_map<PlayerClass, bool> m_hasCorrespondingSubclass{
			{PlayerClass::BARBARIAN, false},
			{PlayerClass::BARD, false},
			{PlayerClass::CLERIC, false},
			{PlayerClass::DRUID, false},
			{PlayerClass::FIGHTER, false},
			{PlayerClass::MONK, false},
			{PlayerClass::PALADIN, false},
			{PlayerClass::RANGER, false},
			{PlayerClass::ROGUE, false},
			{PlayerClass::SORCERER, false},
			{PlayerClass::WARLOCK, false},
			{PlayerClass::WIZARD, false}
		};

		
		// Constant UI labels. These should be static as they shouldn't be instantiated per object/scene.
		inline static const std::unordered_map<Subwindow, std::string> MAPPED_WINDOW_NAMES{
			{Subwindow::ENCOUNTER_CREATOR, "Encounter Creator"},
			{Subwindow::ENCOUNTER_MANAGER, "Encounter Manager"},
			{Subwindow::MENU, "Menu"},
			{Subwindow::ENTITY_BROWSER, "Entity Browser"},
			{Subwindow::MONSTER_BROWSER, "Monster Browser"},
			{Subwindow::PLAYER_CHARACTER_BROWSER, "Player Character Browser"},
			{Subwindow::ENCOUNTER_BROWSER, "Encounter Browser"}
		};
		inline static const std::string PROFICIENCY_SUFFIX{ " Proficiency" };
		inline static const std::string EXPERTISE_SUFFIX{ " Expertise" };
		inline static const std::string TRAIT_LABEL{ "Trait" };
		inline static const std::string EQUIPMENT_LABEL{ "Gear" };
		inline static constexpr float MIN_CHILD_HEIGHT{ 180.f };

		void renderHostWindow();
		void renderMenuWindow();
		void renderEntityBrowserWindow();
		void renderMonsterBrowserWindow();
		void renderPlayerCharacterBrowserWindow();
		void renderEncounterBrowserWindow();
		void renderEncounterManagerWindow();

		void renderEntityCreationPopupModal(bool isMonster, bool editMode);

		void renderGeneralCreatureInfo();
		void renderSpeedInfo();

		// Proficiency and expertise items get changed in this function so pass them as non-const lvalue reference.
		template <typename M, typename N>
		void renderProficiencyAndExpertiseCluster(
			const char* proficiencyLabel, 
			const char* expertiseLabel,
			M& proficiencyItems,
			M& expertiseItems,
			const N& itemNames
		);
		void renderAbilityScoresAndModifiers();
		void renderSavingThrowModifiers();
		void renderSkillModifiers();
		void renderDmgVulnResAndImm();

		template <typename M, typename N>
		void renderMutuallyExclusiveCheckboxCluster(M& items, const N& names, const char* label);
		void renderSenses();

		template <typename I>
		void renderAddables(std::vector<I>& currentItems, const std::string& label);
		void renderActions(bool isMonster);
		void renderMonsterSpecificInfo();
		void renderPlayerClassAndSubclasses();
		void renderPlayerCharacterSpecificInfo();
		void initializeSubBrowserDockspace(Subwindow windowType);

		// We change the value of the type in this function so pass it as non-const lvalue reference.
		template <typename T, typename M>
		void renderListBox(ListBoxParameters& parameters, T& type, const char* listBoxLabel, const M& items);
		void renderNonOverridableInputInt(int& value, const char* label);

		// Pass the value as non-const lvalue reference as we need to modify its value in this function.
		template <typename T, typename M>
		void renderOverriddableInputInt (
			int& value,
			bool& overrideFlag,
			const char* label,
			const char* overrideLabel,
			const M& items,
			const int lowerBound,
			const int upperBound,
			const T key
		);
		void resetParametersAndOverrides();
	};

	// Generic helper for single selectable listbox layouts.
	// Used for creature type, alignment, size, monster CR, action type during action creation.
	template <typename T, typename M>
	void EncounterCreationScene::renderListBox(
		ListBoxParameters& parameters,
		T& type,
		const char* listBoxLabel,
		const M& items
	) {
		int counter{};
		const float height{ ImGui::GetTextLineHeightWithSpacing() * 8.0f };
		if (ImGui::BeginListBox(listBoxLabel, ImVec2{ 0, height })) {
			for (const auto& item : items) {
				const bool isSelected{ counter == parameters.chosenIndex };
				if (ImGui::Selectable(item.second.c_str(), isSelected)) {
					parameters.chosenIndex = counter;
					type = item.first;
				}
				if (parameters.isHighlighted && ImGui::IsItemHovered()) {
					parameters.highlightedIndex = counter;
				}
				++counter;
			}
			ImGui::EndListBox();
		}
	}

	/*
	A slightly more complicated helper used for user input of monster XP and PB.
	The common thing about these two types is that they set mapped values.
	Various other overridable user input can also be put in this format.
	But mapping these types is meaningless as their mapping is quite simple.
	Example: Initiative Bonus: Dexterity Modifier, Passive Perception: Perception Modifier + 10
	*/
	template <typename T, typename M>
	void EncounterCreationScene::renderOverriddableInputInt(
		int& value,
		bool& overrideFlag,
		const char* label,
		const char* overrideLabel,
		const M& items,
		const int lowerBound,
		const int upperBound,
		const T key
	) {
		if (!overrideFlag) {
			value = items.at(key);
		}
		if (ImGui::InputInt(label, &value)) {
			value = std::min(std::max(lowerBound, value), upperBound);
			overrideFlag = true;
		}
		ImGui::SameLine();
		if (ImGui::Button(overrideLabel)) {
			overrideFlag = false;
		}
	}

	/*
	Used for rendering skill and saving throw proficiencies and expertises for both monsters and player characters.
	The two clusters have a simple deduping logic between them.
	We can't have proficiency and expertise of the same "thing" at the same time.
	*/
	template <typename M, typename N>
	void EncounterCreationScene::renderProficiencyAndExpertiseCluster(
		const char* proficiencyLabel,
		const char* expertiseLabel,
		M& proficiencyItems,
		M& expertiseItems,
		const N& itemNames
	) {
		if (ImGui::BeginTable("ProfExpTable", 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV)) {

			ImGui::TableSetupColumn("Left");
			ImGui::TableSetupColumn("Right");
			ImGui::TableNextRow();

			// Proficiency checkboxes.
			ImGui::TableSetColumnIndex(0);
			ImGui::TextWrapped("%s", proficiencyLabel);
			for (const auto& item : itemNames) {
				if (ImGui::Checkbox((item.second + "##" + PROFICIENCY_SUFFIX).c_str(), &proficiencyItems.at(item.first))) {
					if (proficiencyItems.at(item.first) && expertiseItems.at(item.first)) {
						expertiseItems.at(item.first) = false;
					}
				}
			}

			// Expertise checkboxes.
			ImGui::TableSetColumnIndex(1);
			ImGui::TextWrapped("%s", expertiseLabel);
			for (const auto& item : itemNames) {
				if (ImGui::Checkbox((item.second + "##" + EXPERTISE_SUFFIX).c_str(), &expertiseItems.at(item.first))) {
					if (expertiseItems.at(item.first) && proficiencyItems.at(item.first)) {
						proficiencyItems.at(item.first) = false;
					}
				}
			}
			ImGui::EndTable();
		}
	}

	// Simple helper method for rendering multi-selectable creature parameters.
	// Used in condition immunities and languages.
	template <typename M, typename N>
	void EncounterCreationScene::renderMutuallyExclusiveCheckboxCluster(M& items, const N& names, const char* label) {
		ImGui::TextWrapped("%s", label);
		if (ImGui::BeginTable(label, 3, ImGuiTableFlags_SizingStretchSame)) {
			int counter{};
			for (const auto& item : names) {
				ImGui::TableNextColumn();
				ImGui::Checkbox(item.second.c_str(), &items.at(item.first));
				++counter;
			}
			ImGui::EndTable();
		}
	}

	// Chunky helper used for rendering traits and gear items.
	template <typename I>
	void EncounterCreationScene::renderAddables(std::vector<I>& currentItems, const std::string& label) {
		ImGui::TextWrapped("%s", label.c_str());

		// It is fine for this to be actually static as we clear it on each render pass anyways.
		// Furthermore, this vector is only used in this function and nowhere else.
		static std::vector<size_t> s_markedForDeletion{};
		for (size_t i{}; i < currentItems.size(); ++i) {
			ImGui::TableNextColumn();

			// Cache the string to prevent recomputing it.
			std::string popupName{ ("Edit " + label + "##" + std::to_string(i)).c_str() };

			// Push a separate id to the ID stack for each item.
			ImGui::PushID(static_cast<int>(i));

			// We should check separate IO events explicitly instead of generically checking for any IO events for these buttons.
			// Unlike most of the widgets. This is because we need a tooltip for each button.
			ImGui::Button(currentItems[i].name.c_str());
			ImGui::SameLine();
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				ImGui::SetTooltip("%s", currentItems[i].description.c_str());
			}
			if (ImGui::IsItemClicked()) {
				ImGui::OpenPopup(popupName.c_str());
			}

			// Setup configs of the edit popup of items.
			const ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2{ 0.5f, 0.5f });
			const ImVec2 vp{ ImGui::GetMainViewport()->Size };
			ImGui::SetNextWindowSize(ImVec2{ vp.x * 0.70f, vp.y * 0.70f }, ImGuiCond_Appearing);

			// Popup modal for editing/deleting items.				
			if (ImGui::BeginPopupModal(popupName.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {

				// The layouting is quite similar to the big entity creation popup modal.
				ImGui::BeginChild("EditAddableScroll", ImVec2{ 0, -ImGui::GetFrameHeightWithSpacing() * 2.0f }, true);
				ImGui::InputText((label + " Name").c_str(), &currentItems[i].name);
				ImGui::InputText((label + " Description").c_str(), &currentItems[i].description);
				ImGui::EndChild();

				ImGui::Separator();
				if (CombatTrackerLib::alignedButton(("Finish " + label + " Editing").c_str(), 0.5f)) {
					ImGui::CloseCurrentPopup();
				}
				if (CombatTrackerLib::alignedButton(("Delete " + label).c_str(), 0.5f)) {
					s_markedForDeletion.push_back(i);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}

		// Delete marked items safely, starting from the largest index to the smallest index.
		std::sort(s_markedForDeletion.begin(), s_markedForDeletion.end());
		for (size_t i = s_markedForDeletion.size(); i-- > 0;) {
			currentItems.erase(currentItems.begin() + s_markedForDeletion[i]);
		}
		s_markedForDeletion.clear();

		// Add item section.
		ImGui::Spacing();
		if (ImGui::Button(("Add " + label).c_str())) {
			ImGui::OpenPopup(label.c_str());
		}

		// Setup configs of the add item popup.
		ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2{ 0.5f, 0.5f });
		ImVec2 vp{ ImGui::GetMainViewport()->Size };
		ImGui::SetNextWindowSize(ImVec2{ vp.x * 0.70f, vp.y * 0.70f }, ImGuiCond_Appearing);
		if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
			ImGui::BeginChild("AddAddableScroll", ImVec2{ 0, -ImGui::GetFrameHeightWithSpacing() * 2.0f }, true);

			// Again it is fine for these to be static, we only use them here and reset them after adding the item.
			static std::string s_name{};
			static std::string s_description{};
			ImGui::InputText((label + " Name").c_str(), &s_name);
			ImGui::InputText((label + " Description").c_str(), &s_description);
			ImGui::EndChild();

			// Button and cleanup section.
			ImGui::Separator();
			if (CombatTrackerLib::alignedButton(("Finish " + label + " Creation").c_str(), 0.5f) &&
				!s_name.empty() && !s_description.empty()) {
				I item(s_name, s_description);
				currentItems.push_back(item);
				s_name.clear();
				s_description.clear();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}