#pragma once

#include "CombatTrackerApp/Entities/PlayerCharacter.h"

#include <gtest/gtest.h>

namespace CombatTrackerApp {
	class PlayerCharacterTest : public testing::Test {
	protected:
		PlayerCharacterTest();
		PlayerStatBlock& getPlayerBlock();
		PlayerCharacter m_mockPlayerCharacter;
	};
}
