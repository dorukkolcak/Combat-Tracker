#include "Turn.h"

namespace CombatTrackerApp {
	Turn::Turn(Encounter* encounter, int number, std::unordered_map<int, std::vector<CreatureAction>> actions) :
		m_mappedActions{ std::move(actions) },
		m_parentEncounter{ encounter },
		m_turnNumber{ number }
	{		
	}
}