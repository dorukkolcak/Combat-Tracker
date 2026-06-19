#include "Creature.h"

namespace CombatTrackerApp {

	// All of the parameters should be verified before creating the object, using the standard 5e rulings.
	//TODO: Find a method for ID generation. Should be done while running encounters. (See note inside the header file.)
	Creature::Creature(CreatureStatBlock creatureBlock) : statBlockCreaturePortion{ std::move(creatureBlock) }, ID{ -1 } {
	}
	// Ideally, we do stack allocation for all of these members. So the destructor should be empty.
	Creature::~Creature() {}
}