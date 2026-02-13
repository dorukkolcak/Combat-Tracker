#pragma once

#include "CreatureEnums.h"

namespace CombatTrackerApp {
	struct CreatureAction {
		ActionType type;
		std::string name;
		std::string description;
	};
	// Refers to any hyper-specific property of a creature that is active at all times or in certain conditions.
	struct Trait {
		std::string name;
		std::string description;
	};
	// Any relevant gear goes here, including attuned magic items.
	struct Gear {
		std::string name;
		std::string description;
	};
}