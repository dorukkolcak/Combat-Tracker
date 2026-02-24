#include "Turn.h"

namespace CombatTrackerApp {
	Turn::Turn(Encounter* encounter, int number, std::unordered_map<int, std::vector<CreatureAction>> actions) :
		parentEncounter(encounter),
		turnNumber(number),
		mappedActions(std::move(actions))
	{		
	}
	Turn::~Turn() {}
}