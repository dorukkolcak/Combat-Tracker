#include "MonsterCreationScene.h"
#include "../Entities/Creature/CreatureConstants.h"
#include "../Entities/Monster/MonsterConstants.h"
#include "../CombatTrackerLib/Utilities/UIUtils.h"
#include "../Utilities/JSONUtils.h"

#include <climits>

namespace CombatTrackerApp {
	MonsterCreationScene::MonsterCreationScene(CombatTrackerLib::App* app) :
		EntityCreationScene{ app },
		m_currentMonsterCR{},
		m_currentMonsterXP{},
		m_monsterCRParameters{ -1, -1, false }
	{
	}

	std::string_view MonsterCreationScene::getEntityLabel() const {
		return ENTITY_LABEL;
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
			renderCRListBox();

			ImGui::Spacing();
			renderOverriddableInputInt(m_currentMonsterXP, "XP", "Reset XP", MAPPED_CR_TO_XP, 0, INT_MAX, m_currentMonsterCR);

			ImGui::Spacing();
			renderOverriddableInputInt(
				m_currentCreatureStatBlock.proficiencyBonus, 
				"PB", 
				"Reset PB", 
				MAPPED_CR_TO_PB, 
				PB_LOWER_BOUND, 
				INT_MAX, 
				m_currentMonsterCR
			);

			ImGui::EndTable();
		}
	}

	// The CR table carries its own keys, so the row is read straight out of it instead of being derived from the row position.
	void MonsterCreationScene::renderCRListBox() {
		int counter{};
		const float height{ ImGui::GetTextLineHeightWithSpacing() * 8.0f };
		if (ImGui::BeginListBox("##CR", ImVec2{ 0, height })) {
			for (const auto& item: CR_NAMES) {
				const bool isSelected{ counter == m_monsterCRParameters.chosenIndex };
				if (ImGui::Selectable(item.second.data(), isSelected)) {
					m_monsterCRParameters.chosenIndex = counter;
					m_currentMonsterCR = item.first;
				}
				if (m_monsterCRParameters.isHighlighted && ImGui::IsItemHovered()) {
					m_monsterCRParameters.highlightedIndex = counter;
				}
				++counter;
			}
			ImGui::EndListBox();
		}
	}

	// Monsters have nothing to add below the senses, unlike player characters.
	void MonsterCreationScene::renderEntitySpecificSection() {}

	bool MonsterCreationScene::completeEntityCreation() {
		bool isSavingSuccessful{ saveEntity<Monster>(m_currentCreatureStatBlock, m_currentMonsterCR, m_currentMonsterXP, std::nullopt) };
		if (isSavingSuccessful) {
			resetParametersAndOverrides();
			return true;
		}
		return false;
	}
	void MonsterCreationScene::resetParametersAndOverrides() {
		EntityCreationScene::resetParametersAndOverrides();
		m_currentMonsterCR = 0.0;
		m_currentMonsterXP = OverridableInt{};
		m_monsterCRParameters = { -1, -1, false };
	}
}
