#include "PlayerCharacter.h"

namespace CombatTrackerApp {
	PlayerCharacter::PlayerCharacter (CreatureStatBlock creatureBlock, PlayerStatBlock playerBlock) : Creature(creatureBlock), statBlockPlayerPortion(std::move(playerBlock))
	{
	}
	PlayerCharacter::~PlayerCharacter() {}
	PlayerCharacterBuilder& PlayerCharacterBuilder::setSpecies(const Species creatureSpecies) { playerBlock.species = creatureSpecies; return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setLevels(std::unordered_map<PlayerClass, int> creatureLevels) { playerBlock.levels = std::move(creatureLevels); return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setSubclasses(std::unordered_map<PlayerClass, PlayerSubclass> creatureSubclasses) { playerBlock.subclasses = std::move(creatureSubclasses); return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setWeaponProficiencies(std::unordered_set<WeaponType> creatureWeaponProficiencies) { playerBlock.weaponProficiencies = std::move(creatureWeaponProficiencies); return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setArmorProficiencies(std::unordered_set<ArmorType> creatureArmorProficiencies) { playerBlock.armorProficiencies = std::move(creatureArmorProficiencies); return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setToolProficiencies(std::unordered_set<Tool> creatureToolProficiencies) { playerBlock.toolProficiencies = std::move(creatureToolProficiencies); return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setWeaponMasteries(std::unordered_set<WeaponMastery> creatureWeaponMasteries) { playerBlock.weaponMasteries = std::move(creatureWeaponMasteries); return *this; }
	PlayerCharacter PlayerCharacterBuilder::createPlayerCharacter() { return std::move(PlayerCharacter(block, playerBlock)); }
}