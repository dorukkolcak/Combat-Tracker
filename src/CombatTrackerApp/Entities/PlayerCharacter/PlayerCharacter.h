#pragma once

#include <array>
#include <cstddef>
#include <unordered_map>

#include "../Creature/Creature.h"
#include "PlayerCharacterEnums.h"
#include "PlayerCharacterConstants.h"

namespace CombatTrackerApp {

	// The stat block types shouldn't be nested types as the creation scenes hold one directly.
	// This struct should also be an aggregate.
	struct PlayerStatBlock {

		std::array<int, static_cast<std::size_t>(PlayerClass::COUNT)> levels{};

		// Each PlayerClass can only have one PlayerSubclass.
		std::unordered_map<PlayerClass, PlayerSubclass> subclasses{};
		std::array<bool, static_cast<std::size_t>(WeaponType::COUNT)> weaponProficiencies{};
		std::array<bool, static_cast<std::size_t>(ArmorType::COUNT)> armorProficiencies{};
		std::array<bool, static_cast<std::size_t>(Tool::COUNT)> toolProficiencies{};
		std::array<bool, static_cast<std::size_t>(WeaponMastery::COUNT)> weaponMasteries{};
		Species species{};
		Origin origin{};
	};
	class PlayerCharacter : public Creature {
	public:

		// Same deal as most constructors, stat blocks are moved so pass them by value.
		PlayerCharacter(CreatureStatBlock creaturePortion, PlayerStatBlock playerPortion);
	private:
		PlayerStatBlock m_statBlockPlayerPortion{};
	};
}