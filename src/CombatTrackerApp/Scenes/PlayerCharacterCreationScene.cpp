#include "CombatTrackerApp/Scenes/PlayerCharacterCreationScene.h"
#include "CombatTrackerLib/Utilities/UIUtils.h"
#include "CombatTrackerLib/Utilities/GenericUtils.h"
#include "CombatTrackerApp/Entities/CreatureStorage.h"

namespace CombatTrackerApp {
	PlayerCharacterCreationScene::PlayerCharacterCreationScene(CombatTrackerLib::App* app) :
		EntityCreationScene{ app },
		m_currentPlayerStatBlock{},
		m_speciesParameters{},
		m_originParameters{}
	{}
	EntityCreationScene::CreationLabels PlayerCharacterCreationScene::getCreationLabels() const {
		return {
			m_localeProviderPointer->getText("/scenes/playerCharacterCreation/windowTitle"),
			m_localeProviderPointer->getText("/scenes/playerCharacterCreation/completeButton"),
			m_localeProviderPointer->getText("/scenes/playerCharacterCreation/cancelButton")
		};
	}
	void PlayerCharacterCreationScene::renderEntitySpecificInfo() {
		ImGui::SeparatorText(m_localeProviderPointer->getText("/scenes/playerCharacterCreation/classesAndSubclasses").data());
		renderPlayerClassAndSubclasses();
	}
	void PlayerCharacterCreationScene::renderEntitySpecificSection() {
		ImGui::SeparatorText(m_localeProviderPointer->getText("/scenes/playerCharacterCreation/specificData").data());
		renderPlayerCharacterSpecificInfo();
	}
	void PlayerCharacterCreationScene::renderPlayerCharacterSpecificInfo() {

		// The two listboxes sit side by side and split the section between them, instead of stacking at ImGui's default item width.
		if (CombatTrackerLib::ScopedTable speciesAndOrigin{ "##SpeciesAndOrigin", 2, ImGuiTableFlags_SizingStretchSame }) {
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted(m_localeProviderPointer->getText("/scenes/playerCharacterCreation/species").data());
			ImGui::SetNextItemWidth(-1.0f);
			renderListBox(m_speciesParameters, m_currentPlayerStatBlock.species, "##Species", m_localeProviderPointer->getTextList("/domain/species"));

			ImGui::TableSetColumnIndex(1);
			ImGui::TextUnformatted(m_localeProviderPointer->getText("/scenes/playerCharacterCreation/origin").data());
			ImGui::SetNextItemWidth(-1.0f);
			renderListBox(m_originParameters, m_currentPlayerStatBlock.origin, "##Origin", m_localeProviderPointer->getTextList("/domain/origin"));

		}
		CombatTrackerLib::renderCheckboxGrid(
			m_currentPlayerStatBlock.weaponProficiencies,
			m_localeProviderPointer->getTextList("/domain/weaponType"),
			"WeaponProficiencies",
			m_localeProviderPointer->getText("/scenes/playerCharacterCreation/weaponProficiencies"),
			3
		);
		CombatTrackerLib::renderCheckboxGrid(
			m_currentPlayerStatBlock.armorProficiencies,
			m_localeProviderPointer->getTextList("/domain/armorType"),
			"ArmorProficiencies",
			m_localeProviderPointer->getText("/scenes/playerCharacterCreation/armorProficiencies"),
			3
		);
		renderProficiencyLevelGrid(
			m_currentPlayerStatBlock.toolProficiencyLevels,
			m_localeProviderPointer->getTextList("/domain/tool"),
			"ToolProficiencies",
			m_localeProviderPointer->getText("/scenes/playerCharacterCreation/toolProficiencies"),
			3
		);
		CombatTrackerLib::renderCheckboxGrid(
			m_currentPlayerStatBlock.weaponMasteries,
			m_localeProviderPointer->getTextList("/domain/weaponMastery"),
			"WeaponMasteries",
			m_localeProviderPointer->getText("/scenes/playerCharacterCreation/weaponMasteries"),
			3
		);
	}
	void PlayerCharacterCreationScene::renderPlayerClassAndSubclasses() {

		const auto classNames{ m_localeProviderPointer->getTextList("/domain/playerClass") };
		const auto subclassNames{ m_localeProviderPointer->getTextList("/domain/playerSubclass") };

		if (CombatTrackerLib::ScopedTable classesAndSubclasses{ "Classes and Subclasses", 1, ImGuiTableFlags_SizingStretchSame }) {

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			for (size_t i{}; i < classNames.size(); ++i) {
				if (m_currentPlayerStatBlock.levels[i].get() == MIN_CLASS_LEVEL) {
					continue;
				}
				ImGui::TextWrapped("%s %d", classNames[i].data(), m_currentPlayerStatBlock.levels[i].get());
			}
			for (const auto& subclassSlot : m_currentPlayerStatBlock.subclasses) {
				if (subclassSlot.has_value()) {
					ImGui::TextWrapped("%s", subclassNames[CombatTrackerLib::enumToSizeT(subclassSlot.value())].data());
				}
			}
			if (CombatTrackerLib::alignedButton(m_localeProviderPointer->getText("/scenes/playerCharacterCreation/configureButton").data(), 0.5f)) {
				ImGui::OpenPopup("###ClassConfiguration");
			}

			// The popup can neither be moved nor resized by hand, so both its position and its size keep following the viewport.
			CombatTrackerLib::centerNextWindow(0.92f, 0.92f);
			if (CombatTrackerLib::ScopedPopupModal popupModal{
				m_localeProviderPointer->getText("/scenes/playerCharacterCreation/configureButton"),
				nullptr,
				ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize 
			}) {
				{
					CombatTrackerLib::ScopedChild editClassScroll{
						"EditClassScroll",
						ImVec2{ 0.0f, -CombatTrackerLib::reservedFooterHeight(1) },
						true 
					};


					ImGui::TextWrapped("%s", m_localeProviderPointer->getText("/scenes/playerCharacterCreation/classes").data());
					if (CombatTrackerLib::ScopedTable classLevels{ "##ClassLevels", 4, ImGuiTableFlags_SizingStretchSame }) {
						for (size_t i{}; i < classNames.size(); ++i) {
							const auto className{ classNames[i] };
							CombatTrackerLib::ScopedID scopedID{ static_cast<int>(i) };
							ImGui::TableNextColumn();
							ImGui::TextUnformatted(className.data());
							ImGui::SetNextItemWidth(-1.0f);
							CombatTrackerLib::renderBoundedInputInt(m_currentPlayerStatBlock.levels[i], "##ClassLevel");
						}
					}

					static PlayerClass s_currentClass{};
					ImGui::SeparatorText(m_localeProviderPointer->getText("/scenes/playerCharacterCreation/subclasses").data());

					/*
					Each class owns one subclass, so the checkbox writes the subclass directly and choosing a
					second subclass for the same class simply replaces the first.
					Slots belonging to classes below the level threshold are emptied by resolveDerived() at the
					top of the frame, so nothing here has to handle that.
					*/
					for (size_t i{}; i < subclassNames.size(); ++i) {
						const auto subclass{ static_cast<PlayerSubclass>(i) };
						const auto correspondingClass{ SUBCLASSES_TO_CLASSES[CombatTrackerLib::enumToSizeT(subclass)] };
						const auto classIndex{ CombatTrackerLib::enumToSizeT(correspondingClass) };
						auto& subclassSlot{ m_currentPlayerStatBlock.subclasses[classIndex] };

						// This prevents printing multiple class labels. 
						// Otherwise we would have a class label for every subclass.
						if (s_currentClass != correspondingClass) {
							ImGui::SeparatorText(classNames[classIndex].data());
						}

						// The checkbox reports both directions of the change, so unchecking has to empty the slot again.
						bool isChosen{ subclassSlot == subclass };
						if (ImGui::Checkbox(subclassNames[i].data(), &isChosen)) {
							if (isChosen) {
								subclassSlot = subclass;
							}
							else {
								subclassSlot.reset();
							}
						}
						s_currentClass = correspondingClass;
					}
				}

				if (CombatTrackerLib::alignedButton(
					m_localeProviderPointer->getText("/scenes/playerCharacterCreation/finishConfiguration").data(), 
					0.5f)
				) {
					ImGui::CloseCurrentPopup();
				}
			}
		}
	}

	void PlayerCharacterCreationScene::resolveDerivedValues() {
		PlayerCharacter::resolveDerived(m_currentCreatureStatBlock, m_currentPlayerStatBlock);
	}

	bool PlayerCharacterCreationScene::completeEntityCreation() {
		bool isSavingSuccessful{
			CreatureStorage::saveCreature<PlayerCharacter>(
				m_applicationPointer->getDirectoryProvider().getUserDataDirectory(),
				m_currentCreatureStatBlock,
				std::nullopt,
				std::cref(m_currentPlayerStatBlock)
			)
		};
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
		m_speciesParameters = CombatTrackerLib::ListBoxParameters{};
		m_originParameters = CombatTrackerLib::ListBoxParameters{};
	}
}
