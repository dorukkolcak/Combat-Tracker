#pragma once

#include "CombatTrackerApp/Entities/Creature.h"
#include "CombatTrackerLib/Utilities/GenericUtils.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <optional>

namespace CombatTrackerApp {

	enum class PlayerClass {
		BARBARIAN,
		BARD,
		CLERIC,
		DRUID,
		FIGHTER,
		MONK,
		PALADIN,
		RANGER,
		ROGUE,
		SORCERER,
		WARLOCK,
		WIZARD,
		COUNT,
	};
	enum class PlayerSubclass {
		PATH_OF_THE_BERSERKER,
		PATH_OF_THE_WILD_HEART,
		PATH_OF_THE_WORLD_TREE,
		PATH_OF_THE_ZEALOT,
		COLLEGE_OF_DANCE,
		COLLEGE_OF_GLAMOUR,
		COLLEGE_OF_LORE,
		COLLEGE_OF_VALOR,
		LIFE_DOMAIN,
		LIGHT_DOMAIN,
		TRICKERY_DOMAIN,
		WAR_DOMAIN,
		CIRCLE_OF_THE_LAND,
		CIRCLE_OF_THE_MOON,
		CIRCLE_OF_THE_SEA,
		CIRCLE_OF_THE_STARS,
		BATTLE_MASTER,
		CHAMPION,
		ELDRITCH_KNIGHT,
		PSI_WARRIOR,
		WARRIOR_OF_MERCY,
		WARRIOR_OF_SHADOW,
		WARRIOR_OF_THE_ELEMENTS,
		WARRIOR_OF_THE_OPEN_HAND,
		OATH_OF_DEVOTION,
		OATH_OF_GLORY,
		OATH_OF_THE_ANCIENTS,
		OATH_OF_VENGEANCE,
		BEAST_MASTER,
		FEY_WANDERER,
		GLOOM_STALKER,
		HUNTER,
		ARCANE_TRICKSTER,
		ASSASSIN,
		SOULKNIFE,
		THIEF,
		ABERRANT_SORCERY,
		CLOCKWORK_SORCERY,
		DRACONIC_SORCERY,
		WILD_MAGIC_SORCERY,
		ARCHFEY_PATRON,
		CELESTIAL_PATRON,
		FIEND_PATRON,
		GREAT_OLD_ONE_PATRON,
		ABJURER,
		DIVINER,
		EVOKER,
		ILLUSIONIST,
		COUNT,
	};
	enum class Species {
		AASIMAR,
		DRAGONBORN,
		DWARF,
		ELF,
		GNOME,
		GOLIATH,
		HALFLING,
		HUMAN,
		ORC,
		TIEFLING,
		COUNT,
	};
	enum class Origin {
		ACOLYTE,
		ARTISAN,
		CHARLATAN,
		CRIMINAL,
		ENTERTAINER,
		FARMER,
		GUARD,
		GUIDE,
		HERMIT,
		MERCHANT,
		NOBLE,
		SAGE,
		SAILOR,
		SCRIBE,
		SOLDIER,
		WAYFARER,
		COUNT,
	};
	enum class WeaponType {
		SIMPLE_MELEE,
		SIMPLE_RANGED,
		MARTIAL_MELEE,
		MARTIAL_RANGED,
		COUNT,
	};
	enum class ArmorType {
		LIGHT,
		MEDIUM,
		HEAVY,
		SHIELD,
		COUNT,
	};
	enum class WeaponMastery {
		CLEAVE,
		GRAZE,
		NICK,
		PUSH,
		SAP,
		SLOW,
		TOPPLE,
		VEX,
		COUNT,
	};
	enum class Tool {
		ALCHEMISTS_SUPPLIES,
		BREWERS_SUPPLIES,
		CALLIGRAPHERS_SUPPLIES,
		CARPERNTERS_TOOLS,
		CARTOGRAPHERS_TOOLS,
		COBBLERS_TOOLS,
		COOKS_UTENSILS,
		GLASSBLOWERS_TOOLS,
		JEWELERS_TOOLS,
		LEATHERWORKERS_TOOLS,
		MASON_TOOLS,
		PAINTERS_TOOLS,
		POTTERS_TOOLS,
		SMITHS_TOOLS,
		TINKERS_TOOLS,
		WEAVERS_TOOLS,
		WOODCARVERS_TOOLS,
		DISGUISE_KIT,
		FORGERY_KIT,
		GAMING_SET,
		HERBALISM_KIT,
		MUSICAL_INSTRUMENT,
		NAVIGATORS_TOOLS,
		POISONERS_KIT,
		THIEVES_TOOLS,
		COUNT,
	};

