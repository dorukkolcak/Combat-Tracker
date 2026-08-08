#pragma once

#include "CombatTrackerApp/Entities/Creature/Creature.h"

namespace CombatTrackerApp {
	class Monster : public Creature {
	public:

		// Stat block is moved so pass it by value.
		Monster(CreatureStatBlock statBlock, double creatureCR, OverridableInt creatureXP);

		double getCR() const;
		int getXP() const;
	private:
		double m_CR{};
		OverridableInt m_XP{};
	};
}