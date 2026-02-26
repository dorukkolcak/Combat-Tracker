#pragma once

#include <unordered_map>
#include <string>
#include <cstdint>
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
	An INSTANCE of any creature will have a unique ID, that will be generated at the start of encounter runnning.
	This ID isn't really used to refer to creature's type/identity, we use its name for this purpose.
	*/
	struct CreatureStatBlock {

		CreatureStatBlock() :
			name(""),
			size(Size::NO_SIZE),
			type(CreatureType::NO_TYPE),
			alignment(Alignment::UNALIGNED),
			AC{},
			HP{},
			passivePerception{},
			initiativeBonus{},
			proficiencyBonus{},
			walkSpeed{},
			climbingSpeed{},
			swimmingSpeed{},
			burrowSpeed{},
			flySpeed{},
			abilityScores{
				{Ability::STRENGTH, 10},
				{Ability::DEXTERITY, 10},
				{Ability::CONSTITUTION, 10},
				{Ability::WISDOM, 10},
				{Ability::INTELLIGENCE, 10},
				{Ability::CHARISMA, 10}
			},
			abilityScoreModifiers{
				{Ability::STRENGTH, 0},
				{Ability::DEXTERITY, 0},
				{Ability::CONSTITUTION, 0},
				{Ability::WISDOM, 0},
				{Ability::INTELLIGENCE, 0},
				{Ability::CHARISMA, 0}
			},
			savingThrowProficiencies{
				{Ability::STRENGTH, false},
				{Ability::DEXTERITY, false},
				{Ability::CONSTITUTION, false},
				{Ability::WISDOM, false},
				{Ability::INTELLIGENCE, false},
				{Ability::CHARISMA, false}
			},
			savingThrowExpertise{
				{Ability::STRENGTH, false},
				{Ability::DEXTERITY, false},
				{Ability::CONSTITUTION, false},
				{Ability::WISDOM, false},
				{Ability::INTELLIGENCE, false},
				{Ability::CHARISMA, false}
			},
			savingThrowModifiers{
				{Ability::STRENGTH, 0},
				{Ability::DEXTERITY, 0},
				{Ability::CONSTITUTION, 0},
				{Ability::WISDOM, 0},
				{Ability::INTELLIGENCE, 0},
				{Ability::CHARISMA, 0}
			},
			skillProficiencies{
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
			},
			skillExpertise{
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
			},
			skillModifiers{
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
			},
			damageVulnerabilities{
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
			},
			damageResistances{
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
			},
			damageImmunities{
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
			},
			conditionImmunities{
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
			},
			languages{
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
			},
			senses{
				{Sense::BLINDSIGHT, 0},
				{Sense::DARKVISION, 0},
				{Sense::TREMORSENSE, 0},
				{Sense::TRUESIGHT, 0}
			},
			actions{},
			traits{},
			equipment{}
		{}

		//MANUALLY SET
		std::string name;
		// MANUALLY SET
		Size size;
		// MANUALLY SET FOR Monster
		// OVERRIDABLE, COMPUTED IF NOT OVERRIDDEN for PlayerCharacter (HUMANOID for all Species)
		CreatureType type;
		// MANUALLY SET
		Alignment alignment;
		// MANUALLY SET
		int AC;
		// OVERRIDABLE, COMPUTED IF NOT OVERRIDDEN (Dexterity modifier)
		int initiativeBonus;
		// OVERRIDABLE, COMPUTED IF NOT OVERRIDDEN (See MAPPED_CR_TO_PB and MAPPED_PLAYER_LEVEL_TO_PB)
		int proficiencyBonus;
		// MANUALLY SET
		int HP;
		// OVERRIDABLE, COMPUTED IF NOT OVERRIDDEN (Wisdom modifier + Perception Score + 10)
		int passivePerception;
		// MANUALLY SET
		int walkSpeed;
		// MANUALLY SET
		int climbingSpeed;
		// MANUALLY SET
		int swimmingSpeed;
		// MANUALLY SET
		int burrowSpeed;
		// MANUALLY SET
		int flySpeed;
		// MANUALLY SET
		std::unordered_map<Ability, bool> savingThrowProficiencies;
		// MANUALLY SET
		std::unordered_map<Ability, bool> savingThrowExpertise;
		// MANUALLY SET
		std::unordered_map<Skill, bool> skillProficiencies;
		// MANUALLY SET
		std::unordered_map<Skill, bool> skillExpertise;
		// MANUALLY SET
		std::unordered_map<Ability, int> abilityScores;
		// OVERRIDABLE, COMPUTED IF NOT OVERRIDDEN (See MAPPED_ABILITY_SCORES)
		std::unordered_map<Ability, int> abilityScoreModifiers;
		// OVERRIDABLE, COMPUTED IF NOT OVERRIDDEN (Relevant Ability Modifier + PB or Relevant Ability Modifier + 2 x PB, depending on proficiency or expertise.)
		std::unordered_map<Ability, int> savingThrowModifiers;
		// OVERRIDABLE, COMPUTED IF NOT OVERRIDDEN (Relevant Ability Modifier + PB or Relevant Ability Modifier + 2 x PB, depending on proficiency or expertise.)
		std::unordered_map<Skill, int> skillModifiers;
		// MANUALLY SET
		std::vector<CreatureAction> actions;
		// MANUALLY SET
		std::unordered_map<DamageType, bool> damageVulnerabilities;
		// MANUALLY SET
		std::unordered_map<DamageType, bool> damageResistances;
		// MANUALLY SET
		std::unordered_map<DamageType, bool> damageImmunities;
		// MANUALLY SET
		std::unordered_map<Condition, bool> conditionImmunities;
		// MANUALLY SET, feats are conceptually included in this.
		std::vector<Trait> traits;
		// MANUALLY SET
		// The second field is the distance of the sense. 0 feet implies that the creature doesn't have that sense.
		std::unordered_map<Sense, int> senses;
		// MANUALLY SET
		std::unordered_map<Language, bool> languages;
		// MANUALLY SET
		std::vector<Gear> equipment;
	};

	class Creature {
	protected:
		// COMPUTED, NOT OVERRIDABLE
		int ID;
		// COMPUTED, NOT OVERRIDABLE
		CreatureStatBlock statBlockCreaturePortion;
	public:
		Creature(CreatureStatBlock statBlockCreaturePortion);
		~Creature();
		static int findAbilityScoreModifier(int abilityScore);
		void setID();
	};
	// Templating is required so that we can chain the correct type when creating objects with the builders.
	template <typename Derived>
	class CreatureBuilder {
	public:

		Derived& self() { return static_cast<Derived&>(*this); }
		Derived& reset() { block = CreatureStatBlock(); return self(); }

		Derived& setName(std::string creatureName) { block.name = std::move(creatureName); return self(); }
		Derived& setSize(const Size creatureSize) { block.size = creatureSize; return self(); }
		Derived& setType(const CreatureType creatureType) { block.type = creatureType; return self(); }
		Derived& setAlignment(const Alignment creatureAlignment) { block.alignment = creatureAlignment; return self(); }
		Derived& setAC(const int creatureAC) { block.AC = creatureAC; return self(); }
		Derived& setInitiativeBonus(const int creatureInitiativeBonus) { block.initiativeBonus = creatureInitiativeBonus; return self(); }
		Derived& setPB(const int creatureProficiencyBonus) { block.proficiencyBonus = creatureProficiencyBonus; return self(); }
		Derived& setHP(const int creatureHP) { block.HP = creatureHP; return self(); }
		Derived& setPassivePerception(const int creaturePassivePerception) { return self(); }
		Derived& setWalkSpeed(const int creatureWalkSpeed) { block.walkSpeed = creatureWalkSpeed; return self(); }
		Derived& setClimbingSpeed(const int creatureClimbingSpeed) { block.climbingSpeed = creatureClimbingSpeed; return self(); }
		Derived& setSwimmingSpeed(const int creatureSwimmingSpeed) { block.swimmingSpeed = creatureSwimmingSpeed; return self(); }
		Derived& setBurrowSpeed(const int creatureBurrowSpeed) { block.burrowSpeed = creatureBurrowSpeed; return self(); }
		Derived& setFlySpeed(const int creatureFlySpeed) { block.flySpeed = creatureFlySpeed;  return self(); }
		Derived& setSavingThrowProficiencies(const std::unordered_map<Ability, bool>& creatureSavingThrowProficiencies) { block.savingThrowProficiencies = creatureSavingThrowProficiencies; return self(); }
		Derived& setSavingThrowExpertise(const std::unordered_map<Ability, bool>& creatureSavingThrowExpertise) { block.savingThrowExpertise = creatureSavingThrowExpertise;  return self(); }
		Derived& setSkillProficiencies(const std::unordered_map<Skill, bool>& creatureSkillProficiencies) { block.skillProficiencies = creatureSkillProficiencies; return self(); }
		Derived& setSkillExpertise(const std::unordered_map<Skill, bool>& creatureSkillExpertise) { block.skillExpertise = creatureSkillExpertise; return self(); }
		Derived& setAbilityScores(const std::unordered_map<Ability, int>& creatureAbilityScores) { block.abilityScores = creatureAbilityScores;  return self(); }
		Derived& setAbilityScoreModifiers(const std::unordered_map<Ability, int>& creatureAbilityScoreModifiers) { block.abilityScoreModifiers = creatureAbilityScoreModifiers; return self(); }
		Derived& setSavingThrowModifiers(const std::unordered_map<Ability, int>& creatureSavingThrowModifiers) { block.savingThrowModifiers = creatureSavingThrowModifiers; return self(); }
		Derived& setSkillModifiers(const std::unordered_map<Skill, int>& creatureSkillModifiers) { block.skillModifiers = creatureSkillModifiers; return self(); }
		Derived& setActions(const std::vector<CreatureAction>& creatureActions) { block.actions = creatureActions; return self(); }
		Derived& setDamageVulnerabilities(const std::unordered_map<DamageType, bool>& creatureDamageVulnerabilities) { block.damageVulnerabilities = creatureDamageVulnerabilities;  return self(); }
		Derived& setDamageResistances(const std::unordered_map<DamageType, bool>& creatureDamageResistances) { block.damageResistances = creatureDamageResistances;  return self(); }
		Derived& setDamageImmunities(const std::unordered_map<DamageType, bool>& creatureDamageImmunities) { block.damageImmunities = creatureDamageImmunities; return self(); }
		Derived& setConditionImmunities(const std::unordered_map<Condition, bool>& creatureConditionImmunities) { block.conditionImmunities = creatureConditionImmunities; return self(); }
		Derived& setTraits(const std::vector<Trait>& creatureTraits) { block.traits = creatureTraits; return self(); }
		Derived& setSenses(const std::unordered_map<Sense, int>& creatureSenses) { block.senses = creatureSenses; return self(); }
		Derived& setLanguages(const std::unordered_map<Language, bool>& creatureLanguages) { block.languages = creatureLanguages; return self(); }
		Derived& setEquipment(const std::vector<Gear>& creatureEquipment) { block.equipment = creatureEquipment; return self(); }

		CreatureStatBlock& getCreatureBlock() { return block; }
		CreatureStatBlock moveCreatureBlock() { return std::move(block);  }
	protected:
		CreatureStatBlock block;
	};
}
