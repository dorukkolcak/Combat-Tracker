#include "PlayerCharacter.h"

namespace CombatTrackerApp {
	PlayerCharacter::PlayerCharacter (CreatureStatBlock creatureBlock, PlayerStatBlock playerBlock) :
		Creature{ std::move(creatureBlock) },
		m_statBlockPlayerPortion{ std::move(playerBlock) }
	{
	}

	PlayerCharacterBuilder& PlayerCharacterBuilder::setLevels(std::unordered_map<PlayerClass, int>& creatureLevels) { m_playerBlock.levels = creatureLevels; return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setSubclasses(std::unordered_map<PlayerClass, PlayerSubclass>& creatureSubclasses) { m_playerBlock.subclasses = creatureSubclasses; return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setWeaponProficiencies(std::unordered_map<WeaponType, bool>& creatureWeaponProficiencies) { m_playerBlock.weaponProficiencies = creatureWeaponProficiencies; return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setArmorProficiencies(std::unordered_map<ArmorType, bool>& creatureArmorProficiencies) { m_playerBlock.armorProficiencies = creatureArmorProficiencies; return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setToolProficiencies(std::unordered_map<Tool, bool>& creatureToolProficiencies) { m_playerBlock.toolProficiencies = creatureToolProficiencies; return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setWeaponMasteries(std::unordered_map<WeaponMastery, bool>& creatureWeaponMasteries) { m_playerBlock.weaponMasteries = creatureWeaponMasteries; return *this; }
	
	PlayerCharacterBuilder& PlayerCharacterBuilder::setSpecies(Species creatureSpecies) { m_playerBlock.species = creatureSpecies; return *this; }
	PlayerCharacterBuilder& PlayerCharacterBuilder::setOrigin(Origin creatureOrigin) { m_playerBlock.origin = creatureOrigin; return *this; }
	
	// Resets the full builder state: The creature block + the player-specific block.
	PlayerCharacterBuilder& PlayerCharacterBuilder::reset() {
		CreatureBuilder<PlayerCharacterBuilder>::reset();
		m_playerBlock = PlayerStatBlock{};
		return *this;
	}

	PlayerCharacter PlayerCharacterBuilder::movePlayerCharacter() {
		PlayerCharacter playerCharacter{ std::move(block), std::move(m_playerBlock) };
		reset();
		return playerCharacter;
	}
	PlayerStatBlock& PlayerCharacterBuilder::getPlayerCharacterBlock() { return m_playerBlock; }
}