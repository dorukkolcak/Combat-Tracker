#pragma once

#include <unordered_map>
#include <string>
#include <cstdint>
#include <vector>
#include <unordered_set>

#include "CreatureEnums.h"
#include "CreatureTypes.h"

namespace CombatTrackerApp {
	struct CreatureStatBlock {
		// COMPUTED, NOT OVERRIDABLE
		std::uint32_t ID;
		// MANUALLY SET
		std::string name;
		// MANUALLY SET
		Size size;
		// MANUALLY SET FOR Monster
		// OVERRIDABLE, COMPUTED IF NOT OVERRIDDEN for PlayerCharacter (HUMANOID for all Species)
		CreatureType type;
		// MANUALLY SET
		Alignment alignment;
		// MANUALLY SET
		std::uint16_t AC;
		// OVERRIDABLE, COMPUTED IF NOT OVERRIDDEN (Dexterity modifier)
		std::int16_t initiativeBonus;
		// OVERRIDABLE, COMPUTED IF NOT OVERRIDDEN (See MAPPED_CR_TO_PB and MAPPED_PLAYER_LEVEL_TO_PB)
		std::uint16_t proficiencyBonus;
		// MANUALLY SET
		std::uint16_t HP;
		// OVERRIDABLE, COMPUTED IF NOT OVERRIDDEN (Wisdom modifier + Perception Score + 10)
		std::uint16_t passivePerception;
		// MANUALLY SET
		std::uint16_t walkSpeed;
		// MANUALLY SET
		std::uint16_t climbingSpeed;
		// MANUALLY SET
		std::uint16_t swimmingSpeed;
		// MANUALLY SET
		std::uint16_t burrowSpeed;
		// MANUALLY SET
		std::uint16_t flySpeed;
		// MANUALLY SET
		std::unordered_set<Ability> savingThrowProficiencies;
		// MANUALLY SET
		std::unordered_set<Ability> savingThrowExpertise;
		// MANUALLY SET
		std::unordered_set<Skill> skillProficiencies;
		// MANUALLY SET
		std::unordered_set<Skill> skillExpertise;
		// MANUALLY SET
		std::unordered_map<Ability, std::uint16_t> abilityScores;
		// OVERRIDABLE, COMPUTED IF NOT OVERRIDDEN (See MAPPED_ABILITY_SCORES)
		std::unordered_map<Ability, std::int16_t> abilityScoreModifiers;
		// OVERRIDABLE, COMPUTED IF NOT OVERRIDDEN (Relevant Ability Modifier + PB or Relevant Ability Modifier + 2 x PB, depending on proficiency or expertise.)
		std::unordered_map<Ability, std::int16_t> savingThrowModifiers;
		// OVERRIDABLE, COMPUTED IF NOT OVERRIDDEN (Relevant Ability Modifier + PB or Relevant Ability Modifier + 2 x PB, depending on proficiency or expertise.)
		std::unordered_map<Skill, std::int16_t> skillModifiers;
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
		std::unordered_map<Sense, std::uint16_t> senses;
		// MANUALLY SET
		std::unordered_set<Language> languages;
		// MANUALLY SET
		std::vector<Gear> equipment;
	};

	class Creature {
	protected:
		// COMPUTED, NOT OVERRIDABLE
		CreatureStatBlock statBlockCreaturePortion;
		std::uint32_t ID;
	public:
		Creature(CreatureStatBlock statBlockCreaturePortion);
		~Creature();
		static std::int16_t findAbilityScoreModifier(int abilityScore);
	};
	class CreatureBuilder {
	public:
		CreatureBuilder& setName(std::string creatureName);
		CreatureBuilder& setSize(const Size creatureSize);
		CreatureBuilder& setType(const CreatureType creatureType);
		CreatureBuilder& setAlignment(const Alignment creatureAlignment);
		CreatureBuilder& setAC(const std::uint16_t creatureAC);
		CreatureBuilder& setInitiativeBonus(const std::int16_t creatureInitiativeBonus);
		CreatureBuilder& setPB(const std::uint16_t creatureProficiencyBonus);
		CreatureBuilder& setHP(const std::uint16_t creatureHP);
		CreatureBuilder& setPassivePerception(const std::uint16_t creaturePassivePerception);
		CreatureBuilder& setWalkSpeed(const std::uint16_t creatureWalkSpeed);
		CreatureBuilder& setClimbingSpeed(const std::uint16_t creatureClimbingSpeed);
		CreatureBuilder& setSwimmingSpeed(const std::uint16_t creatureSwimmingSpeed);
		CreatureBuilder& setBurrowSpeed(const std::uint16_t creatureBurrowSpeed);
		CreatureBuilder& setFlySpeed(const std::uint16_t creatureFlySpeed);
		CreatureBuilder& setSavingThrowProficiencies(std::unordered_set<Ability> creatureSavingThrowProficiencies);
		CreatureBuilder& setSavingThrowExpertise(std::unordered_set<Ability> creatureSavingThrowExpertise);
		CreatureBuilder& setSkillProficiencies(std::unordered_set<Skill> creatureSkillProficiencies);
		CreatureBuilder& setSkillExpertise(std::unordered_set<Skill> creatureSkillExpertise);
		CreatureBuilder& setAbilityScores(std::unordered_map<Ability, std::uint16_t> creatureAbilityScores);
		CreatureBuilder& setAbilityScoreModifiers(std::unordered_map<Ability, std::int16_t> creatureAbilityScoreModifiers);
		CreatureBuilder& setSavingThrowModifiers(std::unordered_map<Ability, std::int16_t> creatureSavingThrowModifiers);
		CreatureBuilder& setSkillModifiers(std::unordered_map<Skill, std::int16_t> creatureSkillModifiers);
		CreatureBuilder& setActions(std::vector<CreatureAction> creatureActions);
		CreatureBuilder& setDamageVulnerabilities(std::unordered_set<DamageType> creatureDamageVulnerabilities);
		CreatureBuilder& setDamageResistances(std::unordered_set<DamageType> creatureDamageResistances);
		CreatureBuilder& setDamageImmunities(std::unordered_set<DamageType> creatureDamageImmunities);
		CreatureBuilder& setConditionImmunities(std::unordered_set<Condition> creatureConditionImmunities);
		CreatureBuilder& setTraits(std::vector<Trait> creatureTraits);
		CreatureBuilder& setSenses(std::unordered_map<Sense, std::uint16_t> creatureSenses);
		CreatureBuilder& setLanguages(std::unordered_set<Language> creatureLanguages);
		CreatureBuilder& setEquipment(std::vector<Gear> creatureEquipment);
		Creature createCreature();
	protected:
		CreatureStatBlock block;
	};
}
