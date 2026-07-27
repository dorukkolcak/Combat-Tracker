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
		renderMonsterSpecificInfo();
	}

	// Monsters have nothing to add below the senses, unlike player characters.
	void MonsterCreationScene::renderEntitySpecificSection() {}

	// Monster specific info includes CR, XP and PB.
	// Even though PB isn't specific to monsters, how it is computed is different for monsters and player characters.
	void MonsterCreationScene::renderMonsterSpecificInfo () {
		if (ImGui::BeginTable("MonsterSpecificGrid", 1, ImGuiTableFlags_SizingStretchSame)) {

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			renderListBox(m_monsterCRParameters, m_currentMonsterCR, "CR", CR_NAMES);

			ImGui::Spacing();
			renderOverriddableInputInt(m_currentMonsterXP, m_isXPOverridden, "XP", "Reset XP", MAPPED_CR_TO_XP, 0, INT_MAX, m_currentMonsterCR);

			ImGui::Spacing();
			renderOverriddableInputInt(m_currentCreatureStatBlock.proficiencyBonus, m_isPBOverridden, "PB", "Reset PB", MAPPED_CR_TO_PB, PB_LOWER_BOUND, INT_MAX, m_currentMonsterCR);

			ImGui::EndTable();
		}
	}

	/*
	TODO: The built monster is thrown away for now. Once JSON saving exists this is where it gets written out.
	The block is edited in place by the base scene, so it only has to be handed to the builder here.
	*/
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
