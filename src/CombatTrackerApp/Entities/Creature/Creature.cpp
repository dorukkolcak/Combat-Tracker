#include "CombatTrackerApp/Entities/Creature/Creature.h"

namespace CombatTrackerApp {

	Creature::Creature(CreatureStatBlock creatureBlock) :
		m_statBlockCreaturePortion{ std::move(creatureBlock) }, // A creature should own its stat block so use move semantics.
		m_ID{currentIDValue}
	{
		++currentIDValue;
	}
}