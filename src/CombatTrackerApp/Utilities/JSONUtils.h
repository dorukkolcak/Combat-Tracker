#pragma once

#include <string_view>
#include <type_traits>
#include <optional>
#include <functional>

#include "nlohmann/json.hpp"

#include "../Entities/Creature/Creature.h"
#include "../Entities/Creature/CreatureTypes.h"
#include "../Entities/PlayerCharacter/PlayerCharacter.h"

namespace CombatTrackerApp {

	template <typename T> requires std::derived_from<T, Creature>
	inline bool saveEntity(
		const CreatureStatBlock& creatureBlock, 
		std::optional<double> CR, 
		std::optional<OverridableInt> XP, 
		std::optional<std::reference_wrapper<PlayerStatBlock>> playerBlock
	) {
		return true;
	}
	template <typename T> requires std::derived_from<T, Creature>
	inline bool deleteEntity(std::string_view entityName) {
		return true;
	}

	// The return value of this function should use move semantics.
	template <typename T> requires std::derived_from<T, Creature>
	inline CreatureStatBlock loadEntity(
		std::string_view entityName, 
		std::optional<std::reference_wrapper<double>> outCR,
		std::optional<std::reference_wrapper<OverridableInt>> outXP,
		std::optional<std::reference_wrapper<PlayerStatBlock>> outPlayerBlock
	) {
		return {};
	}
	template <typename T> requires std::derived_from<T, Creature>
	inline bool editEntity(
		std::string_view entityName,
		const CreatureStatBlock& creatureBlock,
		std::optional<double> CR,
		std::optional<OverridableInt> XP,
		std::optional<std::reference_wrapper<PlayerStatBlock>> playerBlock
	) {
		return true;
	}
}