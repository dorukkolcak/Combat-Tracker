#pragma once

#include "../Creature/Creature.h"
#include "PlayerCharacterEnums.h"
#include "PlayerCharacterConstants.h"

namespace CombatTrackerApp {
	struct PlayerStatBlock {

		PlayerStatBlock() :
			species(Species::NO_SPECIES),
			origin(Origin::NO_ORIGIN),
			levels{
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
			},
			subclasses{},
			weaponProficiencies{
				{WeaponType::SIMPLE_MELEE, false},
				{WeaponType::SIMPLE_RANGED, false},
				{WeaponType::MARTIAL_MELEE, false},
				{WeaponType::MARTIAL_RANGED, false}
			},
			armorProficiencies{
				{ArmorType::LIGHT, false},
				{ArmorType::MEDIUM, false},
				{ArmorType::HEAVY, false},
				{ArmorType::SHIELD, false}
			},
			toolProficiencies{
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
			},
			weaponMasteries{
				{WeaponMastery::CLEAVE, false},
				{WeaponMastery::GRAZE, false},
				{WeaponMastery::NICK, false},
				{WeaponMastery::PUSH, false},
				{WeaponMastery::SAP, false},
				{WeaponMastery::SLOW, false},
				{WeaponMastery::TOPPLE, false},
				{WeaponMastery::VEX, false}
			}
		{}

		// MANUALLY SET.
		Species species;
		// MANUALLY SET.
		Origin origin;
		// MANUALLY SET, DEFAULT TO ZERO PER PlayerClass
		std::unordered_map<PlayerClass, int> levels;
		// MANUALLY SET, level of the appropriate PlayerClass must surpass SUBCLASS_LEVEL_THRESHOLD for each subclass.
		// Each PlayerClass can only have one PlayerSubclass.
		std::unordered_map<PlayerClass, PlayerSubclass> subclasses;
		// MANUALLY SET
		std::unordered_map<WeaponType, bool> weaponProficiencies;
		// MANUALLY SET
		std::unordered_map<ArmorType, bool> armorProficiencies;
		// MANUALLY SET
		std::unordered_map<Tool, bool> toolProficiencies;
		// MANUALLY SET
		std::unordered_map<WeaponMastery, bool> weaponMasteries;
	};
	class PlayerCharacter : public Creature {
	private:
		PlayerStatBlock statBlockPlayerPortion;
	public:
		PlayerCharacter(CreatureStatBlock creaturePortion, PlayerStatBlock playerPortion);
		~PlayerCharacter();
	};
	class PlayerCharacterBuilder : public CreatureBuilder<PlayerCharacterBuilder> {
	public:
		PlayerCharacterBuilder& setSpecies(const Species creatureSpecies);
		PlayerCharacterBuilder& setOrigin(const Origin creatureOrigin);
		PlayerCharacterBuilder& setLevels(std::unordered_map<PlayerClass, int>& creatureLevels);
		PlayerCharacterBuilder& setSubclasses(std::unordered_map<PlayerClass, PlayerSubclass>& creatureSubclasses);
		PlayerCharacterBuilder& setWeaponProficiencies(std::unordered_map<WeaponType, bool>& creatureWeaponProficiencies);
		PlayerCharacterBuilder& setArmorProficiencies(std::unordered_map<ArmorType, bool>& creatureArmorProficiencies);
		PlayerCharacterBuilder& setToolProficiencies(std::unordered_map<Tool, bool>& creatureToolProficiencies);
		PlayerCharacterBuilder& setWeaponMasteries(std::unordered_map<WeaponMastery, bool>& creatureWeaponMasteries);

		PlayerStatBlock& getPlayerCharacterBlock();
		PlayerCharacter movePlayerCharacter();

	private:
		PlayerStatBlock playerBlock;
	};
}