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
		{Skill::DECEPTION, Ability::CHARISMA},
		{Skill::INTIMIDATION, Ability::CHARISMA},
		{Skill::PERFORMANCE, Ability::CHARISMA},
		{Skill::PERSUASION, Ability::CHARISMA}
	};
	inline constexpr int ABILITY_SCORE_UPPER_BOUND{ 30 };
	inline const std::unordered_map<Size, std::string> SIZE_NAMES {
		{Size::TINY, "Tiny"},
		{Size::SMALL, "Small"},
		{Size::MEDIUM, "Medium"},
		{Size::LARGE, "Large"},
		{Size::HUGE, "Huge"},
		{Size::GARGANTUAN, "Gargantuan"}
	};
	inline const std::unordered_map<CreatureType, std::string> CREATURE_TYPE_NAMES{
		{CreatureType::ABERRATION, "Aberration"},
		{CreatureType::BEAST, "Beast"},
		{CreatureType::CELESTIAL, "Celestial"},
		{CreatureType::CONSTRUCT, "Construct"},
		{CreatureType::DRAGON, "Dragon"},
		{CreatureType::ELEMENTAL, "Elemental"},
		{CreatureType::FEY, "Fey"},
		{CreatureType::FIEND, "Fiend"},
		{CreatureType::GIANT, "Giant"},
		{CreatureType::HUMANOID, "Humanoid"},
		{CreatureType::MONSTROSITY, "Monstrosity"},
		{CreatureType::OOZE, "Ooze"},
		{CreatureType::PLANT, "Plant"},
		{CreatureType::UNDEAD, "Undead"}
	};
	inline const std::unordered_map<Alignment, std::string> ALIGNMENT_NAMES {
		{Alignment::LAWFUL_GOOD, "Lawful Good"},
		{Alignment::NEUTRAL_GOOD, "Neutral Good"},
		{Alignment::CHAOTIC_GOOD, "Chaotic Good"},
		{Alignment::LAWFUL_NEUTRAL, "Lawful Neutral"},
		{Alignment::NEUTRAL, "Neutral"},
		{Alignment::CHAOTIC_NEUTRAL, "Chaotic Neutral"},
		{Alignment::LAWFUL_EVIL, "Lawful Evil"},
		{Alignment::NEUTRAL_EVIL, "Neutral Evil"},
		{Alignment::CHAOTIC_EVIL, "Chaotic Evil"},
		{Alignment::UNALIGNED, "Unaligned"},
	};
	inline const std::unordered_map<Ability, std::string> ABILITY_NAMES {
		{Ability::STRENGTH, "Strength"},
		{Ability::DEXTERITY, "Dexterity"},
		{Ability::CONSTITUTION, "Constitution"},
		{Ability::WISDOM, "Wisdom"},
		{Ability::INTELLIGENCE, "Intelligence"},
		{Ability::CHARISMA, "Charisma"}
	};
	inline const std::unordered_map<Skill, std::string> SKILL_NAMES{
		{Skill::ATHLETICS, "Athletics"},
		{Skill::ACROBATICS, "Acrobatics"},
		{Skill::SLEIGHT_OF_HAND, "Sleight of Hand"},
		{Skill::STEALTH, "Stealth"},
		{Skill::ARCANA, "Arcana"},
		{Skill::HISTORY, "History"},
		{Skill::INVESTIGATION, "Investigation"},
		{Skill::NATURE, "Nature"},
		{Skill::RELIGION, "Religion"},
		{Skill::ANIMAL_HANDLING, "Animal Handling"},
		{Skill::INSIGHT, "Insight"},
		{Skill::MEDICINE, "Medicine"},
		{Skill::PERCEPTION, "Perception"},
		{Skill::SURVIVAL, "Survival"},
		{Skill::DECEPTION, "Deception"},
		{Skill::INTIMIDATION, "Intimidation"},
		{Skill::PERFORMANCE, "Performance"},
		{Skill::PERSUASION, "Persuasion"}
	};
}
