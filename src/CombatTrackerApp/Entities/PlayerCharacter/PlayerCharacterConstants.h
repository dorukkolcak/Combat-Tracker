#pragma once

#include "CombatTrackerApp/Entities/Creature/CreatureEnums.h"
#include "CombatTrackerApp/Entities/PlayerCharacter/PlayerCharacterEnums.h"

#include <array>
#include <cstddef>
#include <unordered_map>
#include <unordered_set>
#include <string_view>

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
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(PlayerClass::COUNT)> CLASS_NAMES { {
		"Barbarian",
		"Bard",
		"Cleric",
		"Druid",
		"Fighter",
		"Monk",
		"Paladin",
		"Ranger",
		"Rogue",
		"Sorcerer",
		"Warlock",
		"Wizard"
	} };
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(PlayerSubclass::COUNT)> SUBCLASS_NAMES { {
		"Path of the Berserker",
		"Path of the Wild Heart",
		"Path of the World Tree",
		"Path of the Zealot",
		"College of Dance",
		"College of Glamour",
		"College of Lore",
		"College of Valor",
		"Life Domain",
		"Light Domain",
		"Trickery Domain",
		"War Domain",
		"Circle of the Land",
		"Circle of the Moon",
		"Circle of the Sea",
		"Circle of the Stars",
		"Battle Master",
		"Champion",
		"Eldritch Knight",
		"Psi Warrior",
		"Warrior of Mercy",
		"Warrior of Shadow",
		"Warrior of the Elements",
		"Warrior of the Open Hand",
		"Oath of Devotion",
		"Oath of Glory",
		"Oath of the Ancients",
		"Oath of Vengeance",
		"Beast Master",
		"Fey Wanderer",
		"Gloom Stalker",
		"Hunter",
		"Arcane Trickster",
		"Assassin",
		"Soulknife",
		"Thief",
		"Aberrant Sorcery",
		"Clockwork Sorcery",
		"Draconic Sorcery",
		"Wild Magic Sorcery",
		"Archfey Patron",
		"Celestial Patron",
		"Fiend Patron",
		"Great Old One Patron",
		"Abjurer",
		"Diviner",
		"Evoker",
		"Illusionist"
	} };
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(Species::COUNT)> SPECIES_NAMES { {
		"Aasimar",
		"Dragonborn",
		"Dwarf",
		"Elf",
		"Gnome",
		"Goliath",
		"Halfling",
		"Human",
		"Orc",
		"Tiefling"
	} };
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(Origin::COUNT)> ORIGIN_NAMES { {
		"Acolyte",
		"Artisan",
		"Charlatan",
		"Criminal",
		"Entertainer",
		"Farmer",
		"Guard",
		"Guide",
		"Hermit",
		"Merchant",
		"Noble",
		"Sage",
		"Sailor",
		"Scribe",
		"Soldier",
		"Wayfarer"
	} };
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(WeaponType::COUNT)> WEAPON_TYPE_NAMES { {
		"Simple Melee",
		"Simple Ranged",
		"Martial Melee",
		"Martial Ranged"
	} };
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(ArmorType::COUNT)> ARMOR_TYPE_NAMES { {
		"Light",
		"Medium",
		"Heavy",
		"Shield"
	} };
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(WeaponMastery::COUNT)> WEAPON_MASTERY_NAMES { {
		"Cleave",
		"Graze",
		"Nick",
		"Push",
		"Sap",
		"Slow",
		"Topple",
		"Vex"
	} };
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(Tool::COUNT)> TOOL_NAMES { {
		"Alchemist's Supplies",
		"Brewer's Supplies",
		"Calligrapher's Supplies",
		"Carpenter's Tool",
		"Cartographer's Tools",
		"Cobbler's Tools",
		"Cook's Utensils",
		"Glassblower's Tools",
		"Jeweler's Tools",
		"Leatherworker's Tools",
		"Mason Tools",
		"Painter's Tools",
		"Potter's Tools",
		"Smith's Tools",
		"Tinker's Tools",
		"Weaver's Tools",
		"Woodcarver's Tools",
		"Disguise Kit",
		"Forgery Kit",
		"Gaming Set",
		"Herbalism Kit",
		"Musical Instrument",
		"Navigator's Tools",
		"Poisoner's Kit",
		"Thieves' Tools"
	} };
}