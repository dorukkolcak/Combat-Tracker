#pragma once

#include "CombatTrackerApp/Entities/Creature/CreatureEnums.h"

#include <string>

// The types here shouldn't be nested types as they are used by the builders.
namespace CombatTrackerApp {

	// An integer that is normally computed from other stat block data, but can also be set by hand.
	// This is to essentially communicate that a value has been overridden to the saved data format.
	struct OverridableInt {
		int value{};
		bool isOverridden{};
	};

	struct CreatureAction {
		std::string name{};
		std::string description{};
		ActionType type{};
	};

	// Refers to any hyper-specific property of a creature that is active at all times or in certain conditions.
	// Note that trait effects aren't actually applied to the creature.
	// They have to be explicitly applied by the player/DM while running the encounter.
	struct Trait {
		std::string name{};
		std::string description{};
	};

	// Any relevant gear goes here, including attuned magic items.
	// Note that like traits the gear effects aren't actually applied to the creature.
	struct Gear {
		std::string name{};
		std::string description{};
	};
}