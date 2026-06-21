#pragma once

#include "../Creature/Creature.h"

namespace CombatTrackerApp {
	class Monster : public Creature {
	private:
		double CR{};
		int XP{};
	public:
		Monster(CreatureStatBlock statBlock, double creatureCR, int creatureXP);

		int getXP();
	};

	class MonsterBuilder : public CreatureBuilder<MonsterBuilder> {
	public:
		MonsterBuilder& setCR(double creatureCR);
		MonsterBuilder& setXP(int XP);

		// Hides CreatureBuilder::reset to also clear the monster-specific fields.
		MonsterBuilder& reset();
		Monster moveMonster();
	private:
		double CR{};
		int XP{};
	};
}