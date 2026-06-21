#include "Monster.h"

namespace CombatTrackerApp {
	Monster::Monster(CreatureStatBlock creatureStatBlock, double creatureCR, int creatureXP) :
		Creature{ std::move(creatureStatBlock) },
		CR{ creatureCR },
		XP{ creatureXP }
	{
	}
	int Monster::getXP() {
		return XP;
	}

	MonsterBuilder& MonsterBuilder::setCR(double creatureCR) { CR = creatureCR; return *this; }
	MonsterBuilder& MonsterBuilder::setXP(int creatureXP) { XP = creatureXP; return *this; }

	// Resets the full builder state: The creature block + the monster-specific fields.
	MonsterBuilder& MonsterBuilder::reset() {
		CreatureBuilder<MonsterBuilder>::reset();
		CR = {};
		XP = {};
		return *this;
	}

	Monster MonsterBuilder::moveMonster() {
		Monster monster{ std::move(block), CR, XP };
		reset();
		return monster;
	}
}