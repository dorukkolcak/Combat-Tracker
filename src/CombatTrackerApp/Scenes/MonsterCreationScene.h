#pragma once

#include "CombatTrackerApp/Scenes/EntityCreationScene.h"
#include "CombatTrackerApp/Entities/Monster/Monster.h"

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
		void renderEntitySpecificSection() override;
		bool completeEntityCreation() override;
		void resetParametersAndOverrides() override;

	private:

		double m_currentMonsterCR{};
		OverridableInt m_currentMonsterXP{};

		CombatTrackerLib::ListBoxParameters m_monsterCRParameters{};

		/*
		CR is the one selectable list in the application that is not backed by an enum.
		Its table pairs each CR value with its label, because the value itself is what keys the XP and PB tables.
		So the generic listbox helper, which turns a row position into an enumerator, does not fit and this does the job instead.
		The labels are numerals rather than words, so they are listed alongside the CR keys instead of coming from a locale file.
		*/
		void renderCRListBox();
	};
}
