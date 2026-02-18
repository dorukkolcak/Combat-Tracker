#include "Creature.h"

namespace CombatTrackerApp {
	// All of the parameters should be verified before creating the object, using the rulings defined in the header files.
	Creature::Creature(CreatureStatBlock creatureBlock) : statBlockCreaturePortion(std::move(creatureBlock)) {
		//TODO: Find a method for ID generation.
	}
	// Ideally, we do stack allocation for all of these members. So the destructor should be empty.
	Creature::~Creature() {}
	std::int16_t Creature::findAbilityScoreModifier(int abilityScore) {
		return (abilityScore - 10) / 2;
	}
	CreatureBuilder& CreatureBuilder::setName(std::string creatureName) { block.name = std::move(creatureName); return *this; }
	CreatureBuilder& CreatureBuilder::setSize(const Size creatureSize) { block.size = creatureSize; return *this; }
	CreatureBuilder& CreatureBuilder::setType(const CreatureType creatureType) { block.type = creatureType; return *this; }
	CreatureBuilder& CreatureBuilder::setAlignment(const Alignment creatureAlignment) { block.alignment = creatureAlignment; return *this; }
	CreatureBuilder& CreatureBuilder::setAC(const std::uint16_t creatureAC) { block.AC = creatureAC; return *this; }
	CreatureBuilder& CreatureBuilder::setInitiativeBonus(const std::int16_t creatureInitiativeBonus) { block.initiativeBonus = creatureInitiativeBonus; return *this; }
	CreatureBuilder& CreatureBuilder::setPB(const std::uint16_t creatureProficiencyBonus) { block.proficiencyBonus = creatureProficiencyBonus; return *this; }
	CreatureBuilder& CreatureBuilder::setHP(const std::uint16_t creatureHP) { block.HP = creatureHP; return *this; }
	CreatureBuilder& CreatureBuilder::setPassivePerception(const std::uint16_t creaturePassivePerception) { return *this; }
	CreatureBuilder& CreatureBuilder::setWalkSpeed(const std::uint16_t creatureWalkSpeed) { block.walkSpeed = creatureWalkSpeed; return *this; }
	CreatureBuilder& CreatureBuilder::setClimbingSpeed(const std::uint16_t creatureClimbingSpeed) { block.climbingSpeed = creatureClimbingSpeed; return *this; }
	CreatureBuilder& CreatureBuilder::setSwimmingSpeed(const std::uint16_t creatureSwimmingSpeed) { block.swimmingSpeed = creatureSwimmingSpeed; return *this; }
	CreatureBuilder& CreatureBuilder::setBurrowSpeed(const std::uint16_t creatureBurrowSpeed) { block.burrowSpeed = creatureBurrowSpeed; return *this; }
	CreatureBuilder& CreatureBuilder::setFlySpeed(const std::uint16_t creatureFlySpeed) { block.flySpeed = creatureFlySpeed;  return *this; }
	CreatureBuilder& CreatureBuilder::setSavingThrowProficiencies(std::unordered_set<Ability> creatureSavingThrowProficiencies) { block.savingThrowProficiencies = std::move(creatureSavingThrowProficiencies); return *this; }
	CreatureBuilder& CreatureBuilder::setSavingThrowExpertise(std::unordered_set<Ability> creatureSavingThrowExpertise) { block.savingThrowExpertise = std::move(creatureSavingThrowExpertise);  return *this; }
	CreatureBuilder& CreatureBuilder::setSkillProficiencies(std::unordered_set<Skill> creatureSkillProficiencies) { block.skillProficiencies = std::move(creatureSkillProficiencies); return *this; }
	CreatureBuilder& CreatureBuilder::setSkillExpertise(std::unordered_set<Skill> creatureSkillExpertise) { block.skillExpertise = std::move(creatureSkillExpertise); return *this; }
	CreatureBuilder& CreatureBuilder::setAbilityScores(std::unordered_map<Ability, std::uint16_t> creatureAbilityScores) { block.abilityScores = std::move(creatureAbilityScores);  return *this; }
	CreatureBuilder& CreatureBuilder::setAbilityScoreModifiers(std::unordered_map<Ability, std::int16_t> creatureAbilityScoreModifiers) { block.abilityScoreModifiers = std::move(creatureAbilityScoreModifiers); return *this; }
	CreatureBuilder& CreatureBuilder::setSavingThrowModifiers(std::unordered_map<Ability, std::int16_t> creatureSavingThrowModifiers) { block.savingThrowModifiers = std::move(creatureSavingThrowModifiers); return *this; }
	CreatureBuilder& CreatureBuilder::setSkillModifiers(std::unordered_map<Skill, std::int16_t> creatureSkillModifiers) { block.skillModifiers = std::move(creatureSkillModifiers); return *this; }
	CreatureBuilder& CreatureBuilder::setActions(std::vector<CreatureAction> creatureActions) { block.actions = std::move(creatureActions); return *this; }
	CreatureBuilder& CreatureBuilder::setDamageVulnerabilities(std::unordered_set<DamageType> creatureDamageVulnerabilities) { block.damageVulnerabilities = std::move(creatureDamageVulnerabilities);  return *this; }
	CreatureBuilder& CreatureBuilder::setDamageResistances(std::unordered_set<DamageType> creatureDamageResistances) { block.damageResistances = std::move(creatureDamageResistances);  return *this; }
	CreatureBuilder& CreatureBuilder::setDamageImmunities(std::unordered_set<DamageType> creatureDamageImmunities) { block.damageImmunities = std::move(creatureDamageImmunities); return *this; }
	CreatureBuilder& CreatureBuilder::setConditionImmunities(std::unordered_set<Condition> creatureConditionImmunities) { block.conditionImmunities = std::move(creatureConditionImmunities); return *this; }
	CreatureBuilder& CreatureBuilder::setTraits(std::vector<Trait> creatureTraits) { block.traits = std::move(creatureTraits); return *this; }
	CreatureBuilder& CreatureBuilder::setSenses(std::unordered_map<Sense, std::uint16_t> creatureSenses) { block.senses = std::move(creatureSenses); return *this; }
	CreatureBuilder& CreatureBuilder::setLanguages(std::unordered_set<Language> creatureLanguages) { block.languages = std::move(creatureLanguages); return *this; }
	CreatureBuilder& CreatureBuilder::setEquipment(std::vector<Gear> creatureEquipment) { block.equipment = std::move(creatureEquipment); return *this; }
	Creature CreatureBuilder::createCreature() { return std::move(Creature(block)); }
}