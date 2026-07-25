#pragma once

#include "../Encounter/Encounter.h"

namespace CombatTrackerApp {

	class Encounter;
	class Turn {
	public:
		Turn(Encounter* encounter, int number, std::unordered_map<int, std::vector<CreatureAction>> actions);
	private:
		std::unordered_map<int, std::vector<CreatureAction>> m_mappedActions{}; //Maps actions by creature ID, just like initiative mapping.

		// Not sure on the correct way of storage here.
		// I can see an argument for turns owning their participants (hence shared_ptr)
		// But I can also see an argument for turns just keeping a reference to owned Creatures of an Encounter.
		// This also works considering Encounter's Smart Pointer will outlive the pointers of the Turn.
		std::vector<Creature*> m_turnParticipants{};

		// A turn should keep a reference to its encounter. There is no concept of ownership here, so use a raw pointer.
		// Pass the address of the Encounter to initialize this.
		// Might also just be a weak_ptr. Accessing the underlying object will be kind of annoying in this case though.
		Encounter* m_parentEncounter{};
		int m_turnNumber{};
		
	};
};