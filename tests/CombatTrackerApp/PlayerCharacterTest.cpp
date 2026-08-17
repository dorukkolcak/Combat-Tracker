#include "tests/CombatTrackerApp/PlayerCharacterTest.h"

#include <array>
#include <cstddef>

namespace CombatTrackerApp {
	PlayerCharacterTest::PlayerCharacterTest() : m_mockPlayerCharacter{ CreatureStatBlock{}, PlayerStatBlock{} } {}
	PlayerStatBlock& PlayerCharacterTest::getPlayerBlock() {
		return m_mockPlayerCharacter.m_statBlockPlayerPortion;
	}
	TEST_F(PlayerCharacterTest, TestSubclassMapping) {
		ASSERT_EQ(CombatTrackerLib::enumToSizeT(PlayerSubclass::COUNT), SUBCLASSES_TO_CLASSES.size());
		EXPECT_EQ(PlayerClass::BARBARIAN, SUBCLASSES_TO_CLASSES[CombatTrackerLib::enumToSizeT(PlayerSubclass::PATH_OF_THE_BERSERKER)]);
		EXPECT_EQ(PlayerClass::CLERIC, SUBCLASSES_TO_CLASSES[CombatTrackerLib::enumToSizeT(PlayerSubclass::LIFE_DOMAIN)]);
		EXPECT_EQ(PlayerClass::FIGHTER, SUBCLASSES_TO_CLASSES[CombatTrackerLib::enumToSizeT(PlayerSubclass::PSI_WARRIOR)]);
		EXPECT_EQ(PlayerClass::ROGUE, SUBCLASSES_TO_CLASSES[CombatTrackerLib::enumToSizeT(PlayerSubclass::ARCANE_TRICKSTER)]);
		EXPECT_EQ(PlayerClass::WIZARD, SUBCLASSES_TO_CLASSES[CombatTrackerLib::enumToSizeT(PlayerSubclass::ILLUSIONIST)]);
		// ...
	}
	TEST_F(PlayerCharacterTest, TestPlayerCharacterValidation) {
		const CreatureStatBlock creatureBlock{};
		auto& playerBlock{ getPlayerBlock() };
		const auto barbarianIndex{ CombatTrackerLib::enumToSizeT(PlayerClass::BARBARIAN) };
		const auto fighterIndex{ CombatTrackerLib::enumToSizeT(PlayerClass::FIGHTER) };

		EXPECT_FALSE(PlayerCharacter::isValid(creatureBlock, playerBlock));

		playerBlock.levels[fighterIndex] = MAX_TOTAL_CLASS_LEVEL;
		EXPECT_TRUE(PlayerCharacter::isValid(creatureBlock, playerBlock));

		playerBlock.levels[barbarianIndex] = 1;
		EXPECT_FALSE(PlayerCharacter::isValid(creatureBlock, playerBlock));

		playerBlock.levels[fighterIndex] = MAX_TOTAL_CLASS_LEVEL + 100;
		EXPECT_EQ(MAX_TOTAL_CLASS_LEVEL, playerBlock.levels[fighterIndex].get());

		playerBlock.levels[fighterIndex] = SUBCLASS_LEVEL_THRESHOLD - 1;
		playerBlock.levels[barbarianIndex] = MIN_CLASS_LEVEL;
		playerBlock.subclasses[fighterIndex] = PlayerSubclass::CHAMPION;
		EXPECT_FALSE(PlayerCharacter::isValid(creatureBlock, playerBlock));

		playerBlock.levels[fighterIndex] = SUBCLASS_LEVEL_THRESHOLD;
		EXPECT_TRUE(PlayerCharacter::isValid(creatureBlock, playerBlock));

		playerBlock.subclasses[barbarianIndex] = PlayerSubclass::PATH_OF_THE_ZEALOT;
		EXPECT_FALSE(PlayerCharacter::isValid(creatureBlock, playerBlock));
	}
	TEST_F(PlayerCharacterTest, TestDerivedMemberResolution) {
		CreatureStatBlock creatureBlock{};
		auto& playerBlock{ getPlayerBlock() };
		const auto fighterIndex{ CombatTrackerLib::enumToSizeT(PlayerClass::FIGHTER) };
		const auto rogueIndex{ CombatTrackerLib::enumToSizeT(PlayerClass::ROGUE) };
		const auto wizardIndex{ CombatTrackerLib::enumToSizeT(PlayerClass::WIZARD) };

		creatureBlock.abilityScores[CombatTrackerLib::enumToSizeT(Ability::DEXTERITY)] = 14;
		playerBlock.levels[fighterIndex] = 5;
		playerBlock.levels[rogueIndex] = 4;
		playerBlock.subclasses[fighterIndex] = PlayerSubclass::CHAMPION;
		playerBlock.subclasses[rogueIndex] = PlayerSubclass::THIEF;
		playerBlock.subclasses[wizardIndex] = PlayerSubclass::EVOKER;
		PlayerCharacter::resolveDerived(creatureBlock, playerBlock);

		EXPECT_EQ(9, playerBlock.derived.getTotalClassLevel());
		EXPECT_EQ(4, creatureBlock.derived.getProficiencyBonus());
		EXPECT_EQ(2, creatureBlock.initiativeBonus.get());
		EXPECT_TRUE(playerBlock.subclasses[fighterIndex].has_value());
		EXPECT_TRUE(playerBlock.subclasses[rogueIndex].has_value());
		EXPECT_FALSE(playerBlock.subclasses[wizardIndex].has_value());

		playerBlock.levels[rogueIndex] = SUBCLASS_LEVEL_THRESHOLD - 1;
		PlayerCharacter::resolveDerived(creatureBlock, playerBlock);
		EXPECT_EQ(7, playerBlock.derived.getTotalClassLevel());
		EXPECT_EQ(3, creatureBlock.derived.getProficiencyBonus());
		EXPECT_TRUE(playerBlock.subclasses[fighterIndex].has_value());
		EXPECT_FALSE(playerBlock.subclasses[rogueIndex].has_value());

		playerBlock.levels[fighterIndex] = MIN_CLASS_LEVEL;
		playerBlock.levels[rogueIndex] = MIN_CLASS_LEVEL;
		PlayerCharacter::resolveDerived(creatureBlock, playerBlock);
		EXPECT_EQ(MIN_CLASS_LEVEL, playerBlock.derived.getTotalClassLevel());
		EXPECT_EQ(PB_LOWER_BOUND, creatureBlock.derived.getProficiencyBonus());
		EXPECT_FALSE(playerBlock.subclasses[fighterIndex].has_value());
	}
}
