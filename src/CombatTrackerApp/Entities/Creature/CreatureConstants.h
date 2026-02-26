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
	inline constexpr int PB_LOWER_BOUND{ 2 };
	inline constexpr int ABILITY_SCORE_UPPER_BOUND{ 30 };
	inline constexpr int ABILITY_SCORE_LOWER_BOUND{};
	inline constexpr int ABILITY_MODIFIERS_UPPER_BOUND{ 10 };
	inline constexpr int ABILITY_MODIFIERS_LOWER_BOUND{ -5 };
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
	inline const std::unordered_map<DamageType, std::string> DAMAGE_TYPE_NAMES {
		{DamageType::ACID, "Acid"},
		{DamageType::BLUDGEONING, "Bludgeoning"},
		{DamageType::COLD, "Cold"},
		{DamageType::FIRE, "Fire"},
		{DamageType::FORCE, "Force"},
		{DamageType::LIGHTNING, "Lightning"},
		{DamageType::NECROTIC, "Necrotic"},
		{DamageType::PIERCING, "Piercing"},
		{DamageType::POISON, "Poison"},
		{DamageType::PSYCHIC, "Psychic"},
		{DamageType::RADIANT, "Radiant"},
		{DamageType::SLASHING, "Slashing"},
		{DamageType::THUNDER, "Thunder"}
	};
	inline const std::unordered_map<Condition, std::string> CONDITTON_NAMES {
		{Condition::BLINDED, "Blinded"},
		{Condition::CHARMED, "Charmed"},
		{Condition::DEAFENED, "Deafened"},
		{Condition::FRIGHTENED, "Frightened"},
		{Condition::GRAPPLED, "Grappled"},
		{Condition::INCAPACITATED, "Incapacitated"},
		{Condition::INVISIBLE, "Invisible"},
		{Condition::PARALYZED, "Paralyzed"},
		{Condition::PETRIFIED, "Petrified"},
		{Condition::POISONED, "Poisoned"},
		{Condition::PRONE, "Prone"},
		{Condition::RESTRAINED, "Restratined"},
		{Condition::STUNNED, "Stunned"},
		{Condition::UNCONSCIOUS, "Unconscious"},
		{Condition::EXHAUSTION, "Exhaustion"}
	};
	inline const std::unordered_map<Language, std::string> LANGUAGE_NAMES {
		{Language::COMMON, "Common"},
		{Language::COMMON_SIGN_LANGUAGE, "Common Sign Language"},
		{Language::DWARVISH, "Dwarvish"},
		{Language::ELVISH, "Elvish"},
		{Language::GIANT, "Giant"},
		{Language::GNOMISH, "Gnomish"},
		{Language::GOBLIN, "Goblin"},
		{Language::HALFLING, "Halfling"},
		{Language::ORC, "Orc"},
		{Language::ABYSSAL, "Abyssal"},
		{Language::CELESTIAL, "Celestial"},
		{Language::DEEP_SPEECH, "Deep Speech"},
		{Language::DRUIDIC, "Druidic"},
		{Language::INFERNAL, "Infernal"},
		{Language::PRIMORDIAL, "Primordial"},
		{Language::SYLVAN, "Sylvan"},
		{Language::THIEVES_CANT, "Thieves' Cant"},
		{Language::UNDERCOMMON, "Undercommon"}
	};
	inline const std::unordered_map<Sense, std::string> SENSE_NAMES {
		{Sense::BLINDSIGHT, "Blindsight"},
		{Sense::DARKVISION, "Darkvision"},
		{Sense::TREMORSENSE, "Tremorsense"},
		{Sense::TRUESIGHT, "Truesight"}
	};
	inline const std::unordered_map<ActionType, std::string> ACTION_NAMES {
		{ActionType::ACTION, "Action"},
		{ActionType::BONUS_ACTION, "Bonus Action"},
		{ActionType::REACTION, "Reaction"},
		{ActionType::LEGENDARY_ACTION, "Legendary Action"},
		{ActionType::LAIR_ACTION, "Lair Action"}
	};
	inline const std::unordered_map<ActionType, std::string> PLAYER_ACTION_NAMES{
		{ActionType::ACTION, "Action"},
		{ActionType::BONUS_ACTION, "Bonus Action"},
		{ActionType::REACTION, "Reaction"}
	};
}
