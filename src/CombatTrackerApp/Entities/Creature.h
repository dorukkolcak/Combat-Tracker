#pragma once

#include "CombatTrackerLib/Utilities/GenericUtils.h"

#include <algorithm>
#include <array>
#include <climits>
#include <cstddef>
#include <string>
#include <vector>

namespace CombatTrackerApp {

	/*
	All enum classes (mostly) have as the last enumerator a count.
	This dictates the size of the array for the corresponding structure that holds information for a particular enum inside an entity.
	The enumerator order should never change (mostly for saving/loading purposes), only additions and removals can be made.
	Also the additions should be made to the end of the enumerator order, just before COUNT.
	As the baseline D&D 5e ruleset won't change probably ever, the constraint on changing is actually quite reasonable I would say.
	*/
	enum class Size {
		TINY,
		SMALL,
		MEDIUM,
		LARGE,
		HUGE_SIZE, // Not HUGE: <math.h> defines that as a macro on macOS/BSD. Funny bug I discovered through Github Workflows.
		GARGANTUAN,
		COUNT
	};
	enum class Alignment {
		UNALIGNED_ALIGNMENT, // Same class of bug as above, found through CI.
		LAWFUL_GOOD,
		NEUTRAL_GOOD,
		CHAOTIC_GOOD,
		LAWFUL_NEUTRAL,
		NEUTRAL,
		CHAOTIC_NEUTRAL,
		LAWFUL_EVIL,
		NEUTRAL_EVIL,
		CHAOTIC_EVIL,
		COUNT
	};
	enum class ActionType {
		ACTION,
		BONUS_ACTION,
		REACTION,
		LEGENDARY_ACTION,
		LAIR_ACTION,
		COUNT
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
		COUNT
	};
	enum class Ability {
		STRENGTH,
		DEXTERITY,
		CONSTITUTION,
		WISDOM,
		INTELLIGENCE,
		CHARISMA,
		COUNT
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
		COUNT
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
		COUNT
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
		COUNT
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
		UNDEAD,
		COUNT
	};
	enum class Sense {
		BLINDSIGHT,
		DARKVISION,
		TREMORSENSE,
		TRUESIGHT,
		COUNT
	};
	enum class MovementType {
		WALK,
		CLIMB,
		SWIM,
		BURROW,
		FLY,
		COUNT
	};

	// These two enums below don't have counts as we don't really need them in context.
	// They don't have a corresponding in memory array.

	// A creature reacts to a damage type in exactly one way, so the three reactions share a single slot
	// instead of sitting in three parallel flags that could all be set at once.
	enum class DamageReaction {
		NONE,
		VULNERABLE,
		RESISTANT,
		IMMUNE,
	};

	// Use this enum as the multiplier for proficiency bonus adding:
	// PROFICIENT = 1, EXPERT = 2.
	enum class ProficiencyLevel {
		NONE,
		PROFICIENT,
		EXPERT,
	};

	struct CreatureAction {
		std::string name{};
		std::string description{};
		ActionType type{};
	};

	// Used for representing traits and gear.
	struct NamedDescription {
		std::string name{};
		std::string description{};
	};

