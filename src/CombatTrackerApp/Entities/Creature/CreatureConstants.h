#pragma once

#include "CombatTrackerApp/Entities/Creature/CreatureEnums.h"

#include <array>
#include <cstddef>
#include <unordered_map>
#include <string_view>

namespace CombatTrackerApp {
	inline const std::unordered_map<Skill, Ability> MAPPED_SKILLS {
		{Skill::ATHLETICS, Ability::STRENGTH},
		{Skill::ACROBATICS, Ability::DEXTERITY},
		{Skill::SLEIGHT_OF_HAND, Ability::DEXTERITY},
		{Skill::STEALTH, Ability::DEXTERITY},
		{Skill::ARCANA, Ability::INTELLIGENCE},
		{Skill::HISTORY, Ability::INTELLIGENCE},
		{Skill::INVESTIGATION, Ability::INTELLIGENCE},
		{Skill::NATURE, Ability::INTELLIGENCE},
		{Skill::RELIGION, Ability::INTELLIGENCE},
		{Skill::ANIMAL_HANDLING, Ability::WISDOM},
		{Skill::INSIGHT, Ability::WISDOM},
		{Skill::MEDICINE, Ability::WISDOM},
		{Skill::PERCEPTION, Ability::WISDOM},
		{Skill::SURVIVAL, Ability::WISDOM},
		{Skill::DECEPTION, Ability::CHARISMA},
		{Skill::INTIMIDATION, Ability::CHARISMA},
		{Skill::PERFORMANCE, Ability::CHARISMA},
		{Skill::PERSUASION, Ability::CHARISMA}
	};
	inline constexpr int PB_LOWER_BOUND{ 2 };
	inline constexpr int ABILITY_SCORE_UPPER_BOUND{ 30 };
	inline constexpr int ABILITY_SCORE_LOWER_BOUND{};
	inline constexpr int ABILITY_MODIFIERS_UPPER_BOUND{ 10 };
	inline constexpr int ABILITY_MODIFIERS_LOWER_BOUND{ -5 };
}
