#include "Monster.h"

namespace CombatTrackerApp {
	Monster::Monster(CreatureStatBlock creatureStatBlock, const double creatureCR, const std::uint32_t creatureXP) : 
		Creature(creatureStatBlock),
		CR(creatureCR),
		XP(creatureXP)
	{
	}
	Monster::~Monster() {}
	MonsterBuilder& MonsterBuilder::setCR(const double creatureCR) { CR = creatureCR; return *this; }
	MonsterBuilder& MonsterBuilder::setXP(const std::uint32_t creatureXP) { XP = creatureXP; return *this; }
	Monster MonsterBuilder::createMonster() { return std::move(Monster(block, CR, XP)); }
}