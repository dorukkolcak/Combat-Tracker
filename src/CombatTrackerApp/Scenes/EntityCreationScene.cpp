#include "CombatTrackerApp/Scenes/EntityCreationScene.h"
#include "CombatTrackerApp/Scenes/EncounterCreationScene.h"
#include "CombatTrackerApp/Entities/Creature/CreatureConstants.h"
#include "CombatTrackerLib/Utilities/UIUtils.h"
#include "CombatTrackerLib/Utilities/GenericUtils.h"

namespace CombatTrackerApp {
	EntityCreationScene::EntityCreationScene(CombatTrackerLib::App* app) :
		CombatTrackerLib::Scene{ app },
		m_currentCreatureStatBlock{},
		m_sizeListBoxParameters{ CombatTrackerLib::enumToSizeT(Size::COUNT), CombatTrackerLib::enumToSizeT(Size::COUNT), false },
		m_creatureTypeParameters{ CombatTrackerLib::enumToSizeT(CreatureType::COUNT), CombatTrackerLib::enumToSizeT(CreatureType::COUNT), false},
		m_alignmentParameters{ CombatTrackerLib::enumToSizeT(Alignment::COUNT), CombatTrackerLib::enumToSizeT(Alignment::COUNT), false}
	{
	}

	void EntityCreationScene::notifyScene(const ImGuiIO&) {} // No non-ImGui based event handling should exist in this scene, I think.
	void EntityCreationScene::updateScene(double) {} // Purely input based scene so no updates necessary.
	void EntityCreationScene::renderScene() {} // The scene consists of just UI widgets so no OpenGL side rendering work.

