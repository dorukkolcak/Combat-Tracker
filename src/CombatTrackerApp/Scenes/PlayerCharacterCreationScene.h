#pragma once

#include <string>
#include <unordered_map>

#include "EntityCreationScene.h"
#include "../Entities/PlayerCharacter/PlayerCharacter.h"

namespace CombatTrackerApp {

	// The player character half of entity creation.
	class PlayerCharacterCreationScene : public EntityCreationScene {
	public:

		explicit PlayerCharacterCreationScene(CombatTrackerLib::App* app);

	protected:

		const std::string& getEntityLabel() const override;
		bool isMonsterScene() const override;
		void renderEntitySpecificInfo() override;
		void renderEntitySpecificSection() override;
		bool completeEntityCreation() override;
		void resetEntity() override;
		void resetParametersAndOverrides() override;

	private:

		inline static const std::string ENTITY_LABEL{ "Player Character" };

		PlayerCharacterBuilder m_playerCharacterBuilder{};

		// Held by value for the same reason the creature block is. See EntityCreationScene.
		PlayerStatBlock m_currentPlayerStatBlock{};

		CombatTrackerLib::ListBoxParameters m_speciesParameters{};
		CombatTrackerLib::ListBoxParameters m_originParameters{};

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

		void renderPlayerClassAndSubclasses();
		void renderPlayerCharacterSpecificInfo();
	};
}
