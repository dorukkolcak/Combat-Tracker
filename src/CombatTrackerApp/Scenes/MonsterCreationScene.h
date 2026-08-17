#pragma once

#include "CombatTrackerApp/Scenes/EntityCreationScene.h"
#include "CombatTrackerApp/Entities/Monster.h"

#include <string_view>

namespace CombatTrackerApp {

	// The monster half of entity creation. 
	// CR determines XP and PB, which is the only place monsters differ from player characters in the top split.
	class MonsterCreationScene : public EntityCreationScene {
	public:

		explicit MonsterCreationScene(CombatTrackerLib::App* app);

	protected:

		CreationLabels getCreationLabels() const override;
		void renderEntitySpecificInfo() override;
		bool completeEntityCreation() override;
		void resolveDerivedValues() override;
		void resetParametersAndOverrides() override;

	private:
		MonsterStatBlock m_currentMonsterStatBlock{};
		CombatTrackerLib::ListBoxParameters m_monsterCRParameters{};
	};
}
