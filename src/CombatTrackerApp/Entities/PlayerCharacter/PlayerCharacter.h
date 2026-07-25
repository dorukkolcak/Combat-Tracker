#pragma once

#include "../Creature/Creature.h"
#include "PlayerCharacterEnums.h"
#include "PlayerCharacterConstants.h"

namespace CombatTrackerApp {

	// The stat block types shouldn't be nested types as they are used by the builders.
	// This struct should also be an aggregate.
	struct PlayerStatBlock {
		std::unordered_map<PlayerClass, int> levels{
			{PlayerClass::BARBARIAN, MIN_CLASS_LEVEL},
			{PlayerClass::BARD, MIN_CLASS_LEVEL},
			{PlayerClass::CLERIC, MIN_CLASS_LEVEL},
			{PlayerClass::DRUID, MIN_CLASS_LEVEL},
			{PlayerClass::FIGHTER, MIN_CLASS_LEVEL},
			{PlayerClass::MONK, MIN_CLASS_LEVEL},
			{PlayerClass::PALADIN, MIN_CLASS_LEVEL},
			{PlayerClass::RANGER, MIN_CLASS_LEVEL},
			{PlayerClass::ROGUE, MIN_CLASS_LEVEL},
			{PlayerClass::SORCERER, MIN_CLASS_LEVEL},
			{PlayerClass::WARLOCK, MIN_CLASS_LEVEL},
			{PlayerClass::WIZARD, MIN_CLASS_LEVEL} 
		};

		// Each PlayerClass can only have one PlayerSubclass.
		std::unordered_map<PlayerClass, PlayerSubclass> subclasses{};
		std::unordered_map<WeaponType, bool> weaponProficiencies{
			{WeaponType::SIMPLE_MELEE, false},
			{WeaponType::SIMPLE_RANGED, false},
			{WeaponType::MARTIAL_MELEE, false},
			{WeaponType::MARTIAL_RANGED, false}
		};
		std::unordered_map<ArmorType, bool> armorProficiencies{
			{ArmorType::LIGHT, false},
			{ArmorType::MEDIUM, false},
			{ArmorType::HEAVY, false},
			{ArmorType::SHIELD, false}
		};
		std::unordered_map<Tool, bool> toolProficiencies{
			{Tool::ALCHEMISTS_SUPPLIES, false},
			{Tool::BREWERS_SUPPLIES, false},
			{Tool::CALLIGRAPHERS_SUPPLIES, false},
			{Tool::CARPERNTERS_TOOLS, false},
			{Tool::CARTOGRAPHERS_TOOLS, false},
			{Tool::COBBLERS_TOOLS, false},
			{Tool::COOKS_UTENSILS, false},
			{Tool::GLASSBLOWERS_TOOLS, false},
			{Tool::JEWELERS_TOOLS, false},
			{Tool::LEATHERWORKERS_TOOLS, false},
			{Tool::MASON_TOOLS, false},
			{Tool::PAINTERS_TOOLS, false},
			{Tool::POTTERS_TOOLS, false},
			{Tool::SMITHS_TOOLS, false},
			{Tool::TINKERS_TOOLS, false},
			{Tool::WEAVERS_TOOLS, false},
			{Tool::WOODCARVERS_TOOLS, false},
			{Tool::DISGUISE_KIT, false},
			{Tool::FORGERY_KIT, false},
			{Tool::GAMING_SET, false},
			{Tool::HERBALISM_KIT, false},
			{Tool::MUSICAL_INSTRUMENT, false},
			{Tool::NAVIGATORS_TOOLS, false},
			{Tool::POISONERS_KIT, false},
			{Tool::THIEVES_TOOLS, false}
		};
		std::unordered_map<WeaponMastery, bool> weaponMasteries{
			{WeaponMastery::CLEAVE, false},
			{WeaponMastery::GRAZE, false},
			{WeaponMastery::NICK, false},
			{WeaponMastery::PUSH, false},
			{WeaponMastery::SAP, false},
			{WeaponMastery::SLOW, false},
			{WeaponMastery::TOPPLE, false},
			{WeaponMastery::VEX, false}
		};
		Species species{};
		Origin origin{};
	};
	class PlayerCharacter : public Creature {
	public:

		// Same deal as most constructors, stat blocks are moved so pass them by value.
		PlayerCharacter(CreatureStatBlock creaturePortion, PlayerStatBlock playerPortion);
	private:
		PlayerStatBlock m_statBlockPlayerPortion{};
	};
	class PlayerCharacterBuilder : public CreatureBuilder<PlayerCharacterBuilder> {
	public:

		PlayerCharacterBuilder& setLevels(const std::unordered_map<PlayerClass, int>& creatureLevels);
		PlayerCharacterBuilder& setSubclasses(const std::unordered_map<PlayerClass, PlayerSubclass>& creatureSubclasses);
		PlayerCharacterBuilder& setWeaponProficiencies(const std::unordered_map<WeaponType, bool>& creatureWeaponProficiencies);
		PlayerCharacterBuilder& setArmorProficiencies(const std::unordered_map<ArmorType, bool>& creatureArmorProficiencies);
		PlayerCharacterBuilder& setToolProficiencies(const std::unordered_map<Tool, bool>& creatureToolProficiencies);
		PlayerCharacterBuilder& setWeaponMasteries(const std::unordered_map<WeaponMastery, bool>& creatureWeaponMasteries);

		PlayerCharacterBuilder& setSpecies(Species creatureSpecies);
		PlayerCharacterBuilder& setOrigin(Origin creatureOrigin);

		// Hides CreatureBuilder::reset to also clear the player-specific block.
		PlayerCharacterBuilder& reset();
		PlayerStatBlock& getPlayerCharacterBlock();
		PlayerCharacter movePlayerCharacter();

	private:
		PlayerStatBlock m_playerBlock{};
	};
}