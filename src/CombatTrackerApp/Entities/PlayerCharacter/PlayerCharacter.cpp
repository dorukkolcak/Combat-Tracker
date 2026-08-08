#include "CombatTrackerApp/Entities/PlayerCharacter/PlayerCharacter.h"

namespace CombatTrackerApp {
	PlayerCharacter::PlayerCharacter (CreatureStatBlock creatureBlock, PlayerStatBlock playerBlock) :
		Creature{ std::move(creatureBlock) },
		m_statBlockPlayerPortion{ std::move(playerBlock) }
	{}
}