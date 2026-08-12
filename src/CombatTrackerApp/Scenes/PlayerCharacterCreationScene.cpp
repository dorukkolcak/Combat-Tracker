#include "CombatTrackerApp/Scenes/PlayerCharacterCreationScene.h"
#include "CombatTrackerLib/Utilities/UIUtils.h"
#include "CombatTrackerLib/Utilities/GenericUtils.h"
#include "CombatTrackerApp/Entities/EntityStorage.h"

namespace CombatTrackerApp {
	PlayerCharacterCreationScene::PlayerCharacterCreationScene(CombatTrackerLib::App* app) :
		EntityCreationScene{ app },
		m_currentPlayerStatBlock{},
		m_speciesParameters{ -1, -1, false },
		m_originParameters{ -1, -1, false }
	{
	}

	std::string_view PlayerCharacterCreationScene::getEntityLabel() const {
		return m_applicationPointer->getLocaleReader().getText("/scenes/playerCharacterCreation/entityLabel");
	}
	void PlayerCharacterCreationScene::renderEntitySpecificInfo() {
		ImGui::SeparatorText(m_applicationPointer->getLocaleReader().getText("/scenes/playerCharacterCreation/classesAndSubclasses").data());
		renderPlayerClassAndSubclasses();
	}
	void PlayerCharacterCreationScene::renderEntitySpecificSection() {
		ImGui::SeparatorText(m_applicationPointer->getLocaleReader().getText("/scenes/playerCharacterCreation/specificData").data());
		renderPlayerCharacterSpecificInfo();
	}
	void PlayerCharacterCreationScene::renderPlayerCharacterSpecificInfo() {

		const auto& localeReader{ m_applicationPointer->getLocaleReader() };

		// The two listboxes sit side by side and split the section between them, instead of stacking at ImGui's default item width.
		if (ImGui::BeginTable("##SpeciesAndOrigin", 2, ImGuiTableFlags_SizingStretchSame)) {
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted(localeReader.getText("/scenes/playerCharacterCreation/species").data());
			ImGui::SetNextItemWidth(-1.0f);
			renderListBox(m_speciesParameters, m_currentPlayerStatBlock.species, "##Species", localeReader.getList("/domain/species"));

			ImGui::TableSetColumnIndex(1);
			ImGui::TextUnformatted(localeReader.getText("/scenes/playerCharacterCreation/origin").data());
			ImGui::SetNextItemWidth(-1.0f);
			renderListBox(m_originParameters, m_currentPlayerStatBlock.origin, "##Origin", localeReader.getList("/domain/origin"));

			ImGui::EndTable();
		}
		renderMutuallyExclusiveCheckboxCluster(
			m_currentPlayerStatBlock.weaponProficiencies,
			localeReader.getList("/domain/weaponType"),
			localeReader.getText("/scenes/playerCharacterCreation/weaponProficiencies"),
			3
		);
		renderMutuallyExclusiveCheckboxCluster(
			m_currentPlayerStatBlock.armorProficiencies,
			localeReader.getList("/domain/armorType"),
			localeReader.getText("/scenes/playerCharacterCreation/armorProficiencies"),
			3
		);
		renderMutuallyExclusiveCheckboxCluster(
			m_currentPlayerStatBlock.toolProficiencies,
			localeReader.getList("/domain/tool"),
			localeReader.getText("/scenes/playerCharacterCreation/toolProficiencies"),
			3
		);
		renderMutuallyExclusiveCheckboxCluster(
			m_currentPlayerStatBlock.weaponMasteries,
			localeReader.getList("/domain/weaponMastery"),
			localeReader.getText("/scenes/playerCharacterCreation/weaponMasteries"),
			3
		);
	}
	void PlayerCharacterCreationScene::renderPlayerClassAndSubclasses() {

		const auto& localeReader{ m_applicationPointer->getLocaleReader() };
		const auto& classNames{ localeReader.getList("/domain/playerClass") };
		const auto& subclassNames{ localeReader.getList("/domain/playerSubclass") };

		if (ImGui::BeginTable("Classes and Subclasses", 1, ImGuiTableFlags_SizingStretchSame)) {

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			// The names and the levels are two arrays of the same length, so one index reads both.
			for (size_t i{}; i < classNames.size(); ++i) {
				if (m_currentPlayerStatBlock.levels[i] == 0) {
					continue;
				}
				ImGui::TextWrapped("%s %d", classNames[i].get_ref<const std::string&>().c_str(), m_currentPlayerStatBlock.levels[i]);
			}
			for (const auto& item : m_currentPlayerStatBlock.subclasses) {
				ImGui::TextWrapped("%s", subclassNames[CombatTrackerLib::enumToSizeT(item.second)].get_ref<const std::string&>().c_str());
			}
			if (CombatTrackerLib::alignedButton(localeReader.getText("/scenes/playerCharacterCreation/configureButton").data(), 0.5f)) {
				ImGui::OpenPopup("###ClassConfiguration");
			}

			// The popup can neither be moved nor resized by hand, so both its position and its size keep following the viewport.
			const auto center{ ImGui::GetMainViewport()->GetCenter() };
			ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2{ 0.5f, 0.5f });
			const auto viewportSize{ ImGui::GetMainViewport()->Size };
			ImGui::SetNextWindowSize(ImVec2{ viewportSize.x * 0.92f, viewportSize.y * 0.92f }, ImGuiCond_Always);
			if (ImGui::BeginPopupModal(
				std::format("{}###ClassConfiguration", localeReader.getText("/scenes/playerCharacterCreation/configureButton")).c_str(),
				NULL,
				ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
			)) {
				ImGui::BeginChild("EditClassScroll", ImVec2{ 0.0f, -CombatTrackerLib::reservedFooterHeight(1) }, true);


				ImGui::TextWrapped("%s", localeReader.getText("/scenes/playerCharacterCreation/classes").data());

				// The level fields are laid out as a grid with the labels above them, so the twelve of them don't turn into twelve near empty rows.
				if (ImGui::BeginTable("##ClassLevels", 4, ImGuiTableFlags_SizingStretchSame)) {
					for (size_t i{}; i < classNames.size(); ++i) {
						const auto& className{ classNames[i].get_ref<const std::string&>() };
						ImGui::TableNextColumn();
						ImGui::TextUnformatted(className.c_str());
						ImGui::SetNextItemWidth(-1.0f);
						renderNonOverridableInputInt(m_currentPlayerStatBlock.levels[i], std::format("##ClassLevel{}", i));
					}
					ImGui::EndTable();
				}

				static PlayerClass s_currentClass{};
				ImGui::SeparatorText(localeReader.getText("/scenes/playerCharacterCreation/subclasses").data());

				for (size_t i{}; i < subclassNames.size(); ++i) {
					const auto subclass{ static_cast<PlayerSubclass>(i) };
					const auto correspondingClass{ MAPPED_SUBCLASSES.at(subclass) };
					const auto classIndex{ CombatTrackerLib::enumToSizeT(correspondingClass) };

					if (s_currentClass != correspondingClass) {
						ImGui::SeparatorText(classNames[classIndex].get_ref<const std::string&>().c_str());
					}
					if (m_currentPlayerStatBlock.levels[classIndex] < SUBCLASS_LEVEL_THRESHOLD) {
						m_chosenSubclasses[i] = false;
						m_currentPlayerStatBlock.subclasses.erase(correspondingClass);
					}
					// The checkbox reports both directions of the change, so unchecking has to take the subclass back out of the map.
					if (ImGui::Checkbox(subclassNames[i].get_ref<const std::string&>().c_str(), &m_chosenSubclasses[i])) {
						if (!m_chosenSubclasses[i]) {
							m_currentPlayerStatBlock.subclasses.erase(correspondingClass);
						}
						else {
							m_currentPlayerStatBlock.subclasses[correspondingClass] = subclass;
							for (const auto& correspondingSubclass : MAPPED_CLASSES.at(correspondingClass)) {
								const auto subclassIndex{ CombatTrackerLib::enumToSizeT(correspondingSubclass) };
								if ((m_chosenSubclasses[subclassIndex] && correspondingSubclass != subclass)
									|| m_currentPlayerStatBlock.levels[classIndex] < SUBCLASS_LEVEL_THRESHOLD )
								{
									m_chosenSubclasses[subclassIndex] = false;
								}
							}
						}
					}
					s_currentClass = correspondingClass;
				}
				ImGui::EndChild();

				if (CombatTrackerLib::alignedButton(localeReader.getText("/scenes/playerCharacterCreation/finishConfiguration").data(), 0.5f)) {

					// Lengthy but necessary validation.
					int sum{};
					bool isValid{ false };
					for (const auto& level : m_currentPlayerStatBlock.levels) {
						if (level > MIN_CLASS_LEVEL) {
							isValid = true;
						}
						sum += level;
						if (sum > MAX_TOTAL_CLASS_LEVEL) {
							isValid = false;
							break;
						}
					}
					if (isValid) {
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}
			ImGui::EndTable();
		}
	}

	bool PlayerCharacterCreationScene::completeEntityCreation() {
		bool isSavingSuccessful{ saveEntity<PlayerCharacter>(m_currentCreatureStatBlock, std::nullopt, std::nullopt, m_currentPlayerStatBlock)};
		if (isSavingSuccessful) {
			resetParametersAndOverrides();
			return true;
		}
		return false;
	}

	// TODO: Properly setup initial values of the entity that is being edited, once editing exists.
	void PlayerCharacterCreationScene::resetParametersAndOverrides() {
		EntityCreationScene::resetParametersAndOverrides();
		m_currentPlayerStatBlock = PlayerStatBlock{};
		m_speciesParameters = { -1, -1, false };
		m_originParameters = { -1, -1, false };
		m_chosenSubclasses.fill(false);
	}
}
