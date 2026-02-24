#pragma once

#include "../Encounter/Encounter.h"

namespace CombatTrackerApp {
	class Encounter;
	class Turn {
	private:
		int turnNumber;
		// A turn should keep a reference to its encounter. There is no concept of ownership here, so use a raw pointer.
		// Pass the address of the Encounter to initialize this.
		Encounter* parentEncounter;
		// TODO: Not sure on the correct way of storage here.
		// I can see an argument for turns owning their participants (hence shared_ptr)
		// But I can also see an argument for turns just keeping a reference to owned Creatures of an Encounter.
		// This also works considering Encounter's Smart Pointer will outlive the pointers of the Turn.
		std::vector<Creature*> turnParticipants;
		std::unordered_map<int, std::vector<CreatureAction>> mappedActions; //Maps actions by creature ID, just like initiative mapping.
	public:
		Turn(Encounter* encounter, int number, std::unordered_map<int, std::vector<CreatureAction>> actions);
		~Turn();
	};
};