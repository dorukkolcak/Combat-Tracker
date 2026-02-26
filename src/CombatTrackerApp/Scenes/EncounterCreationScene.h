#pragma once

#include <string>

#include "../../CombatTrackerLib/Scene.h"
#include "../Entities/Creature/CreatureEnums.h"
#include "../Entities/Creature/CreatureTypes.h"
#include "../Entities/Creature/Creature.h"
#include "../Entities/Monster/Monster.h"
#include "../Entities/PlayerCharacter/PlayerCharacter.h"

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
		ImGuiWindowFlags popupWindowFlags{};

		// Dockspace variables.
		std::unordered_map<EncounterCreationWindows, ImGuiID> dockspaceIDs;
		std::unordered_map<EncounterCreationWindows, bool> dockspaceInitializations {
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

		const std::string proficiencySuffix{ " Proficiency" };
		const std::string expertiseSuffix{ " Expertise" };
		const std::string traitLabel{ "Trait" };
		const std::string equipmentLabel{ "Gear" };
		std::string popupName;

		MonsterBuilder monsterBuilder;
		PlayerCharacterBuilder playerCharacterBuilder;

		CreatureStatBlock& currentCreatureStatBlock;
		PlayerStatBlock& currentPlayerStatBlock;

		double currentMonsterCR;
		int currentMonsterXP;

		ListBoxParameters monsterCRParameters;
		ListBoxParameters sizeListBoxParameters;
		ListBoxParameters creatureTypeParameters;
		ListBoxParameters alignmentParameters;
		ListBoxParameters speciesParameters;
		ListBoxParameters originParameters;

		bool isInitiativeOverriden;
		bool isPassivePerceptionOverridden;
		bool isXPOverridden;
		bool isPBOverridden;

		std::unordered_map<Ability, bool> abilityModifiersOverride;
		std::unordered_map<Ability, bool> savingThrowsOverride;
		std::unordered_map<Skill, bool> skillModifiersOverride;

		std::unordered_map<PlayerSubclass, bool> chosenSubclasses{
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
		std::unordered_map<PlayerClass, bool> hasCorrespondingSubclass{
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
		void renderHostWindow();
		void renderMenuWindow();
		void renderEntityBrowserWindow();
		void renderMonsterBrowserWindow();
		void renderPlayerCharacterBrowserWindow();
		void renderEncounterBrowserWindow();
		void renderEncounterManagerWindow();
		void initializeSubBrowserDockspace(EncounterCreationWindows windowType);

		void renderEntityCreationPopupModal(bool isMonster, bool editMode);

		void renderGeneralCreatureInfo ();
		void renderSpeedInfo ();
		template <typename MapType, typename NameMap>
		void renderProficiencyAndExpertiseCluster (
			const char* proficiencyLabel, 
			const char* expertiseLabel,
			MapType& proficiencyItems,
			MapType& expertiseItems,
			const NameMap& itemNames
		);

		// These are way too specific so I don't think I can template this in a convenient way.
		void renderAbilityScoresAndModifiers ();
		void renderSavingThrowModifiers ();
		void renderSkillModifiers ();
		void renderDmgVulnResAndImm ();
		template <typename MapType, typename NameMap>
		void renderMutuallyExclusiveCheckboxCluster(MapType& items, NameMap& names, const char* label);
		void renderSenses();
		// Reserved for traits and gear.
		template <typename Item>
		void renderAddables(std::vector<Item>& currentItems, const std::string& label);

		void renderActions(bool isMonster);

		void renderMonsterSpecificInfo ();
		void renderPlayerClassAndSubclasses();
		void renderPlayerCharacterSpecificInfo();

		template <typename T, typename MapType>
		void renderListBox(ListBoxParameters& parameters, T& type, const char* listBoxLabel, const MapType& items);
		void renderNonOverridableInputInt(int& value, const char* label);
		template <typename T, typename MapType>
		void renderOverriddableInputInt (
			int& value,
			bool& overrideFlag,
			const char* label,
			const char* overrideLabel,
			const MapType& items,
			const int lowerBound,
			const int upperBound,
			const T key
		);
		void resetParametersAndOverrides();
	};
}