#pragma once

#include <string>

#include "EntityCreationScene.h"
#include "../Entities/Monster/Monster.h"

namespace CombatTrackerApp {

	// The monster half of entity creation. 
	// CR determines XP and PB, which is the only place monsters differ from player characters in the top split.
	class MonsterCreationScene : public EntityCreationScene {
	public:

		explicit MonsterCreationScene(CombatTrackerLib::App* app);

	protected:

		const std::string& getEntityLabel() const override;
		bool isMonsterScene() const override;
		void renderEntitySpecificInfo() override;
		void renderEntitySpecificSection() override;
		bool completeEntityCreation() override;
		void resetEntity() override;
		void resetParametersAndOverrides() override;

	private:

		inline static const std::string ENTITY_LABEL{ "Monster" };

		MonsterBuilder m_monsterBuilder{};

		double m_currentMonsterCR{};
		int m_currentMonsterXP{};

		CombatTrackerLib::ListBoxParameters m_monsterCRParameters{};

		bool m_isXPOverridden{};
		bool m_isPBOverridden{};

	};
}
