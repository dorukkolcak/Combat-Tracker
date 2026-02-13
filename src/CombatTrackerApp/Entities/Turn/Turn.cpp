#include "Turn.h"

namespace CombatTrackerApp {
	Turn::Turn(Encounter* encounter, std::uint16_t number, std::unordered_map<std::uint32_t, std::vector<CreatureAction>> actions) :
		parentEncounter(encounter),
		turnNumber(number),
		mappedActions(std::move(actions))
	{		
	}
	Turn::~Turn() {}
}