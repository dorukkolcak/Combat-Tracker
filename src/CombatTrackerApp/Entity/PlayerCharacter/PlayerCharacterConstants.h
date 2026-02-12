#pragma once

#include <unordered_map>
#include <cstdint>

#include "../Creature/CreatureEnums.h"
#include "PlayerCharacterEnums.h"

namespace CombatTrackerApp {
	inline const std::unordered_map<PlayerSubclass, PlayerClass> MAPPED_SUBCLASSES {
		{PlayerSubclass::PATH_OF_THE_BERSERKER, PlayerClass::BARBARIAN},
		{PlayerSubclass::PATH_OF_THE_WILD_HEART, PlayerClass::BARBARIAN},
		{PlayerSubclass::PATH_OF_THE_WORLD_TREE, PlayerClass::BARBARIAN},
		{PlayerSubclass::PATH_OF_THE_ZEALOT, PlayerClass::BARBARIAN},
		{PlayerSubclass::COLLEGE_OF_DANCE, PlayerClass::BARD},
		{PlayerSubclass::COLLEGE_OF_GLAMOUR, PlayerClass::BARD},
		{PlayerSubclass::COLLEGE_OF_LORE, PlayerClass::BARD},
		{PlayerSubclass::COLLEGE_OF_VALOR, PlayerClass::BARD},
		{PlayerSubclass::LIFE_DOMAIN, PlayerClass::CLERIC},
		{PlayerSubclass::LIGHT_DOMAIN, PlayerClass::CLERIC},
		{PlayerSubclass::TRICKERY_DOMAIN, PlayerClass::CLERIC},
		{PlayerSubclass::WAR_DOMAIN, PlayerClass::CLERIC},
		{PlayerSubclass::CIRCLE_OF_THE_LAND, PlayerClass::DRUID},
		{PlayerSubclass::CIRCLE_OF_THE_MOON, PlayerClass::DRUID},
		{PlayerSubclass::CIRCLE_OF_THE_SEA, PlayerClass::DRUID},
		{PlayerSubclass::CIRCLE_OF_THE_STARS, PlayerClass::DRUID},
		{PlayerSubclass::BATTLE_MASTER, PlayerClass::FIGHTER},
		{PlayerSubclass::CHAMPION, PlayerClass::FIGHTER},
		{PlayerSubclass::ELDRITCH_KNIGHT, PlayerClass::FIGHTER},
		{PlayerSubclass::PSI_WARRIOR, PlayerClass::FIGHTER},
		{PlayerSubclass::WARRIOR_OF_MERCY, PlayerClass::MONK},
		{PlayerSubclass::WARRIOR_OF_SHADOW, PlayerClass::MONK},
		{PlayerSubclass::WARRIOR_OF_THE_ELEMENTS, PlayerClass::MONK},
		{PlayerSubclass::WARRIOR_OF_THE_OPEN_HAND, PlayerClass::MONK},
		{PlayerSubclass::OATH_OF_DEVOTION, PlayerClass::PALADIN},
		{PlayerSubclass::OATH_OF_GLORY, PlayerClass::PALADIN},
		{PlayerSubclass::OATH_OF_THE_ANCIENTS, PlayerClass::PALADIN},
		{PlayerSubclass::OATH_OF_VENGEANCE, PlayerClass::PALADIN},
		{PlayerSubclass::BEAST_MASTER, PlayerClass::RANGER},
		{PlayerSubclass::FEY_WANDERER, PlayerClass::RANGER},
		{PlayerSubclass::GLOOM_STALKER, PlayerClass::RANGER},
		{PlayerSubclass::HUNTER, PlayerClass::RANGER},
		{PlayerSubclass::ARCANE_TRICKSTER, PlayerClass::ROGUE},
		{PlayerSubclass::ASSASSIN, PlayerClass::ROGUE},
		{PlayerSubclass::SOULKNIFE, PlayerClass::ROGUE},
		{PlayerSubclass::THIEF, PlayerClass::ROGUE},
		{PlayerSubclass::ABERRANT_SORCERY, PlayerClass::SORCERER},
		{PlayerSubclass::CLOCKWORK_SORCERY, PlayerClass::SORCERER},
		{PlayerSubclass::DRACONIC_SORCERY, PlayerClass::SORCERER},
		{PlayerSubclass::WILD_MAGIC_SORCERY, PlayerClass::SORCERER},
		{PlayerSubclass::ARCHFEY_PATRON, PlayerClass::WARLOCK},
		{PlayerSubclass::CELESTIAL_PATRON, PlayerClass::WARLOCK},
		{PlayerSubclass::FIEND_PATRON, PlayerClass::WARLOCK},
		{PlayerSubclass::GREAT_OLD_ONE_PATRON, PlayerClass::WARLOCK},
		{PlayerSubclass::ABJURER, PlayerClass::WIZARD},
		{PlayerSubclass::DIVINER, PlayerClass::WIZARD},
		{PlayerSubclass::EVOKER, PlayerClass::WIZARD},
		{PlayerSubclass::ILLUSIONIST, PlayerClass::WIZARD}
	};
	// Will probably be unused.
	inline const std::unordered_map<Tool, Ability> MAPPED_TOOLS {
		{Tool::ALCHEMISTS_SUPPLIES, Ability::INTELLIGENCE},
		{Tool::BREWERS_SUPPLIES, Ability::INTELLIGENCE},
		{Tool::CALLIGRAPHERS_SUPPLIES, Ability::DEXTERITY},
		{Tool::CARPERNTERS_TOOLS, Ability::STRENGTH},
		{Tool::CARTOGRAPHERS_TOOLS, Ability::WISDOM},
		{Tool::COBBLERS_TOOLS, Ability::DEXTERITY},
		{Tool::COOKS_UTENSILS, Ability::WISDOM},
		{Tool::GLASSBLOWERS_TOOLS, Ability::INTELLIGENCE},
		{Tool::JEWELERS_TOOLS, Ability::INTELLIGENCE},
		{Tool::LEATHERWORKERS_TOOLS, Ability::DEXTERITY},
		{Tool::MASON_TOOLS, Ability::STRENGTH},
		{Tool::PAINTERS_TOOLS, Ability::WISDOM},
		{Tool::POTTERS_TOOLS, Ability::INTELLIGENCE},
		{Tool::SMITHS_TOOLS, Ability::STRENGTH},
		{Tool::TINKERS_TOOLS, Ability::DEXTERITY},
		{Tool::WEAVERS_TOOLS, Ability::DEXTERITY},
		{Tool::WOODCARVERS_TOOLS, Ability::DEXTERITY},
		{Tool::DISGUISE_KIT,Ability::CHARISMA},
		{Tool::FORGERY_KIT, Ability::DEXTERITY},
		{Tool::GAMING_SET, Ability::WISDOM},
		{Tool::HERBALISM_KIT, Ability::INTELLIGENCE},
		{Tool::MUSICAL_INSTRUMENT, Ability::CHARISMA},
		{Tool::NAVIGATORS_TOOLS, Ability::WISDOM},
		{Tool::POISONERS_KIT, Ability::INTELLIGENCE},
		{Tool::THIEVES_TOOLS, Ability::DEXTERITY}
	};
	inline constexpr std::uint16_t MIN_CLASS_LEVEL { 0 }; 
	inline constexpr std::uint16_t MAX_TOTAL_CLASS_LEVEL { 20 };
	inline constexpr std::uint16_t SUBCLASS_LEVEL_THRESHOLD { 3 };
}