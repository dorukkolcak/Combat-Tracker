#include "CombatTrackerApp/Entities/Creature.h"

namespace CombatTrackerApp {

	Creature::Creature(CreatureStatBlock creatureBlock) :

		// Don't use move semantics here as we might create multiple creatures using the same stat block. 
		//Maybe pass by reference? I mean copying should be fine as well.
		m_statBlockCreaturePortion{ creatureBlock }, 
		m_ID{s_currentIDValue}
	{
		++s_currentIDValue;
	}
}
