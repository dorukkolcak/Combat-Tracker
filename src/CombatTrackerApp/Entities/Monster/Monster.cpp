#include "Monster.h"

namespace CombatTrackerApp {
	Monster::Monster(CreatureStatBlock creatureStatBlock, double creatureCR, int creatureXP) :
		Creature{ creatureStatBlock },
		CR{ creatureCR },
		XP{ creatureXP }
	{
	}
	Monster::~Monster() {}
	int Monster::getXP() {
		return XP;
	}

	MonsterBuilder& MonsterBuilder::setCR(double creatureCR) { CR = creatureCR; return *this; }
	MonsterBuilder& MonsterBuilder::setXP(int creatureXP) { XP = creatureXP; return *this; }
	Monster MonsterBuilder::moveMonster() { return std::move(Monster{ block, CR, XP }); }
}