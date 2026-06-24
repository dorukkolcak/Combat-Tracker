#include "Creature.h"

namespace CombatTrackerApp {

	// All of the parameters should be verified before creating the object, using the standard 5e rulings.
	//TODO: Find a method for ID generation. Should be done while running encounters. (See note inside the header file.)
	Creature::Creature(CreatureStatBlock creatureBlock) : 
		m_ID{currentIDValue}, 
		m_statBlockCreaturePortion{ std::move(creatureBlock) } 
	{
		++currentIDValue;
	}
}