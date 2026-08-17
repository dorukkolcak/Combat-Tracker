#include "tests/CombatTrackerApp/CreatureTest.h"
#include "CombatTrackerLib/Utilities/GenericUtils.h"

#include <array>
#include <cmath>
#include <cstddef>
#include <utility>

namespace CombatTrackerApp {
	CreatureTest::CreatureTest() : m_mockCreature{ CreatureStatBlock{} } {}

	CreatureStatBlock& CreatureTest::getCreatureBlock() {
		return m_mockCreature.m_statBlockCreaturePortion;
	}

	// Just amazing stuff right here.
	void CreatureTest::resolveCreatureDerived(CreatureStatBlock& statBlock, double proficiencyTier) {
		Creature::resolveDerived(statBlock, proficiencyTier);
	}
	int CreatureTest::findProficiencyBonus(double proficiencyTier) {
		return Creature::findProficiencyBonus(proficiencyTier);
	}
	int CreatureTest::findAbilityScoreModifier(int abilityScore) {
		return Creature::findAbilityScoreModifier(abilityScore);
	}
	TEST_F(CreatureTest, TestSkillMapping) {
		constexpr std::array<
			std::pair<Skill, Ability>,
			CombatTrackerLib::enumToSizeT(Skill::COUNT)
		> expectedMappings{ {
			{ Skill::ATHLETICS, Ability::STRENGTH },
			{ Skill::ACROBATICS, Ability::DEXTERITY },
			{ Skill::SLEIGHT_OF_HAND, Ability::DEXTERITY },
			{ Skill::STEALTH, Ability::DEXTERITY },
			{ Skill::ARCANA, Ability::INTELLIGENCE },
			{ Skill::HISTORY, Ability::INTELLIGENCE },
			{ Skill::INVESTIGATION, Ability::INTELLIGENCE },
			{ Skill::NATURE, Ability::INTELLIGENCE },
			{ Skill::RELIGION, Ability::INTELLIGENCE },
			{ Skill::ANIMAL_HANDLING, Ability::WISDOM },
			{ Skill::INSIGHT, Ability::WISDOM },
			{ Skill::MEDICINE, Ability::WISDOM },
			{ Skill::PERCEPTION, Ability::WISDOM },
			{ Skill::SURVIVAL, Ability::WISDOM },
			{ Skill::DECEPTION, Ability::CHARISMA },
			{ Skill::INTIMIDATION, Ability::CHARISMA },
			{ Skill::PERFORMANCE, Ability::CHARISMA },
			{ Skill::PERSUASION, Ability::CHARISMA }
		} };
		ASSERT_EQ(expectedMappings.size(), MAPPED_SKILLS.size());
		for (const auto& pair : expectedMappings) {
			EXPECT_EQ(pair.second, MAPPED_SKILLS[CombatTrackerLib::enumToSizeT(pair.first)])
				<< "Skill " << CombatTrackerLib::enumToSizeT(pair.first) << " is backed by the wrong ability." << std::endl;
		}
	}
	TEST_F(CreatureTest, TestPBCalculation) {
		EXPECT_EQ(PB_LOWER_BOUND, findProficiencyBonus(0.0));
		EXPECT_EQ(PB_LOWER_BOUND, findProficiencyBonus(0.125));
		EXPECT_EQ(PB_LOWER_BOUND, findProficiencyBonus(0.25));
		EXPECT_EQ(PB_LOWER_BOUND, findProficiencyBonus(0.5));
		EXPECT_EQ(2, findProficiencyBonus(1.0));
		EXPECT_EQ(2, findProficiencyBonus(4.0));
		EXPECT_EQ(3, findProficiencyBonus(5.0));
		EXPECT_EQ(3, findProficiencyBonus(8.0));
		EXPECT_EQ(4, findProficiencyBonus(9.0));
		EXPECT_EQ(4, findProficiencyBonus(12.0));
		EXPECT_EQ(5, findProficiencyBonus(13.0));
		EXPECT_EQ(5, findProficiencyBonus(16.0));
		EXPECT_EQ(6, findProficiencyBonus(17.0));
		EXPECT_EQ(6, findProficiencyBonus(20.0));
		EXPECT_EQ(7, findProficiencyBonus(21.0));
		EXPECT_EQ(7, findProficiencyBonus(24.0));
		EXPECT_EQ(8, findProficiencyBonus(25.0));
		EXPECT_EQ(8, findProficiencyBonus(28.0));
		EXPECT_EQ(9, findProficiencyBonus(29.0));
		EXPECT_EQ(PB_UPPER_BOUND, findProficiencyBonus(30.0));

		auto& statBlock{ getCreatureBlock() };
		resolveCreatureDerived(statBlock, 1000.0);
		EXPECT_EQ(PB_UPPER_BOUND, statBlock.derived.getProficiencyBonus());
		resolveCreatureDerived(statBlock, -1000.0);
		EXPECT_EQ(PB_LOWER_BOUND, statBlock.derived.getProficiencyBonus());
	}
	TEST_F(CreatureTest, TestAbilityScoreModifierCalculation) {
		EXPECT_EQ(ABILITY_MODIFIERS_LOWER_BOUND, findAbilityScoreModifier(ABILITY_SCORE_LOWER_BOUND));
		EXPECT_EQ(ABILITY_MODIFIERS_UPPER_BOUND, findAbilityScoreModifier(ABILITY_SCORE_UPPER_BOUND));
		EXPECT_EQ(-4, findAbilityScoreModifier(3));
		EXPECT_EQ(-1, findAbilityScoreModifier(8));
		EXPECT_EQ(-1, findAbilityScoreModifier(9));
		EXPECT_EQ(0, findAbilityScoreModifier(10));
		EXPECT_EQ(0, findAbilityScoreModifier(11));
		EXPECT_EQ(1, findAbilityScoreModifier(12));
		EXPECT_EQ(5, findAbilityScoreModifier(20));

		auto& statBlock{ getCreatureBlock() };
		statBlock.abilityScores[CombatTrackerLib::enumToSizeT(Ability::STRENGTH)] = ABILITY_SCORE_UPPER_BOUND + 20;
		statBlock.abilityScores[CombatTrackerLib::enumToSizeT(Ability::DEXTERITY)] = ABILITY_SCORE_LOWER_BOUND - 20;
		resolveCreatureDerived(statBlock, 1.0);
		EXPECT_EQ(ABILITY_MODIFIERS_UPPER_BOUND, statBlock.derived.getAbilityScoreModifier(Ability::STRENGTH));
		EXPECT_EQ(ABILITY_MODIFIERS_LOWER_BOUND, statBlock.derived.getAbilityScoreModifier(Ability::DEXTERITY));
	}
	TEST_F(CreatureTest, TestSavingThrowModifierCalculation) {
		auto& statBlock{ getCreatureBlock() };
		constexpr std::array<int, CombatTrackerLib::enumToSizeT(Ability::COUNT)> abilityScores{ 10, 14, 18, 7, 3, 20 };
		constexpr std::array<
			ProficiencyLevel,
			CombatTrackerLib::enumToSizeT(Ability::COUNT)
		> proficiencyLevels{
			ProficiencyLevel::NONE,
			ProficiencyLevel::PROFICIENT,
			ProficiencyLevel::EXPERT,
			ProficiencyLevel::NONE,
			ProficiencyLevel::PROFICIENT,
			ProficiencyLevel::EXPERT
		};
		constexpr std::array<int, CombatTrackerLib::enumToSizeT(Ability::COUNT)> expectedModifiers{ 0, 5, 10, -2, -1, 11 };

		for (std::size_t abilityIndex{}; abilityIndex < CombatTrackerLib::enumToSizeT(Ability::COUNT); ++abilityIndex) {
			statBlock.abilityScores[abilityIndex] = abilityScores[abilityIndex];
			statBlock.savingThrowProficiencyLevels[abilityIndex] = proficiencyLevels[abilityIndex];
		}
		resolveCreatureDerived(statBlock, 5.0);
		ASSERT_EQ(3, statBlock.derived.getProficiencyBonus());
		for (std::size_t abilityIndex{}; abilityIndex < CombatTrackerLib::enumToSizeT(Ability::COUNT); ++abilityIndex) {
			EXPECT_EQ(expectedModifiers[abilityIndex], statBlock.savingThrowModifiers[abilityIndex].get())
				<< "Ability " << abilityIndex << " has the wrong saving throw modifier." << std::endl;
		}
	}
	TEST_F(CreatureTest, TestSkillModifierCalculation) {
		auto& statBlock{ getCreatureBlock() };
		constexpr std::array<int, CombatTrackerLib::enumToSizeT(Ability::COUNT)> abilityScores{ 10, 14, 18, 7, 3, 20 };
		constexpr std::array<ProficiencyLevel, 3> cyclingProficiencyLevels{
			ProficiencyLevel::NONE,
			ProficiencyLevel::PROFICIENT,
			ProficiencyLevel::EXPERT
		};
		for (std::size_t abilityIndex{}; abilityIndex < CombatTrackerLib::enumToSizeT(Ability::COUNT); ++abilityIndex) {
			statBlock.abilityScores[abilityIndex] = abilityScores[abilityIndex];
		}
		for (std::size_t skillIndex{}; skillIndex < CombatTrackerLib::enumToSizeT(Skill::COUNT); ++skillIndex) {
			statBlock.skillProficiencyLevels[skillIndex] = cyclingProficiencyLevels[skillIndex % cyclingProficiencyLevels.size()];
		}

		resolveCreatureDerived(statBlock, 9.0);
		ASSERT_EQ(4, statBlock.derived.getProficiencyBonus());
		EXPECT_EQ(0, statBlock.skillModifiers[CombatTrackerLib::enumToSizeT(Skill::ATHLETICS)].get());
		EXPECT_EQ(6, statBlock.skillModifiers[CombatTrackerLib::enumToSizeT(Skill::ACROBATICS)].get());
		EXPECT_EQ(10, statBlock.skillModifiers[CombatTrackerLib::enumToSizeT(Skill::SLEIGHT_OF_HAND)].get());
		EXPECT_EQ(2, statBlock.skillModifiers[CombatTrackerLib::enumToSizeT(Skill::STEALTH)].get());
		EXPECT_EQ(-2, statBlock.skillModifiers[CombatTrackerLib::enumToSizeT(Skill::PERCEPTION)].get());
		EXPECT_EQ(13, statBlock.skillModifiers[CombatTrackerLib::enumToSizeT(Skill::PERSUASION)].get());
	}
	TEST_F(CreatureTest, TestPassivePerceptionCalculation) {
		auto& statBlock{ getCreatureBlock() };
		const auto perceptionIndex{ CombatTrackerLib::enumToSizeT(Skill::PERCEPTION) };
		statBlock.abilityScores[CombatTrackerLib::enumToSizeT(Ability::WISDOM)] = 16;
		statBlock.skillProficiencyLevels[perceptionIndex] = ProficiencyLevel::PROFICIENT;

		resolveCreatureDerived(statBlock, 5.0);
		ASSERT_EQ(6, statBlock.skillModifiers[perceptionIndex].get());
		EXPECT_EQ(PASSIVE_PERCEPTION_BASE + 6, statBlock.passivePerception.get());

		statBlock.skillModifiers[perceptionIndex].setOverride(11);
		resolveCreatureDerived(statBlock, 5.0);
		EXPECT_EQ(PASSIVE_PERCEPTION_BASE + 11, statBlock.passivePerception.get());

		statBlock.passivePerception.setOverride(30);
		resolveCreatureDerived(statBlock, 5.0);
		EXPECT_EQ(30, statBlock.passivePerception.get());
		EXPECT_EQ(PASSIVE_PERCEPTION_BASE + 11, statBlock.passivePerception.getDefault());
	}
	TEST_F(CreatureTest, TestInitiativeBonusCalculation) {
		auto& statBlock{ getCreatureBlock() };
		statBlock.abilityScores[CombatTrackerLib::enumToSizeT(Ability::DEXTERITY)] = 18;

		resolveCreatureDerived(statBlock, 20.0);
		ASSERT_EQ(6, statBlock.derived.getProficiencyBonus());
		EXPECT_EQ(4, statBlock.initiativeBonus.get());
		EXPECT_EQ(statBlock.derived.getAbilityScoreModifier(Ability::DEXTERITY), statBlock.initiativeBonus.get());

		statBlock.savingThrowProficiencyLevels[CombatTrackerLib::enumToSizeT(Ability::DEXTERITY)] = ProficiencyLevel::EXPERT;
		statBlock.skillProficiencyLevels[CombatTrackerLib::enumToSizeT(Skill::ACROBATICS)] = ProficiencyLevel::EXPERT;
		resolveCreatureDerived(statBlock, 20.0);
		EXPECT_EQ(4, statBlock.initiativeBonus.get());

		statBlock.initiativeBonus.setOverride(-3);
		resolveCreatureDerived(statBlock, 20.0);
		EXPECT_EQ(-3, statBlock.initiativeBonus.get());
		EXPECT_EQ(4, statBlock.initiativeBonus.getDefault());
	}
	TEST_F(CreatureTest, TestDerivedMemberResolution) {
		auto& statBlock{ getCreatureBlock() };
		const auto strengthIndex{ CombatTrackerLib::enumToSizeT(Ability::STRENGTH) };
		const auto perceptionIndex{ CombatTrackerLib::enumToSizeT(Skill::PERCEPTION) };
		statBlock.abilityScores[strengthIndex] = 16;
		statBlock.abilityScores[CombatTrackerLib::enumToSizeT(Ability::WISDOM)] = 12;
		statBlock.savingThrowProficiencyLevels[strengthIndex] = ProficiencyLevel::PROFICIENT;
		statBlock.skillProficiencyLevels[perceptionIndex] = ProficiencyLevel::EXPERT;
		statBlock.savingThrowModifiers[strengthIndex].setOverride(20);
		statBlock.skillModifiers[perceptionIndex].setOverride(1);
		resolveCreatureDerived(statBlock, 13.0);

		ASSERT_EQ(5, statBlock.derived.getProficiencyBonus());
		EXPECT_EQ(3, statBlock.derived.getAbilityScoreModifier(Ability::STRENGTH));
		EXPECT_EQ(1, statBlock.derived.getAbilityScoreModifier(Ability::WISDOM));
		EXPECT_EQ(20, statBlock.savingThrowModifiers[strengthIndex].get());
		EXPECT_EQ(8, statBlock.savingThrowModifiers[strengthIndex].getDefault());
		EXPECT_EQ(1, statBlock.skillModifiers[perceptionIndex].get());
		EXPECT_EQ(11, statBlock.skillModifiers[perceptionIndex].getDefault());
		EXPECT_EQ(PASSIVE_PERCEPTION_BASE + 1, statBlock.passivePerception.get());

		statBlock.savingThrowModifiers[strengthIndex].clearOverride();
		statBlock.skillModifiers[perceptionIndex].clearOverride();
		resolveCreatureDerived(statBlock, 13.0);
		EXPECT_EQ(8, statBlock.savingThrowModifiers[strengthIndex].get());
		EXPECT_EQ(11, statBlock.skillModifiers[perceptionIndex].get());
		EXPECT_EQ(PASSIVE_PERCEPTION_BASE + 11, statBlock.passivePerception.get());

		statBlock.abilityScores[strengthIndex] = 8;
		resolveCreatureDerived(statBlock, 1.0);
		ASSERT_EQ(2, statBlock.derived.getProficiencyBonus());
		EXPECT_EQ(-1, statBlock.derived.getAbilityScoreModifier(Ability::STRENGTH));
		EXPECT_EQ(1, statBlock.savingThrowModifiers[strengthIndex].get());
		EXPECT_EQ(5, statBlock.skillModifiers[perceptionIndex].get());
		EXPECT_EQ(PASSIVE_PERCEPTION_BASE + 5, statBlock.passivePerception.get());
	}
}
