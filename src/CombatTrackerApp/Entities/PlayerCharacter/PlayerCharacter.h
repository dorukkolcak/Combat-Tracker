#pragma once

#include "../Creature/Creature.h"
#include "PlayerCharacterEnums.h"

namespace CombatTrackerApp {
	struct PlayerStatBlock {
		// MANUALLY SET, OVERRIDES CreatureType to HUMANOID. => CreatureType can be overriden by the user again, if they want.
		Species species;
		// MANUALLY SET, DEFAULT TO ZERO PER PlayerClass
		std::unordered_map<PlayerClass, int> levels;
		// MANUALLY SET, level of the appropriate PlayerClass must surpass SUBCLASS_LEVEL_THRESHOLD for each subclass.
		// Each PlayerClass can only have one PlayerSubclass.
		std::unordered_map<PlayerClass, PlayerSubclass> subclasses;
		// MANUALLY SET
		std::unordered_set<WeaponType> weaponProficiencies;
		// MANUALLY SET
		std::unordered_set<ArmorType> armorProficiencies;
		// MANUALLY SET
		std::unordered_set<Tool> toolProficiencies;
		// MANUALLY SET
		std::unordered_set<WeaponMastery> weaponMasteries;
		// TODO: Could also track money as well but not keeping it for now as it is pretty irrelevant for combat scenarios.
		// TODO: Spellcasting stats might also be relevant.
	};
	class PlayerCharacter : public Creature {
	private:
		PlayerStatBlock statBlockPlayerPortion;
	public:
		PlayerCharacter(CreatureStatBlock creaturePortion, PlayerStatBlock playerPortion);
		~PlayerCharacter();
	};
	class PlayerCharacterBuilder : public CreatureBuilder {
	public:
		PlayerCharacterBuilder& setSpecies(const Species creatureSpecies);
		PlayerCharacterBuilder& setLevels(std::unordered_map<PlayerClass, int> creatureLevels);
		PlayerCharacterBuilder& setSubclasses(std::unordered_map<PlayerClass, PlayerSubclass> creatureSubclasses);
		PlayerCharacterBuilder& setWeaponProficiencies(std::unordered_set<WeaponType> creatureWeaponProficiencies);
		PlayerCharacterBuilder& setArmorProficiencies(std::unordered_set<ArmorType> creatureArmorProficiencies);
		PlayerCharacterBuilder& setToolProficiencies(std::unordered_set<Tool> creatureToolProficiencies);
		PlayerCharacterBuilder& setWeaponMasteries(std::unordered_set<WeaponMastery> creatureWeaponMasteries);
		PlayerCharacter createPlayerCharacter();
	private:
		PlayerStatBlock playerBlock;
	};
}