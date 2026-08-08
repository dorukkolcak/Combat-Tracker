#pragma once

#include "CombatTrackerApp/Entities/Creature/Creature.h"
#include "CombatTrackerApp/Entities/Creature/CreatureTypes.h"
#include "CombatTrackerApp/Entities/Monster/Monster.h"
#include "CombatTrackerApp/Entities/PlayerCharacter/PlayerCharacter.h"
#include "CombatTrackerLib/Utilities/FileUtils.h"

#include <concepts>
#include <filesystem>
#include <string_view>
#include <type_traits>
#include <optional>
#include <functional>
#include <format>
#include <nlohmann/json.hpp>

// The utilities here are application specific and use the library-provided JSON reading/writing functionality.
namespace CombatTrackerApp {
	
	// Use Nlohmann's macros to serialize structs/types/classes to json easily.
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(OverridableInt, value, isOverridden)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(CreatureAction, name, description, type)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Trait, name, description)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Gear, name, description)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(CreatureStatBlock,
		savingThrowProficiencies, 
		savingThrowExpertise, 
		skillProficiencies, 
		skillExpertise,
		abilityScores, 
		abilityScoreModifiers,
		savingThrowModifiers, 
		skillModifiers,
		damageVulnerabilities, 
		damageResistances, 
		damageImmunities, 
		conditionImmunities,
		senses, 
		languages,
		actions, 
		traits, 
		equipment,
		name, 
		size, 
		type, 
		alignment,
		initiativeBonus, 
		proficiencyBonus, 
		passivePerception,
		AC, 
		HP, 
		walkSpeed, 
		climbingSpeed, 
		swimmingSpeed, 
		burrowSpeed, 
		flySpeed
	)

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(PlayerStatBlock,
		levels, 
		subclasses, 
		weaponProficiencies, 
		armorProficiencies,
		toolProficiencies, 
		weaponMasteries, 
		species, 
		origin
	)
	inline constexpr std::string_view MONSTER_DIRECTORY{ "data/Monsters" };
	inline constexpr std::string_view PLAYER_DIRECTORY{ "data/PlayerCharacters" };

	inline bool isValidEntityName(std::string_view entityName) {
		if (entityName.empty() || entityName == "." || entityName == "..") {
			return false;
		}

		// find_first_of() returns npos if it fails to locate the a character from the provided regex.
		return entityName.find_first_of("\\/:*?\"<>|") == std::string_view::npos; 
	}

	// Monsters and player characters are saved separately, so the two can hold the same name without colliding.
	template <typename T> requires std::derived_from<T, Creature>
	inline std::filesystem::path findCreatureFilePath(std::string_view entityName) {
		if constexpr (std::same_as<T, Monster>) {
			return std::filesystem::path{ MONSTER_DIRECTORY } / std::format("{}.json", entityName);
		}
		else {
			return std::filesystem::path{ PLAYER_DIRECTORY } / std::format("{}.json", entityName);
		}
	}
	template <typename T> requires std::derived_from<T, Creature>
	inline bool saveEntity(
		const CreatureStatBlock& creatureBlock,
		std::optional<double> CR,
		std::optional<OverridableInt> XP,
		std::optional<std::reference_wrapper<PlayerStatBlock>> playerBlock
	) {
		if (!isValidEntityName(creatureBlock.name)) {
			return false;
		}

		nlohmann::json contents{};
		contents["creatureBlock"] = creatureBlock;
		if constexpr (std::same_as<T, Monster>) {
			contents["CR"] = CR.value_or(0.0);
			contents["XP"] = XP.value_or(OverridableInt{});
		}
		else {
			if (!playerBlock.has_value()) {
				return false;
			}
			contents["playerBlock"] = playerBlock.value();
		}
		return CombatTrackerLib::writeJSONFile(findCreatureFilePath<T>(creatureBlock.name), contents);
	}
	template <typename T> requires std::derived_from<T, Creature>
	inline bool deleteEntity(std::string_view entityName) {
		if (!isValidEntityName(entityName)) {
			return false;
		}

		// The overload taking an error code reports a failure through it instead of throwing. 
		// We don't want the application to throw on a failed delete and result in a complete halt.
		// See: https://en.cppreference.com/cpp/filesystem/remove
		std::error_code errorCode{};
		return std::filesystem::remove(findCreatureFilePath<T>(entityName), errorCode);
	}
	template <typename T> requires std::derived_from<T, Creature>
	inline CreatureStatBlock loadEntity(
		std::string_view entityName,
		std::optional<std::reference_wrapper<double>> outCR,
		std::optional<std::reference_wrapper<OverridableInt>> outXP,
		std::optional<std::reference_wrapper<PlayerStatBlock>> outPlayerBlock
	) {
		if (!isValidEntityName(entityName)) {
			return {};
		}

		// Retrieve the creature's contents.
		const std::optional<nlohmann::json> contents{ CombatTrackerLib::readJSONFile(findCreatureFilePath<T>(entityName)) };
		if (!contents.has_value()) {
			return CreatureStatBlock{};
		}
		auto creatureBlock{ contents->at("creatureBlock").get<CreatureStatBlock>() };
		if constexpr (std::same_as<T, Monster>) {

			// Access specified JSON value with a default through the use of ->value().
			// The out parameters hold reference wrappers, so ->get() is needed to reach the referenced object.
			// Assigning to the optional's own value() would try to rebind the wrapper instead, which does not compile.
			outCR->get() = contents->value("CR", 0.0);
			outXP->get() = contents->value("XP", OverridableInt{});
		}
		else {
			outPlayerBlock->get() = contents->value("playerBlock", PlayerStatBlock{});
		}
		return creatureBlock; // Should internally use NRVO so no need for explicit std::move().
	}
	template <typename T> requires std::derived_from<T, Creature>
	inline bool editEntity(
		std::string_view entityName,
		const CreatureStatBlock& creatureBlock,
		std::optional<double> CR,
		std::optional<OverridableInt> XP,
		std::optional<std::reference_wrapper<PlayerStatBlock>> playerBlock
	) {

		// entityName refers to the old name of the creature and the name inside the creatureBlock refers to the new name.
		if (!isValidEntityName(entityName) || !isValidEntityName(creatureBlock.name)) {
			return false;
		}
		const bool isRenamed{ entityName != creatureBlock.name };

		// If the creature is renamed to a creature that already exists, we should reject the edit.
		std::error_code errorCode{};
		if (isRenamed && std::filesystem::exists(findCreatureFilePath<T>(creatureBlock.name), errorCode)) {
			return false;
		}

		// The new file has to be on disk before the old one goes away, otherwise a failed write loses the entity altogether.
		// So we first attempt to save the new entity and delete the old file if it is needed.
		if (!saveEntity<T>(creatureBlock, CR, XP, playerBlock)) {
			return false;
		}
		if (isRenamed) {
			return deleteEntity<T>(entityName);
		}
		return true;
	}
}
