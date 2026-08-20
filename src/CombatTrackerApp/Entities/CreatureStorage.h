#pragma once

#include "CombatTrackerApp/Entities/Creature.h"
#include "CombatTrackerApp/Entities/Monster.h"
#include "CombatTrackerApp/Entities/PlayerCharacter.h"
#include "CombatTrackerLib/App.h"
#include "CombatTrackerLib/Utilities/FileUtils.h"

#include <nlohmann/json.hpp>
#include <concepts>
#include <filesystem>
#include <format>
#include <functional>
#include <optional>
#include <string_view>

namespace CombatTrackerApp {

	// Use Nlohmann's macros to serialize structs/types/classes to json easily.
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(
		CreatureAction, name, description, type
	)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(
		NamedDescription, name, description
	)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(
		CreatureStatBlock,
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
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(
		PlayerStatBlock,
		levels,
		subclasses,
		weaponProficiencies,
		armorProficiencies,
		toolProficiencyLevels,
		weaponMasteries,
		species,
		origin
	)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(MonsterStatBlock, CR)

	class CreatureStorage {
	public:
		template <typename T>
			requires std::derived_from<T, Creature>
		static bool saveCreature(
			const std::filesystem::path& userDataDirectory,
			const CreatureStatBlock& creatureBlock,
			std::optional<std::reference_wrapper<const MonsterStatBlock>>
				monsterBlock,
			std::optional<std::reference_wrapper<const PlayerStatBlock>>
				playerBlock
		) {
			std::error_code errorCode{};
			if (std::filesystem::exists(
					findFilePath<T>(userDataDirectory, creatureBlock.name),
					errorCode
				)) {
				return false;
			}
			return writeCreature<T>(
				userDataDirectory, creatureBlock, monsterBlock, playerBlock
			);
		}
		template <typename T>
			requires std::derived_from<T, Creature>
		static bool deleteCreature(
			const std::filesystem::path& userDataDirectory,
			std::string_view creatureName
		) {
			std::error_code errorCode{};
			return std::filesystem::remove(
				findFilePath<T>(userDataDirectory, creatureName), errorCode
			);
		}
		template <typename T>
			requires std::derived_from<T, Creature>
		static CreatureStatBlock loadCreature(
			const std::filesystem::path& userDataDirectory,
			std::string_view entityName,
			std::optional<std::reference_wrapper<MonsterStatBlock>>
				outMonsterBlock,
			std::optional<std::reference_wrapper<PlayerStatBlock>>
				outPlayerBlock
		) {
			const auto contents{ CombatTrackerLib::readJSONFile(
				findFilePath<T>(userDataDirectory, entityName)
			) };
			if (contents == std::nullopt) {
				return CreatureStatBlock{};
			}
			auto creatureBlock{
				contents->at("creatureBlock").get<CreatureStatBlock>()
			};
			if constexpr (std::same_as<T, Monster>) {
				outMonsterBlock->get() =
					contents->at("monsterBlock").get<MonsterStatBlock>();
				Monster::resolveDerived(creatureBlock, outMonsterBlock->get());
			}
			else {
				outPlayerBlock->get() =
					contents->at("playerBlock").get<PlayerStatBlock>();
				PlayerCharacter::resolveDerived(
					creatureBlock, outPlayerBlock->get()
				);
			}
			return creatureBlock; // Should internally use NRVO so no need for explicit std::move().
		}
		template <typename T>
			requires std::derived_from<T, Creature>
		static bool editCreature(
			const std::filesystem::path& userDataDirectory,
			std::string_view entityName,
			const CreatureStatBlock& creatureBlock,
			std::optional<std::reference_wrapper<const MonsterStatBlock>>
				monsterBlock,
			std::optional<std::reference_wrapper<const PlayerStatBlock>>
				playerBlock
		) {
			const bool isRenamed{ entityName != creatureBlock.name };

			// If the creature is renamed to a creature that already exists, we should reject the edit.
			std::error_code errorCode{};
			if (isRenamed &&
				std::filesystem::exists(
					findFilePath<T>(userDataDirectory, creatureBlock.name),
					errorCode
				)) {
				return false;
			}

			// The new file has to be on disk before the old one goes away.
			// So we first attempt to save the new entity and delete the old file if it is needed.
			if (!writeCreature<T>(
					userDataDirectory, creatureBlock, monsterBlock, playerBlock
				)) {
				return false;
			}
			if (isRenamed) {
				return deleteCreature<T>(userDataDirectory, entityName);
			}
			return true;
		}

	private:
		template <typename T>
			requires std::derived_from<T, Creature>
		static std::filesystem::path findFilePath(
			const std::filesystem::path& userDataDirectory,
			std::string_view creatureName
		) {
			if constexpr (std::same_as<T, Monster>) {
				return userDataDirectory / "Monsters" /
					   std::format("{}.json", creatureName);
			}
			else {
				return userDataDirectory / "PlayerCharacters" /
					   std::format("{}.json", creatureName);
			}
		}
		template <typename T>
			requires std::derived_from<T, Creature>
		static bool writeCreature(
			const std::filesystem::path& userDataDirectory,
			const CreatureStatBlock& creatureBlock,
			std::optional<std::reference_wrapper<const MonsterStatBlock>>
				monsterBlock,
			std::optional<std::reference_wrapper<const PlayerStatBlock>>
				playerBlock
		) {
			nlohmann::json contents{};
			if constexpr (std::same_as<T, Monster>) {
				if (!monsterBlock.has_value() ||
					!Monster::isValid(creatureBlock, monsterBlock->get())) {
					return false;
				}
				contents["monsterBlock"] = monsterBlock->get();
			}
			else {
				if (!playerBlock.has_value() ||
					!PlayerCharacter::isValid(
						creatureBlock, playerBlock->get()
					)) {
					return false;
				}
				contents["playerBlock"] = playerBlock->get();
			}
			contents["creatureBlock"] = creatureBlock;
			return CombatTrackerLib::writeJSONFile(
				findFilePath<T>(userDataDirectory, creatureBlock.name), contents
			);
		}
	};
}
