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
		m_monsterCRParameters{ CHALLENGE_RATINGS.size(), CHALLENGE_RATINGS.size(), false }
	{
	}

	EntityCreationScene::CreationLabels MonsterCreationScene::getCreationLabels() const {
		return {
			m_localeProviderRef.getText("/scenes/monsterCreation/windowTitle"),
			m_localeProviderRef.getText("/scenes/monsterCreation/completeButton"),
			m_localeProviderRef.getText("/scenes/monsterCreation/cancelButton")
		};
	}
	void MonsterCreationScene::renderEntitySpecificInfo() {
		ImGui::SeparatorText(m_localeProviderRef.getText("/scenes/monsterCreation/specificData").data());
		if (ImGui::BeginTable("MonsterSpecificGrid", 1, ImGuiTableFlags_SizingStretchSame)) {

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			// The label is hidden on the listbox itself so the widget can take the whole column, which is a narrow one here.
			// The label is hidden by passing a ## prefix to the label name, which is something that is done extensively over the entire codebase.
			ImGui::TextUnformatted(m_localeProviderRef.getText("/scenes/monsterCreation/challengeRating").data());
			ImGui::SetNextItemWidth(-1.0f);
			renderCRListBox();

			const auto& challengeRating{ findChallengeRating(m_currentMonsterCR) };

			ImGui::Spacing();
			ImGui::PushID("XP");
			CombatTrackerLib::renderOverridableInputInt(
				m_currentMonsterXP.value,
				m_currentMonsterXP.isOverridden,
				challengeRating.XP,
				m_localeProviderRef.getText("/scenes/monsterCreation/experiencePoints"),
				m_localeProviderRef.getText("/scenes/monsterCreation/resetExperiencePoints"),
				0,
				INT_MAX
			);
			ImGui::PopID();

			ImGui::Spacing();
			ImGui::PushID("PB");
			CombatTrackerLib::renderOverridableInputInt(
				m_currentCreatureStatBlock.proficiencyBonus.value,
				m_currentCreatureStatBlock.proficiencyBonus.isOverridden,
				challengeRating.proficiencyBonus,
				m_localeProviderRef.getText("/scenes/monsterCreation/proficiencyBonus"),
				m_localeProviderRef.getText("/scenes/monsterCreation/resetProficiencyBonus"),
				PB_LOWER_BOUND,
				INT_MAX
			);
			ImGui::PopID();

			ImGui::EndTable();
		}
	}

	// The CR table carries its own keys, so the row is read straight out of it instead of being derived from the row position.
	void MonsterCreationScene::renderCRListBox() {
		int counter{};
		const float height{ ImGui::GetTextLineHeightWithSpacing() * 8.0f };
		if (ImGui::BeginListBox("##CR", ImVec2{ 0, height })) {
			for (const auto& challengeRating: CHALLENGE_RATINGS) {
				const bool isSelected{ counter == m_monsterCRParameters.chosenIndex };

				if (ImGui::Selectable(challengeRating.label.data(), isSelected)) {
					m_monsterCRParameters.chosenIndex = counter;
					m_currentMonsterCR = challengeRating.value;
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
		m_monsterCRParameters = { CHALLENGE_RATINGS.size(), CHALLENGE_RATINGS.size(), false};
	}
}
