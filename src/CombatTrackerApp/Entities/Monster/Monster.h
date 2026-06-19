#pragma once

#include "../Creature/Creature.h"

namespace CombatTrackerApp {
	class Monster : public Creature {
	private:
		double CR{};
		int XP{};
	public:
		Monster(CreatureStatBlock statBlock, double creatureCR, int creatureXP);
		~Monster();

		int getXP();
	};

	class MonsterBuilder : public CreatureBuilder<MonsterBuilder> {
	public:
		MonsterBuilder& setCR(double creatureCR);
		MonsterBuilder& setXP(int XP);
		Monster moveMonster();
	private:
		double CR{};
		int XP{};
	};
}