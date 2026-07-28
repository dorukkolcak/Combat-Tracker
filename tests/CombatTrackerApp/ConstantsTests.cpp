#include <gtest/gtest.h>

#include <cstddef>

#include "Entities/Creature/CreatureConstants.h"
#include "Entities/Monster/MonsterConstants.h"
#include "Entities/PlayerCharacter/PlayerCharacterConstants.h"
#include "Utilities/GenericUtils.h"

using namespace CombatTrackerApp;

namespace {

	/*
	A name table is sized from the COUNT enumerator of its enum, but a table with too few initializers is still legal:
	the entries that were left out are silently empty. The UI walks these tables by position, so a gap shows up
	as a blank checkbox or a blank listbox row that still selects a real enumerator.
	*/
	template <typename T>
	void expectEveryEntryNamed(const T& names) {
		for (std::size_t i{}; i < names.size(); ++i) {
			EXPECT_FALSE(names[i].empty()) << "unnamed entry at position " << i;
		}
	}
}

TEST(CreatureConstantsTests, EveryNameTableIsFullyPopulated) {
	expectEveryEntryNamed(SIZE_NAMES);
	expectEveryEntryNamed(CREATURE_TYPE_NAMES);
	expectEveryEntryNamed(ALIGNMENT_NAMES);
	expectEveryEntryNamed(ABILITY_NAMES);
	expectEveryEntryNamed(SKILL_NAMES);
	expectEveryEntryNamed(DAMAGE_TYPE_NAMES);
	expectEveryEntryNamed(CONDITTON_NAMES);
	expectEveryEntryNamed(LANGUAGE_NAMES);
	expectEveryEntryNamed(SENSE_NAMES);
	expectEveryEntryNamed(ACTION_NAMES);
}

TEST(CreatureConstantsTests, EverySkillIsGovernedByAnAbility) {
	for (std::size_t i{}; i < SKILL_NAMES.size(); ++i) {
		const auto skill{ static_cast<Skill>(i) };
		EXPECT_TRUE(MAPPED_SKILLS.contains(skill)) << "no ability mapped for " << SKILL_NAMES[i];
	}
}

TEST(PlayerCharacterConstantsTests, EveryNameTableIsFullyPopulated) {
	expectEveryEntryNamed(CLASS_NAMES);
	expectEveryEntryNamed(SUBCLASS_NAMES);
	expectEveryEntryNamed(SPECIES_NAMES);
	expectEveryEntryNamed(ORIGIN_NAMES);
	expectEveryEntryNamed(WEAPON_TYPE_NAMES);
	expectEveryEntryNamed(ARMOR_TYPE_NAMES);
	expectEveryEntryNamed(WEAPON_MASTERY_NAMES);
	expectEveryEntryNamed(TOOL_NAMES);
}

// The subclass panel looks a subclass up in one direction and its siblings up in the other, so the two tables have to agree.
TEST(PlayerCharacterConstantsTests, SubclassAndClassMappingsAgree) {
	for (std::size_t i{}; i < SUBCLASS_NAMES.size(); ++i) {
		const auto subclass{ static_cast<PlayerSubclass>(i) };

		ASSERT_TRUE(MAPPED_SUBCLASSES.contains(subclass)) << "no class mapped for " << SUBCLASS_NAMES[i];
		const auto owningClass{ MAPPED_SUBCLASSES.at(subclass) };

		ASSERT_TRUE(MAPPED_CLASSES.contains(owningClass)) << "no subclass set for " << CLASS_NAMES[enumToSizeT(owningClass)];
		EXPECT_TRUE(MAPPED_CLASSES.at(owningClass).contains(subclass))
			<< SUBCLASS_NAMES[i] << " is missing from the subclass set of " << CLASS_NAMES[enumToSizeT(owningClass)];
	}
}

/*
The monster scene indexes both of these maps with the selected CR through at(), which throws when a key is missing.
So every CR the listbox can offer has to be present in both.
*/
TEST(MonsterConstantsTests, EveryChallengeRatingHasXPAndProficiencyBonus) {
	for (const auto& [challengeRating, challengeRatingName] : CR_NAMES) {
		EXPECT_TRUE(MAPPED_CR_TO_XP.contains(challengeRating)) << "no XP mapped for CR " << challengeRatingName;
		EXPECT_TRUE(MAPPED_CR_TO_PB.contains(challengeRating)) << "no proficiency bonus mapped for CR " << challengeRatingName;
	}
}

TEST(GenericUtilsTests, EnumToSizeTKeepsTheDeclarationOrder) {
	EXPECT_EQ(enumToSizeT(Ability::STRENGTH), 0u);
	EXPECT_EQ(enumToSizeT(Ability::CHARISMA), ABILITY_NAMES.size() - 1);
	EXPECT_EQ(enumToSizeT(Skill::ATHLETICS), 0u);
	EXPECT_EQ(enumToSizeT(Skill::PERSUASION), SKILL_NAMES.size() - 1);
}
