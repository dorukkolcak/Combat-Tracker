#pragma once

#include "CombatTrackerApp/Entities/PlayerCharacter.h"
#include "CombatTrackerApp/Scenes/EntityCreationScene.h"

#include <string_view>

namespace CombatTrackerApp {

	// The player character half of entity creation.
	class PlayerCharacterCreationScene : public EntityCreationScene {
	public:
		explicit PlayerCharacterCreationScene(CombatTrackerLib::App* app);

	protected:
		CreationLabels getCreationLabels() const override;
		void renderEntitySpecificInfo() override;
		void renderEntitySpecificSection() override;
		bool completeEntityCreation() override;
		void resolveDerivedValues() override;
		void resetParametersAndOverrides() override;

	private:
		// Held by value for the same reason the creature block is. See EntityCreationScene.
		PlayerStatBlock m_currentPlayerStatBlock{};

		CombatTrackerLib::ListBoxParameters m_speciesParameters{};
		CombatTrackerLib::ListBoxParameters m_originParameters{};

		void renderPlayerClassAndSubclasses();
		void renderPlayerCharacterSpecificInfo();
	};
}
