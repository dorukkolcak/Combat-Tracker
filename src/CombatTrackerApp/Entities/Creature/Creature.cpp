#include "Creature.h"

namespace CombatTrackerApp {
	// All of the parameters should be verified before creating the object, using the rulings defined in the header files.
	Creature::Creature(CreatureStatBlock creatureBlock) : statBlockCreaturePortion(std::move(creatureBlock)), ID(-1) {
		//TODO: Find a method for ID generation. Should be done while running encounters.
	}
	// Ideally, we do stack allocation for all of these members. So the destructor should be empty.
	Creature::~Creature() {}
	int Creature::findAbilityScoreModifier(int abilityScore) {
		return (abilityScore - 10) / 2;
	}
	void setID() {

	}
}