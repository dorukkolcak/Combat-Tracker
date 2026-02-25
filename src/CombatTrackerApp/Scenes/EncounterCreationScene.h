#pragma once

#include <string>

#include "../../CombatTrackerLib/Scene.h"
#include "../Entities/Creature/CreatureEnums.h"
#include "../Entities/Creature/CreatureTypes.h"

namespace CombatTrackerApp {

	// Ordered from high level windows to low level windows. 
	enum class EncounterCreationWindows {
		ENCOUNTER_CREATOR,
		MENU,
		ENTITY_BROWSER,
		ENCOUNTER_MANAGER,
		MONSTER_BROWSER,
		PLAYER_CHARACTER_BROWSER,
		ENCOUNTER_BROWSER
	};

	// No need to repeat these, they will exist throughout the entire namespace from this point onwards if the header gets included.
	using EncounterCreationWindows::ENCOUNTER_CREATOR;
	using EncounterCreationWindows::MENU;
	using EncounterCreationWindows::ENTITY_BROWSER;
	using EncounterCreationWindows::ENCOUNTER_MANAGER;
	using EncounterCreationWindows::MONSTER_BROWSER;
	using EncounterCreationWindows::PLAYER_CHARACTER_BROWSER;
	using EncounterCreationWindows::ENCOUNTER_BROWSER;


	struct ListBoxParameters {
		int chosenIndex;
		int highlightedIndex;
		bool isHighlighted;
	};

	class EncounterCreationScene : public CombatTrackerLib::Scene {

	public:
		EncounterCreationScene(CombatTrackerLib::App* app);
		virtual ~EncounterCreationScene();

		virtual void notifyScene(ImGuiIO& imguiIO) override;
		virtual void updateScene(double timeInterval) override;
		virtual void renderUI() override;
		virtual void renderScene() override;
	private:
		// Flags.
		ImGuiWindowFlags encounterCreationWindowFlags{};
		ImGuiDockNodeFlags encounterCreationDockFlags{};
		
		// Dockspace variables.
		std::unordered_map<EncounterCreationWindows, ImGuiID> dockspaceIDs;
		std::unordered_map<EncounterCreationWindows, bool> dockspaceInitializations{
			{ENCOUNTER_CREATOR, false},
			{ENTITY_BROWSER, false},
			{MONSTER_BROWSER, false},
			{PLAYER_CHARACTER_BROWSER, false},
			{ENCOUNTER_BROWSER, false}
		};

		const std::unordered_map<EncounterCreationWindows, std::string> MAPPED_WINDOW_NAMES{
			{ENCOUNTER_CREATOR, "Encounter Creator"},
			{ENCOUNTER_MANAGER, "Encounter Manager"},
			{MENU, "Menu"},
			{ENTITY_BROWSER, "Entity Browser"},
			{MONSTER_BROWSER, "Monster Browser"},
			{PLAYER_CHARACTER_BROWSER, "Player Character Browser"},
			{ENCOUNTER_BROWSER, "Encounter Browser"}
		};

		// TODO: Try to template, minimize the number of functions as much as possible here.
		void renderHostWindow();
		void renderMenuWindow();
		void renderEntityBrowserWindow();
		void renderMonsterBrowserWindow();
		void renderPlayerCharacterBrowserWindow();
		void renderEncounterBrowserWindow();
		void renderEncounterManagerWindow();
		void initializeSubBrowserDockspace(EncounterCreationWindows windowType);

		// TODO: Consider passing a Creature object as a parameter here when implementing editing functionality.
		// So that we can populate all static parameters through that object and just reuse all rendering code.
		void renderMonsterCreationPopupModal();
		void renderPlayerCharacterCreationPopupModal();

		void renderGeneralCreatureInfo(
			std::string& name,
			Size& size,
			CreatureType& type,
			Alignment& alignment,
			int& AC,
			int& HP
		);
		void renderSpeedInfo (
			int& walkSpeed,
			int& climbingSpeed,
			int& swimmingSpeed,
			int& burrowSpeed,
			int& flySpeed
		);
		template <typename MapType, typename NameMap>
		void renderProficiencyAndExpertiseCluster (
			const char* proficiencyLabel, 
			std::string& proficiencyLabelSuffix,
			const char* expertiseLabel,
			std::string& expertiseLabelSuffix,
			MapType& proficiencyItems,
			MapType& expertiseItems,
			const NameMap& itemNames
		);

		// These are way too specific so I don't think I can template this in a convenient way.
		void renderAbilityScoresAndModifiers (
			std::unordered_map<Ability, int>& abilityScores,
			std::unordered_map<Ability, int>& abilityModifiers,
			std::unordered_map<Ability, bool>& abilityScoresOverride
		);
		void renderSavingThrowModifiers (
			std::unordered_map<Ability, int>& savingThrowModifiers, 
			std::unordered_map<Ability, bool>& savingThrowModifiersOverride,
			std::unordered_map<Ability, bool>& proficientSavingThrows,
			std::unordered_map<Ability, bool>& expertiseSavingThrows,
			std::unordered_map<Ability, int>& abilityModifiers,
			const int PB
		);
		void renderSkillModifiers( 
			std::unordered_map<Skill, int>& skillModifiers, 
			std::unordered_map<Skill, bool>& skillModifiersOverride,
			std::unordered_map<Skill, bool>& proficientSkills,
			std::unordered_map<Skill, bool>& expertiseSkills,
			std::unordered_map<Ability, int>& abilityModifiers,
			const int PB
		);
		void renderDmgVulnResAndImm(
			std::unordered_map<DamageType, bool>& damageVulnerabilities,
			std::unordered_map<DamageType, bool>& damageResistances,
			std::unordered_map<DamageType, bool>& damageImmunities
		);
		template <typename MapType, typename NameMap>
		void renderMutuallyExclusiveCheckboxCluster(MapType& items, NameMap& names, const char* label);
		void renderSenses(std::unordered_map<Sense, int>& senses);
		// Reserved for traits and gear.
		template <typename Items>
		void renderAddables(Items& currentItems, const char* label);
		void renderActions(std::vector<CreatureAction>& currentActions);

		void renderMonsterSpecificInfo(
			double& CR,
			int& XP,
			int& PB
		);
		void renderPlayerCharacterSpecificInfo();

		template <typename T, typename MapType>
		void renderListBox(ListBoxParameters& parameters, T& type, const char* listBoxLabel, const MapType& items);
		void renderNonOverridableInputInt(int& value, const char* label);
		template <typename T, typename MapType>
		void renderOveriddableInputInt(
			int& value,
			bool& overrideFlag,
			const char* label,
			const char* overrideLabel,
			const MapType& items,
			const int lowerBound,
			const int upperBound,
			const T key
		);
	};
}