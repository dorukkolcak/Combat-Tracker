#pragma once
#include "Entities/Creature/Creature.h"
#include "Entities/PlayerCharacter/PlayerCharacter.h"

namespace CombatTrackerApp {
	bool saveMonster(const CreatureStatBlock& creatureBlock, const double& CR, const int& XP);
	bool updateMonster(const CreatureStatBlock& creatureBlock, const double& CR, const int& XP);
	bool savePlayerCharacter(const CreatureStatBlock& blockCreaturePortion, const PlayerStatBlock& blockPlayerPortion);
	bool updatePlayerCharacter(const CreatureStatBlock& blockCreaturePortion, const PlayerStatBlock& blockPlayerPortion);
}
