#pragma once

#include <unordered_map>
#include <unordered_set>

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
	inline const std::unordered_map<PlayerClass, std::unordered_set<PlayerSubclass>> MAPPED_CLASSES{
		{PlayerClass::BARBARIAN, {
			PlayerSubclass::PATH_OF_THE_BERSERKER,
			PlayerSubclass::PATH_OF_THE_WILD_HEART,
			PlayerSubclass::PATH_OF_THE_WORLD_TREE,
			PlayerSubclass::PATH_OF_THE_ZEALOT
		}},
		{PlayerClass::BARD, {
			PlayerSubclass::COLLEGE_OF_DANCE,
			PlayerSubclass::COLLEGE_OF_GLAMOUR,
			PlayerSubclass::COLLEGE_OF_LORE,
			PlayerSubclass::COLLEGE_OF_VALOR
		}},
		{PlayerClass::CLERIC, {
			PlayerSubclass::LIFE_DOMAIN,
			PlayerSubclass::LIGHT_DOMAIN,
			PlayerSubclass::TRICKERY_DOMAIN,
			PlayerSubclass::WAR_DOMAIN
		}},
		{PlayerClass::DRUID, {
			PlayerSubclass::CIRCLE_OF_THE_LAND,
			PlayerSubclass::CIRCLE_OF_THE_MOON,
			PlayerSubclass::CIRCLE_OF_THE_SEA,
			PlayerSubclass::CIRCLE_OF_THE_STARS
		}},
		{PlayerClass::FIGHTER, {
			PlayerSubclass::BATTLE_MASTER,
			PlayerSubclass::CHAMPION,
			PlayerSubclass::ELDRITCH_KNIGHT,
			PlayerSubclass::PSI_WARRIOR
		}},
		{PlayerClass::MONK, {
			PlayerSubclass::WARRIOR_OF_MERCY,
			PlayerSubclass::WARRIOR_OF_SHADOW,
			PlayerSubclass::WARRIOR_OF_THE_ELEMENTS,
			PlayerSubclass::WARRIOR_OF_THE_OPEN_HAND
		}},
		{PlayerClass::PALADIN, {
			PlayerSubclass::OATH_OF_DEVOTION,
			PlayerSubclass::OATH_OF_GLORY,
			PlayerSubclass::OATH_OF_THE_ANCIENTS,
			PlayerSubclass::OATH_OF_VENGEANCE
		}},
		{PlayerClass::RANGER, {
			PlayerSubclass::BEAST_MASTER,
			PlayerSubclass::FEY_WANDERER,
			PlayerSubclass::GLOOM_STALKER,
			PlayerSubclass::HUNTER
		}},
		{PlayerClass::ROGUE, {
			PlayerSubclass::ARCANE_TRICKSTER,
			PlayerSubclass::ASSASSIN,
			PlayerSubclass::SOULKNIFE,
			PlayerSubclass::THIEF
		}},
		{PlayerClass::SORCERER, {
			PlayerSubclass::ABERRANT_SORCERY,
			PlayerSubclass::CLOCKWORK_SORCERY,
			PlayerSubclass::DRACONIC_SORCERY,
			PlayerSubclass::WILD_MAGIC_SORCERY
		}},
		{PlayerClass::WARLOCK, {
			PlayerSubclass::ARCHFEY_PATRON,
			PlayerSubclass::CELESTIAL_PATRON,
			PlayerSubclass::FIEND_PATRON,
			PlayerSubclass::GREAT_OLD_ONE_PATRON
		}},
		{PlayerClass::WIZARD, {
			PlayerSubclass::ABJURER,
			PlayerSubclass::DIVINER,
			PlayerSubclass::EVOKER,
			PlayerSubclass::ILLUSIONIST
		}}
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
	inline constexpr int MIN_CLASS_LEVEL { 0 }; 
	inline constexpr int MAX_TOTAL_CLASS_LEVEL { 20 };
	inline constexpr int SUBCLASS_LEVEL_THRESHOLD { 3 };
	inline const std::unordered_map<PlayerClass, std::string> CLASS_NAMES{
		{PlayerClass::BARBARIAN, "Barbarian"},
		{PlayerClass::BARD, "Bard"},
		{PlayerClass::CLERIC, "Cleric"},
		{PlayerClass::DRUID, "Druid"},
		{PlayerClass::FIGHTER, "Fighter"},
		{PlayerClass::MONK, "Monk"},
		{PlayerClass::PALADIN, "Paladin"},
		{PlayerClass::RANGER, "Ranger"},
		{PlayerClass::ROGUE, "Rogue"},
		{PlayerClass::SORCERER, "Sorcerer"},
		{PlayerClass::WARLOCK, "Warlock"},
		{PlayerClass::WIZARD, "Wizard"}
	};
	inline const std::unordered_map<PlayerSubclass, std::string> SUBCLASS_NAMES{
		{PlayerSubclass::PATH_OF_THE_BERSERKER, "Path of the Berserker"},
		{PlayerSubclass::PATH_OF_THE_WILD_HEART, "Path of the Wild Heart"},
		{PlayerSubclass::PATH_OF_THE_WORLD_TREE, "Path of the World Tree"},
		{PlayerSubclass::PATH_OF_THE_ZEALOT, "Path of the Zealot"},
		{PlayerSubclass::COLLEGE_OF_DANCE, "College of Dance"},
		{PlayerSubclass::COLLEGE_OF_GLAMOUR, "College of Glamour"},
		{PlayerSubclass::COLLEGE_OF_LORE, "College of Lore"},
		{PlayerSubclass::COLLEGE_OF_VALOR, "College of Valor"},
		{PlayerSubclass::LIFE_DOMAIN, "Life Domain"},
		{PlayerSubclass::LIGHT_DOMAIN, "Light Domain"},
		{PlayerSubclass::TRICKERY_DOMAIN, "Trickery Domain"},
		{PlayerSubclass::WAR_DOMAIN, "War Domain"},
		{PlayerSubclass::CIRCLE_OF_THE_LAND, "Circle of the Land"},
		{PlayerSubclass::CIRCLE_OF_THE_MOON, "Circle of the Moon"},
		{PlayerSubclass::CIRCLE_OF_THE_SEA, "Circle of the Sea"},
		{PlayerSubclass::CIRCLE_OF_THE_STARS, "Circle of the Stars"},
		{PlayerSubclass::BATTLE_MASTER, "Battle Master"},
		{PlayerSubclass::CHAMPION, "Champion"},
		{PlayerSubclass::ELDRITCH_KNIGHT, "Eldritch Knight"},
		{PlayerSubclass::PSI_WARRIOR, "Psi Warrior"},
		{PlayerSubclass::WARRIOR_OF_MERCY, "Warrior of Mercy"},
		{PlayerSubclass::WARRIOR_OF_SHADOW, "Warrior of Shadow"},
		{PlayerSubclass::WARRIOR_OF_THE_ELEMENTS, "Warrior of the Elements"},
		{PlayerSubclass::WARRIOR_OF_THE_OPEN_HAND, "Warrior of the Open Hand"},
		{PlayerSubclass::OATH_OF_DEVOTION, "Oath of Devotion"},
		{PlayerSubclass::OATH_OF_GLORY, "Oath of Glory"},
		{PlayerSubclass::OATH_OF_THE_ANCIENTS, "Oath of the Ancients"},
		{PlayerSubclass::OATH_OF_VENGEANCE, "Oath of Vengeance"},
		{PlayerSubclass::BEAST_MASTER, "Beast Master"},
		{PlayerSubclass::FEY_WANDERER, "Fey Wanderer"},
		{PlayerSubclass::GLOOM_STALKER, "Gloom Stalker"},
		{PlayerSubclass::HUNTER, "Hunter"},
		{PlayerSubclass::ARCANE_TRICKSTER, "Arcane Trickster"},
		{PlayerSubclass::ASSASSIN, "Assassin"},
		{PlayerSubclass::SOULKNIFE, "Soulknife"},
		{PlayerSubclass::THIEF, "Thief"},
		{PlayerSubclass::ABERRANT_SORCERY, "Aberrant Sorcery"},
		{PlayerSubclass::CLOCKWORK_SORCERY, "Clockwork Sorcery"},
		{PlayerSubclass::DRACONIC_SORCERY, "Draconic Sorcery"},
		{PlayerSubclass::WILD_MAGIC_SORCERY, "Wild Magic Sorcery"},
		{PlayerSubclass::ARCHFEY_PATRON, "Archfey Patron"},
		{PlayerSubclass::CELESTIAL_PATRON, "Celestial Patron"},
		{PlayerSubclass::FIEND_PATRON, "Fiend Patron"},
		{PlayerSubclass::GREAT_OLD_ONE_PATRON, "Great Old One Patron"},
		{PlayerSubclass::ABJURER, "Abjurer"},
		{PlayerSubclass::DIVINER, "Diviner"},
		{PlayerSubclass::EVOKER, "Evoker"},
		{PlayerSubclass::ILLUSIONIST, "Illusionist"}
	};
	inline const std::unordered_map<Species, std::string> SPECIES_NAMES{
		{Species::AASIMAR, "Aasimar"},
		{Species::DRAGONBORN, "Dragonborn"},
		{Species::DWARF, "Dwarf"},
		{Species::ELF, "Elf"},
		{Species::GNOME, "Gnome"},
		{Species::GOLIATH, "Goliath"},
		{Species::HALFLING, "Halfling"},
		{Species::HUMAN, "Human"},
		{Species::ORC, "Orc"},
		{Species::TIEFLING, "Tiefling"}
	};
	inline const std::unordered_map<Origin, std::string> ORIGIN_NAMES{
		{Origin::ACOLYTE, "Acolyte"},
		{Origin::ARTISAN, "Artisan"},
		{Origin::CHARLATAN, "Charlatan"},
		{Origin::CRIMINAL, "Criminal"},
		{Origin::ENTERTAINER, "Entertainer"},
		{Origin::FARMER, "Farmer"},
		{Origin::GUARD, "Guard"},
		{Origin::GUIDE, "Guide"},
		{Origin::HERMIT, "Hermit"},
		{Origin::MERCHANT, "Merchant"},
		{Origin::NOBLE, "Noble"},
		{Origin::SAGE, "Sage"},
		{Origin::SAILOR, "Sailor"},
		{Origin::SCRIBE, "Scribe"},
		{Origin::SOLDIER, "Soldier"},
		{Origin::WAYFARER, "Wayfarer"}
	};
	inline const std::unordered_map<WeaponType, std::string> WEAPON_TYPE_NAMES{
		{WeaponType::SIMPLE_MELEE, "Simple Melee"},
		{WeaponType::SIMPLE_RANGED, "Simple Ranged"},
		{WeaponType::MARTIAL_MELEE, "Martial Melee"},
		{WeaponType::MARTIAL_RANGED, "Martial Ranged"}
	};
	inline const std::unordered_map<ArmorType, std::string> ARMOR_TYPE_NAMES{
		{ArmorType::LIGHT, "Light"},
		{ArmorType::MEDIUM, "Medium"},
		{ArmorType::HEAVY, "Heavy"},
		{ArmorType::SHIELD, "Shield"}
	};
	inline const std::unordered_map<WeaponMastery, std::string> WEAPON_MASTERY_NAMES{
		{WeaponMastery::CLEAVE, "Cleave"},
		{WeaponMastery::GRAZE, "Graze"},
		{WeaponMastery::NICK, "Nick"},
		{WeaponMastery::PUSH, "Push"},
		{WeaponMastery::SAP, "Sap"},
		{WeaponMastery::SLOW, "Slow"},
		{WeaponMastery::TOPPLE, "Topple"},
		{WeaponMastery::VEX, "Vex"}
	};
	inline const std::unordered_map<Tool, std::string> TOOL_NAMES{
		{Tool::ALCHEMISTS_SUPPLIES, "Alchemist's Supplies"},
		{Tool::BREWERS_SUPPLIES, "Brewer's Supplies"},
		{Tool::CALLIGRAPHERS_SUPPLIES, "Calligrapher's Supplies"},
		{Tool::CARPERNTERS_TOOLS, "Carpenter's Tool"},
		{Tool::CARTOGRAPHERS_TOOLS, "Cartographer's Tools"},
		{Tool::COBBLERS_TOOLS, "Cobbler's Tools"},
		{Tool::COOKS_UTENSILS, "Cook's Utensils"},
		{Tool::GLASSBLOWERS_TOOLS, "Glassblower's Tools"},
		{Tool::JEWELERS_TOOLS, "Jeweler's Tools"},
		{Tool::LEATHERWORKERS_TOOLS, "Leatherworker's Tools"},
		{Tool::MASON_TOOLS, "Mason Tools"},
		{Tool::PAINTERS_TOOLS, "Painter's Tools"},
		{Tool::POTTERS_TOOLS, "Potter's Tools"},
		{Tool::SMITHS_TOOLS, "Smith's Tools"},
		{Tool::TINKERS_TOOLS, "Tinker's Tools"},
		{Tool::WEAVERS_TOOLS, "Weaver's Tools"},
		{Tool::WOODCARVERS_TOOLS, "Woodcarver's Tools"},
		{Tool::DISGUISE_KIT, "Disguise Kit"},
		{Tool::FORGERY_KIT, "Forgery Kit"},
		{Tool::GAMING_SET, "Gaming Set"},
		{Tool::HERBALISM_KIT, "Herbalism Kit"},
		{Tool::MUSICAL_INSTRUMENT, "Musical Instrument"},
		{Tool::NAVIGATORS_TOOLS, "Navigator's Tools"},
		{Tool::POISONERS_KIT, "Poisoner's Kit"},
		{Tool::THIEVES_TOOLS, "Thieves' Tools"}
	};
}