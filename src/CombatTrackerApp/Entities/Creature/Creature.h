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
		std::unordered_set<Ability> savingThrowProficiencies;
		// MANUALLY SET
		std::unordered_set<Ability> savingThrowExpertise;
		// MANUALLY SET
		std::unordered_set<Skill> skillProficiencies;
		// MANUALLY SET
		std::unordered_set<Skill> skillExpertise;
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
		std::unordered_set<DamageType> damageVulnerabilities;
		// MANUALLY SET
		std::unordered_set<DamageType> damageResistances;
		// MANUALLY SET
		std::unordered_set<DamageType> damageImmunities;
		// MANUALLY SET
		std::unordered_set<Condition> conditionImmunities;
		// MANUALLY SET, feats are conceptually included in this.
		std::vector<Trait> traits;
		// MANUALLY SET
		// The second field is the distance of the sense. 0 feet implies that the creature doesn't have that sense.
		std::unordered_map<Sense, int> senses;
		// MANUALLY SET
		std::unordered_set<Language> languages;
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
	};
	class CreatureBuilder {
	public:
		CreatureBuilder& setName(std::string creatureName);
		CreatureBuilder& setSize(const Size creatureSize);
		CreatureBuilder& setType(const CreatureType creatureType);
		CreatureBuilder& setAlignment(const Alignment creatureAlignment);
		CreatureBuilder& setAC(const int creatureAC);
		CreatureBuilder& setInitiativeBonus(const int creatureInitiativeBonus);
		CreatureBuilder& setPB(const int creatureProficiencyBonus);
		CreatureBuilder& setHP(const int creatureHP);
		CreatureBuilder& setPassivePerception(const int creaturePassivePerception);
		CreatureBuilder& setWalkSpeed(const int creatureWalkSpeed);
		CreatureBuilder& setClimbingSpeed(const int creatureClimbingSpeed);
		CreatureBuilder& setSwimmingSpeed(const int creatureSwimmingSpeed);
		CreatureBuilder& setBurrowSpeed(const int creatureBurrowSpeed);
		CreatureBuilder& setFlySpeed(const int creatureFlySpeed);
		CreatureBuilder& setSavingThrowProficiencies(std::unordered_set<Ability> creatureSavingThrowProficiencies);
		CreatureBuilder& setSavingThrowExpertise(std::unordered_set<Ability> creatureSavingThrowExpertise);
		CreatureBuilder& setSkillProficiencies(std::unordered_set<Skill> creatureSkillProficiencies);
		CreatureBuilder& setSkillExpertise(std::unordered_set<Skill> creatureSkillExpertise);
		CreatureBuilder& setAbilityScores(std::unordered_map<Ability, int> creatureAbilityScores);
		CreatureBuilder& setAbilityScoreModifiers(std::unordered_map<Ability, int> creatureAbilityScoreModifiers);
		CreatureBuilder& setSavingThrowModifiers(std::unordered_map<Ability, int> creatureSavingThrowModifiers);
		CreatureBuilder& setSkillModifiers(std::unordered_map<Skill, int> creatureSkillModifiers);
		CreatureBuilder& setActions(std::vector<CreatureAction> creatureActions);
		CreatureBuilder& setDamageVulnerabilities(std::unordered_set<DamageType> creatureDamageVulnerabilities);
		CreatureBuilder& setDamageResistances(std::unordered_set<DamageType> creatureDamageResistances);
		CreatureBuilder& setDamageImmunities(std::unordered_set<DamageType> creatureDamageImmunities);
		CreatureBuilder& setConditionImmunities(std::unordered_set<Condition> creatureConditionImmunities);
		CreatureBuilder& setTraits(std::vector<Trait> creatureTraits);
		CreatureBuilder& setSenses(std::unordered_map<Sense, int> creatureSenses);
		CreatureBuilder& setLanguages(std::unordered_set<Language> creatureLanguages);
		CreatureBuilder& setEquipment(std::vector<Gear> creatureEquipment);

		CreatureStatBlock& getCreatureStatBlock(); 
		Creature createCreature();
	protected:
		CreatureStatBlock block;
	};
}
