#pragma once

#include "CombatTrackerApp/Scenes/EntityCreationScene.h"
#include "CombatTrackerApp/Entities/PlayerCharacter/PlayerCharacter.h"

#include <array>
#include <cstddef>
#include <string_view>

namespace CombatTrackerApp {

	// The player character half of entity creation.
	class PlayerCharacterCreationScene : public EntityCreationScene {
	public:

		explicit PlayerCharacterCreationScene(CombatTrackerLib::App* app);

	protected:

		std::string_view getEntityLabel() const override;
		void renderEntitySpecificInfo() override;
		void renderEntitySpecificSection() override;
		bool completeEntityCreation() override;
		void resetParametersAndOverrides() override;

	private:

		inline static constexpr std::string_view ENTITY_LABEL{ "Player Character" };

		// Held by value for the same reason the creature block is. See EntityCreationScene.
		PlayerStatBlock m_currentPlayerStatBlock{};

		CombatTrackerLib::ListBoxParameters m_speciesParameters{};
		CombatTrackerLib::ListBoxParameters m_originParameters{};

		std::array<bool, static_cast<std::size_t>(PlayerSubclass::COUNT)> m_chosenSubclasses{};

		void renderPlayerClassAndSubclasses();
		void renderPlayerCharacterSpecificInfo();
	};
}