	/*
	The order is via enumeration order, same idea as every other array structure.
	Every subclass belongs to exactly one class, so the class is looked up by the subclass's underlying integral value.
	Once again, using an array instead of an unordered_map has the advantage of constexpr support.
	And since no changes will be made to the order other than additions, storing them in this format isn't cumbersome.
	*/
	inline constexpr std::
		array<PlayerClass, static_cast<std::size_t>(PlayerSubclass::COUNT)>
			SUBCLASSES_TO_CLASSES{
				PlayerClass::BARBARIAN, // PATH_OF_THE_BERSERKER
				PlayerClass::BARBARIAN, // PATH_OF_THE_WILD_HEART
				PlayerClass::BARBARIAN, // PATH_OF_THE_WORLD_TREE
				PlayerClass::BARBARIAN, // PATH_OF_THE_ZEALOT
				PlayerClass::BARD,		// COLLEGE_OF_DANCE
				PlayerClass::BARD,		// COLLEGE_OF_GLAMOUR
				PlayerClass::BARD,		// COLLEGE_OF_LORE
				PlayerClass::BARD,		// COLLEGE_OF_VALOR
				PlayerClass::CLERIC,	// LIFE_DOMAIN
				PlayerClass::CLERIC,	// LIGHT_DOMAIN
				PlayerClass::CLERIC,	// TRICKERY_DOMAIN
				PlayerClass::CLERIC,	// WAR_DOMAIN
				PlayerClass::DRUID,		// CIRCLE_OF_THE_LAND
				PlayerClass::DRUID,		// CIRCLE_OF_THE_MOON
				PlayerClass::DRUID,		// CIRCLE_OF_THE_SEA
				PlayerClass::DRUID,		// CIRCLE_OF_THE_STARS
				PlayerClass::FIGHTER,	// BATTLE_MASTER
				PlayerClass::FIGHTER,	// CHAMPION
				PlayerClass::FIGHTER,	// ELDRITCH_KNIGHT
				PlayerClass::FIGHTER,	// PSI_WARRIOR
				PlayerClass::MONK,		// WARRIOR_OF_MERCY
				PlayerClass::MONK,		// WARRIOR_OF_SHADOW
				PlayerClass::MONK,		// WARRIOR_OF_THE_ELEMENTS
				PlayerClass::MONK,		// WARRIOR_OF_THE_OPEN_HAND
				PlayerClass::PALADIN,	// OATH_OF_DEVOTION
				PlayerClass::PALADIN,	// OATH_OF_GLORY
				PlayerClass::PALADIN,	// OATH_OF_THE_ANCIENTS
				PlayerClass::PALADIN,	// OATH_OF_VENGEANCE
				PlayerClass::RANGER,	// BEAST_MASTER
				PlayerClass::RANGER,	// FEY_WANDERER
				PlayerClass::RANGER,	// GLOOM_STALKER
				PlayerClass::RANGER,	// HUNTER
				PlayerClass::ROGUE,		// ARCANE_TRICKSTER
				PlayerClass::ROGUE,		// ASSASSIN
				PlayerClass::ROGUE,		// SOULKNIFE
				PlayerClass::ROGUE,		// THIEF
				PlayerClass::SORCERER,	// ABERRANT_SORCERY
				PlayerClass::SORCERER,	// CLOCKWORK_SORCERY
				PlayerClass::SORCERER,	// DRACONIC_SORCERY
				PlayerClass::SORCERER,	// WILD_MAGIC_SORCERY
				PlayerClass::WARLOCK,	// ARCHFEY_PATRON
				PlayerClass::WARLOCK,	// CELESTIAL_PATRON
				PlayerClass::WARLOCK,	// FIEND_PATRON
				PlayerClass::WARLOCK,	// GREAT_OLD_ONE_PATRON
				PlayerClass::WIZARD,	// ABJURER
				PlayerClass::WIZARD,	// DIVINER
				PlayerClass::WIZARD,	// EVOKER
				PlayerClass::WIZARD		// ILLUSIONIST
			};
	inline constexpr int MIN_CLASS_LEVEL{ 0 };
	inline constexpr int MAX_TOTAL_CLASS_LEVEL{ 20 };
	inline constexpr int SUBCLASS_LEVEL_THRESHOLD{ 3 };

	// Exact same idea as creature/monster side, currently only applied to total class levels.
	class PlayerDerivedStats {
	public:
		constexpr int getTotalClassLevel() const { return m_totalClassLevel; }

	private:
		friend class PlayerCharacter;

		int m_totalClassLevel{};
	};
	struct PlayerStatBlock {

		std::array<
			CombatTrackerLib::
				BoundedInt<MIN_CLASS_LEVEL, MAX_TOTAL_CLASS_LEVEL>,
			static_cast<std::size_t>(PlayerClass::COUNT)>
			levels{};

