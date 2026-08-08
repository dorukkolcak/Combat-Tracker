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
	
	/*
	The ordering for all name constants is EXTREMELY important. They should be the same order as the enum order.
	This allows us to achive multiple things:
	1) Constexpr support so performance gain (should be VERY, VERY minimal but it is good practice nonetheless).
	2) Constant time element access using enumerators as indices. This was mostly the reason why I used unordered_map's in the first place.
	3) Labels are actually ordered. The ordering within unordered_map was completely arbitrary and could actually change during runs.
	We can just use a map for the same purpose but we get logaritmic access time in that case.
	So I am pretty sure this is the best of every world.
	*/
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(Size::COUNT)> SIZE_NAMES { {
		"Tiny",
		"Small",
		"Medium",
		"Large",
		"Huge",
		"Gargantuan"
	} };
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(CreatureType::COUNT)> CREATURE_TYPE_NAMES { {
		"Aberration",
		"Beast",
		"Celestial",
		"Construct",
		"Dragon",
		"Elemental",
		"Fey",
		"Fiend",
		"Giant",
		"Humanoid",
		"Monstrosity",
		"Ooze",
		"Plant",
		"Undead"
	} };
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(Alignment::COUNT)> ALIGNMENT_NAMES { {
		"Unaligned",
		"Lawful Good",
		"Neutral Good",
		"Chaotic Good",
		"Lawful Neutral",
		"Neutral",
		"Chaotic Neutral",
		"Lawful Evil",
		"Neutral Evil",
		"Chaotic Evil"
	} };
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(Ability::COUNT)> ABILITY_NAMES { {
		"Strength",
		"Dexterity",
		"Constitution",
		"Wisdom",
		"Intelligence",
		"Charisma"
	} };
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(Skill::COUNT)> SKILL_NAMES { {
		"Athletics",
		"Acrobatics",
		"Sleight of Hand",
		"Stealth",
		"Arcana",
		"History",
		"Investigation",
		"Nature",
		"Religion",
		"Animal Handling",
		"Insight",
		"Medicine",
		"Perception",
		"Survival",
		"Deception",
		"Intimidation",
		"Performance",
		"Persuasion"
	} };
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(DamageType::COUNT)> DAMAGE_TYPE_NAMES { {
		"Acid",
		"Bludgeoning",
		"Cold",
		"Fire",
		"Force",
		"Lightning",
		"Necrotic",
		"Piercing",
		"Poison",
		"Psychic",
		"Radiant",
		"Slashing",
		"Thunder"
	} };
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(Condition::COUNT)> CONDITTON_NAMES { {
		"Blinded",
		"Charmed",
		"Deafened",
		"Frightened",
		"Grappled",
		"Incapacitated",
		"Invisible",
		"Paralyzed",
		"Petrified",
		"Poisoned",
		"Prone",
		"Restrained",
		"Stunned",
		"Unconscious",
		"Exhaustion"
	} };
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(Language::COUNT)> LANGUAGE_NAMES { {
		"Common",
		"Common Sign Language",
		"Dwarvish",
		"Elvish",
		"Giant",
		"Gnomish",
		"Goblin",
		"Halfling",
		"Orc",
		"Abyssal",
		"Celestial",
		"Deep Speech",
		"Druidic",
		"Infernal",
		"Primordial",
		"Sylvan",
		"Thieves' Cant",
		"Undercommon"
	} };
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(Sense::COUNT)> SENSE_NAMES { {
		"Blindsight",
		"Darkvision",
		"Tremorsense",
		"Truesight"
	} };
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(ActionType::COUNT)> ACTION_NAMES { {
		"Action",
		"Bonus Action",
		"Reaction",
		"Legendary Action",
		"Lair Action"
	} };
}
