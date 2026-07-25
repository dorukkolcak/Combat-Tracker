#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>

#include "CreatureEnums.h"
#include "CreatureTypes.h"


namespace CombatTrackerApp {

	/*
	A creature's name must be unique and its name will be used to refer to the overall creature.
	This also implies that the saved named creatures will have their own unique names.
	This DOESN'T mean that we can't have creatures with the same name in an encounter.
	We can have multiples of the same creature. To refer to each instance of the same creature, we will use IDs.
	An INSTANCE of any creature will have a unique ID, that will be generated at the start of encounter running.
	This ID isn't really used to refer to creature's type/identity, we use its name for this purpose.
	The stat block types shouldn't be nested types as they are used by the builders.
	Also the struct representing the stat blocks should be an aggregate.
	*/
	struct CreatureStatBlock {

		// Order struct data members in decreasing size to potentially minimize padding.
		std::unordered_map<Ability, bool> savingThrowProficiencies {
			{Ability::STRENGTH, false},
			{Ability::DEXTERITY, false},
			{Ability::CONSTITUTION, false},
			{Ability::WISDOM, false},
			{Ability::INTELLIGENCE, false},
			{Ability::CHARISMA, false} 
		};
		std::unordered_map<Ability, bool> savingThrowExpertise {
			{Ability::STRENGTH, false},
			{Ability::DEXTERITY, false},
			{Ability::CONSTITUTION, false},
			{Ability::WISDOM, false},
			{Ability::INTELLIGENCE, false},
			{Ability::CHARISMA, false} 
		};
		std::unordered_map<Skill, bool> skillProficiencies {
			{Skill::ATHLETICS, false},
			{Skill::ACROBATICS, false},
			{Skill::SLEIGHT_OF_HAND, false},
			{Skill::STEALTH, false},
			{Skill::ARCANA, false},
			{Skill::HISTORY, false},
			{Skill::INVESTIGATION, false},
			{Skill::NATURE, false},
			{Skill::RELIGION, false},
			{Skill::ANIMAL_HANDLING, false},
			{Skill::INSIGHT, false},
			{Skill::MEDICINE, false},
			{Skill::PERCEPTION, false},
			{Skill::SURVIVAL, false},
			{Skill::DECEPTION, false},
			{Skill::INTIMIDATION, false},
			{Skill::PERFORMANCE, false},
			{Skill::PERSUASION, false}
		};
		std::unordered_map<Skill, bool> skillExpertise {
			{Skill::ATHLETICS, false},
			{Skill::ACROBATICS, false},
			{Skill::SLEIGHT_OF_HAND, false},
			{Skill::STEALTH, false},
			{Skill::ARCANA, false},
			{Skill::HISTORY, false},
			{Skill::INVESTIGATION, false},
			{Skill::NATURE, false},
			{Skill::RELIGION, false},
			{Skill::ANIMAL_HANDLING, false},
			{Skill::INSIGHT, false},
			{Skill::MEDICINE, false},
			{Skill::PERCEPTION, false},
			{Skill::SURVIVAL, false},
			{Skill::DECEPTION, false},
			{Skill::INTIMIDATION, false},
			{Skill::PERFORMANCE, false},
			{Skill::PERSUASION, false} 
		};
		std::unordered_map<Ability, int> abilityScores {
			{Ability::STRENGTH, 10},
			{Ability::DEXTERITY, 10},
			{Ability::CONSTITUTION, 10},
			{Ability::WISDOM, 10},
			{Ability::INTELLIGENCE, 10},
			{Ability::CHARISMA, 10} 
		};
		std::unordered_map<Ability, int> abilityScoreModifiers {
			{Ability::STRENGTH, 0},
			{Ability::DEXTERITY, 0},
			{Ability::CONSTITUTION, 0},
			{Ability::WISDOM, 0},
			{Ability::INTELLIGENCE, 0},
			{Ability::CHARISMA, 0} 
		};
		std::unordered_map<Ability, int> savingThrowModifiers {
			{Ability::STRENGTH, 0},
			{Ability::DEXTERITY, 0},
			{Ability::CONSTITUTION, 0},
			{Ability::WISDOM, 0},
			{Ability::INTELLIGENCE, 0},
			{Ability::CHARISMA, 0} 
		};
		std::unordered_map<Skill, int> skillModifiers {
			{Skill::ATHLETICS, 0},
			{Skill::ACROBATICS, 0},
			{Skill::SLEIGHT_OF_HAND, 0},
			{Skill::STEALTH, 0},
			{Skill::ARCANA, 0},
			{Skill::HISTORY, 0},
			{Skill::INVESTIGATION, 0},
			{Skill::NATURE, 0},
			{Skill::RELIGION, 0},
			{Skill::ANIMAL_HANDLING, 0},
			{Skill::INSIGHT, 0},
			{Skill::MEDICINE, 0},
			{Skill::PERCEPTION, 0},
			{Skill::SURVIVAL, 0},
			{Skill::DECEPTION, 0},
			{Skill::INTIMIDATION, 0},
			{Skill::PERFORMANCE, 0},
			{Skill::PERSUASION, 0} 
		};
		std::unordered_map<DamageType, bool> damageVulnerabilities {
			{DamageType::ACID, false},
			{DamageType::BLUDGEONING, false},
			{DamageType::COLD, false},
			{DamageType::FIRE, false},
			{DamageType::FORCE, false},
			{DamageType::LIGHTNING, false},
			{DamageType::NECROTIC, false},
			{DamageType::PIERCING, false},
			{DamageType::POISON, false},
			{DamageType::PSYCHIC, false},
			{DamageType::RADIANT, false},
			{DamageType::SLASHING, false},
			{DamageType::THUNDER, false} 
		};
		std::unordered_map<DamageType, bool> damageResistances {
			{DamageType::ACID, false},
			{DamageType::BLUDGEONING, false},
			{DamageType::COLD, false},
			{DamageType::FIRE, false},
			{DamageType::FORCE, false},
			{DamageType::LIGHTNING, false},
			{DamageType::NECROTIC, false},
			{DamageType::PIERCING, false},
			{DamageType::POISON, false},
			{DamageType::PSYCHIC, false},
			{DamageType::RADIANT, false},
			{DamageType::SLASHING, false},
			{DamageType::THUNDER, false} 
		};
		std::unordered_map<DamageType, bool> damageImmunities {
			{DamageType::ACID, false},
			{DamageType::BLUDGEONING, false},
			{DamageType::COLD, false},
			{DamageType::FIRE, false},
			{DamageType::FORCE, false},
			{DamageType::LIGHTNING, false},
			{DamageType::NECROTIC, false},
			{DamageType::PIERCING, false},
			{DamageType::POISON, false},
			{DamageType::PSYCHIC, false},
			{DamageType::RADIANT, false},
			{DamageType::SLASHING, false},
			{DamageType::THUNDER, false}
		};
		std::unordered_map<Condition, bool> conditionImmunities {
			{Condition::BLINDED, false},
			{Condition::CHARMED, false},
			{Condition::DEAFENED, false},
			{Condition::FRIGHTENED, false},
			{Condition::GRAPPLED, false},
			{Condition::INCAPACITATED, false},
			{Condition::INVISIBLE, false},
			{Condition::PARALYZED, false},
			{Condition::PETRIFIED, false},
			{Condition::POISONED, false},
			{Condition::PRONE, false},
			{Condition::RESTRAINED, false},
			{Condition::STUNNED, false},
			{Condition::UNCONSCIOUS, false},
			{Condition::EXHAUSTION, false} 
		};
		std::unordered_map<Sense, int> senses {
			{Sense::BLINDSIGHT, 0},
			{Sense::DARKVISION, 0},
			{Sense::TREMORSENSE, 0},
			{Sense::TRUESIGHT, 0}
		};
		std::unordered_map<Language, bool> languages {
			{Language::COMMON, false},
			{Language::COMMON_SIGN_LANGUAGE, false},
			{Language::DWARVISH, false},
			{Language::ELVISH, false},
			{Language::GIANT, false},
			{Language::GNOMISH, false},
			{Language::GOBLIN, false},
			{Language::HALFLING, false},
			{Language::ORC, false},
			{Language::ABYSSAL, false},
			{Language::CELESTIAL, false},
			{Language::DEEP_SPEECH, false},
			{Language::DRUIDIC, false},
			{Language::INFERNAL, false},
			{Language::PRIMORDIAL, false},
			{Language::SYLVAN, false},
			{Language::THIEVES_CANT, false},
			{Language::UNDERCOMMON, false} 
		};