	void EntityCreationScene::renderUI() {

		const auto creationLabels{ getCreationLabels() };

		// Take over the whole viewport, the same way the other scene hosts do.
		const auto viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::Begin(creationLabels.windowTitle.data(), nullptr, ENTITY_CREATION_WINDOW_FLAGS);

		// TODO: Properly setup initial values of the entity that is being created/edited.
		if (ImGui::IsWindowAppearing()) {
			resetParametersAndOverrides();
		}

		ImGui::SeparatorText(creationLabels.windowTitle.data());

		// Scrollable region for the entire scene.
		// The footer below it is a separator plus the two completion buttons.
		const float footerHeight{ CombatTrackerLib::reservedFooterHeight(2) };
		ImGui::BeginChild("EntityManagementScroll", ImVec2{ 0.0f, -footerHeight });

		// Top UI elements, which are laid out as follow:
		// Left: General Creature info + speed data.
		// Right: Player character or monster specific info.
		if (ImGui::BeginTable("TopEntitySplit", 2, ImGuiTableFlags_SizingStretchProp)) {
			ImGui::TableSetupColumn("Left", ImGuiTableColumnFlags_WidthStretch, 0.75f);
			ImGui::TableSetupColumn("Right", ImGuiTableColumnFlags_WidthStretch, 0.25f);
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::SeparatorText(m_localeProviderRef.getText("/scenes/entityCreation/sections/general").data());
			renderGeneralCreatureInfo();

			ImGui::Spacing();
			ImGui::SeparatorText(m_localeProviderRef.getText("/scenes/entityCreation/sections/movement").data());
			renderSpeedInfo();

			ImGui::TableSetColumnIndex(1);
			renderEntitySpecificInfo();
			ImGui::EndTable();
		}

		ImGui::Spacing();
		ImGui::SeparatorText(m_localeProviderRef.getText("/scenes/entityCreation/sections/abilities").data());
		renderAbilityScoresAndModifiers();

		ImGui::Spacing();
		ImGui::SeparatorText(m_localeProviderRef.getText("/scenes/entityCreation/sections/savingThrowsAndSkills").data());
		if (ImGui::BeginTable("##SavingThrowSkillsSplit", 2, ImGuiTableFlags_SizingStretchProp)) {
			ImGui::TableSetupColumn("Left", ImGuiTableColumnFlags_WidthStretch, 0.5f);
			ImGui::TableSetupColumn("Right", ImGuiTableColumnFlags_WidthStretch, 0.5f);
			ImGui::TableNextRow();

			// Compute a sensible dynamic height for the children so both columns match.
			const auto childHeight{ std::max(MIN_CHILD_HEIGHT, viewport->WorkSize.y * 0.28f) };

			// Saving throw data as a separate collapsable header.
			ImGui::TableSetColumnIndex(0);
			if (ImGui::CollapsingHeader(
				m_localeProviderRef.getText("/scenes/entityCreation/savingThrows/header").data(),
				ImGuiTreeNodeFlags_DefaultOpen
			)) {

				// Saving Throw proficiency and expertise.
				ImGui::BeginChild("##SaveProfandExp", ImVec2{ 0.0f, childHeight }, true);
				renderProficiencyAndExpertiseCluster(
					m_localeProviderRef.getText("/scenes/entityCreation/savingThrows/proficiencies"),
					m_localeProviderRef.getText("/scenes/entityCreation/savingThrows/expertises"),
					m_currentCreatureStatBlock.savingThrowProficiencies,
					m_currentCreatureStatBlock.savingThrowExpertise,
					m_localeProviderRef.getTextList("/domain/ability")
				);
				ImGui::EndChild();

				// Saving Throw Modifiers.
				ImGui::BeginChild("##SaveMods", ImVec2{ 0.0f, childHeight }, true);
				renderSavingThrowModifiers();
				ImGui::EndChild();
			}

			// Skill data as a seperate collapsable header.
			ImGui::TableSetColumnIndex(1);
			if (ImGui::CollapsingHeader(
				m_localeProviderRef.getText("/scenes/entityCreation/skills/header").data(),
				ImGuiTreeNodeFlags_DefaultOpen
			)) {

				// Skill proficiency and expertise.
				ImGui::BeginChild("##SkillProfandExp", ImVec2{ 0.0f, childHeight }, true);
				renderProficiencyAndExpertiseCluster(
					m_localeProviderRef.getText("/scenes/entityCreation/skills/proficiencies"),
					m_localeProviderRef.getText("/scenes/entityCreation/skills/expertises"),
					m_currentCreatureStatBlock.skillProficiencies,
					m_currentCreatureStatBlock.skillExpertise,
					m_localeProviderRef.getTextList("/domain/skill")
				);
				ImGui::EndChild();

				// Skill modifiers.
				ImGui::BeginChild("##SkillMods", ImVec2{ 0.0f, childHeight }, true);
				renderSkillModifiers();
				ImGui::EndChild();
			}
			ImGui::EndTable();
		}

		ImGui::Spacing();
		ImGui::SeparatorText(m_localeProviderRef.getText("/scenes/entityCreation/sections/damageAndConditions").data());
		renderDmgVulnResAndImm();
		CombatTrackerLib::renderCheckboxGrid(
			m_currentCreatureStatBlock.conditionImmunities,
			m_localeProviderRef.getTextList("/domain/condition"),
			"ConditionImmunities",
			m_localeProviderRef.getText("/scenes/entityCreation/damage/conditionImmunities"),
			3
		);

		ImGui::Spacing();
		ImGui::SeparatorText(m_localeProviderRef.getText("/scenes/entityCreation/sections/sensesAndLanguages").data());
		CombatTrackerLib::renderCheckboxGrid(
			m_currentCreatureStatBlock.languages,
			m_localeProviderRef.getTextList("/domain/creatureLanguage"),
			"Languages",
			m_localeProviderRef.getText("/scenes/entityCreation/languages"),
			3
		);
		renderSenses();

		ImGui::Spacing();
		renderEntitySpecificSection(); 	// Some parts of the player character specific info goes here.
		ImGui::Spacing();
		ImGui::SeparatorText(m_localeProviderRef.getText("/scenes/entityCreation/sections/actionsTraitsEquipment").data());
		renderActions();
		renderAddables(m_currentCreatureStatBlock.traits, AddableLabels{
			m_localeProviderRef.getText("/scenes/entityCreation/traits/header"),
			m_localeProviderRef.getText("/scenes/entityCreation/traits/add"),
			m_localeProviderRef.getText("/scenes/entityCreation/traits/edit"),
			m_localeProviderRef.getText("/scenes/entityCreation/traits/name"),
			m_localeProviderRef.getText("/scenes/entityCreation/traits/description"),
			m_localeProviderRef.getText("/scenes/entityCreation/traits/finishCreation"),
			m_localeProviderRef.getText("/scenes/entityCreation/traits/finishEditing"),
			m_localeProviderRef.getText("/scenes/entityCreation/traits/delete")
		});
		renderAddables(m_currentCreatureStatBlock.equipment, AddableLabels{
			m_localeProviderRef.getText("/scenes/entityCreation/equipment/header"),
			m_localeProviderRef.getText("/scenes/entityCreation/equipment/add"),
			m_localeProviderRef.getText("/scenes/entityCreation/equipment/edit"),
			m_localeProviderRef.getText("/scenes/entityCreation/equipment/name"),
			m_localeProviderRef.getText("/scenes/entityCreation/equipment/description"),
			m_localeProviderRef.getText("/scenes/entityCreation/equipment/finishCreation"),
			m_localeProviderRef.getText("/scenes/entityCreation/equipment/finishEditing"),
			m_localeProviderRef.getText("/scenes/entityCreation/equipment/delete")
		});
		ImGui::EndChild();

		ImGui::Separator();
		if (CombatTrackerLib::alignedButton(creationLabels.completeButton.data(), 0.5f)) {
			if (completeEntityCreation()) {
				m_applicationPointer->queueSceneTransition(this, std::make_unique<EncounterCreationScene>(m_applicationPointer));
			}
		}
		if (CombatTrackerLib::alignedButton(creationLabels.cancelButton.data(), 0.5f)) {
			m_applicationPointer->queueSceneTransition(this, std::make_unique<EncounterCreationScene>(m_applicationPointer));
		}
		ImGui::End();
	}

