#include "EntityCreationScene.h"
#include "EncounterCreationScene.h"
#include "../Entities/Creature/CreatureConstants.h"
#include "../CombatTrackerLib/Utilities/UIUtils.h"

namespace CombatTrackerApp {
	EntityCreationScene::EntityCreationScene(CombatTrackerLib::App* app) :
		CombatTrackerLib::Scene{ app },
		m_currentCreatureStatBlock{},
		m_sizeListBoxParameters{ -1, -1, false },
		m_creatureTypeParameters{ -1, -1, false },
		m_alignmentParameters{ -1, -1, false },
		m_isInitiativeOverridden{ false },
		m_isPassivePerceptionOverridden{ false }
	{
		for (const auto& ability : ABILITY_NAMES) {
			m_abilityModifiersOverride[ability.first] = false;
			m_savingThrowsOverride[ability.first] = false;
		}
		for (const auto& skill : SKILL_NAMES) {
			m_skillModifiersOverride[skill.first] = false;
		}
	}

	void EntityCreationScene::notifyScene(const ImGuiIO& imguiIO) {} // No non-ImGui based event handling should exist in this scene, I think.
	void EntityCreationScene::updateScene(double timeInterval) {} // Purely input based scene so no updates necessary.
	void EntityCreationScene::renderScene() {} // The scene consists of just UI widgets so no OpenGL side rendering work.

	void EntityCreationScene::renderUI() {

		// Take over the whole viewport, the same way the other scene hosts do.
		m_mainViewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(m_mainViewport->WorkPos);
		ImGui::SetNextWindowSize(m_mainViewport->WorkSize);
		ImGui::SetNextWindowViewport(m_mainViewport->ID);

		ImGui::Begin((getEntityLabel() + " Creation").c_str(), nullptr, ENTITY_CREATION_WINDOW_FLAGS);

		// TODO: Properly setup initial values of the entity that is being created/edited.
		if (ImGui::IsWindowAppearing()) {
			resetEntity();
		}

		ImGui::SeparatorText((getEntityLabel() + " Creation").c_str());

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
			ImGui::SeparatorText("General");
			renderGeneralCreatureInfo();

			ImGui::Spacing();
			ImGui::SeparatorText("Movement");
			renderSpeedInfo();

			ImGui::TableSetColumnIndex(1);
			renderEntitySpecificInfo();
			ImGui::EndTable();
		}

		ImGui::Spacing();
		ImGui::SeparatorText("Abilities");
		renderAbilityScoresAndModifiers();

