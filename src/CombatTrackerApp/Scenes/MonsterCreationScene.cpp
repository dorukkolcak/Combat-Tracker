#include "MonsterCreationScene.h"
#include "../Entities/Creature/CreatureConstants.h"
#include "../Entities/Monster/MonsterConstants.h"
#include "../CombatTrackerLib/Utilities/UIUtils.h"

#include <climits>

namespace CombatTrackerApp {
	MonsterCreationScene::MonsterCreationScene(CombatTrackerLib::App* app) :
		EntityCreationScene{ app },
		m_monsterBuilder{},
		m_currentMonsterCR{},
		m_currentMonsterXP{},
		m_monsterCRParameters{ -1, -1, false },
		m_isXPOverridden{ false },
		m_isPBOverridden{ false }
	{
	}

	const std::string& MonsterCreationScene::getEntityLabel() const {
		return ENTITY_LABEL;
	}
	bool MonsterCreationScene::isMonsterScene() const {
		return true;
	}
	void MonsterCreationScene::renderEntitySpecificInfo() {
		ImGui::SeparatorText("Monster Specific Data");
		if (ImGui::BeginTable("MonsterSpecificGrid", 1, ImGuiTableFlags_SizingStretchSame)) {

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			// The label is hidden on the listbox itself so the widget can take the whole column, which is a narrow one here.
			// The label is hidden by passing a ## prefix to the label name, which is something that is done extensively over the entire codebase.
			ImGui::TextUnformatted("CR");
			ImGui::SetNextItemWidth(-1.0f);
			renderListBox(m_monsterCRParameters, m_currentMonsterCR, "##CR", CR_NAMES);

			ImGui::Spacing();
			renderOverriddableInputInt(m_currentMonsterXP, m_isXPOverridden, "XP", "Reset XP", MAPPED_CR_TO_XP, 0, INT_MAX, m_currentMonsterCR);

			ImGui::Spacing();
			renderOverriddableInputInt(m_currentCreatureStatBlock.proficiencyBonus, m_isPBOverridden, "PB", "Reset PB", MAPPED_CR_TO_PB, PB_LOWER_BOUND, INT_MAX, m_currentMonsterCR);

			ImGui::EndTable();
		}
	}

	// Monsters have nothing to add below the senses, unlike player characters.
	void MonsterCreationScene::renderEntitySpecificSection() {}

	// TODO: The built monster is thrown away for now. Once JSON saving exists this is where it should get written.
	bool MonsterCreationScene::completeEntityCreation() {
		m_monsterBuilder.getCreatureBlock() = m_currentCreatureStatBlock;
		m_monsterBuilder.setCR(m_currentMonsterCR).setXP(m_currentMonsterXP);
		Monster monster{ m_monsterBuilder.moveMonster() };
		return true;
	}

	// TODO: Properly setup initial values of the entity that is being edited, once editing exists.
	void MonsterCreationScene::resetEntity() {
		m_monsterBuilder.reset();
		m_currentCreatureStatBlock = m_monsterBuilder.getCreatureBlock();
		m_currentMonsterCR = m_monsterBuilder.getCR();
		m_currentMonsterXP = m_monsterBuilder.getXP();
		resetParametersAndOverrides();
	}
	void MonsterCreationScene::resetParametersAndOverrides() {
		EntityCreationScene::resetParametersAndOverrides();
		m_monsterCRParameters = { -1, -1, false };
		m_isXPOverridden = false;
		m_isPBOverridden = false;
	}
}