	void EntityCreationScene::renderAbilityScoresAndModifiers () {

		const auto abilityNames{ m_localeProviderRef.getTextList("/domain/ability") };
		const auto resetLabels{ m_localeProviderRef.getTextList("/scenes/entityCreation/abilityTable/resetModifiers") };

		if (ImGui::BeginTable("AbilityTable",
			4,
			ImGuiTableFlags_SizingStretchProp |
			ImGuiTableFlags_BordersInnerV |
			ImGuiTableFlags_RowBg
		)) {

			/*
			The table is split into four parts:
			1) Ability name.
			2) Ability score.
			3) Ability modifier.
			4) Override button.
			*/
			ImGui::TableSetupColumn(
				m_localeProviderRef.getText("/scenes/entityCreation/abilityTable/abilityColumn").data(),
				ImGuiTableColumnFlags_WidthStretch, 
				0.25f
			);
			ImGui::TableSetupColumn(
				m_localeProviderRef.getText("/scenes/entityCreation/abilityTable/scoreColumn").data(),
				ImGuiTableColumnFlags_WidthStretch, 
				0.20f
			);
			ImGui::TableSetupColumn(
				m_localeProviderRef.getText("/scenes/entityCreation/abilityTable/modifierColumn").data(),
				ImGuiTableColumnFlags_WidthStretch,
				0.20f
			);
			ImGui::TableSetupColumn(
				m_localeProviderRef.getText("/scenes/entityCreation/abilityTable/overrideColumn").data(),
				ImGuiTableColumnFlags_WidthStretch, 
				0.35f
			);

			// Render each column of the table for each ability.
			// Every ability keyed array is the same length, so one index walks the names, the scores and the modifiers together.
			for (size_t i{}; i < abilityNames.size(); ++i) {
				const auto abilityName{ abilityNames[i] };
				ImGui::PushID(static_cast<int>(i));
				ImGui::TableNextRow();

				// Compute and show default ability score modifiers.
				if (!m_currentCreatureStatBlock.abilityScoreModifiers[i].isOverridden) {
					m_currentCreatureStatBlock.abilityScoreModifiers[i].value =
						Creature::findAbilityScoreModifier(m_currentCreatureStatBlock.abilityScores[i]);
				}

				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(abilityName.data());

				// General special value reserved for stretching items.
				ImGui::SetNextItemWidth(-1);
				ImGui::TableSetColumnIndex(1);
				if (ImGui::InputInt("##AbilityScore",
					&m_currentCreatureStatBlock.abilityScores[i])
					) {
					m_currentCreatureStatBlock.abilityScores[i] =
						std::max(std::min(m_currentCreatureStatBlock.abilityScores[i],
							ABILITY_SCORE_UPPER_BOUND), ABILITY_SCORE_LOWER_BOUND);
				}

				ImGui::TableSetColumnIndex(2);
				ImGui::SetNextItemWidth(-1);
				if (ImGui::InputInt("##AbilityModifier",
					&m_currentCreatureStatBlock.abilityScoreModifiers[i].value)
					) {
					m_currentCreatureStatBlock.abilityScoreModifiers[i].value =
						std::max(std::min(m_currentCreatureStatBlock.abilityScoreModifiers[i].value,
							ABILITY_MODIFIERS_UPPER_BOUND), ABILITY_MODIFIERS_LOWER_BOUND);
					m_currentCreatureStatBlock.abilityScoreModifiers[i].isOverridden = true;
				}

				// The reset buttons fill their column instead of auto sizing to their label, which would overflow the cell.
				ImGui::TableSetColumnIndex(3);
				if (ImGui::Button(
					resetLabels[i].data(),
					ImVec2{ -1.0f, 0.0f }
				)) {
					m_currentCreatureStatBlock.abilityScoreModifiers[i].isOverridden = false;
				}
				ImGui::PopID();
			}
			ImGui::EndTable();
		}
	}
	void EntityCreationScene::renderSavingThrowModifiers () {

		const auto abilityNames{ m_localeProviderRef.getTextList("/domain/ability") };
		const auto resetLabels{ m_localeProviderRef.getTextList("/scenes/entityCreation/savingThrows/resetModifiers") };

		ImGui::TextWrapped("%s", m_localeProviderRef.getText("/scenes/entityCreation/savingThrows/modifiers").data());
		if (ImGui::BeginTable("SaveModsTable",
			3,
			ImGuiTableFlags_SizingStretchProp |
			ImGuiTableFlags_RowBg |
			ImGuiTableFlags_BordersInnerV))
		{
			// This table is split into only three parts.
			// The modifiers of saving throws are computed from ability scores/modifiers if not overridden.
			ImGui::TableSetupColumn("Ability##Save", ImGuiTableColumnFlags_WidthStretch, 0.35f);
			ImGui::TableSetupColumn("Modifier##Save", ImGuiTableColumnFlags_WidthStretch, 0.25f);
			ImGui::TableSetupColumn("Reset##Save", ImGuiTableColumnFlags_WidthStretch, 0.40f);

			// Saving throws are keyed by ability as well, so this walks the same arrays by index that the ability table does.
			for (size_t i{}; i < abilityNames.size(); ++i) {
				const auto abilityName{ abilityNames[i] };
				ImGui::PushID(static_cast<int>(i));

				// Saving throw modifier calculations.
				if (!m_currentCreatureStatBlock.savingThrowModifiers[i].isOverridden) {
					if (m_currentCreatureStatBlock.savingThrowExpertise[i]) {
						m_currentCreatureStatBlock.savingThrowModifiers[i].value =
							m_currentCreatureStatBlock.abilityScoreModifiers[i].value +
							2 * m_currentCreatureStatBlock.proficiencyBonus.value;
					}
					else if (m_currentCreatureStatBlock.savingThrowProficiencies[i]) {
						m_currentCreatureStatBlock.savingThrowModifiers[i].value =
							m_currentCreatureStatBlock.abilityScoreModifiers[i].value +
							m_currentCreatureStatBlock.proficiencyBonus.value;
					}
					else {
						m_currentCreatureStatBlock.savingThrowModifiers[i].value =
							m_currentCreatureStatBlock.abilityScoreModifiers[i].value;
					}
				}
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(abilityName.data());

				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1);
				if (ImGui::InputInt("##SavingThrowModifier",
					&m_currentCreatureStatBlock.savingThrowModifiers[i].value
				)) {
					m_currentCreatureStatBlock.savingThrowModifiers[i].isOverridden = true;
				}

				ImGui::TableSetColumnIndex(2);
				if (ImGui::Button(
					resetLabels[i].data(),
					ImVec2{ -1.0f, 0.0f }
				)) {
					m_currentCreatureStatBlock.savingThrowModifiers[i].isOverridden = false;
				}
				ImGui::PopID();
			}
			ImGui::EndTable();
		}
	}
	void EntityCreationScene::renderSkillModifiers () {

		const auto skillNames{ m_localeProviderRef.getTextList("/domain/skill") };
		const auto resetLabels{ m_localeProviderRef.getTextList("/scenes/entityCreation/skills/resetModifiers") };

		ImGui::TextWrapped("%s", m_localeProviderRef.getText("/scenes/entityCreation/skills/modifiers").data());
		if (ImGui::BeginTable("SkillModsTable",
			3,
			ImGuiTableFlags_SizingStretchProp |
			ImGuiTableFlags_RowBg |
			ImGuiTableFlags_BordersInnerV
		)) {

			// Exact same setup as saving throws.
			ImGui::TableSetupColumn(
				m_localeProviderRef.getText("/scenes/entityCreation/skills/skillColumn").data(),
				ImGuiTableColumnFlags_WidthStretch,
				0.45f
			);
			ImGui::TableSetupColumn(
				m_localeProviderRef.getText("/scenes/entityCreation/skills/valueColumn").data(),
				ImGuiTableColumnFlags_WidthStretch,
				0.20f
			);
			ImGui::TableSetupColumn(
				m_localeProviderRef.getText("/scenes/entityCreation/skills/resetColumn").data(),
				ImGuiTableColumnFlags_WidthStretch, 
				0.35f
			);

			// Default skill modifier calculations.
			for (size_t i{}; i < skillNames.size(); ++i) {
				const auto skillName{ skillNames[i] };
				ImGui::PushID(static_cast<int>(i));

				const auto correspondingAbility{ MAPPED_SKILLS.at(static_cast<Skill>(i)) };

				if (!m_currentCreatureStatBlock.skillModifiers[i].isOverridden) {
					if (m_currentCreatureStatBlock.skillExpertise[i]) {
						m_currentCreatureStatBlock.skillModifiers[i].value =
							m_currentCreatureStatBlock.abilityScoreModifiers[CombatTrackerLib::enumToSizeT(correspondingAbility)].value +
							2 * m_currentCreatureStatBlock.proficiencyBonus.value;
					}
					else if (m_currentCreatureStatBlock.skillProficiencies[i]) {
						m_currentCreatureStatBlock.skillModifiers[i].value =
							m_currentCreatureStatBlock.abilityScoreModifiers[CombatTrackerLib::enumToSizeT(correspondingAbility)].value
							+ m_currentCreatureStatBlock.proficiencyBonus.value;
					}
					else {
						m_currentCreatureStatBlock.skillModifiers[i].value =
							m_currentCreatureStatBlock.abilityScoreModifiers[CombatTrackerLib::enumToSizeT(correspondingAbility)].value;
					}
				}

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(skillName.data());

				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1);
				if (ImGui::InputInt("##SkillModifier",
					&m_currentCreatureStatBlock.skillModifiers[i].value
				)) {
					m_currentCreatureStatBlock.skillModifiers[i].isOverridden = true;
				}

				ImGui::TableSetColumnIndex(2);
				if (ImGui::Button(
					resetLabels[i].data(),
					ImVec2{ -1.0f, 0.0f }
				)) {
					m_currentCreatureStatBlock.skillModifiers[i].isOverridden = false;
				}
				ImGui::PopID();
			}
			ImGui::EndTable();
		}
	}

	/*
	The general creature info includes these parameters:
	1) Name
	2) Size
	3) Type
	4) Alignment
	5) Initiative Bonus
	6) Passive Perception
	7) AC
	8) HP
	*/
	void EntityCreationScene::renderGeneralCreatureInfo() {

		const auto nameLabel{ m_localeProviderRef.getText("/scenes/entityCreation/general/name") };
		const auto armorClassLabel{ m_localeProviderRef.getText("/scenes/entityCreation/general/armorClass") };
		const auto hitPointsLabel{ m_localeProviderRef.getText("/scenes/entityCreation/general/hitPoints") };

		if (ImGui::BeginTable("##GeneralCreatureInfo_NameRow", 2, ImGuiTableFlags_SizingStretchProp)) {

			// The label column is measured from its own text so it follows the font instead of a magic pixel width.
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize(nameLabel.data()).x + ImGui::GetStyle().ItemSpacing.x * 2.0f);
			ImGui::TableSetupColumn("Field", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted(nameLabel.data());

			ImGui::TableSetColumnIndex(1);
			ImGui::SetNextItemWidth(-1.0f);
			ImGui::InputText("##Name", &m_currentCreatureStatBlock.name);

			ImGui::EndTable();
		}

		ImGui::Spacing();

		if (ImGui::BeginTable("##GeneralCreatureInfo_MainSplit", 2, ImGuiTableFlags_SizingStretchProp)) {

			// Give right side a reasonable fixed width so it never expands into the Entity Specific column.
			const auto avail{ ImGui::GetContentRegionAvail().x };
			const auto rightW{ std::clamp(avail * 0.30f, std::min(240.0f, avail * 0.45f), 300.0f) };

			ImGui::TableSetupColumn("Left", ImGuiTableColumnFlags_WidthStretch, 1.0f);
			ImGui::TableSetupColumn("Right", ImGuiTableColumnFlags_WidthFixed, rightW);

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			if (ImGui::BeginTable("##GeneralCreatureInfo_Listboxes", 3, ImGuiTableFlags_SizingStretchProp)) {
				ImGui::TableSetupColumn("SizeCol", ImGuiTableColumnFlags_WidthStretch, 0.33f);
				ImGui::TableSetupColumn("TypeCol", ImGuiTableColumnFlags_WidthStretch, 0.34f);
				ImGui::TableSetupColumn("AlignmentCol", ImGuiTableColumnFlags_WidthStretch, 0.33f);

				ImGui::TableNextRow();

				// The labels are hidden on the listboxes themselves, so each one can take its whole column.
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted(m_localeProviderRef.getText("/scenes/entityCreation/general/size").data());
				ImGui::SetNextItemWidth(-1.0f);
				renderListBox(
					m_sizeListBoxParameters,
					m_currentCreatureStatBlock.size,
					"##Size", 
					m_localeProviderRef.getTextList("/domain/size")
				);

				ImGui::TableSetColumnIndex(1);
				ImGui::TextUnformatted(m_localeProviderRef.getText("/scenes/entityCreation/general/creatureType").data());
				ImGui::SetNextItemWidth(-1.0f);
				renderListBox(
					m_creatureTypeParameters,
					m_currentCreatureStatBlock.type,
					"##CreatureType",
					m_localeProviderRef.getTextList("/domain/creatureType")
				);

				ImGui::TableSetColumnIndex(2);
				ImGui::TextUnformatted(m_localeProviderRef.getText("/scenes/entityCreation/general/alignment").data());
				ImGui::SetNextItemWidth(-1.0f);
				renderListBox(
					m_alignmentParameters,
					m_currentCreatureStatBlock.alignment,
					"##Alignment", 
					m_localeProviderRef.getTextList("/domain/alignment")
				);

				ImGui::EndTable();
			}

			ImGui::TableSetColumnIndex(1);

			if (!m_currentCreatureStatBlock.initiativeBonus.isOverridden) {
				m_currentCreatureStatBlock.initiativeBonus.value =
					m_currentCreatureStatBlock.abilityScoreModifiers[CombatTrackerLib::enumToSizeT(Ability::DEXTERITY)].value;
			}

			// Hide the label of the input field. Instead use text field for the label for better layout.
			ImGui::TextUnformatted(m_localeProviderRef.getText("/scenes/entityCreation/general/initiativeBonus").data());
			ImGui::SetNextItemWidth(-1.0f);
			if (ImGui::InputInt("##InitiativeBonus", &m_currentCreatureStatBlock.initiativeBonus.value, 0, 0)) {
				m_currentCreatureStatBlock.initiativeBonus.value =
					std::min(std::max(ABILITY_MODIFIERS_LOWER_BOUND, m_currentCreatureStatBlock.initiativeBonus.value),
						ABILITY_MODIFIERS_UPPER_BOUND);
				m_currentCreatureStatBlock.initiativeBonus.isOverridden = true;
			}
			if (ImGui::Button(
				m_localeProviderRef.getText("/scenes/entityCreation/general/resetInitiativeBonus").data(),
				ImVec2{ -1.0f, 0.0f }
			)) {
				m_currentCreatureStatBlock.initiativeBonus.isOverridden = false;
			}

			ImGui::Spacing();
			if (!m_currentCreatureStatBlock.passivePerception.isOverridden) {
				m_currentCreatureStatBlock.passivePerception.value = 
					m_currentCreatureStatBlock.skillModifiers[CombatTrackerLib::enumToSizeT(Skill::PERCEPTION)].value + 10;
			}

			// Same thing here.
			ImGui::TextUnformatted(m_localeProviderRef.getText("/scenes/entityCreation/general/passivePerception").data());
			ImGui::SetNextItemWidth(-1.0f);
			if (ImGui::InputInt("##PassivePerception", &m_currentCreatureStatBlock.passivePerception.value, 0, 0)) {
				m_currentCreatureStatBlock.passivePerception.value =
					std::min(std::max(10 + ABILITY_MODIFIERS_LOWER_BOUND, m_currentCreatureStatBlock.passivePerception.value),
						10 + ABILITY_MODIFIERS_UPPER_BOUND);
				m_currentCreatureStatBlock.passivePerception.isOverridden = true;
			}
			if (ImGui::Button(
				m_localeProviderRef.getText("/scenes/entityCreation/general/resetPassivePerception").data(),
				ImVec2{ -1.0f, 0.0f }
			)) {
				m_currentCreatureStatBlock.passivePerception.isOverridden = false;
			}
			ImGui::EndTable();
		}

		ImGui::Spacing();
		if (ImGui::BeginTable("##ACHP", 2, ImGuiTableFlags_SizingStretchProp)) {
			ImGui::TableSetupColumn("ACCol", ImGuiTableColumnFlags_WidthStretch, 0.5f);
			ImGui::TableSetupColumn("HPCol", ImGuiTableColumnFlags_WidthStretch, 0.5f);
			ImGui::TableNextRow();

			// AC inner table.
			ImGui::TableSetColumnIndex(0);
			if (ImGui::BeginTable("##ACRow", 2, ImGuiTableFlags_SizingStretchProp)) {
				ImGui::TableSetupColumn(
					"ACLabel", 
					ImGuiTableColumnFlags_WidthFixed, 
					ImGui::CalcTextSize(armorClassLabel.data()).x + ImGui::GetStyle().ItemSpacing.x * 2.0f
				);
				ImGui::TableSetupColumn("ACField", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(armorClassLabel.data());

				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1.0f);
				CombatTrackerLib::renderNonOverridableInputInt(m_currentCreatureStatBlock.AC, "##AC", 0);

				ImGui::EndTable();
			}

			// HP inner cell.
			ImGui::TableSetColumnIndex(1);
			if (ImGui::BeginTable("##HPRow", 2, ImGuiTableFlags_SizingStretchProp)) {
				ImGui::TableSetupColumn(
					"HPLabel", 
					ImGuiTableColumnFlags_WidthFixed, 
					ImGui::CalcTextSize(hitPointsLabel.data()).x + ImGui::GetStyle().ItemSpacing.x * 2.0f
				);
				ImGui::TableSetupColumn("HPField", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(hitPointsLabel.data());

				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1.0f);
				CombatTrackerLib::renderNonOverridableInputInt(m_currentCreatureStatBlock.HP, "##HP", 0);

				ImGui::EndTable();
			}
			ImGui::EndTable();
		}
	}
	void EntityCreationScene::renderSpeedInfo () {

		if (ImGui::BeginTable("SpeedGrid", 5, ImGuiTableFlags_SizingStretchSame)) {
			ImGui::TableNextRow();

			// An inline label plus the stepper buttons does not fit into a fifth of the column, so the labels go above the fields here too.
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted(m_localeProviderRef.getText("/scenes/entityCreation/speeds/walk").data());
			ImGui::SetNextItemWidth(-1.0f);
			CombatTrackerLib::renderNonOverridableInputInt(m_currentCreatureStatBlock.walkSpeed, "##Walk", 0);

			ImGui::TableSetColumnIndex(1);
			ImGui::TextUnformatted(m_localeProviderRef.getText("/scenes/entityCreation/speeds/climb").data());
			ImGui::SetNextItemWidth(-1.0f);
			CombatTrackerLib::renderNonOverridableInputInt(m_currentCreatureStatBlock.climbingSpeed, "##Climb", 0);

			ImGui::TableSetColumnIndex(2);
			ImGui::TextUnformatted(m_localeProviderRef.getText("/scenes/entityCreation/speeds/swim").data());
			ImGui::SetNextItemWidth(-1.0f);
			CombatTrackerLib::renderNonOverridableInputInt(m_currentCreatureStatBlock.swimmingSpeed, "##Swim", 0);

			ImGui::TableSetColumnIndex(3);
			ImGui::TextUnformatted(m_localeProviderRef.getText("/scenes/entityCreation/speeds/burrow").data());
			ImGui::SetNextItemWidth(-1.0f);
			CombatTrackerLib::renderNonOverridableInputInt(m_currentCreatureStatBlock.burrowSpeed, "##Burrow", 0);

			ImGui::TableSetColumnIndex(4);
			ImGui::TextUnformatted(m_localeProviderRef.getText("/scenes/entityCreation/speeds/fly").data());
			ImGui::SetNextItemWidth(-1.0f);
			CombatTrackerLib::renderNonOverridableInputInt(m_currentCreatureStatBlock.flySpeed, "##Fly", 0);

			ImGui::EndTable();
		}
	}
	void EntityCreationScene::renderDmgVulnResAndImm () {

		const auto damageTypeNames{ m_localeProviderRef.getTextList("/domain/damageType") };

		if (ImGui::BeginTable("DmgTri", 3, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV)) {
			ImGui::TableNextRow();

			// The three flag arrays and the name table all line up, so one index covers a damage type across all four.
			ImGui::TableSetColumnIndex(0);
			ImGui::TextWrapped("%s", m_localeProviderRef.getText("/scenes/entityCreation/damage/vulnerabilities").data());
			ImGui::PushID("Vulnerabilities");
			for (size_t i{}; i < damageTypeNames.size(); ++i) {
				if (ImGui::Checkbox(damageTypeNames[i].data(),
					&m_currentCreatureStatBlock.damageVulnerabilities[i]
				)) {

					// Triple simple deduping logic.
					if (m_currentCreatureStatBlock.damageVulnerabilities[i]
						&& m_currentCreatureStatBlock.damageResistances[i]) {
						m_currentCreatureStatBlock.damageResistances[i] = false;
					}
					if (m_currentCreatureStatBlock.damageVulnerabilities[i]
						&& m_currentCreatureStatBlock.damageImmunities[i]) {
						m_currentCreatureStatBlock.damageImmunities[i] = false;
					}
				}
			}
			ImGui::PopID();

			ImGui::TableSetColumnIndex(1);
			ImGui::TextWrapped("%s", m_localeProviderRef.getText("/scenes/entityCreation/damage/resistances").data());
			ImGui::PushID("Resistances");
			for (size_t i{}; i < damageTypeNames.size(); ++i) {
				if (ImGui::Checkbox(damageTypeNames[i].data(),
					&m_currentCreatureStatBlock.damageResistances[i]
				)) {

					// Exact same logic here.
					if (m_currentCreatureStatBlock.damageVulnerabilities[i]
						&& m_currentCreatureStatBlock.damageResistances[i]) {
						m_currentCreatureStatBlock.damageVulnerabilities[i] = false;
					}
					if (m_currentCreatureStatBlock.damageResistances[i]
						&& m_currentCreatureStatBlock.damageImmunities[i]) {
						m_currentCreatureStatBlock.damageImmunities[i] = false;
					}
				}
			}
			ImGui::PopID();

			ImGui::TableSetColumnIndex(2);
			ImGui::TextWrapped("%s", m_localeProviderRef.getText("/scenes/entityCreation/damage/immunities").data());
			ImGui::PushID("Immunities");
			for (size_t i{}; i < damageTypeNames.size(); ++i) {
				if (ImGui::Checkbox(damageTypeNames[i].data(),
					&m_currentCreatureStatBlock.damageImmunities[i]
				)) {

					// Same here.
					if (m_currentCreatureStatBlock.damageVulnerabilities[i]
						&& m_currentCreatureStatBlock.damageImmunities[i]) {
						m_currentCreatureStatBlock.damageVulnerabilities[i] = false;
					}
					if (m_currentCreatureStatBlock.damageResistances[i]
						&& m_currentCreatureStatBlock.damageImmunities[i]) {
						m_currentCreatureStatBlock.damageResistances[i] = false;
					}
				}
			}
			ImGui::PopID();
			ImGui::EndTable();
		}
	}

	// Specific function for sense rendering, just calls CombatTrackerLib::renderNonOverridableInputInt() for all senses.
	void EntityCreationScene::renderSenses() {
		const auto senseNames{ m_localeProviderRef.getTextList("/domain/sense") };
		if (ImGui::BeginTable("SensesGrid", 4, ImGuiTableFlags_SizingStretchSame)) {
			ImGui::TableNextRow();
			for (size_t i{}; i < senseNames.size(); ++i) {
				const auto senseName{ senseNames[i] };
				ImGui::PushID(static_cast<int>(i));
				ImGui::TableNextColumn();

				// Same layout as the speed grid, the label sits above its field.
				ImGui::TextUnformatted(senseName.data());
				ImGui::SetNextItemWidth(-1.0f);
				CombatTrackerLib::renderNonOverridableInputInt(m_currentCreatureStatBlock.senses[i], "##Sense", 0);
				ImGui::PopID();
			}
			ImGui::EndTable();
		}
	}

	// Really similar to renderAddables() just adds actionType as a separate parameter.
	void EntityCreationScene::renderActions() {

		const auto actionNames{ m_localeProviderRef.getTextList("/domain/actionType") };

		ImGui::TextWrapped("%s", m_localeProviderRef.getText("/scenes/entityCreation/actions/header").data());
		static std::vector<size_t> s_actionsMarkedForDeletion{};
		for (size_t i{}; i < m_currentCreatureStatBlock.actions.size(); ++i) {
			ImGui::PushID(static_cast<int> (i));

			const auto editPopupName{ m_localeProviderRef.getText("/scenes/entityCreation/actions/edit") };

			ImGui::Button(m_currentCreatureStatBlock.actions[i].name.c_str(), ImVec2{ -1.0f, 0.0f });
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				ImGui::SetTooltip(
					"%s: %s",
					actionNames[CombatTrackerLib::enumToSizeT(m_currentCreatureStatBlock.actions[i].type)].data(),
					m_currentCreatureStatBlock.actions[i].description.c_str()
				);
			}
			if (ImGui::IsItemClicked()) {
				ImGui::OpenPopup(editPopupName.data());
			}

			// The popup can neither be moved nor resized by hand, so it simply keeps following the viewport.
			const auto center{ ImGui::GetMainViewport()->GetCenter() };
			ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2{ 0.5f, 0.5f });
			const auto vp{ ImGui::GetMainViewport()->Size };
			ImGui::SetNextWindowSize(ImVec2{ vp.x * 0.70f, vp.y * 0.70f }, ImGuiCond_Always);
			if (ImGui::BeginPopupModal(editPopupName.data(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
				ImGui::BeginChild("EditActionScroll", ImVec2{ 0.0f, -CombatTrackerLib::reservedFooterHeight(2) }, true);
				ImGui::InputText(
					m_localeProviderRef.getText("/scenes/entityCreation/actions/name").data(),
					&m_currentCreatureStatBlock.actions[i].name
				);
				ImGui::InputText(
					m_localeProviderRef.getText("/scenes/entityCreation/actions/description").data(),
					&m_currentCreatureStatBlock.actions[i].description
				);
				ImGui::EndChild();

				ImGui::Separator();
				if (CombatTrackerLib::alignedButton(m_localeProviderRef.getText("/scenes/entityCreation/actions/finishEditing").data(), 0.5f)) {
					ImGui::CloseCurrentPopup();
				}
				if (CombatTrackerLib::alignedButton(m_localeProviderRef.getText("/scenes/entityCreation/actions/delete").data(), 0.5f)) {
					s_actionsMarkedForDeletion.push_back(i);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}

		std::sort(s_actionsMarkedForDeletion.begin(), s_actionsMarkedForDeletion.end());
		for (size_t i{ s_actionsMarkedForDeletion.size() }; i-- > 0;) {
			m_currentCreatureStatBlock.actions.erase(m_currentCreatureStatBlock.actions.begin() + static_cast<std::ptrdiff_t>(s_actionsMarkedForDeletion[i]));
		}
		s_actionsMarkedForDeletion.clear();

		// Action adding popup modal.
		ImGui::Spacing();
		if (ImGui::Button(m_localeProviderRef.getText("/scenes/entityCreation/actions/add").data())) {
			ImGui::OpenPopup("###ActionAdd");
		}
		const auto center{ ImGui::GetMainViewport()->GetCenter() };
		ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2{ 0.5f, 0.5f });
		const auto vp{ ImGui::GetMainViewport()->Size };
		ImGui::SetNextWindowSize(ImVec2(vp.x * 0.70f, vp.y * 0.70f), ImGuiCond_Always);
		if (ImGui::BeginPopupModal(
			m_localeProviderRef.getText("/scenes/entityCreation/actions/add").data(),
			nullptr,
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
		)) {
			ImGui::BeginChild("##AddActionScroll", ImVec2{ 0.0f, -CombatTrackerLib::reservedFooterHeight(1) }, true);

			// Again, fine for these to be static as they are reset after each addition of actions.
			static CombatTrackerLib::ListBoxParameters s_actionParameters{ 
				CombatTrackerLib::enumToSizeT(ActionType::COUNT),
				CombatTrackerLib::enumToSizeT(ActionType::COUNT), 
				false 
			};
			static ActionType s_type{};

			CombatTrackerLib::renderListBox(
				s_actionParameters,
				s_type,
				m_localeProviderRef.getText("/scenes/entityCreation/actions/type").data(),
				actionNames
			);

			static std::string s_name{};
			static std::string s_description{};
			ImGui::InputText(m_localeProviderRef.getText("/scenes/entityCreation/actions/name").data(), &s_name);
			ImGui::InputText(m_localeProviderRef.getText("/scenes/entityCreation/actions/description").data(), &s_description);
			ImGui::EndChild();

			ImGui::Separator();

			if (CombatTrackerLib::alignedButton(m_localeProviderRef.getText("/scenes/entityCreation/actions/finishCreation").data(), 0.5f) &&
				!s_name.empty() && !s_description.empty() && s_actionParameters.chosenIndex < CombatTrackerLib::enumToSizeT(ActionType::COUNT)) {
				CreatureAction action{ s_name, s_description, s_type };
				m_currentCreatureStatBlock.actions.push_back(action);
				s_actionParameters = { 
					CombatTrackerLib::enumToSizeT(ActionType::COUNT), 
					CombatTrackerLib::enumToSizeT(ActionType::COUNT), 
					false 
				};
				s_type = {};
				s_name.clear();
				s_description.clear();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	void EntityCreationScene::resetParametersAndOverrides() {
		m_currentCreatureStatBlock = CreatureStatBlock{};
		m_sizeListBoxParameters = {
			CombatTrackerLib::enumToSizeT(Size::COUNT),
			CombatTrackerLib::enumToSizeT(Size::COUNT),
			false
		};
		m_creatureTypeParameters = {
			CombatTrackerLib::enumToSizeT(CreatureType::COUNT),
			CombatTrackerLib::enumToSizeT(CreatureType::COUNT),
			false
		};
		m_alignmentParameters = {
			CombatTrackerLib::enumToSizeT(Alignment::COUNT),
			CombatTrackerLib::enumToSizeT(Alignment::COUNT),
			false
		};
	}
}
