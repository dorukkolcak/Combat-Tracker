/*
#pragma once
#include "Entities/Creature/Creature.h"
#include "Entities/PlayerCharacter/PlayerCharacter.h"

namespace CombatTrackerApp {

	void to_json(nlohmann::json& jsonData, const CreatureAction& action);
	void from_json(const nlohmann::json& jsonData, CreatureAction& action);
	void to_json(nlohmann::json& jsonData, const Trait& trait);
	void from_json(const nlohmann::json& jsonData, Trait& trait);
	void to_json(nlohmann::json& jsonData, const Gear& gear);
	void from_json(const nlohmann::json& jsonData, Gear& gear);

	bool validateCreatureStatBlock(const CreatureStatBlock& block);

	bool writeCreatureBlockToJSON(const CreatureStatBlock& block, nlohmann::json& jsonData);
	bool writePlayerCharacterBlockToJSON(const PlayerStatBlock& block, nlohmann::json& jsonData);

	bool saveMonster(const CreatureStatBlock& creatureBlock, const double& CR, const int& XP);
	bool savePlayerCharacter(const CreatureStatBlock& blockCreaturePortion, const PlayerStatBlock& blockPlayerPortion);
}
*/
