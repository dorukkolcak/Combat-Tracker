#include "PlayerCharacterCreationScene.h"
#include "../CombatTrackerLib/Utilities/UIUtils.h"

namespace CombatTrackerApp {
	PlayerCharacterCreationScene::PlayerCharacterCreationScene(CombatTrackerLib::App* app) :
		EntityCreationScene{ app },
		m_playerCharacterBuilder{},
		m_currentPlayerStatBlock{},
		m_speciesParameters{ -1, -1, false },
		m_originParameters{ -1, -1, false }
	{
	}

	const std::string& PlayerCharacterCreationScene::getEntityLabel() const {
		return ENTITY_LABEL;
	}
	bool PlayerCharacterCreationScene::isMonsterScene() const {
		return false;
	}
	void PlayerCharacterCreationScene::renderEntitySpecificInfo() {
		ImGui::SeparatorText("Classes and Subclasses");
		renderPlayerClassAndSubclasses();
	}
	void PlayerCharacterCreationScene::renderEntitySpecificSection() {
		ImGui::SeparatorText("Player Character Specific Data");
		renderPlayerCharacterSpecificInfo();
	}
	void PlayerCharacterCreationScene::renderPlayerCharacterSpecificInfo() {

		// The two listboxes sit side by side and split the section between them, instead of stacking at ImGui's default item width.
		if (ImGui::BeginTable("##SpeciesAndOrigin", 2, ImGuiTableFlags_SizingStretchSame)) {
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted("Species");
			ImGui::SetNextItemWidth(-1.0f);
			renderListBox(m_speciesParameters, m_currentPlayerStatBlock.species, "##Species", SPECIES_NAMES);

			ImGui::TableSetColumnIndex(1);
			ImGui::TextUnformatted("Origin");
			ImGui::SetNextItemWidth(-1.0f);
			renderListBox(m_originParameters, m_currentPlayerStatBlock.origin, "##Origin", ORIGIN_NAMES);

			ImGui::EndTable();
		}
		renderMutuallyExclusiveCheckboxCluster(m_currentPlayerStatBlock.weaponProficiencies, WEAPON_TYPE_NAMES, "Weapon Proficiencies", 3);
		renderMutuallyExclusiveCheckboxCluster(m_currentPlayerStatBlock.armorProficiencies, ARMOR_TYPE_NAMES, "Armor Proficiencies", 3);
		renderMutuallyExclusiveCheckboxCluster(m_currentPlayerStatBlock.toolProficiencies, TOOL_NAMES, "Tool Proficiencies", 3);
		renderMutuallyExclusiveCheckboxCluster(m_currentPlayerStatBlock.weaponMasteries, WEAPON_MASTERY_NAMES, "Weapon Masteries", 3);
	}
	void PlayerCharacterCreationScene::renderPlayerClassAndSubclasses() {
		if (ImGui::BeginTable("Classes and Subclasses", 1, ImGuiTableFlags_SizingStretchSame)) {

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			for (const auto& item : m_currentPlayerStatBlock.levels) {
				if (item.second == 0) {
					continue;
				}
				ImGui::TextWrapped("%s %d", CLASS_NAMES.at(item.first).c_str(), item.second);
			}
			for (const auto& item : m_currentPlayerStatBlock.subclasses) {
				ImGui::TextWrapped("%s", SUBCLASS_NAMES.at(item.second).c_str() );
			}
			if (CombatTrackerLib::alignedButton("Configure Classes and Subclasses", 0.5f)) {
				ImGui::OpenPopup("Configuration");
			}
			// The popup can neither be moved nor resized by hand, so both its position and its size keep following the viewport.
			const auto center{ ImGui::GetMainViewport()->GetCenter() };
			ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2{ 0.5f, 0.5f });
			const auto viewportSize{ ImGui::GetMainViewport()->Size };
			ImGui::SetNextWindowSize(ImVec2{ viewportSize.x * 0.92f, viewportSize.y * 0.92f }, ImGuiCond_Always);
			if (ImGui::BeginPopupModal("Configuration", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
				ImGui::BeginChild("EditClassScroll", ImVec2{ 0.0f, -CombatTrackerLib::reservedFooterHeight(1) }, true);


				ImGui::TextWrapped("Classes");

				// The level fields are laid out as a grid with the labels above them, so the twelve of them don't turn into twelve near empty rows.
				if (ImGui::BeginTable("##ClassLevels", 4, ImGuiTableFlags_SizingStretchSame)) {
					for (const auto& item : CLASS_NAMES) {
						ImGui::TableNextColumn();
						ImGui::TextUnformatted(item.second.c_str());
						ImGui::SetNextItemWidth(-1.0f);
						renderNonOverridableInputInt(m_currentPlayerStatBlock.levels.at(item.first), ("##" + item.second).c_str());
					}
					ImGui::EndTable();
				}

				static PlayerClass s_currentClass{};
				ImGui::SeparatorText("Subclasses");
				for (const auto& item : SUBCLASS_NAMES) {
					if (s_currentClass != MAPPED_SUBCLASSES.at(item.first)) {
						ImGui::SeparatorText(CLASS_NAMES.at(MAPPED_SUBCLASSES.at(item.first)).c_str());
					}
					if (m_currentPlayerStatBlock.levels.at(MAPPED_SUBCLASSES.at(item.first)) < SUBCLASS_LEVEL_THRESHOLD) {
						m_chosenSubclasses.at(item.first) = false;
						m_currentPlayerStatBlock.subclasses.erase(MAPPED_SUBCLASSES.at(item.first));
					}
					if (ImGui::Checkbox(item.second.c_str(), &m_chosenSubclasses.at(item.first))) {
						m_hasCorrespondingSubclass.at(MAPPED_SUBCLASSES.at(item.first)) = true;
						m_currentPlayerStatBlock.subclasses[MAPPED_SUBCLASSES.at(item.first)] = item.first;
						for (const auto& secondItem : MAPPED_CLASSES.at(MAPPED_SUBCLASSES.at(item.first))) {
							if ((m_chosenSubclasses.at(secondItem) && secondItem != item.first)
								|| m_currentPlayerStatBlock.levels.at(MAPPED_SUBCLASSES.at(secondItem)) < SUBCLASS_LEVEL_THRESHOLD )
							{
								m_chosenSubclasses.at(secondItem) = false;
							}
						}
					}
					s_currentClass = MAPPED_SUBCLASSES.at(item.first);
				}
				ImGui::EndChild();

				if (CombatTrackerLib::alignedButton("Finish Class and Subclass Configuration", 0.5f)) {

					// Lengthy but necessary validation.
					int sum{};
					bool isValid{ false };
					for (auto& item : m_currentPlayerStatBlock.levels) {
						if (item.second > MIN_CLASS_LEVEL) {
							isValid = true;
						}
						sum += item.second;
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

	
	// TODO: JSON saving functionality.
	bool PlayerCharacterCreationScene::completeEntityCreation() {
		m_playerCharacterBuilder.getCreatureBlock() = m_currentCreatureStatBlock;
		m_playerCharacterBuilder.getPlayerCharacterBlock() = m_currentPlayerStatBlock;
		PlayerCharacter playerCharacter{ m_playerCharacterBuilder.movePlayerCharacter() };
		return true;
	}

	// TODO: Properly setup initial values of the entity that is being edited, once editing exists.
	void PlayerCharacterCreationScene::resetEntity() {
		m_playerCharacterBuilder.reset();
		m_currentCreatureStatBlock = m_playerCharacterBuilder.getCreatureBlock();
		m_currentPlayerStatBlock = m_playerCharacterBuilder.getPlayerCharacterBlock();
		resetParametersAndOverrides();
	}
	void PlayerCharacterCreationScene::resetParametersAndOverrides() {
		EntityCreationScene::resetParametersAndOverrides();
		m_speciesParameters = { -1, -1, false };
		m_originParameters = { -1, -1, false };
		for (auto& subclass : m_chosenSubclasses) {
			subclass.second = false;
		}
		for (auto& playerClass : m_hasCorrespondingSubclass) {
			playerClass.second = false;
		}
	}
}
