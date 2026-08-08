#include "CombatTrackerApp/Entities/Monster/Monster.h"

namespace CombatTrackerApp {
	Monster::Monster(CreatureStatBlock creatureStatBlock, double creatureCR, OverridableInt creatureXP) :
		Creature{ std::move(creatureStatBlock) },
		m_CR{ creatureCR },
		m_XP{ creatureXP }
	{
	}
	double Monster::getCR() const {
		return m_CR;
	}
	int Monster::getXP() const {
		return m_XP.value;
	}
}