/*
#include "JSONUtils.h"
#include "./Entities/Creature/CreatureConstants.h"

#include "json.hpp"

namespace CombatTrackerApp {
	void to_json(nlohmann::json& jsonData, const CreatureAction& action) {
		jsonData = nlohmann::json{
			{"Type", ACTION_NAMES.at(action.type)},
			{"Name", action.name},
			{"Description", action.description}
		};
	}
	void from_json(const nlohmann::json& jsonData, CreatureAction& action) {

	}
	void to_json(nlohmann::json& jsonData, const Trait& trait) {
		jsonData = nlohmann::json{
			{"Name", trait.name},
			{"Description", trait.description}
		};
	}
	void from_json(const nlohmann::json& jsonData, Trait& trait) {

	}
	void to_json(nlohmann::json& jsonData, const Gear& gear) {
		jsonData = nlohmann::json{
			{"Name", gear.name},
			{"Description", gear.description}
		};
	}
	void from_json(const nlohmann::json& jsonData, Gear& gear) {

	}
	bool validateCreatureStatBlock(const CreatureStatBlock& block) {
		if (block.name.empty()) {
			return false;
		}
		if (block.size == Size::NO_SIZE) {
			return false;
		}
		
		return true;
	}
	bool writeCreatureBlockToJSON(const CreatureStatBlock& block, nlohmann::json& jsonData) {
		jsonData["Name"] = block.name;
		jsonData["Size"] = block.size;
		jsonData["Alignment"] = block.alignment;
		jsonData["AC"] = block.AC;
		jsonData["HP"] = block.HP;
		jsonData["Passive Perception"] = block.passivePerception;
		jsonData["Iniative Bonus"] = block.initiativeBonus;
		jsonData["Proficiency Bonus"] = block.proficiencyBonus;
		jsonData["Walk Speed"] = block.walkSpeed;
		jsonData["Climbing Speed"] = block.climbingSpeed;
		jsonData["Swimming Speed"] = block.swimmingSpeed;
		jsonData["Burrow Speed"] = block.burrowSpeed;
		jsonData["Fly Speed"] = block.flySpeed;
		for (const auto& item : block.abilityScores) {
			jsonData[ABILITY_NAMES.at(item.first) + " Ability Score"] = item.second;
		}
		for (const auto& item : block.abilityScoreModifiers) {
			jsonData[ABILITY_NAMES.at(item.first) + " Ability Modifier"] = item.second;
		}
		for (const auto& item : block.savingThrowProficiencies) {
			jsonData[ABILITY_NAMES.at(item.first) + " Saving Throw Proficiency"] = item.second;
		}
		for (const auto& item : block.savingThrowExpertise) {
			jsonData[ABILITY_NAMES.at(item.first) + " Saving Throw Expertise"] = item.second;
		}
		for (const auto& item : block.savingThrowModifiers) {
			jsonData[ABILITY_NAMES.at(item.first) + " Saving Throw Modifier"] = item.second;
		}
		for (const auto& item : block.skillProficiencies) {
			jsonData[SKILL_NAMES.at(item.first) + " Proficiency"] = item.second;
		}
		for (const auto& item : block.skillExpertise) {
			jsonData[SKILL_NAMES.at(item.first) + " Expertise"] = item.second;
		}
		for (const auto& item : block.skillModifiers) {
			jsonData[SKILL_NAMES.at(item.first) + " Modifier"] = item.second;
		}
		for (const auto& item : block.damageVulnerabilities) {
			jsonData[DAMAGE_TYPE_NAMES.at(item.first) + " Vulnerability"] = item.second;
		}
		for (const auto& item : block.damageResistances) {
			jsonData[DAMAGE_TYPE_NAMES.at(item.first) + " Resistance"] = item.second;
		}
		for (const auto& item : block.damageImmunities) {
			jsonData[DAMAGE_TYPE_NAMES.at(item.first) + " Immunity"] = item.second;
		}
		for (const auto& item : block.conditionImmunities) {
			jsonData[CONDITTON_NAMES.at(item.first) + " Immunity"] = item.second;
		}
		for (const auto& item : block.languages) {
			jsonData[LANGUAGE_NAMES.at(item.first)] = item.second;
		}
		jsonData["Actions"] = block.actions;
		jsonData["Traits"] = block.traits;
		jsonData["Gear"] = block.equipment;
		return true;
	}
	bool writePlayerCharacterBlockToJSON(const PlayerStatBlock& block, nlohmann::json& jsonData) {

	}
	bool saveMonster(const CreatureStatBlock& creatureBlock, const double& CR, const int& XP) {
		nlohmann::json jsonData;
		jsonData["CR"] = CR;
		jsonData["XP"] = XP;
		if (validateCreatureStatBlock(creatureBlock)) {
			return writeCreatureBlockToJSON(creatureBlock, jsonData);
		}
		return false;
	}
	bool savePlayerCharacter(const CreatureStatBlock& blockCreaturePortion, const PlayerStatBlock& blockPlayerPortion) {
		return true;
	}
}
*/
