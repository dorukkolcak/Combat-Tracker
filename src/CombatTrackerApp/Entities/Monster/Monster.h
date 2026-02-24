#pragma once

#include "../Creature/Creature.h"

namespace CombatTrackerApp {
	class Monster : public Creature {
	private:
		// MANUALLY SET
		double CR;
		// OVERRIDABLE, COMPUTED IF NOT OVERRIDDEN (See MAPPED_CR_TO_XP)
		int XP;
	public:
		Monster(CreatureStatBlock statBlock, const double creatureCR, const int creatureXP);
		~Monster();

		int getXP();
	};
	class MonsterBuilder : public CreatureBuilder {
	public:
		MonsterBuilder& setCR(const double creatureCR);
		MonsterBuilder& setXP(const int XP);
		Monster createMonster();
	private:
		double CR;
		int XP;
	};
}