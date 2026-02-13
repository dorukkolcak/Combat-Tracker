#pragma once

#include <unordered_map>

#include "CreatureEnums.h"

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
		{Skill::INTIMIDATION, Ability::CHARISMA},
		{Skill::PERFORMANCE, Ability::CHARISMA},
		{Skill::PERSUASION, Ability::CHARISMA}
	};
	inline constexpr std::uint16_t ABILITY_SCORE_UPPER_BOUND { 30 };
}