	// The order is via enumeration order, same idea as every other array structure.
	inline constexpr std::array<Ability, static_cast<std::size_t>(Skill::COUNT)> MAPPED_SKILLS{
		Ability::STRENGTH,     // ATHLETICS
		Ability::DEXTERITY,    // ACROBATICS
		Ability::DEXTERITY,    // SLEIGHT_OF_HAND
		Ability::DEXTERITY,    // STEALTH
		Ability::INTELLIGENCE, // ARCANA
		Ability::INTELLIGENCE, // HISTORY
		Ability::INTELLIGENCE, // INVESTIGATION
		Ability::INTELLIGENCE, // NATURE
		Ability::INTELLIGENCE, // RELIGION
		Ability::WISDOM,       // ANIMAL_HANDLING
		Ability::WISDOM,       // INSIGHT
		Ability::WISDOM,       // MEDICINE
		Ability::WISDOM,       // PERCEPTION
		Ability::WISDOM,       // SURVIVAL
		Ability::CHARISMA,     // DECEPTION
		Ability::CHARISMA,     // INTIMIDATION
		Ability::CHARISMA,     // PERFORMANCE
		Ability::CHARISMA      // PERSUASION
	};
	inline constexpr int PB_LOWER_BOUND{2};
	inline constexpr int PB_UPPER_BOUND{9};
	inline constexpr int ABILITY_SCORE_LOWER_BOUND{};
	inline constexpr int ABILITY_SCORE_UPPER_BOUND{30};
	inline constexpr int ABILITY_MODIFIERS_UPPER_BOUND{10};
	inline constexpr int ABILITY_MODIFIERS_LOWER_BOUND{-5};
	inline constexpr int PASSIVE_PERCEPTION_BASE{10};

	/*
	Represents the unserialized/derived stats of a creature.
	The members of this class should only be readable by others.
	The members shouldn't be arbitraly writable like aggregate struct members (our stat blocks).
	This is the reason there is a distinction between the members of stat blocks and the members of this class.
	*/
	class CreatureDerivedStats {
	public:
		constexpr int getAbilityScoreModifier(Ability ability) const {
			return m_abilityScoreModifiers[CombatTrackerLib::enumToSizeT(ability)];
		}

		constexpr int getProficiencyBonus() const { return m_proficiencyBonus.get(); }

	private:
		// Creature class is the class responsible for actually deriving the member values.
		// So we grant it private access by making it a friend.
		friend class Creature;

	private:
		// This doesn't need to be bounded as it is directly calculated from ability scores (which itself is bounded).
		std::array<int, static_cast<std::size_t>(Ability::COUNT)> m_abilityScoreModifiers{};

		/*
		PB is calculated for both PlayerCharacters and Monsters, and has a hard cap of 9.
		I don't think this needs to be bounded as well as it is derived from a bounded value.
		But I am not as sure on this as I am with ability score modifiers.
		So I will keep this bounded for now.
		*/
		CombatTrackerLib::BoundedInt<PB_LOWER_BOUND, PB_UPPER_BOUND> m_proficiencyBonus{};
	};

	/*
	A creature's name must be unique and its name will be used to refer to the overall creature.
	This also implies that the saved named creatures will have their own unique names.
	This DOESN'T mean that we can't have creatures with the same name in an encounter.
	We can have multiples of the same creature. To refer to each instance of the same creature, we will use IDs.
	An INSTANCE of any creature will have a unique ID, that will be generated at the start of encounter running.
	This ID isn't really used to refer to creature's type/identity, we use its name for this purpose.
	*/
	struct CreatureStatBlock {

		std::array<ProficiencyLevel, static_cast<std::size_t>(Ability::COUNT)> savingThrowProficiencyLevels{};
		std::array<ProficiencyLevel, static_cast<std::size_t>(Skill::COUNT)> skillProficiencyLevels{};

		// Should be set by the user but have defined lower and upper bounds, per the game rules.
		std::array<
			CombatTrackerLib::BoundedInt<ABILITY_SCORE_LOWER_BOUND, ABILITY_SCORE_UPPER_BOUND>,
			static_cast<std::size_t>(Ability::COUNT)>
			abilityScores{{10, 10, 10, 10, 10, 10}};

		// These two values should be overriddable because of item/class effects.
		std::array<CombatTrackerLib::OverridableInt, static_cast<std::size_t>(Ability::COUNT)> savingThrowModifiers{};
		std::array<CombatTrackerLib::OverridableInt, static_cast<std::size_t>(Skill::COUNT)> skillModifiers{};

		// Represents the status of a player character towards a damage type (None, resistant, vulnerable, immune).
		std::array<DamageReaction, static_cast<std::size_t>(DamageType::COUNT)> damageReactions{};

