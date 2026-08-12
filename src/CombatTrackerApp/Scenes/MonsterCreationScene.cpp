#include "CombatTrackerApp/Scenes/MonsterCreationScene.h"
#include "CombatTrackerApp/Entities/Creature/CreatureConstants.h"
#include "CombatTrackerApp/Entities/Monster/MonsterConstants.h"
#include "CombatTrackerLib/Utilities/UIUtils.h"
#include "CombatTrackerApp/Entities/EntityStorage.h"

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
		return m_applicationPointer->getLocaleReader().getText("/scenes/monsterCreation/entityLabel");
	}
	void MonsterCreationScene::renderEntitySpecificInfo() {

		const auto& localeReader{ m_applicationPointer->getLocaleReader() };

		ImGui::SeparatorText(localeReader.getText("/scenes/monsterCreation/specificData").data());
		if (ImGui::BeginTable("MonsterSpecificGrid", 1, ImGuiTableFlags_SizingStretchSame)) {

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			// The label is hidden on the listbox itself so the widget can take the whole column, which is a narrow one here.
			// The label is hidden by passing a ## prefix to the label name, which is something that is done extensively over the entire codebase.
			ImGui::TextUnformatted(localeReader.getText("/scenes/monsterCreation/challengeRating").data());
			ImGui::SetNextItemWidth(-1.0f);
			renderCRListBox();

			ImGui::Spacing();
			renderOverriddableInputInt(
				m_currentMonsterXP,
				localeReader.getText("/scenes/monsterCreation/experiencePoints"),
				localeReader.getText("/scenes/monsterCreation/resetExperiencePoints"),
				MAPPED_CR_TO_XP,
				0,
				INT_MAX,
				m_currentMonsterCR
			);

			ImGui::Spacing();
			renderOverriddableInputInt(
				m_currentCreatureStatBlock.proficiencyBonus,
				localeReader.getText("/scenes/monsterCreation/proficiencyBonus"),
				localeReader.getText("/scenes/monsterCreation/resetProficiencyBonus"),
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
			for (const auto& item: MAPPED_CR_TO_XP) {
				const bool isSelected{ counter == m_monsterCRParameters.chosenIndex };
				if (ImGui::Selectable(std::format("{}", item.first).c_str(), isSelected)) {
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
