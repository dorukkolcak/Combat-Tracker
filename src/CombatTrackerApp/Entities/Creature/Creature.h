#pragma once

#include <array>
#include <cstddef>
#include <string>
#include <vector>

#include "CreatureEnums.h"
#include "CreatureTypes.h"


namespace CombatTrackerApp {

	/*
	A creature's name must be unique and its name will be used to refer to the overall creature.
	This also implies that the saved named creatures will have their own unique names.
	This DOESN'T mean that we can't have creatures with the same name in an encounter.
	We can have multiples of the same creature. To refer to each instance of the same creature, we will use IDs.
	An INSTANCE of any creature will have a unique ID, that will be generated at the start of encounter running.
	This ID isn't really used to refer to creature's type/identity, we use its name for this purpose.
	The stat block types shouldn't be nested types as the creation scenes hold one directly.
	Also the struct representing the stat blocks should be an aggregate.
	*/
	struct CreatureStatBlock {
		
		std::array<bool, static_cast<std::size_t>(Ability::COUNT)> savingThrowProficiencies{};
		std::array<bool, static_cast<std::size_t>(Ability::COUNT)> savingThrowExpertise{};
		std::array<bool, static_cast<std::size_t>(Skill::COUNT)> skillProficiencies{};
		std::array<bool, static_cast<std::size_t>(Skill::COUNT)> skillExpertise{};

		std::array<int, static_cast<std::size_t>(Ability::COUNT)> abilityScores{ { 10, 10, 10, 10, 10, 10 } }; // Every ability starts at the default score of 10.
		std::array<OverridableInt, static_cast<std::size_t>(Ability::COUNT)> abilityScoreModifiers{};
		std::array<OverridableInt, static_cast<std::size_t>(Ability::COUNT)> savingThrowModifiers{};
		std::array<OverridableInt, static_cast<std::size_t>(Skill::COUNT)> skillModifiers{};
		std::array<bool, static_cast<std::size_t>(DamageType::COUNT)> damageVulnerabilities{};
		std::array<bool, static_cast<std::size_t>(DamageType::COUNT)> damageResistances{};
		std::array<bool, static_cast<std::size_t>(DamageType::COUNT)> damageImmunities{};
		std::array<bool, static_cast<std::size_t>(Condition::COUNT)> conditionImmunities{};
		std::array<int, static_cast<std::size_t>(Sense::COUNT)> senses{};
		std::array<bool, static_cast<std::size_t>(Language::COUNT)> languages{};

		std::vector<CreatureAction> actions{};
		std::vector<Trait> traits{};
		std::vector<Gear> equipment{};

		std::string name{""};

		Size size{};
		CreatureType type{};
		Alignment alignment{};

		OverridableInt initiativeBonus{};
		OverridableInt proficiencyBonus{};
		OverridableInt passivePerception{};

		int AC{};
		int HP{};
		int walkSpeed{};
		int climbingSpeed{};
		int swimmingSpeed{};
		int burrowSpeed{};
		int flySpeed{};
	};

	class Creature {
	public:
		static constexpr int findAbilityScoreModifier(int abilityScore) {
			return (abilityScore - 10) / 2;
		}
	protected:

		// A creature will own its stat block, hence we use std::move in the constructor.
		// The block itself is filled in by the creation scenes, which hand it over once the entity is completed.
		// Also this constructor should be protected as we don't really want to allow anyone to create a Creature instance.
		explicit Creature(CreatureStatBlock statBlockCreaturePortion);

		CreatureStatBlock m_statBlockCreaturePortion{};
		inline static int currentIDValue{1};
		int m_ID{};
	};
}
