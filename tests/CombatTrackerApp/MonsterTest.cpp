#include "tests/CombatTrackerApp/MonsterTest.h"

#include <array>
#include <cstddef>

namespace CombatTrackerApp {
	MonsterTest::MonsterTest() : m_mockMonster{CreatureStatBlock{}, 0.0} {}

	MonsterStatBlock& MonsterTest::getMonsterBlock() {
		return m_mockMonster.m_statBlockMonsterPortion;
	}

	int MonsterTest::findXP(double CR) {
		return Monster::findXP(CR);
	}

	TEST_F(MonsterTest, TestCRMapping) {
		constexpr std::array<ChallengeRating, 34> expectedChallengeRatings{
			{{0.0, 10},     {0.125, 25},   {0.25, 50},     {0.5, 100},     {1.0, 200},     {2.0, 450},    {3.0, 700},
		     {4.0, 1100},   {5.0, 1800},   {6.0, 2300},    {7.0, 2900},    {8.0, 3900},    {9.0, 5000},   {10.0, 5900},
		     {11.0, 7200},  {12.0, 8400},  {13.0, 10000},  {14.0, 11500},  {15.0, 13000},  {16.0, 15000}, {17.0, 18000},
		     {18.0, 20000}, {19.0, 22000}, {20.0, 25000},  {21.0, 33000},  {22.0, 41000},  {23.0, 50000}, {24.0, 62000},
		     {25.0, 75000}, {26.0, 90000}, {27.0, 105000}, {28.0, 120000}, {29.0, 135000}, {30.0, 155000}}
		};
		ASSERT_EQ(expectedChallengeRatings.size(), CHALLENGE_RATINGS.size());
		for (std::size_t ratingIndex{}; ratingIndex < expectedChallengeRatings.size(); ++ratingIndex) {
			const auto& expectedChallengeRating{expectedChallengeRatings[ratingIndex]};
			EXPECT_DOUBLE_EQ(expectedChallengeRating.value, CHALLENGE_RATINGS[ratingIndex].value)
				<< "Challenge rating " << ratingIndex << " is out of order." << std::endl;
			EXPECT_EQ(expectedChallengeRating.XP, findXP(expectedChallengeRating.value))
				<< "Challenge rating " << expectedChallengeRating.value << " maps to the wrong XP." << std::endl;
		}

		EXPECT_EQ(expectedChallengeRatings.front().XP, findXP(0.75));
		EXPECT_EQ(expectedChallengeRatings.front().XP, findXP(-1.0));
		EXPECT_EQ(expectedChallengeRatings.front().XP, findXP(31.0));
	}

	TEST_F(MonsterTest, TestDerivedMemberResolution) {
		EXPECT_DOUBLE_EQ(CHALLENGE_RATINGS.front().value, m_mockMonster.getCR());
		EXPECT_EQ(CHALLENGE_RATINGS.front().XP, m_mockMonster.getXP());

		CreatureStatBlock creatureBlock{};
		getMonsterBlock().CR = 9.0;
		Monster::resolveDerived(creatureBlock, getMonsterBlock());
		EXPECT_DOUBLE_EQ(9.0, m_mockMonster.getCR());
		EXPECT_EQ(5000, m_mockMonster.getXP());
		EXPECT_EQ(4, creatureBlock.derived.getProficiencyBonus());

		getMonsterBlock().CR = 0.5;
		Monster::resolveDerived(creatureBlock, getMonsterBlock());
		EXPECT_EQ(100, m_mockMonster.getXP());
		EXPECT_EQ(PB_LOWER_BOUND, creatureBlock.derived.getProficiencyBonus());

		getMonsterBlock().CR = 0.75;
		Monster::resolveDerived(creatureBlock, getMonsterBlock());
		EXPECT_EQ(CHALLENGE_RATINGS.front().XP, m_mockMonster.getXP());
	}
}
