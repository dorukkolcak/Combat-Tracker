#include "CombatTrackerApp/Scenes/MonsterCreationScene.h"
#include "CombatTrackerLib/Utilities/UIUtils.h"
#include "CombatTrackerApp/Entities/CreatureStorage.h"

#include <optional>

namespace CombatTrackerApp {
	MonsterCreationScene::MonsterCreationScene(CombatTrackerLib::App* app) :
		EntityCreationScene{ app },
		m_currentMonsterStatBlock{},
		m_monsterCRParameters{}
	{
	}

	EntityCreationScene::CreationLabels MonsterCreationScene::getCreationLabels() const {
		return {
			m_localeProviderPointer->getText("/scenes/monsterCreation/windowTitle"),
			m_localeProviderPointer->getText("/scenes/monsterCreation/completeButton"),
			m_localeProviderPointer->getText("/scenes/monsterCreation/cancelButton")
		};
	}
	void MonsterCreationScene::resolveDerivedValues() {
		Monster::resolveDerived(m_currentCreatureStatBlock, m_currentMonsterStatBlock);
	}
	void MonsterCreationScene::renderEntitySpecificInfo() {
		ImGui::SeparatorText(m_localeProviderPointer->getText("/scenes/monsterCreation/specificData").data());
		if (CombatTrackerLib::ScopedTable monsterSpecificGrid{ "MonsterSpecificGrid", 1, ImGuiTableFlags_SizingStretchSame }) {

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			// The label is hidden on the listbox itself so the widget can take the whole column, which is a narrow one here.
			// The label is hidden by passing a ## prefix to the label name, which is something that is done extensively over the entire codebase.
			ImGui::TextUnformatted(m_localeProviderPointer->getText("/scenes/monsterCreation/challengeRating").data());
			ImGui::SetNextItemWidth(-1.0f);

			const auto challengeRatingNames{ m_localeProviderPointer->getTextList("/domain/challengeRating") };
			const auto clickedRating{ CombatTrackerLib::renderListBox(m_monsterCRParameters, "##CR", challengeRatingNames) };
			if (clickedRating != std::nullopt) {
				m_currentMonsterStatBlock.CR = CHALLENGE_RATINGS[*clickedRating].value;
			}

			ImGui::Spacing();
			ImGui::TextUnformatted(m_localeProviderPointer->getText("/scenes/monsterCreation/experiencePoints").data());
			if (m_monsterCRParameters.isHighlighted) {
				ImGui::TextDisabled("%d", CHALLENGE_RATINGS[m_monsterCRParameters.highlightedIndex].XP);
			}
			else {
				ImGui::Text("%d", m_currentMonsterStatBlock.derived.getXP());
			}

			ImGui::Spacing();
			ImGui::TextUnformatted(m_localeProviderPointer->getText("/scenes/monsterCreation/proficiencyBonus").data());
			ImGui::Text("%d", m_currentCreatureStatBlock.derived.getProficiencyBonus());

		}
	}

	bool MonsterCreationScene::completeEntityCreation() {
		bool isSavingSuccessful{
			CreatureStorage::saveCreature<Monster>(
				m_applicationPointer->getDirectoryProvider().getUserDataDirectory(),
				m_currentCreatureStatBlock,
				std::cref(m_currentMonsterStatBlock),
				std::nullopt
			)
		};
		if (isSavingSuccessful) {
			resetParametersAndOverrides();
			return true;
		}
		return false;
	}
	void MonsterCreationScene::resetParametersAndOverrides() {
		EntityCreationScene::resetParametersAndOverrides();
		m_currentMonsterStatBlock = MonsterStatBlock{};
		m_monsterCRParameters = CombatTrackerLib::ListBoxParameters{};
	}
}
