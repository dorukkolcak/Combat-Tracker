#include <gtest/gtest.h>

#include "Entities/Creature/Creature.h"
#include "Entities/Creature/CreatureConstants.h"

using namespace CombatTrackerApp;

/*
The modifier of an ability score is the score minus ten, halved and rounded down.
Rounding down is what the odd scores below ten are here for: integer division on its own truncates towards zero,
which would report a score of 9 as a modifier of 0 rather than -1.
*/
TEST(CreatureTests, AbilityScoreModifierRoundsDown) {
	EXPECT_EQ(Creature::findAbilityScoreModifier(10), 0);
	EXPECT_EQ(Creature::findAbilityScoreModifier(11), 0);
	EXPECT_EQ(Creature::findAbilityScoreModifier(12), 1);
	EXPECT_EQ(Creature::findAbilityScoreModifier(20), 5);
	EXPECT_EQ(Creature::findAbilityScoreModifier(ABILITY_SCORE_UPPER_BOUND), 10);

	EXPECT_EQ(Creature::findAbilityScoreModifier(9), -1);
	EXPECT_EQ(Creature::findAbilityScoreModifier(8), -1);
	EXPECT_EQ(Creature::findAbilityScoreModifier(7), -2);
	EXPECT_EQ(Creature::findAbilityScoreModifier(3), -4);
	EXPECT_EQ(Creature::findAbilityScoreModifier(ABILITY_SCORE_LOWER_BOUND), -5);
}

// The modifier of any score the creation scene allows has to stay inside the bounds the same scene clamps to.
TEST(CreatureTests, AbilityScoreModifierStaysInsideItsBounds) {
	for (int abilityScore{ ABILITY_SCORE_LOWER_BOUND }; abilityScore <= ABILITY_SCORE_UPPER_BOUND; ++abilityScore) {
		const int modifier{ Creature::findAbilityScoreModifier(abilityScore) };
		EXPECT_GE(modifier, ABILITY_MODIFIERS_LOWER_BOUND) << "for a score of " << abilityScore;
		EXPECT_LE(modifier, ABILITY_MODIFIERS_UPPER_BOUND) << "for a score of " << abilityScore;
	}
}

// A fresh block is what every creation scene starts from and returns to, so its defaults are part of the behaviour.
TEST(CreatureTests, DefaultStatBlockStartsEveryAbilityAtTen) {
	const CreatureStatBlock statBlock{};
	for (std::size_t i{}; i < statBlock.abilityScores.size(); ++i) {
		EXPECT_EQ(statBlock.abilityScores[i], 10) << "for " << ABILITY_NAMES[i];
	}
}

TEST(CreatureTests, DefaultStatBlockHasNothingSelected) {
	const CreatureStatBlock statBlock{};

	for (const auto& isProficient : statBlock.savingThrowProficiencies) {
		EXPECT_FALSE(isProficient);
	}
	for (const auto& isProficient : statBlock.skillProficiencies) {
		EXPECT_FALSE(isProficient);
	}
	for (const auto& isImmune : statBlock.conditionImmunities) {
		EXPECT_FALSE(isImmune);
	}
	for (const auto& senseRange : statBlock.senses) {
		EXPECT_EQ(senseRange, 0);
	}

	EXPECT_TRUE(statBlock.actions.empty());
	EXPECT_TRUE(statBlock.traits.empty());
	EXPECT_TRUE(statBlock.equipment.empty());
	EXPECT_TRUE(statBlock.name.empty());
	EXPECT_FALSE(statBlock.initiativeBonus.isOverridden);
}
