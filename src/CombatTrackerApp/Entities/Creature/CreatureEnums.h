#pragma once

// It is actually quite important that these are enum classes, not plain enums.
// If these were plain enums, implicit conversion of enum values would be an absolute nightmare to track.
// For all of the scoped enumerations, the default/invalid value should be the first enumerator (integral value zero).
// This causes any zero-initialized enum types to be initialized to the default/invalid value.
namespace CombatTrackerApp {
	enum class Size {
		NO_SIZE,
		TINY,
		SMALL,
		MEDIUM,
		LARGE,
		HUGE_SIZE, // Not HUGE: <math.h> defines that as a macro on macOS/BSD.
		GARGANTUAN,
	};
	enum class Alignment {
		UNALIGNED,
		LAWFUL_GOOD,
		NEUTRAL_GOOD,
		CHAOTIC_GOOD,
		LAWFUL_NEUTRAL,
		NEUTRAL,
		CHAOTIC_NEUTRAL,
		LAWFUL_EVIL,
		NEUTRAL_EVIL,
		CHAOTIC_EVIL,
	};
	enum class ActionType {
		NO_ACTION,
		ACTION,
		BONUS_ACTION,
		REACTION,
		LEGENDARY_ACTION, // Should only be available to Monster. 
		LAIR_ACTION, // Should only be available to Monster.
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
		NO_ABILITY,
		STRENGTH,
		DEXTERITY,
		CONSTITUTION,
		WISDOM,
		INTELLIGENCE,
		CHARISMA,
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
		THUNDER,
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
		EXHAUSTION,
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
		UNDERCOMMON,
	};
	enum class CreatureType {
		NO_TYPE,
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
		UNDEAD,
	};
	enum class Sense {
		BLINDSIGHT,
		DARKVISION,
		TREMORSENSE,
		TRUESIGHT,
	};
}