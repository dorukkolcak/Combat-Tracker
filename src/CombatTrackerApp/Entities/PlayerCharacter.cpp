#include "CombatTrackerApp/Entities/PlayerCharacter.h"

namespace CombatTrackerApp {
	PlayerCharacter::PlayerCharacter (CreatureStatBlock creatureBlock, PlayerStatBlock playerBlock) :
		Creature{ creatureBlock },
		m_statBlockPlayerPortion{ playerBlock }
	{}
}
