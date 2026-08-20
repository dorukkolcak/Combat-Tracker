#pragma once

#include "CombatTrackerApp/Entities/Creature.h"

#include <gtest/gtest.h>

namespace CombatTrackerApp {
	class CreatureTest : public testing::Test {
	protected:
		CreatureTest();
		CreatureStatBlock& getCreatureBlock();

		// Friendship isn't inherited so we create protected versions for the CreatureTest
		// That call the same function in Creature. I love C++!
		static void resolveCreatureDerived(CreatureStatBlock& statBlock, double proficiencyTier);
		static int findProficiencyBonus(double proficiencyTier);
		static int findAbilityScoreModifier(int abilityScore);

		Creature m_mockCreature;
	};
}
