#pragma once

#include "../Creature/Creature.h"

namespace CombatTrackerApp {
	class Monster : public Creature {
	private:
		// MANUALLY SET
		double CR;
		// OVERRIDABLE, COMPUTED IF NOT OVERRIDDEN (See MAPPED_CR_TO_XP)
		std::uint32_t XP;
	public:
		Monster(CreatureStatBlock statBlock, const double creatureCR, const std::uint32_t creatureXP);
		~Monster();

		std::uint32_t getXP();
	};
	class MonsterBuilder : public CreatureBuilder {
	public:
		MonsterBuilder& setCR(const double creatureCR);
		MonsterBuilder& setXP(const std::uint32_t XP);
		Monster createMonster();
	private:
		double CR;
		std::uint32_t XP;
	};
}