		std::vector<CreatureAction> actions{};
		std::vector<Trait> traits{};
		std::vector<Gear> equipment{};

		std::string name{""};

		Size size{};
		CreatureType type{};
		Alignment alignment{};

		int AC{};
		int initiativeBonus{};
		int proficiencyBonus{};
		int HP{};
		int passivePerception{};
		int walkSpeed{};
		int climbingSpeed{};
		int swimmingSpeed{};
		int burrowSpeed{};
		int flySpeed{};
	};

	class Creature {
	public:
		static constexpr int findAbilityScoreModifier(int abilityScore) {
			return (abilityScore - 10) / 2;
		}
	protected:

		// A creature will own its stat block, hence we use std::move in the constructor. 
		// As for how the stat block is created, we use the Builder design pattern and pass the corresponding stat block from the builder.
		// Also this constructor should be protected as we don't really want to allow anyone to create a Creature instance.
		explicit Creature(CreatureStatBlock statBlockCreaturePortion);

		CreatureStatBlock m_statBlockCreaturePortion{};
		inline static int currentIDValue{1};
		int m_ID{};
	};

	// Templating is required so that we can chain the correct type when creating objects with the builders.
	// Also note that for template instantiation, the class definition has to be in the header. 
	// Unlike the subclasses of this class, which have only declarations in the headers.
	template <typename Base>
	class CreatureBuilder {
	public:

