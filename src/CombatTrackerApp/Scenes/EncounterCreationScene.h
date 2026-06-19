#pragma once

#include <string>
#include <utility>

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

	struct ListBoxParameters {
		int chosenIndex;
		int highlightedIndex;
		bool isHighlighted;
	};

	class EncounterCreationScene : public CombatTrackerLib::Scene {

	public:
		EncounterCreationScene(CombatTrackerLib::App* app);
		virtual ~EncounterCreationScene();

		virtual void notifyScene(const ImGuiIO& imguiIO) override;
		virtual void updateScene(double timeInterval) override;
		virtual void renderUI() override;
		virtual void renderScene() override;
	private:
		
		// Flags.
		ImGuiWindowFlags encounterCreationWindowFlags{};
		ImGuiDockNodeFlags encounterCreationDockFlags{};
		ImGuiWindowFlags popupWindowFlags{};

		// Dockspace variables.
		std::unordered_map<EncounterCreationWindows, ImGuiID> dockspaceIDs{};
		std::unordered_map<EncounterCreationWindows, bool> dockspaceInitializations {
			{EncounterCreationWindows::ENCOUNTER_CREATOR, false},
			{EncounterCreationWindows::ENTITY_BROWSER, false},
			{EncounterCreationWindows::MONSTER_BROWSER, false},
			{EncounterCreationWindows::PLAYER_CHARACTER_BROWSER, false},
			{EncounterCreationWindows::ENCOUNTER_BROWSER, false}
		};

		std::string currentPopupName{};
		MonsterBuilder monsterBuilder{};
		PlayerCharacterBuilder playerCharacterBuilder{};

		CreatureStatBlock& currentCreatureStatBlock{ monsterBuilder.getCreatureBlock() };
		PlayerStatBlock& currentPlayerStatBlock{ playerCharacterBuilder.getPlayerCharacterBlock() };

		double currentMonsterCR{};
		int currentMonsterXP{};

		ListBoxParameters monsterCRParameters{};
		ListBoxParameters sizeListBoxParameters{};
		ListBoxParameters creatureTypeParameters{};
		ListBoxParameters alignmentParameters{};
		ListBoxParameters speciesParameters{};
		ListBoxParameters originParameters{};

		bool isInitiativeOverriden{};
		bool isPassivePerceptionOverridden{};
		bool isXPOverridden{};
		bool isPBOverridden{};

		std::unordered_map<Ability, bool> abilityModifiersOverride{};
		std::unordered_map<Ability, bool> savingThrowsOverride{};
		std::unordered_map<Skill, bool> skillModifiersOverride{};

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

		
		// Constant UI labels. These should be static as they shouldn't be instantiated per object/scene.
		inline static const std::unordered_map<EncounterCreationWindows, std::string> MAPPED_WINDOW_NAMES{
			{EncounterCreationWindows::ENCOUNTER_CREATOR, "Encounter Creator"},
			{EncounterCreationWindows::ENCOUNTER_MANAGER, "Encounter Manager"},
			{EncounterCreationWindows::MENU, "Menu"},
			{EncounterCreationWindows::ENTITY_BROWSER, "Entity Browser"},
			{EncounterCreationWindows::MONSTER_BROWSER, "Monster Browser"},
			{EncounterCreationWindows::PLAYER_CHARACTER_BROWSER, "Player Character Browser"},
			{EncounterCreationWindows::ENCOUNTER_BROWSER, "Encounter Browser"}
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

		void renderGeneralCreatureInfo ();
		void renderSpeedInfo ();

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
		void initializeSubBrowserDockspace(EncounterCreationWindows windowType);

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
}