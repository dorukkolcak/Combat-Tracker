#pragma once

#include "CombatTrackerApp/Entities/Creature.h"
#include "CombatTrackerApp/Entities/Monster.h"
#include "CombatTrackerApp/Entities/PlayerCharacter.h"
#include "CombatTrackerLib/Utilities/FileUtils.h"

#include <concepts>
#include <filesystem>
#include <string_view>
#include <type_traits>
#include <optional>
#include <functional>
#include <format>
#include <nlohmann/json.hpp>


namespace CombatTrackerLib {

	template <int LOWER_BOUND, int UPPER_BOUND>
	inline void to_json(nlohmann::json& contents, const BoundedInt<LOWER_BOUND, UPPER_BOUND>& boundedInt) {
		contents = boundedInt.get();
	}
	template <int LOWER_BOUND, int UPPER_BOUND>
	inline void from_json(const nlohmann::json& contents, BoundedInt<LOWER_BOUND, UPPER_BOUND>& boundedInt) {
		boundedInt = contents.get<int>();
	}
	inline void to_json(nlohmann::json& contents, const OverridableInt& overridableInt) {
		if (overridableInt.isOverridden()) {
			contents = overridableInt.get();
			return;
		}
		contents = nullptr;
	}
	inline void from_json(const nlohmann::json& contents, OverridableInt& overridableInt) {
		if (contents.is_null()) {
			overridableInt.clearOverride();
			return;
		}
		overridableInt.setOverride(contents.get<int>());
	}
}

// The utilities here are application specific and use the library-provided JSON reading/writing functionality.
namespace CombatTrackerApp {

	// Use Nlohmann's macros to serialize structs/types/classes to json easily.
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(CreatureAction, name, description, type)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(NamedDescription, name, description)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(CreatureStatBlock,
		savingThrowProficiencyLevels,
		skillProficiencyLevels,
		abilityScores,
		savingThrowModifiers,
		skillModifiers,
		damageReactions,
		conditionImmunities,
		senses,
		speeds,
		languages,
		actions,
		traits,
		equipment,
		name,
		size,
		type,
		alignment,
		initiativeBonus,
		passivePerception,
		AC,
		HP
	)

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(PlayerStatBlock,
		levels,
		subclasses,
		weaponProficiencies,
		armorProficiencies,
		toolProficiencyLevels,
		weaponMasteries,
		species,
		origin
	)

	// The challenge rating is the only authored half of a monster, as the XP hangs off it.
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(MonsterStatBlock, CR)
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
		std::optional<std::reference_wrapper<const MonsterStatBlock>> monsterBlock,
		std::optional<std::reference_wrapper<const PlayerStatBlock>> playerBlock
	) {
		if (!isValidEntityName(creatureBlock.name)) {
			return false;
		}

		/*
		A stat block being edited is a draft and is free to be half finished, so this is the one place
		the rules are actually enforced. Nothing invalid reaches disk, and nothing has to police every
		individual write a creation scene makes.
		*/
		nlohmann::json contents{};
		if constexpr (std::same_as<T, Monster>) {
			if (!monsterBlock.has_value() || !Monster::isValid(creatureBlock, monsterBlock->get())) {
				return false;
			}

			// The XP is not written, as the challenge rating recovers it through the table on load.
			contents["monsterBlock"] = monsterBlock->get();
		}
		else {
			if (!playerBlock.has_value() || !PlayerCharacter::isValid(creatureBlock, playerBlock->get())) {
				return false;
			}
			contents["playerBlock"] = playerBlock->get();
		}
		contents["creatureBlock"] = creatureBlock;
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
		std::optional<std::reference_wrapper<MonsterStatBlock>> outMonsterBlock,
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

		// Nothing derived is on disk, so the block is only whole once the rules have run over it.
		// Each entity type resolves through its own overload, as the proficiency bonus comes from
		// the challenge rating for a monster and from the total class level for a player character.
		if constexpr (std::same_as<T, Monster>) {

			// Access specified JSON value with a default through the use of ->value().
			// The out parameters hold reference wrappers, so ->get() is needed to reach the referenced object.
			// Assigning to the optional's own value() would try to rebind the wrapper instead, which does not compile.
			outMonsterBlock->get() = contents->value("monsterBlock", MonsterStatBlock{});
			Monster::resolveDerived(creatureBlock, outMonsterBlock->get());
		}
		else {
			outPlayerBlock->get() = contents->value("playerBlock", PlayerStatBlock{});
			PlayerCharacter::resolveDerived(creatureBlock, outPlayerBlock->get());
		}
		return creatureBlock; // Should internally use NRVO so no need for explicit std::move().
	}
	template <typename T> requires std::derived_from<T, Creature>
	inline bool editEntity(
		std::string_view entityName,
		const CreatureStatBlock& creatureBlock,
		std::optional<std::reference_wrapper<const MonsterStatBlock>> monsterBlock,
		std::optional<std::reference_wrapper<const PlayerStatBlock>> playerBlock
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
		if (!saveEntity<T>(creatureBlock, monsterBlock, playerBlock)) {
			return false;
		}
		if (isRenamed) {
			return deleteEntity<T>(entityName);
		}
		return true;
	}
}