		/*
		A class holds at most one subclass, so each index in the array corresponds to a class (mapped to a subclass).
		This has the advantage of making the stat block usable in a constant expression, which an unordered map doesn't allow.
		An empty slot is a class with no subclass chosen, marked by std::nullopt.
		*/
		std::array<
			std::optional<PlayerSubclass>,
			static_cast<std::size_t>(PlayerClass::COUNT)>
			subclasses{};

		std::array<bool, static_cast<std::size_t>(WeaponType::COUNT)>
			weaponProficiencies{};
		std::array<bool, static_cast<std::size_t>(ArmorType::COUNT)>
			armorProficiencies{};
		std::array<bool, static_cast<std::size_t>(WeaponMastery::COUNT)>
			weaponMasteries{};

		// Tools can have expertise as well so we use the proficiency level idea here as well.
		std::array<ProficiencyLevel, static_cast<std::size_t>(Tool::COUNT)>
			toolProficiencyLevels{};

		Species species{};
		Origin origin{};

		PlayerDerivedStats derived{};
	};

	class PlayerCharacter : public Creature {
	public:
		friend class PlayerCharacterTest;

		PlayerCharacter(
			CreatureStatBlock creaturePortion, PlayerStatBlock playerPortion
		);

		PlayerCharacter(const PlayerCharacter& playerCharacter) = delete;
		PlayerCharacter& operator=(const PlayerCharacter& playerCharacter
		) = delete;

		PlayerCharacter(PlayerCharacter&& playerCharacter) noexcept = default;
		PlayerCharacter& operator=(PlayerCharacter&& playerCharacter
		) noexcept = default;

		~PlayerCharacter() = default;

		// For a player character the proficiency tier is the total class level across every class.
		// The total is written back into the block so the a scene can display it without
		// recomputing it, the same way a monster's XP is cached next to its challenge rating.
		static constexpr void resolveDerived(
			CreatureStatBlock& creatureBlock, PlayerStatBlock& playerBlock
		) {
			resolveSubclasses(playerBlock);
			playerBlock.derived.m_totalClassLevel =
				findTotalClassLevel(playerBlock);
			Creature::resolveDerived(
				creatureBlock, playerBlock.derived.m_totalClassLevel
			);
		}

		/*
		The gate the storage layer asks before a player character is written out.
		Validation for total player class levels is done via two steps:
		1) At least one class has a level allocated.
		2) The total allocated level doesn't surpass MAX_TOTAL_CLASS_LEVEL
		A single level can never be negative as it is bounded, so a positive total covers the first step.
		The total is recomputed rather than read off the block, so the answer never depends on when the last resolve ran.
		Every stored subclass also has to belong to a class that actually reached the threshold, which is
		the one rule a draft is free to break while its levels are still being typed in.
		*/
		static constexpr bool isValid(
			const CreatureStatBlock& creatureBlock,
			const PlayerStatBlock& playerBlock
		) {

			// Should be computed from scratch as we don't have a guarentee of calling resolveDerived() everywhere.
			const auto totalClassLevel{ findTotalClassLevel(playerBlock) };
			if (!Creature::isValid(creatureBlock) ||
				totalClassLevel <= MIN_CLASS_LEVEL ||
				totalClassLevel > MAX_TOTAL_CLASS_LEVEL) {
				return false;
			}
			for (std::size_t classIndex{};
				 classIndex < CombatTrackerLib::enumToSizeT(PlayerClass::COUNT);
				 ++classIndex) {
				if (playerBlock.subclasses[classIndex].has_value() &&
					playerBlock.levels[classIndex].get() <
						SUBCLASS_LEVEL_THRESHOLD) {
					return false;
				}
			}
			return true;
		}

	private:
		PlayerStatBlock m_statBlockPlayerPortion{};
		static constexpr int
		findTotalClassLevel(const PlayerStatBlock& statBlock) {
			return std::ranges::fold_left(
				statBlock.levels,
				0,
				[](int sum,
				   CombatTrackerLib::BoundedInt<
					   MIN_CLASS_LEVEL,
					   MAX_TOTAL_CLASS_LEVEL> boundedInt) {
					return sum + boundedInt.get();
				}
			);
		}

		// Empties the subclass slots for classes that have allocated levels below SUBCLASS_LEVEL_THRESHOLD.
		static constexpr void resolveSubclasses(PlayerStatBlock& statBlock) {
			for (std::size_t classIndex{};
				 classIndex < CombatTrackerLib::enumToSizeT(PlayerClass::COUNT);
				 ++classIndex) {
				if (statBlock.levels[classIndex].get() <
					SUBCLASS_LEVEL_THRESHOLD) {
					statBlock.subclasses[classIndex].reset();
				}
			}
		}
	};
}
