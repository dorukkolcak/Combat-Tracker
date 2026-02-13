#pragma once

#include <cstdint>
#include <string>

// It is actually quite important that these are enum classes, not plain enums.
// If these were plain enums, implicit conversion of enum values would be an absolute nightmare to track.
namespace CombatTrackerApp {
	enum class Size {
		TINY,
		SMALL,
		MEDIUM,
		LARGE,
		HUGE,
		GARGANTUAN
	};
	enum class Alignment {
		LAWFUL_GOOD,
		NEUTRAL_GOOD,
		CHAOTIC_GOOD,
		LAWFUL_NEUTRAL,
		NEUTRAL,
		CHAOTIC_NEUTRAL,
		LAWFUL_EVIL,
		NEUTRAL_EVIL,
		CHAOTIC_EVIL,
		UNALIGNED
	};
	enum class ActionType {
		ACTION,
		BONUS_ACTION,
		REACTION,
		LEGENDARY_ACTION, // Should only be available to Monster. 
		LAIR_ACTION // Should only be available to Monster.
		/*
		Unleash Actions need a lot more custom functionality compared to other actions.
		Action Progress Bar, Immediate Trigger support
		References to Progressive Actions (Monster Actions that contribute progress to Unleash Action's trigger.)
		Should only be available to Monster.
		*/
		//UNLEASH_ACTION
	};
	enum class Skill {
		ATHLETICS,
		ACROBATICS,
		SLEIGHT_OF_HAND,
		STEALTH,
		ARCANA,
		HISTORY,
		INVESTIGATION,
		NATURE,
		RELIGION,
		ANIMAL_HANDLING,
		INSIGHT,
		MEDICINE,
		PERCEPTION,
		SURVIVAL,
		DECEPTION,
		INTIMIDATION,
		PERFORMANCE,
		PERSUASION,
	};
	enum class Ability {
		STRENGTH,
		DEXTERITY,
		CONSTITUTION,
		WISDOM,
		INTELLIGENCE,
		CHARISMA
	};
	enum class DamageType {
		ACID,
		BLUDGEONING,
		COLD,
		FIRE,
		FORCE,
		LIGHTNING,
		NECROTIC,
		PIERCING,
		POISON,
		PSYCHIC,
		RADIANT,
		SLASHING,
		THUNDER
	};
	enum class Condition {
		BLINDED,
		CHARMED,
		DEAFENED,
		FRIGHTENED,
		GRAPPLED,
		INCAPACITATED,
		INVISIBLE,
		PARALYZED,
		PETRIFIED,
		POISONED,
		PRONE,
		RESTRAINED,
		STUNNED,
		UNCONSCIOUS,
		EXHAUSTION
	};
	enum class Language {
		COMMON,
		COMMON_SIGN_LANGUAGE,
		DWARVISH,
		ELVISH,
		GIANT,
		GNOMISH,
		GOBLIN,
		HALFLING,
		ORC,
		ABYSSAL,
		CELESTIAL,
		DEEP_SPEECH,
		DRUIDIC,
		INFERNAL,
		PRIMORDIAL,
		SYLVAN,
		THIEVES_CANT,
		UNDERCOMMON
	};
	enum class CreatureType {
		ABERRATION,
		BEAST,
		CELESTIAL,
		CONSTRUCT,
		DRAGON,
		ELEMENTAL,
		FEY,
		FIEND,
		GIANT,
		HUMANOID,
		MONSTROSITY,
		OOZE,
		PLANT,
		UNDEAD
	};
	enum class Sense {
		BLINDSIGHT,
		DARKVISION,
		TREMORSENSE,
		TRUESIGHT
	};
}