		std::array<bool, static_cast<std::size_t>(Condition::COUNT)> conditionImmunities{};

		std::array<CombatTrackerLib::BoundedInt<0, INT_MAX>, static_cast<std::size_t>(Sense::COUNT)> senses{};
		std::array<CombatTrackerLib::BoundedInt<0, INT_MAX>, static_cast<std::size_t>(MovementType::COUNT)> speeds{};
		std::array<bool, static_cast<std::size_t>(Language::COUNT)> languages{};

		std::vector<CreatureAction> actions{};
		std::vector<NamedDescription> traits{};
		std::vector<NamedDescription> equipment{};

		std::string name{};

		Size size{};
		CreatureType type{};
		Alignment alignment{};

		// These can be calculated via other stats but should also be overridable thanks to special effects.
		CombatTrackerLib::OverridableInt initiativeBonus{};
		CombatTrackerLib::OverridableInt passivePerception{};

		CombatTrackerLib::BoundedInt<1, INT_MAX> AC{};
		CombatTrackerLib::BoundedInt<1, INT_MAX> HP{};

		/*
		Everything above is set by the user and is free to be half finished during entity creation, edit, etc.
		The derived stats are as the name suggests derived from the above members so they shouldn't be half finished.
		This is why their members are hidden. But the derived stat itself is not hidden.
		So this struct is still an aggregate while technically having private members.
		*/
		CreatureDerivedStats derived{};
	};

	class Creature {
	public:
		friend class CreatureTest;

	protected:
		CreatureStatBlock m_statBlockCreaturePortion{};
		int m_ID{};

	protected:
		/*
		This constructor should be protected as we don't really want to allow anyone to create a Creature instance.
		The actual Creature object should only be created for encounter runnning as the use of ID values make sense there.
		Otherwise, the stat block management should be done just through the struct.
		This is also why this class has a lot of static functions as those functions should operate on a stat block instance,
		not necessarily a creature instance.
		*/
		explicit Creature(CreatureStatBlock statBlockCreaturePortion);

		Creature(const Creature&) = delete;
		Creature& operator=(const Creature&) = delete;
		Creature(Creature&&) noexcept = default;
		Creature& operator=(Creature&&) noexcept = default;
		~Creature() = default;

		/*
		This function should be "indirectly" called by the scenes at the end of an UI render 
		for values of members that are derived from other members.
		This isn't the most efficient way of doing this, we recompute everything even though the dependent members might not have changed.
		However, because the resolving process depends on a certain order of operations
		And breaking up this function creates a lot of boilerplate that is hard to track.
		Also the performance gain is very, very minimal compared to rendering UI components.
		*/
		static constexpr void resolveDerived(CreatureStatBlock& statBlock, double proficiencyTier) {

			// Taking the tier as an argument means the proficiency bonus cannot be left stale by a caller
			// that forgets to set it first, which matters as every saving throw and skill below scales by it.
			statBlock.derived.m_proficiencyBonus = findProficiencyBonus(proficiencyTier);
			for (std::size_t abilityIndex{}; abilityIndex < CombatTrackerLib::enumToSizeT(Ability::COUNT);
			     ++abilityIndex) {
				statBlock.derived.m_abilityScoreModifiers[abilityIndex] =
					findAbilityScoreModifier(statBlock.abilityScores[abilityIndex].get());
			}
			for (std::size_t abilityIndex{}; abilityIndex < CombatTrackerLib::enumToSizeT(Ability::COUNT);
			     ++abilityIndex) {
				statBlock.savingThrowModifiers[abilityIndex].setDefault(
					findSavingThrowModifier(statBlock, static_cast<Ability>(abilityIndex))
				);
			}
			for (std::size_t skillIndex{}; skillIndex < CombatTrackerLib::enumToSizeT(Skill::COUNT); ++skillIndex) {
				statBlock.skillModifiers[skillIndex].setDefault(
					findSkillModifier(statBlock, static_cast<Skill>(skillIndex))
				);
			}
			statBlock.initiativeBonus.setDefault(findInitiativeBonus(statBlock));
			statBlock.passivePerception.setDefault(findPassivePerception(statBlock));
		}