		// These two member functions can also be virtual but using the templated class parameter for the return type also works.
		Base& self() { return static_cast<Base&>(*this); }
		Base& reset() { m_block = CreatureStatBlock{}; return self(); } // Assign an empty-defaulted stat block. 

		Base& setSavingThrowProficiencies(const std::unordered_map<Ability, bool>& creatureSavingThrowProficiencies) { m_block.savingThrowProficiencies = creatureSavingThrowProficiencies; return self(); }
		Base& setSavingThrowExpertise(const std::unordered_map<Ability, bool>& creatureSavingThrowExpertise) { m_block.savingThrowExpertise = creatureSavingThrowExpertise;  return self(); }
		Base& setSkillProficiencies(const std::unordered_map<Skill, bool>& creatureSkillProficiencies) { m_block.skillProficiencies = creatureSkillProficiencies; return self(); }
		Base& setSkillExpertise(const std::unordered_map<Skill, bool>& creatureSkillExpertise) { m_block.skillExpertise = creatureSkillExpertise; return self(); }
		Base& setAbilityScores(const std::unordered_map<Ability, int>& creatureAbilityScores) { m_block.abilityScores = creatureAbilityScores;  return self(); }
		Base& setAbilityScoreModifiers(const std::unordered_map<Ability, int>& creatureAbilityScoreModifiers) { m_block.abilityScoreModifiers = creatureAbilityScoreModifiers; return self(); }
		Base& setSavingThrowModifiers(const std::unordered_map<Ability, int>& creatureSavingThrowModifiers) { m_block.savingThrowModifiers = creatureSavingThrowModifiers; return self(); }
		Base& setSkillModifiers(const std::unordered_map<Skill, int>& creatureSkillModifiers) { m_block.skillModifiers = creatureSkillModifiers; return self(); }
		Base& setActions(const std::vector<CreatureAction>& creatureActions) { m_block.actions = creatureActions; return self(); }
		Base& setDamageVulnerabilities(const std::unordered_map<DamageType, bool>& creatureDamageVulnerabilities) { m_block.damageVulnerabilities = creatureDamageVulnerabilities;  return self(); }
		Base& setDamageResistances(const std::unordered_map<DamageType, bool>& creatureDamageResistances) { m_block.damageResistances = creatureDamageResistances;  return self(); }
		Base& setDamageImmunities(const std::unordered_map<DamageType, bool>& creatureDamageImmunities) { m_block.damageImmunities = creatureDamageImmunities; return self(); }
		Base& setConditionImmunities(const std::unordered_map<Condition, bool>& creatureConditionImmunities) { m_block.conditionImmunities = creatureConditionImmunities; return self(); }
		Base& setTraits(const std::vector<Trait>& creatureTraits) { m_block.traits = creatureTraits; return self(); }
		Base& setSenses(const std::unordered_map<Sense, int>& creatureSenses) { m_block.senses = creatureSenses; return self(); }
		Base& setLanguages(const std::unordered_map<Language, bool>& creatureLanguages) { m_block.languages = creatureLanguages; return self(); }
		Base& setEquipment(const std::vector<Gear>& creatureEquipment) { m_block.equipment = creatureEquipment; return self(); }

