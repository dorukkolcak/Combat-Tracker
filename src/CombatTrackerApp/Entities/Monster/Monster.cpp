#include "Monster.h"

namespace CombatTrackerApp {
	Monster::Monster(CreatureStatBlock creatureStatBlock, double creatureCR, int creatureXP) :
		Creature{ std::move(creatureStatBlock) },
		m_CR{ creatureCR },
		m_XP{ creatureXP }
	{
	}
	int Monster::getXP() const {
		return m_XP;
	}

	MonsterBuilder& MonsterBuilder::setCR(double creatureCR) { m_CR = creatureCR; return *this; }
	MonsterBuilder& MonsterBuilder::setXP(int creatureXP) { m_XP = creatureXP; return *this; }

	// Resets the full builder state: The creature block + the monster-specific fields.
	MonsterBuilder& MonsterBuilder::reset() {
		CreatureBuilder<MonsterBuilder>::reset();
		m_CR = {};
		m_XP = {};
		return *this;
	}
	double MonsterBuilder::getCR() const {
		return m_CR;
	}
	int MonsterBuilder::getXP() const {
		return m_XP;
	}
	Monster MonsterBuilder::moveMonster() {
		Monster monster{ std::move(m_block), m_CR, m_XP };
		reset();
		return monster;
	}
}