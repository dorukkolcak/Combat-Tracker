#pragma once

#include "CombatTrackerApp/Entities/Monster.h"

#include <gtest/gtest.h>

namespace CombatTrackerApp {
	class MonsterTest : public testing::Test {
	protected:
		MonsterTest();
		MonsterStatBlock& getMonsterBlock();

		static int findXP(double CR);

		Monster m_mockMonster;
	};
}