		Base& setName(std::string creatureName) { m_block.name = std::move(creatureName); return self(); } // This should probably be a string_view but I don't want to refactor a large chunk of code currently.

		Base& setSize(Size creatureSize) { m_block.size = creatureSize; return self(); }
		Base& setType(CreatureType creatureType) { m_block.type = creatureType; return self(); }
		Base& setAlignment(Alignment creatureAlignment) { m_block.alignment = creatureAlignment; return self(); }
		Base& setAC(int creatureAC) { m_block.AC = creatureAC; return self(); }
		Base& setInitiativeBonus(int creatureInitiativeBonus) { m_block.initiativeBonus = creatureInitiativeBonus; return self(); }
		Base& setPB(int creatureProficiencyBonus) { m_block.proficiencyBonus = creatureProficiencyBonus; return self(); }
		Base& setHP(int creatureHP) { m_block.HP = creatureHP; return self(); }
		Base& setPassivePerception(const int creaturePassivePerception) { m_block.passivePerception = creaturePassivePerception; return self(); }
		Base& setWalkSpeed(int creatureWalkSpeed) { m_block.walkSpeed = creatureWalkSpeed; return self(); }
		Base& setClimbingSpeed(int creatureClimbingSpeed) { m_block.climbingSpeed = creatureClimbingSpeed; return self(); }
		Base& setSwimmingSpeed(int creatureSwimmingSpeed) { m_block.swimmingSpeed = creatureSwimmingSpeed; return self(); }
		Base& setBurrowSpeed(int creatureBurrowSpeed) { m_block.burrowSpeed = creatureBurrowSpeed; return self(); }
		Base& setFlySpeed(int creatureFlySpeed) { m_block.flySpeed = creatureFlySpeed;  return self(); }

		CreatureStatBlock& getCreatureBlock() { return m_block; }

		/*
		The builder is meant to be single used:
		It moves the block out and re-initializes it to a fresh default, so the builder is ready to be used from scratch after a move.
		Note that a moved-from block would be left empty, not default valued, so we must reinitialize it.
		*/
		CreatureStatBlock moveCreatureBlock() {
			CreatureStatBlock movedBlock{ std::move(m_block) };
			m_block = CreatureStatBlock{};
			return movedBlock; // Important: No std::move() here to utilize NRVO.
		}
	protected:
		CreatureStatBlock m_block{};
	};
}
