#pragma once

#include "../Creature/Creature.h"

namespace CombatTrackerApp {
	class Monster : public Creature {
	public:

		// Stat block is moved so pass it by value.
		Monster(CreatureStatBlock statBlock, double creatureCR, int creatureXP);
		int getXP() const;
	private:
		double m_CR{};
		int m_XP{};
	};
	class MonsterBuilder : public CreatureBuilder<MonsterBuilder> {
	public:
		MonsterBuilder& setCR(double creatureCR);
		MonsterBuilder& setXP(int XP);

		// Hides CreatureBuilder::reset to also clear the monster-specific fields.
		MonsterBuilder& reset();
		Monster moveMonster();
	private:
		double m_CR{};
		int m_XP{};
	};
}