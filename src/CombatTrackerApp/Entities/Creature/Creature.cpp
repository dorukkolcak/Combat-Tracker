#include "Creature.h"

namespace CombatTrackerApp {

	Creature::Creature(CreatureStatBlock creatureBlock) : 
		m_ID{currentIDValue}, 
		m_statBlockCreaturePortion{ std::move(creatureBlock) } // A creature should own its stat block so use move semantics.
	{
		++currentIDValue;
	}
}