#include "PlayerCharacterCreationScene.h"
#include "../CombatTrackerLib/Utilities/UIUtils.h"
#include "../Utilities/GenericUtils.h"

namespace CombatTrackerApp {
	PlayerCharacterCreationScene::PlayerCharacterCreationScene(CombatTrackerLib::App* app) :
		EntityCreationScene{ app },
		m_currentPlayerStatBlock{},
		m_speciesParameters{ -1, -1, false },
		m_originParameters{ -1, -1, false }
	{
	}

	std::string_view PlayerCharacterCreationScene::getEntityLabel() const {
		return ENTITY_LABEL;
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

			// The names and the levels are two arrays of the same length, so one index reads both.
			for (size_t i{}; i < CLASS_NAMES.size(); ++i) {
				if (m_currentPlayerStatBlock.levels[i] == 0) {
					continue;
				}
				ImGui::TextWrapped("%s %d", CLASS_NAMES[i].data(), m_currentPlayerStatBlock.levels[i]);
			}
			for (const auto& item : m_currentPlayerStatBlock.subclasses) {
				ImGui::TextWrapped("%s", SUBCLASS_NAMES[enumToSizeT(item.second)].data() );
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
					for (size_t i{}; i < CLASS_NAMES.size(); ++i) {
						const auto className{ CLASS_NAMES[i] };
						ImGui::TableNextColumn();
						ImGui::TextUnformatted(className.data());
						ImGui::SetNextItemWidth(-1.0f);
						renderNonOverridableInputInt(m_currentPlayerStatBlock.levels[i], std::format("##{}", className).c_str());
					}
					ImGui::EndTable();
				}

				static PlayerClass s_currentClass{};
				ImGui::SeparatorText("Subclasses");

				for (size_t i{}; i < SUBCLASS_NAMES.size(); ++i) {
					const auto subclass{ static_cast<PlayerSubclass>(i) };
					const auto correspondingClass{ MAPPED_SUBCLASSES.at(subclass) };
					const auto classIndex{ enumToSizeT(correspondingClass) };

					if (s_currentClass != correspondingClass) {
						ImGui::SeparatorText(CLASS_NAMES[classIndex].data());
					}
					if (m_currentPlayerStatBlock.levels[classIndex] < SUBCLASS_LEVEL_THRESHOLD) {
						m_chosenSubclasses[i] = false;
						m_currentPlayerStatBlock.subclasses.erase(correspondingClass);
					}
					// The checkbox reports both directions of the change, so unchecking has to take the subclass back out of the map.
					if (ImGui::Checkbox(SUBCLASS_NAMES[i].data(), &m_chosenSubclasses[i])) {
						if (!m_chosenSubclasses[i]) {
							m_currentPlayerStatBlock.subclasses.erase(correspondingClass);
						}
						else {
							m_currentPlayerStatBlock.subclasses[correspondingClass] = subclass;
							for (const auto& correspondingSubclass : MAPPED_CLASSES.at(correspondingClass)) {
								const auto subclassIndex{ enumToSizeT(correspondingSubclass) };
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

				if (CombatTrackerLib::alignedButton("Finish Class and Subclass Configuration", 0.5f)) {

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

	// TODO: JSON saving functionality.
	bool PlayerCharacterCreationScene::completeEntityCreation() {
		PlayerCharacter playerCharacter{std::move(m_currentCreatureStatBlock), std::move(m_currentPlayerStatBlock)};
		return true;
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