		ImGui::Spacing();
		ImGui::SeparatorText("Saving Throws & Skills");
		if (ImGui::BeginTable("##SavingThrowSkillsSplit", 2, ImGuiTableFlags_SizingStretchProp)) {
			ImGui::TableSetupColumn("Left", ImGuiTableColumnFlags_WidthStretch, 0.5f);
			ImGui::TableSetupColumn("Right", ImGuiTableColumnFlags_WidthStretch, 0.5f);
			ImGui::TableNextRow();

			// Compute a sensible dynamic height for the children so both columns match.
			const auto childHeight{ std::max(MIN_CHILD_HEIGHT, m_mainViewport->WorkSize.y * 0.28f) };

			// Saving throw data as a separate collapsable header.
			ImGui::TableSetColumnIndex(0);
			if (ImGui::CollapsingHeader("Saving Throws", ImGuiTreeNodeFlags_DefaultOpen)) {

				// Saving Throw proficiency and expertise.
				ImGui::BeginChild("##SaveProfandExp", ImVec2{ 0.0f, childHeight }, true);
				renderProficiencyAndExpertiseCluster(
					"Saving Throw Proficiencies",
					"Saving Throw Expertises",
					m_currentCreatureStatBlock.savingThrowProficiencies,
					m_currentCreatureStatBlock.savingThrowExpertise,
					ABILITY_NAMES
				);
				ImGui::EndChild();

				// Saving Throw Modifiers.
				ImGui::BeginChild("##SaveMods", ImVec2{ 0.0f, childHeight }, true);
				renderSavingThrowModifiers();
				ImGui::EndChild();
			}

			// Skill data as a seperate collapsable header.
			ImGui::TableSetColumnIndex(1);
			if (ImGui::CollapsingHeader("Skills", ImGuiTreeNodeFlags_DefaultOpen)) {

				// Skill proficiency and expertise.
				ImGui::BeginChild("##SkillProfandExp", ImVec2{ 0.0f, childHeight }, true);
				renderProficiencyAndExpertiseCluster(
					"Skill Proficiencies",
					"Skill Expertises",
					m_currentCreatureStatBlock.skillProficiencies,
					m_currentCreatureStatBlock.skillExpertise,
					SKILL_NAMES
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
		ImGui::SeparatorText("Damage & Conditions");
		renderDmgVulnResAndImm();
		renderMutuallyExclusiveCheckboxCluster(
			m_currentCreatureStatBlock.conditionImmunities,
			CONDITTON_NAMES,
			"Condition Immunities",
			3
		);

		ImGui::Spacing();
		ImGui::SeparatorText("Senses & Languages");
		renderMutuallyExclusiveCheckboxCluster(
			m_currentCreatureStatBlock.languages,
			LANGUAGE_NAMES,
			"Languages",
			3
		);
		renderSenses();

		ImGui::Spacing();
		renderEntitySpecificSection(); 	// Some parts of the player character specific info goes here.
		ImGui::Spacing();
		ImGui::SeparatorText("Actions, Traits, Equipment");
		renderActions(isMonsterScene()); // This function needs to know the type of entity so that it can disable Lair/Legendary Actions.
		renderAddables(m_currentCreatureStatBlock.traits, TRAIT_LABEL);
		renderAddables(m_currentCreatureStatBlock.equipment, EQUIPMENT_LABEL);
		ImGui::EndChild();

		ImGui::Separator();
		if (CombatTrackerLib::alignedButton(("Complete " + getEntityLabel() + " Creation").c_str(), 0.5f)) {
			if (completeEntityCreation()) {
				m_applicationReference->queueSceneTransition(this, std::make_unique<EncounterCreationScene>(m_applicationReference));
			}
		}
		if (CombatTrackerLib::alignedButton(("Cancel " + getEntityLabel() + " Creation").c_str(), 0.5f)) {
			m_applicationReference->queueSceneTransition(this, std::make_unique<EncounterCreationScene>(m_applicationReference));
		}
		ImGui::End();
	}

	// Simple helper method used for user input of HP, AC, movement speed and sense distances.
	void EntityCreationScene::renderNonOverridableInputInt(int& value, const char* label) {
		if (ImGui::InputInt(label, &value)) {
			value = std::max(0, value);
		}
	}
	void EntityCreationScene::renderAbilityScoresAndModifiers () {
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
			ImGui::TableSetupColumn("Ability", ImGuiTableColumnFlags_WidthStretch, 0.25f);
			ImGui::TableSetupColumn("Score", ImGuiTableColumnFlags_WidthStretch, 0.20f);
			ImGui::TableSetupColumn("Modifier", ImGuiTableColumnFlags_WidthStretch, 0.20f);
			ImGui::TableSetupColumn("Override", ImGuiTableColumnFlags_WidthStretch, 0.35f);

			// Render each column of the table for each ability.
			for (auto& item : m_currentCreatureStatBlock.abilityScores) {
				ImGui::TableNextRow();

				// Compute and show default ability score modifiers.
				if (!m_abilityModifiersOverride.at(item.first)) {
					m_currentCreatureStatBlock.abilityScoreModifiers.at(item.first) = Creature::findAbilityScoreModifier(item.second);
				}

				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(ABILITY_NAMES.at(item.first).c_str());

				// General special value reserved for stretching items.
				ImGui::SetNextItemWidth(-1);
				ImGui::TableSetColumnIndex(1);
				if (ImGui::InputInt(("##" + ABILITY_NAMES.at(item.first) + " Ability Score").c_str(), &item.second)) {
					item.second = std::max(std::min(item.second, ABILITY_SCORE_UPPER_BOUND), ABILITY_SCORE_LOWER_BOUND);
				}

				ImGui::TableSetColumnIndex(2);
				ImGui::SetNextItemWidth(-1);
				if (ImGui::InputInt(("##" + ABILITY_NAMES.at(item.first) + " Ability Modifier").c_str(),
					&m_currentCreatureStatBlock.abilityScoreModifiers.at(item.first))
					) {
					m_currentCreatureStatBlock.abilityScoreModifiers.at(item.first) =
						std::max(std::min(m_currentCreatureStatBlock.abilityScoreModifiers.at(item.first),
							ABILITY_MODIFIERS_UPPER_BOUND), ABILITY_MODIFIERS_LOWER_BOUND);
					m_abilityModifiersOverride.at(item.first) = true;
				}

				// The reset buttons fill their column instead of auto sizing to their label, which would overflow the cell.
				ImGui::TableSetColumnIndex(3);
				if (ImGui::Button(("Reset " + ABILITY_NAMES.at(item.first) + " Modifier").c_str(), ImVec2{ -1.0f, 0.0f })) {
					m_abilityModifiersOverride.at(item.first) = false;
				}
			}
			ImGui::EndTable();
		}
	}
	void EntityCreationScene::renderSavingThrowModifiers () {
		ImGui::TextWrapped("Saving Throw Modifiers");
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

			for (auto& item : ABILITY_NAMES) {

				// Saving throw modifier calculations.
				if (!m_savingThrowsOverride.at(item.first)) {
					if (m_currentCreatureStatBlock.savingThrowExpertise.at(item.first)) {
						m_currentCreatureStatBlock.savingThrowModifiers.at(item.first) =
							m_currentCreatureStatBlock.abilityScoreModifiers.at(item.first) +
							2 * m_currentCreatureStatBlock.proficiencyBonus;
					}
					else if (m_currentCreatureStatBlock.savingThrowProficiencies.at(item.first)) {
						m_currentCreatureStatBlock.savingThrowModifiers.at(item.first) =
							m_currentCreatureStatBlock.abilityScoreModifiers.at(item.first) +
							m_currentCreatureStatBlock.proficiencyBonus;
					}
					else {
						m_currentCreatureStatBlock.savingThrowModifiers.at(item.first) =
							m_currentCreatureStatBlock.abilityScoreModifiers.at(item.first);
					}
				}
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(item.second.c_str());

				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1);
				if (ImGui::InputInt(("##" + item.second + "Saving Throw Modifiers").c_str(),
					&m_currentCreatureStatBlock.savingThrowModifiers.at(item.first)
				)) {
					m_savingThrowsOverride.at(item.first) = true;
				}

				ImGui::TableSetColumnIndex(2);
				if (ImGui::Button(("Reset " + item.second + " Saving Throw Modifier").c_str(), ImVec2{ -1.0f, 0.0f })) {
					m_savingThrowsOverride.at(item.first) = false;
				}
			}
			ImGui::EndTable();
		}
	}
	void EntityCreationScene::renderSkillModifiers () {

		ImGui::TextWrapped("Skill Modifiers");
		if (ImGui::BeginTable("SkillModsTable",
			3,
			ImGuiTableFlags_SizingStretchProp |
			ImGuiTableFlags_RowBg |
			ImGuiTableFlags_BordersInnerV
		)) {

			// Exact same setup as saving throws.
			ImGui::TableSetupColumn("Skill", ImGuiTableColumnFlags_WidthStretch, 0.45f);
			ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 0.20f);
			ImGui::TableSetupColumn("Reset", ImGuiTableColumnFlags_WidthStretch, 0.35f);

			// Default skill modifier calculations.
			for (const auto& item : SKILL_NAMES) {
				if (!m_skillModifiersOverride.at(item.first)) {
					if (m_currentCreatureStatBlock.skillExpertise.at(item.first)) {
						m_currentCreatureStatBlock.skillModifiers.at(item.first) =
							m_currentCreatureStatBlock.abilityScoreModifiers.at(MAPPED_SKILLS.at(item.first)) +
							2 * m_currentCreatureStatBlock.proficiencyBonus;
					}
					else if (m_currentCreatureStatBlock.skillProficiencies.at(item.first)) {
						m_currentCreatureStatBlock.skillModifiers.at(item.first) =
							m_currentCreatureStatBlock.abilityScoreModifiers.at(MAPPED_SKILLS.at(item.first))
							+ m_currentCreatureStatBlock.proficiencyBonus;
					}
					else {
						m_currentCreatureStatBlock.skillModifiers.at(item.first) =
							m_currentCreatureStatBlock.abilityScoreModifiers.at(MAPPED_SKILLS.at(item.first));
					}
				}

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(item.second.c_str());

				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1);
				if (ImGui::InputInt(("##" + item.second + "Skill Modifier").c_str(),
					&m_currentCreatureStatBlock.skillModifiers.at(item.first)
				)) {
					m_skillModifiersOverride.at(item.first) = true;
				}

				ImGui::TableSetColumnIndex(2);
				if (ImGui::Button(("Reset " + item.second + " Skill Modifier").c_str(), ImVec2{ -1.0f, 0.0f })) {
					m_skillModifiersOverride.at(item.first) = false;
				}
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

		if (ImGui::BeginTable("##GeneralCreatureInfo_NameRow", 2, ImGuiTableFlags_SizingStretchProp)) {

			// The label column is measured from its own text so it follows the font instead of a magic pixel width.
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("Name").x + ImGui::GetStyle().ItemSpacing.x * 2.0f);
			ImGui::TableSetupColumn("Field", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted("Name");

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
				ImGui::TextUnformatted("Size");
				ImGui::SetNextItemWidth(-1.0f);
				renderListBox(m_sizeListBoxParameters, m_currentCreatureStatBlock.size, "##Size", SIZE_NAMES);

				ImGui::TableSetColumnIndex(1);
				ImGui::TextUnformatted("Creature Type");
				ImGui::SetNextItemWidth(-1.0f);
				renderListBox(m_creatureTypeParameters, m_currentCreatureStatBlock.type, "##CreatureType", CREATURE_TYPE_NAMES);

				ImGui::TableSetColumnIndex(2);
				ImGui::TextUnformatted("Alignment");
				ImGui::SetNextItemWidth(-1.0f);
				renderListBox(m_alignmentParameters, m_currentCreatureStatBlock.alignment, "##Alignment", ALIGNMENT_NAMES);

				ImGui::EndTable();
			}

			ImGui::TableSetColumnIndex(1);

			if (!m_isInitiativeOverridden) {
				m_currentCreatureStatBlock.initiativeBonus = m_currentCreatureStatBlock.abilityScoreModifiers.at(Ability::DEXTERITY);
			}

			// Hide the label of the input field. Instead use text field for the label for better layout.
			ImGui::TextUnformatted("Initiative Bonus");
			ImGui::SetNextItemWidth(-1.0f);
			if (ImGui::InputInt("##InitiativeBonus", &m_currentCreatureStatBlock.initiativeBonus, 0, 0)) {
				m_currentCreatureStatBlock.initiativeBonus =
					std::min(std::max(ABILITY_SCORE_LOWER_BOUND, m_currentCreatureStatBlock.initiativeBonus),
						ABILITY_MODIFIERS_UPPER_BOUND);
				m_isInitiativeOverridden = true;
			}
			ImGui::SetNextItemWidth(-1.0f);
			if (ImGui::Button("Reset Initiative Bonus", ImVec2{ -1.0f, 0.0f })) {
				m_isInitiativeOverridden = false;
			}

			ImGui::Spacing();
			if (!m_isPassivePerceptionOverridden) {
				m_currentCreatureStatBlock.passivePerception = m_currentCreatureStatBlock.skillModifiers.at(Skill::PERCEPTION) + 10;
			}

			// Same thing here.
			ImGui::TextUnformatted("Passive Perception");
			ImGui::SetNextItemWidth(-1.0f);
			if (ImGui::InputInt("##PassivePerception", &m_currentCreatureStatBlock.passivePerception, 0, 0)) {
				m_currentCreatureStatBlock.passivePerception =
					std::min(std::max(10 + ABILITY_MODIFIERS_UPPER_BOUND, m_currentCreatureStatBlock.passivePerception),
						10 + ABILITY_MODIFIERS_LOWER_BOUND);
				m_isPassivePerceptionOverridden = true;
			}
			ImGui::SetNextItemWidth(-1.0f);
			if (ImGui::Button("Reset Passive Perception", ImVec2{ -1.0f, 0.0f })) {
				m_isPassivePerceptionOverridden = false;
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
				ImGui::TableSetupColumn("ACLabel", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("AC").x + ImGui::GetStyle().ItemSpacing.x * 2.0f);
				ImGui::TableSetupColumn("ACField", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted("AC");

				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1.0f);
				renderNonOverridableInputInt(m_currentCreatureStatBlock.AC, "##AC");

				ImGui::EndTable();
			}

			// HP inner cell.
			ImGui::TableSetColumnIndex(1);
			if (ImGui::BeginTable("##HPRow", 2, ImGuiTableFlags_SizingStretchProp)) {
				ImGui::TableSetupColumn("HPLabel", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("HP").x + ImGui::GetStyle().ItemSpacing.x * 2.0f);
				ImGui::TableSetupColumn("HPField", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted("HP");

				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1.0f);
				renderNonOverridableInputInt(m_currentCreatureStatBlock.HP, "##HP");

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
			ImGui::TextUnformatted("Walk");
			ImGui::SetNextItemWidth(-1.0f);
			renderNonOverridableInputInt(m_currentCreatureStatBlock.walkSpeed, "##Walk");

			ImGui::TableSetColumnIndex(1);
			ImGui::TextUnformatted("Climb");
			ImGui::SetNextItemWidth(-1.0f);
			renderNonOverridableInputInt(m_currentCreatureStatBlock.climbingSpeed, "##Climb");

			ImGui::TableSetColumnIndex(2);
			ImGui::TextUnformatted("Swim");
			ImGui::SetNextItemWidth(-1.0f);
			renderNonOverridableInputInt(m_currentCreatureStatBlock.swimmingSpeed, "##Swim");

			ImGui::TableSetColumnIndex(3);
			ImGui::TextUnformatted("Burrow");
			ImGui::SetNextItemWidth(-1.0f);
			renderNonOverridableInputInt(m_currentCreatureStatBlock.burrowSpeed, "##Burrow");

			ImGui::TableSetColumnIndex(4);
			ImGui::TextUnformatted("Fly");
			ImGui::SetNextItemWidth(-1.0f);
			renderNonOverridableInputInt(m_currentCreatureStatBlock.flySpeed, "##Fly");

			ImGui::EndTable();
		}
	}
	void EntityCreationScene::renderDmgVulnResAndImm () {
		if (ImGui::BeginTable("DmgTri", 3, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV)) {
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::TextWrapped("Vulnerabilities");
			for (const auto& item : DAMAGE_TYPE_NAMES) {
				if (ImGui::Checkbox((item.second + " ##Vuln").c_str(),
					&m_currentCreatureStatBlock.damageVulnerabilities.at(item.first)
				)) {

					// Triple simple deduping logic.
					if (m_currentCreatureStatBlock.damageVulnerabilities.at(item.first)
						&& m_currentCreatureStatBlock.damageResistances.at(item.first)) {
						m_currentCreatureStatBlock.damageResistances.at(item.first) = false;
					}
					if (m_currentCreatureStatBlock.damageVulnerabilities.at(item.first)
						&& m_currentCreatureStatBlock.damageImmunities.at(item.first)) {
						m_currentCreatureStatBlock.damageImmunities.at(item.first) = false;
					}
				}
			}

			ImGui::TableSetColumnIndex(1);
			ImGui::TextWrapped("Resistances");
			for (const auto& item : DAMAGE_TYPE_NAMES) {
				if (ImGui::Checkbox((item.second + " ##Res").c_str(),
					&m_currentCreatureStatBlock.damageResistances.at(item.first)
				)) {

					// Exact same logic here.
					if (m_currentCreatureStatBlock.damageVulnerabilities.at(item.first)
						&& m_currentCreatureStatBlock.damageResistances.at(item.first)) {
						m_currentCreatureStatBlock.damageVulnerabilities.at(item.first) = false;
					}
					if (m_currentCreatureStatBlock.damageResistances.at(item.first)
						&& m_currentCreatureStatBlock.damageImmunities.at(item.first)) {
						m_currentCreatureStatBlock.damageImmunities.at(item.first) = false;
					}
				}
			}

			ImGui::TableSetColumnIndex(2);
			ImGui::TextWrapped("Immunities");
			for (auto& item : DAMAGE_TYPE_NAMES) {
				if (ImGui::Checkbox((item.second + " ##Imm").c_str(),
					&m_currentCreatureStatBlock.damageImmunities.at(item.first)
				)) {

					// Same here.
					if (m_currentCreatureStatBlock.damageVulnerabilities.at(item.first)
						&& m_currentCreatureStatBlock.damageImmunities.at(item.first)) {
						m_currentCreatureStatBlock.damageVulnerabilities.at(item.first) = false;
					}
					if (m_currentCreatureStatBlock.damageResistances.at(item.first)
						&& m_currentCreatureStatBlock.damageImmunities.at(item.first)) {
						m_currentCreatureStatBlock.damageResistances.at(item.first) = false;
					}
				}
			}
			ImGui::EndTable();
		}
	}

	// Specific function for sense rendering, just calls renderNonOverridableInputInt() for all senses.
	void EntityCreationScene::renderSenses() {
		if (ImGui::BeginTable("SensesGrid", 4, ImGuiTableFlags_SizingStretchSame)) {
			ImGui::TableNextRow();
			for (auto& item : m_currentCreatureStatBlock.senses) {
				ImGui::TableNextColumn();

				// Same layout as the speed grid, the label sits above its field.
				ImGui::TextUnformatted(SENSE_NAMES.at(item.first).c_str());
				ImGui::SetNextItemWidth(-1.0f);
				renderNonOverridableInputInt(item.second, ("##" + SENSE_NAMES.at(item.first)).c_str());
			}
			ImGui::EndTable();
		}
	}

	// Really similar to renderAddables() just adds actionType as a separate parameter.
	void EntityCreationScene::renderActions(bool isMonster) {
		ImGui::TextWrapped("Actions");
		static std::vector<size_t> s_actionsMarkedForDeletion{};
		for (size_t i{}; i < m_currentCreatureStatBlock.actions.size(); ++i) {
			ImGui::TableNextColumn();
			ImGui::PushID(static_cast<int> (i));

			ImGui::Button(m_currentCreatureStatBlock.actions[i].name.c_str(), ImVec2{ -1.0f, 0.0f });
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				ImGui::SetTooltip("%s: %s", ACTION_NAMES.at(m_currentCreatureStatBlock.actions[i].type).c_str(), m_currentCreatureStatBlock.actions[i].description.c_str());
			}
			if (ImGui::IsItemClicked()) {
				ImGui::OpenPopup(("Edit Action##" + std::to_string(i)).c_str());
			}

			// The popup can neither be moved nor resized by hand, so it simply keeps following the viewport.
			const auto center{ ImGui::GetMainViewport()->GetCenter() };
			ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2{ 0.5f, 0.5f });
			const auto vp{ ImGui::GetMainViewport()->Size };
			ImGui::SetNextWindowSize(ImVec2{ vp.x * 0.70f, vp.y * 0.70f }, ImGuiCond_Always);
			if (ImGui::BeginPopupModal(("Edit Action##" + std::to_string(i)).c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
				ImGui::BeginChild("EditActionScroll", ImVec2{ 0.0f, -CombatTrackerLib::reservedFooterHeight(2) }, true);
				ImGui::InputText("Action Name", &m_currentCreatureStatBlock.actions[i].name);
				ImGui::InputText("Action Description", &m_currentCreatureStatBlock.actions[i].description);
				ImGui::EndChild();

				ImGui::Separator();
				if (CombatTrackerLib::alignedButton("Finish Action Editing", 0.5f)) {
					ImGui::CloseCurrentPopup();
				}
				if (CombatTrackerLib::alignedButton("Delete Action", 0.5f)) {
					s_actionsMarkedForDeletion.push_back(i);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}

		std::sort(s_actionsMarkedForDeletion.begin(), s_actionsMarkedForDeletion.end());
		for (size_t i{ s_actionsMarkedForDeletion.size() }; i-- > 0;) {
			m_currentCreatureStatBlock.actions.erase(m_currentCreatureStatBlock.actions.begin() + s_actionsMarkedForDeletion[i]);
		}
		s_actionsMarkedForDeletion.clear();

		// Action adding popup modal.
		ImGui::Spacing();
		if (ImGui::Button("Add Action")) {
			ImGui::OpenPopup("Action");
		}
		const auto center{ ImGui::GetMainViewport()->GetCenter() };
		ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2{ 0.5f, 0.5f });
		const auto vp{ ImGui::GetMainViewport()->Size };
		ImGui::SetNextWindowSize(ImVec2(vp.x * 0.70f, vp.y * 0.70f), ImGuiCond_Always);
		if (ImGui::BeginPopupModal("Action", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
			ImGui::BeginChild("##AddActionScroll", ImVec2{ 0.0f, -CombatTrackerLib::reservedFooterHeight(1) }, true);

			// Again, fine for these to be static as they are reset after each addition of actions.
			static CombatTrackerLib::ListBoxParameters s_actionParameters{ -1, -1, false };
			static ActionType s_type{};

			// Important logic to prevent PCs from creating Legendary Actions and Lair Actions.
			if (isMonster) {
				CombatTrackerLib::renderListBox(s_actionParameters, s_type, "Action Type", ACTION_NAMES);
			}
			else {
				CombatTrackerLib::renderListBox(s_actionParameters, s_type, "Action Type", PLAYER_ACTION_NAMES);
			}

			static std::string s_name{};
			static std::string s_description{};
			ImGui::InputText("Action Name", &s_name);
			ImGui::InputText("Action Description", &s_description);
			ImGui::EndChild();

			ImGui::Separator();
			if (CombatTrackerLib::alignedButton("Finish Action Creation", 0.5f) &&
				!s_name.empty() && !s_description.empty() && s_type != ActionType::NO_ACTION) {
				CreatureAction action{ s_name, s_description, s_type };
				m_currentCreatureStatBlock.actions.push_back(action);
				s_actionParameters = { -1, -1, false };
				s_type = { ActionType::NO_ACTION };
				s_name.clear();
				s_description.clear();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	void EntityCreationScene::resetParametersAndOverrides() {
		m_sizeListBoxParameters = { -1, -1, false };
		m_creatureTypeParameters = { -1, -1, false };
		m_alignmentParameters = { -1, -1, false };
		m_isInitiativeOverridden = false;
		m_isPassivePerceptionOverridden = false;
		for (const auto& ability : ABILITY_NAMES) {
			m_abilityModifiersOverride[ability.first] = false;
			m_savingThrowsOverride[ability.first] = false;
		}
		for (const auto& skill : SKILL_NAMES) {
			m_skillModifiersOverride[skill.first] = false;
		}
	}
}
