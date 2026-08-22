#include "CombatTrackerApp/Entities/Monster.h"

namespace CombatTrackerApp {
	Monster::Monster(CreatureStatBlock creatureStatBlock, double creatureCR)
		: Creature{creatureStatBlock},
		  m_statBlockMonsterPortion{creatureCR, MonsterDerivedStats{}} {
		resolveDerived(m_statBlockCreaturePortion, m_statBlockMonsterPortion);
	}

	double Monster::getCR() const {
		return m_statBlockMonsterPortion.CR;
	}

	int Monster::getXP() const {
		return m_statBlockMonsterPortion.derived.getXP();
	}
}
