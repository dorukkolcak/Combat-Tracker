#include "Creature.h"

namespace CombatTrackerApp {

	// All of the parameters should be verified before creating the object, using the standard 5e rulings.
	Creature::Creature(CreatureStatBlock creatureBlock) : 
		m_ID{currentIDValue}, 
		m_statBlockCreaturePortion{ std::move(creatureBlock) } 
	{
		++currentIDValue;
	}
}