		/*
		Before writing the creature info to disk, this function should be called (indirectly via base classes).
		It validates values that can be typed freely and have no real checking elsewhere (via inherent validation done by types).
		TODO: Feel free to add more validation, particularly to creature naming. 
		Currently empty as all validation goes through inherent stages (for creatures).
		*/
		static constexpr bool isValid(const CreatureStatBlock&) { return true; }

	private:
		inline static int s_currentIDValue{1};

		/*
		These static functions are used for entity creation/modification purposes.
		The way we create entities is via their stat blocks, not through the actual class type.
		We use the class type for encounter running purposes where we actually use the id field.
		Hence we never instantiate a Creature type during entity creation/modification.
		This is why these functions are static.
		Furthermore, the functions are put here and not inside the scenes 
		or not inside the only top level function that calls them for a couple of reasons:
		1) These are entity specific rules so the Creature class should own them.
		2) If these functions weren't standalone, testing them would require a Window context, which we definitely don't want.
		So even though these won't probably be used other than entity creation scenes, they are still kept here.
		*/

		// Surprisingly PB is calculated the same way for both PlayerCharacters and Monsters.
		static constexpr int findProficiencyBonus(double proficiencyTier) {
			return PB_LOWER_BOUND + (proficiencyTier < 1 ? 0 : static_cast<int>((proficiencyTier - 1) / 4));
		}

		// The modifier is the difference from ten, halved and rounded down.
		// Integer division truncates towards zero instead, so a negative odd difference has to be decremented by one first.
		static constexpr int findAbilityScoreModifier(int abilityScore) {
			const auto difference{abilityScore - 10};
			if (difference < 0 && difference % 2 != 0) {
				return (difference - 1) / 2;
			}
			return difference / 2;
		}

		/*
		Saving Mod = relevant ability mod + PB if proficient.
		Saving Mod = relevant ability mod + 2 * PB if expert.
		Saving Mod = relevant ability mod if none.
		The proficiency level's ordinal is the multiplier that scales the proficiency bonus.
		*/
		static constexpr int findSavingThrowModifier(const CreatureStatBlock& statBlock, Ability ability) {
			const auto abilityIndex{CombatTrackerLib::enumToSizeT(ability)};
			const auto proficiencyLevel{statBlock.savingThrowProficiencyLevels[abilityIndex]};
			return statBlock.derived.m_abilityScoreModifiers[abilityIndex] +
			       statBlock.derived.m_proficiencyBonus.get() * static_cast<int>(proficiencyLevel);
		}

		// Skill Mod = backing ability mod, scaled by proficiency the same way saving throws are.
		static constexpr int findSkillModifier(const CreatureStatBlock& statBlock, Skill skill) {
			const auto skillIndex{CombatTrackerLib::enumToSizeT(skill)};
			const auto abilityIndex{CombatTrackerLib::enumToSizeT(MAPPED_SKILLS[skillIndex])};
			const auto proficiencyLevel{statBlock.skillProficiencyLevels[skillIndex]};
			return statBlock.derived.m_abilityScoreModifiers[abilityIndex] +
			       statBlock.derived.m_proficiencyBonus.get() * static_cast<int>(proficiencyLevel);
		}

		// Passive Perception = Perception mod + 10.
		static constexpr int findPassivePerception(const CreatureStatBlock& statBlock) {
			return statBlock.skillModifiers[CombatTrackerLib::enumToSizeT(Skill::PERCEPTION)].get() +
			       PASSIVE_PERCEPTION_BASE;
		}

		// Initiative Bonus = Dexterity mod.
		static constexpr int findInitiativeBonus(const CreatureStatBlock& statBlock) {
			return statBlock.derived.m_abilityScoreModifiers[CombatTrackerLib::enumToSizeT(Ability::DEXTERITY)];
		}
	};
}
