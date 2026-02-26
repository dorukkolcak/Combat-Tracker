#include "PlayerCharacter.h"

namespace CombatTrackerApp {
	PlayerCharacter::PlayerCharacter (CreatureStatBlock creatureBlock, PlayerStatBlock playerBlock) : Creature(creatureBlock), statBlockPlayerPortion(std::move(playerBlock))
	{
	}
	PlayerCharacter::~PlayerCharacter() {}
	PlayerCharacterBuilder& PlayerCharacterBuilder::setSpecies(const Species creatureSpecies) { playerBlock.species = creatureSpecies; return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setOrigin(const Origin creatureOrigin) { playerBlock.origin = creatureOrigin; return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setLevels(std::unordered_map<PlayerClass, int>& creatureLevels) { playerBlock.levels = creatureLevels; return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setSubclasses(std::unordered_map<PlayerClass, PlayerSubclass>& creatureSubclasses) { playerBlock.subclasses = creatureSubclasses; return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setWeaponProficiencies(std::unordered_map<WeaponType, bool>& creatureWeaponProficiencies) { playerBlock.weaponProficiencies = creatureWeaponProficiencies; return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setArmorProficiencies(std::unordered_map<ArmorType, bool>& creatureArmorProficiencies) { playerBlock.armorProficiencies = creatureArmorProficiencies; return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setToolProficiencies(std::unordered_map<Tool, bool>& creatureToolProficiencies) { playerBlock.toolProficiencies = creatureToolProficiencies; return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setWeaponMasteries(std::unordered_map<WeaponMastery, bool>& creatureWeaponMasteries) { playerBlock.weaponMasteries = creatureWeaponMasteries; return *this; }
	PlayerCharacter PlayerCharacterBuilder::movePlayerCharacter() { return std::move(PlayerCharacter(block, playerBlock)); }
	PlayerStatBlock& PlayerCharacterBuilder::getPlayerCharacterBlock() { return playerBlock; }
}