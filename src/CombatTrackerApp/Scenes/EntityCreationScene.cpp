#include "CombatTrackerApp/Scenes/EntityCreationScene.h"

#include "CombatTrackerApp/Scenes/EncounterCreationScene.h"
#include "CombatTrackerLib/Utilities/GenericUtils.h"
#include "CombatTrackerLib/Utilities/UIUtils.h"

namespace CombatTrackerApp {
	EntityCreationScene::EntityCreationScene(CombatTrackerLib::App* app)
		: CombatTrackerLib::Scene{app},
		  m_currentCreatureStatBlock{},
		  m_sizeListBoxParameters{},
		  m_creatureTypeParameters{},
		  m_alignmentParameters{} {}

	// No non-ImGui based event handling should exist in this scene, I think.
	void EntityCreationScene::notifyScene(const ImGuiIO&) {} 

	// Purely input based scene so no updates necessary.
	void EntityCreationScene::updateScene(double) {} 

	// The scene consists of just UI widgets so no OpenGL side rendering work.
	void EntityCreationScene::renderScene() {} 

	void EntityCreationScene::renderUI() {
		const auto creationLabels{getCreationLabels()};

		// Take over the whole viewport, the same way the other scene hosts do.
		const auto viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		CombatTrackerLib::ScopedWindow window{creationLabels.windowTitle, nullptr, ENTITY_CREATION_WINDOW_FLAGS};

		// TODO: Properly setup initial values of the entity that is being created/edited.
		if (ImGui::IsWindowAppearing()) {
			resetParametersAndOverrides();
		}
		resolveDerivedValues();

		ImGui::SeparatorText(creationLabels.windowTitle.data());

		// Scrollable region for the entire scene.
		// The footer below it is a separator plus the two completion buttons.
		const float footerHeight{CombatTrackerLib::reservedFooterHeight(2)};
		{
			CombatTrackerLib::ScopedChild entityManagementScroll{"EntityManagementScroll", ImVec2{0.0f, -footerHeight}};

			// Top UI elements, which are laid out as follow:
			// Left: General Creature info + speed data.
			// Right: Player character or monster specific info.
			if (CombatTrackerLib::ScopedTable topEntitySplit{"TopEntitySplit", 2, ImGuiTableFlags_SizingStretchProp}) {
				ImGui::TableSetupColumn("Left", ImGuiTableColumnFlags_WidthStretch, 0.75f);
				ImGui::TableSetupColumn("Right", ImGuiTableColumnFlags_WidthStretch, 0.25f);
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::SeparatorText(m_localeProviderPointer->getText("/scenes/entityCreation/sections/general").data()
				);
				renderGeneralCreatureInfo();

				ImGui::Spacing();
				ImGui::SeparatorText(m_localeProviderPointer->getText("/scenes/entityCreation/sections/movement").data()
				);
				renderSpeedInfo();

				ImGui::TableSetColumnIndex(1);
				renderEntitySpecificInfo();
			}

			ImGui::Spacing();
			ImGui::SeparatorText(m_localeProviderPointer->getText("/scenes/entityCreation/sections/abilities").data());
			renderAbilityScoresAndModifiers();

			ImGui::Spacing();
			ImGui::SeparatorText(
				m_localeProviderPointer->getText("/scenes/entityCreation/sections/savingThrowsAndSkills").data()
			);
			if (CombatTrackerLib::ScopedTable
			        savingThrowSkillsSplit{"##SavingThrowSkillsSplit", 2, ImGuiTableFlags_SizingStretchProp}) {
				ImGui::TableSetupColumn("Left", ImGuiTableColumnFlags_WidthStretch, 0.5f);
				ImGui::TableSetupColumn("Right", ImGuiTableColumnFlags_WidthStretch, 0.5f);
				ImGui::TableNextRow();

				// Compute a sensible dynamic height for the children so both columns match.
				const auto childHeight{std::max(MIN_CHILD_HEIGHT, viewport->WorkSize.y * 0.28f)};

				// Saving throw data as a separate collapsable header.
				ImGui::TableSetColumnIndex(0);
				if (ImGui::CollapsingHeader(
						m_localeProviderPointer->getText("/scenes/entityCreation/savingThrows/header").data(),
						ImGuiTreeNodeFlags_DefaultOpen
					)) {

					// Saving Throw proficiency and expertise.
					{
						CombatTrackerLib::ScopedChild saveProfandExp{
							"##SaveProfandExp",
							ImVec2{0.0f, childHeight},
							true
						};
						renderProficiencyAndExpertiseCluster(
							m_localeProviderPointer->getText("/scenes/entityCreation/savingThrows/"
						                                     "proficiencies"),
							m_localeProviderPointer->getText("/scenes/entityCreation/savingThrows/expertises"),
							m_currentCreatureStatBlock.savingThrowProficiencyLevels,
							m_localeProviderPointer->getTextList("/domain/ability")
						);
					}

					// Saving Throw Modifiers.
					CombatTrackerLib::ScopedChild saveMods{"##SaveMods", ImVec2{0.0f, childHeight}, true};
					renderSavingThrowModifiers();
				}

				// Skill data as a seperate collapsable header.
				ImGui::TableSetColumnIndex(1);
				if (ImGui::CollapsingHeader(
						m_localeProviderPointer->getText("/scenes/entityCreation/skills/header").data(),
						ImGuiTreeNodeFlags_DefaultOpen
					)) {

					// Skill proficiency and expertise.
					{
						CombatTrackerLib::ScopedChild skillProfandExp{
							"##SkillProfandExp",
							ImVec2{0.0f, childHeight},
							true
						};
						renderProficiencyAndExpertiseCluster(
							m_localeProviderPointer->getText("/scenes/entityCreation/skills/proficiencies"),
							m_localeProviderPointer->getText("/scenes/entityCreation/skills/expertises"),
							m_currentCreatureStatBlock.skillProficiencyLevels,
							m_localeProviderPointer->getTextList("/domain/skill")
						);
					}

					// Skill modifiers.
					CombatTrackerLib::ScopedChild skillMods{"##SkillMods", ImVec2{0.0f, childHeight}, true};
					renderSkillModifiers();
				}
			}

			ImGui::Spacing();
			ImGui::SeparatorText(
				m_localeProviderPointer->getText("/scenes/entityCreation/sections/damageAndConditions").data()
			);
			renderDmgVulnResAndImm();
			CombatTrackerLib::renderCheckboxGrid(
				m_currentCreatureStatBlock.conditionImmunities,
				m_localeProviderPointer->getTextList("/domain/condition"),
				"ConditionImmunities",
				m_localeProviderPointer->getText("/scenes/entityCreation/damage/conditionImmunities"),
				3
			);

			ImGui::Spacing();
			ImGui::SeparatorText(
				m_localeProviderPointer->getText("/scenes/entityCreation/sections/sensesAndLanguages").data()
			);
			CombatTrackerLib::renderCheckboxGrid(
				m_currentCreatureStatBlock.languages,
				m_localeProviderPointer->getTextList("/domain/creatureLanguage"),
				"Languages",
				m_localeProviderPointer->getText("/scenes/entityCreation/languages"),
				3
			);
			renderSenses();

			ImGui::Spacing();
			renderEntitySpecificSection(); // Some parts of the player character specific info goes here.
			ImGui::Spacing();
			ImGui::SeparatorText(
				m_localeProviderPointer->getText("/scenes/entityCreation/sections/actionsTraitsEquipment").data()
			);
			renderActions();
			renderAddables(
				m_currentCreatureStatBlock.traits,
				AddableLabels{
					m_localeProviderPointer->getText("/scenes/entityCreation/traits/header"),
					m_localeProviderPointer->getText("/scenes/entityCreation/traits/add"),
					m_localeProviderPointer->getText("/scenes/entityCreation/traits/edit"),
					m_localeProviderPointer->getText("/scenes/entityCreation/traits/name"),
					m_localeProviderPointer->getText("/scenes/entityCreation/traits/description"),
					m_localeProviderPointer->getText("/scenes/entityCreation/traits/finishCreation"),
					m_localeProviderPointer->getText("/scenes/entityCreation/traits/finishEditing"),
					m_localeProviderPointer->getText("/scenes/entityCreation/traits/delete")
				}
			);
			renderAddables(
				m_currentCreatureStatBlock.equipment,
				AddableLabels{
					m_localeProviderPointer->getText("/scenes/entityCreation/equipment/header"),
					m_localeProviderPointer->getText("/scenes/entityCreation/equipment/add"),
					m_localeProviderPointer->getText("/scenes/entityCreation/equipment/edit"),
					m_localeProviderPointer->getText("/scenes/entityCreation/equipment/name"),
					m_localeProviderPointer->getText("/scenes/entityCreation/equipment/description"),
					m_localeProviderPointer->getText("/scenes/entityCreation/equipment/finishCreation"),
					m_localeProviderPointer->getText("/scenes/entityCreation/equipment/finishEditing"),
					m_localeProviderPointer->getText("/scenes/entityCreation/equipment/delete")
				}
			);
		}

		ImGui::Separator();
		if (CombatTrackerLib::alignedButton(creationLabels.completeButton.data(), 0.5f)) {
			if (completeEntityCreation()) {
				m_applicationPointer->getSceneManager().queueSceneTransition(
					this,
					std::make_unique<EncounterCreationScene>(m_applicationPointer)
				);
			}
		}
		if (CombatTrackerLib::alignedButton(creationLabels.cancelButton.data(), 0.5f)) {
			m_applicationPointer->getSceneManager().queueSceneTransition(
				this,
				std::make_unique<EncounterCreationScene>(m_applicationPointer)
			);
		}
	}

	void EntityCreationScene::renderAbilityScoresAndModifiers() {

		const auto abilityNames{m_localeProviderPointer->getTextList("/domain/ability")};

		if (CombatTrackerLib::ScopedTable abilityTable{
				"AbilityTable",
				3,
				ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg
			}) {

			/*
			The table is split into three parts:
			1) Ability name.
			2) Ability score.
			3) Ability modifier.
			The modifier follows the score by rule with nothing able to detach the two, so it is shown
			rather than edited and there is no override column to reset.
			*/
			ImGui::TableSetupColumn(
				m_localeProviderPointer->getText("/scenes/entityCreation/abilityTable/abilityColumn").data(),
				ImGuiTableColumnFlags_WidthStretch,
				0.40f
			);
			ImGui::TableSetupColumn(
				m_localeProviderPointer->getText("/scenes/entityCreation/abilityTable/scoreColumn").data(),
				ImGuiTableColumnFlags_WidthStretch,
				0.35f
			);
			ImGui::TableSetupColumn(
				m_localeProviderPointer->getText("/scenes/entityCreation/abilityTable/modifierColumn").data(),
				ImGuiTableColumnFlags_WidthStretch,
				0.25f
			);

			// Render each column of the table for each ability.
			// Every ability keyed array is the same length, so one index walks the names, the scores and the modifiers together.
			for (size_t i{}; i < abilityNames.size(); ++i) {
				const auto abilityName{abilityNames[i]};
				CombatTrackerLib::ScopedID scopedID{static_cast<int>(i)};
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(abilityName.data());

				// General special value reserved for stretching items.
				ImGui::SetNextItemWidth(-1);
				ImGui::TableSetColumnIndex(1);
				CombatTrackerLib::renderBoundedInputInt(m_currentCreatureStatBlock.abilityScores[i], "##AbilityScore");

				ImGui::TableSetColumnIndex(2);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("%d", m_currentCreatureStatBlock.derived.getAbilityScoreModifier(static_cast<Ability>(i)));
			}
		}
	}

	/*
	Used for rendering skill and saving throw proficiencies and expertises for both monsters and player characters.
	One proficiency level backs both columns, so a proficiency checkbox moves between none and proficient
	while an expertise checkbox moves between proficient and expert.
	Being proficient and expert at once is no longer something that has to be deduped, it simply cannot be spelled.
	*/
	void EntityCreationScene::renderProficiencyAndExpertiseCluster(
		std::string_view proficiencyLabel,
		std::string_view expertiseLabel,
		std::span<ProficiencyLevel> proficiencyLevels,
		std::span<const std::string_view> itemNames
	) {
		if (CombatTrackerLib::ScopedTable
		        profExpTable{"ProfExpTable", 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV}) {

			ImGui::TableSetupColumn("Left");
			ImGui::TableSetupColumn("Right");
			ImGui::TableNextRow();

			// The names and the level array are the same length, so a single index walks both.
			// This is also why the helper works for abilities and skills alike, as nothing here is tied to one enum.
			ImGui::TableSetColumnIndex(0);
			ImGui::TextWrapped("%s", proficiencyLabel.data());
			{
				CombatTrackerLib::ScopedID proficiencyID{"Proficiency"};
				for (size_t i{}; i < itemNames.size(); ++i) {
					bool isProficient{proficiencyLevels[i] != ProficiencyLevel::NONE};
					if (ImGui::Checkbox(itemNames[i].data(), &isProficient)) {
						proficiencyLevels[i] = isProficient ? ProficiencyLevel::PROFICIENT : ProficiencyLevel::NONE;
					}
				}
			}

			// Expertise checkboxes. Ticking one implies the proficiency it is built on.
			ImGui::TableSetColumnIndex(1);
			ImGui::TextWrapped("%s", expertiseLabel.data());
			CombatTrackerLib::ScopedID expertiseID{"Expertise"};
			for (size_t i{}; i < itemNames.size(); ++i) {
				bool isExpert{proficiencyLevels[i] == ProficiencyLevel::EXPERT};
				if (ImGui::Checkbox(itemNames[i].data(), &isExpert)) {
					proficiencyLevels[i] = isExpert ? ProficiencyLevel::EXPERT : ProficiencyLevel::PROFICIENT;
				}
			}
		}
	}

	// The grid form, for items that only ever have a proficiency or not.
	void EntityCreationScene::renderProficiencyLevelGrid(
		std::span<ProficiencyLevel> proficiencyLevels,
		std::span<const std::string_view> itemNames,
		std::string_view stableID,
		std::string_view label,
		int columnCount
	) {
		ImGui::TextWrapped("%s", label.data());
		if (CombatTrackerLib::ScopedTable table{stableID, columnCount, ImGuiTableFlags_SizingStretchSame}) {
			for (std::size_t i{}; i < itemNames.size(); ++i) {
				ImGui::TableNextColumn();
				bool isProficient{proficiencyLevels[i] != ProficiencyLevel::NONE};
				if (ImGui::Checkbox(itemNames[i].data(), &isProficient)) {
					proficiencyLevels[i] = isProficient ? ProficiencyLevel::PROFICIENT : ProficiencyLevel::NONE;
				}
			}
		}
	}

	/*
	Saving throws and skills present the same table, so both gather their own labels and hand the work
	to one implementation. The value is overridable, so the field shows whatever the integer currently
	reads as and the reset button gives it back to the pass that recomputes the default.
	*/
	void EntityCreationScene::renderModifierTable(
		std::string_view stableID,
		const ModifierTableLabels& labels,
		std::span<CombatTrackerLib::OverridableInt> modifiers,
		std::span<const std::string_view> itemNames,
		std::span<const std::string_view> resetLabels,
		std::span<const float, 3> columnWidths
	) {
		ImGui::TextWrapped("%s", labels.header.data());
		if (CombatTrackerLib::ScopedTable table{
				stableID,
				3,
				ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerV
			}) {
			ImGui::TableSetupColumn(labels.itemColumn.data(), ImGuiTableColumnFlags_WidthStretch, columnWidths[0]);
			ImGui::TableSetupColumn(labels.valueColumn.data(), ImGuiTableColumnFlags_WidthStretch, columnWidths[1]);
			ImGui::TableSetupColumn(labels.resetColumn.data(), ImGuiTableColumnFlags_WidthStretch, columnWidths[2]);

			// The names, the modifiers and the reset labels are all keyed the same way, so one index walks all three.
			for (size_t i{}; i < itemNames.size(); ++i) {
				CombatTrackerLib::ScopedID scopedID{static_cast<int>(i)};
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(itemNames[i].data());

				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1);

				// ImGui writes through an int pointer, so the widget edits a copy that is handed back on change.
				int modifier{modifiers[i].get()};
				if (ImGui::InputInt("##Modifier", &modifier)) {
					modifiers[i].setOverride(modifier);
				}

				ImGui::TableSetColumnIndex(2);
				if (ImGui::Button(resetLabels[i].data(), ImVec2{-1.0f, 0.0f})) {
					modifiers[i].clearOverride();
				}
			}
		}
	}

	void EntityCreationScene::renderSavingThrowModifiers() {
		static constexpr std::array<float, 3> COLUMN_WIDTHS{0.35f, 0.25f, 0.40f};
		renderModifierTable(
			"SaveModsTable",
			ModifierTableLabels{
				m_localeProviderPointer->getText("/scenes/entityCreation/savingThrows/modifiers"),
				m_localeProviderPointer->getText("/scenes/entityCreation/savingThrows/abilityColumn"),
				m_localeProviderPointer->getText("/scenes/entityCreation/savingThrows/valueColumn"),
				m_localeProviderPointer->getText("/scenes/entityCreation/savingThrows/resetColumn")
			},
			m_currentCreatureStatBlock.savingThrowModifiers,
			m_localeProviderPointer->getTextList("/domain/ability"),
			m_localeProviderPointer->getTextList("/scenes/entityCreation/savingThrows/resetModifiers"),
			COLUMN_WIDTHS
		);
	}

	void EntityCreationScene::renderSkillModifiers() {
		static constexpr std::array<float, 3> COLUMN_WIDTHS{0.45f, 0.20f, 0.35f};
		renderModifierTable(
			"SkillModsTable",
			ModifierTableLabels{
				m_localeProviderPointer->getText("/scenes/entityCreation/skills/modifiers"),
				m_localeProviderPointer->getText("/scenes/entityCreation/skills/skillColumn"),
				m_localeProviderPointer->getText("/scenes/entityCreation/skills/valueColumn"),
				m_localeProviderPointer->getText("/scenes/entityCreation/skills/resetColumn")
			},
			m_currentCreatureStatBlock.skillModifiers,
			m_localeProviderPointer->getTextList("/domain/skill"),
			m_localeProviderPointer->getTextList("/scenes/entityCreation/skills/resetModifiers"),
			COLUMN_WIDTHS
		);
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
	Each of those groups is its own table, so each gets its own function and this one is just the layout.
	*/
	void EntityCreationScene::renderGeneralCreatureInfo() {
		renderNameRow();
		ImGui::Spacing();

		if (CombatTrackerLib::ScopedTable
		        generalCreatureInfoMainSplit{"##GeneralCreatureInfo_MainSplit", 2, ImGuiTableFlags_SizingStretchProp}) {

			// Give right side a reasonable fixed width so it never expands into the Entity Specific column.
			const auto avail{ImGui::GetContentRegionAvail().x};
			const auto rightW{std::clamp(avail * 0.30f, std::min(240.0f, avail * 0.45f), 300.0f)};

			ImGui::TableSetupColumn("Left", ImGuiTableColumnFlags_WidthStretch, 1.0f);
			ImGui::TableSetupColumn("Right", ImGuiTableColumnFlags_WidthFixed, rightW);
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			renderCreatureListBoxes();

			ImGui::TableSetColumnIndex(1);
			renderDerivedOverridables();
		}

		ImGui::Spacing();
		renderArmorClassAndHitPoints();
	}

	// The label column is measured from its own text so it follows the font instead of a magic pixel width.
	void EntityCreationScene::renderNameRow() {

		const auto nameLabel{m_localeProviderPointer->getText("/scenes/entityCreation/general/name")};
		if (CombatTrackerLib::ScopedTable
		        generalCreatureInfoNameRow{"##GeneralCreatureInfo_NameRow", 2, ImGuiTableFlags_SizingStretchProp}) {

			// The label column is measured from its own text so it follows the font instead of a magic pixel width.
			ImGui::TableSetupColumn(
				"Label",
				ImGuiTableColumnFlags_WidthFixed,
				ImGui::CalcTextSize(nameLabel.data()).x + ImGui::GetStyle().ItemSpacing.x * 2.0f
			);
			ImGui::TableSetupColumn("Field", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted(nameLabel.data());

			ImGui::TableSetColumnIndex(1);
			ImGui::SetNextItemWidth(-1.0f);
			ImGui::InputText("##Name", &m_currentCreatureStatBlock.name);
		}
	}

	// Size, creature type and alignment, which are all single choice lists over a dense enumeration.
	void EntityCreationScene::renderCreatureListBoxes() {
		if (CombatTrackerLib::ScopedTable
		        generalCreatureInfoListboxes{"##GeneralCreatureInfo_Listboxes", 3, ImGuiTableFlags_SizingStretchProp}) {
			ImGui::TableSetupColumn("SizeCol", ImGuiTableColumnFlags_WidthStretch, 0.33f);
			ImGui::TableSetupColumn("TypeCol", ImGuiTableColumnFlags_WidthStretch, 0.34f);
			ImGui::TableSetupColumn("AlignmentCol", ImGuiTableColumnFlags_WidthStretch, 0.33f);

			ImGui::TableNextRow();

			// The labels are hidden on the listboxes themselves, so each one can take its whole column.
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted(m_localeProviderPointer->getText("/scenes/entityCreation/general/size").data());
			ImGui::SetNextItemWidth(-1.0f);
			renderListBox(
				m_sizeListBoxParameters,
				m_currentCreatureStatBlock.size,
				"##Size",
				m_localeProviderPointer->getTextList("/domain/size")
			);

			ImGui::TableSetColumnIndex(1);
			ImGui::TextUnformatted(
				m_localeProviderPointer->getText("/scenes/entityCreation/general/creatureType").data()
			);
			ImGui::SetNextItemWidth(-1.0f);
			renderListBox(
				m_creatureTypeParameters,
				m_currentCreatureStatBlock.type,
				"##CreatureType",
				m_localeProviderPointer->getTextList("/domain/creatureType")
			);

			ImGui::TableSetColumnIndex(2);
			ImGui::TextUnformatted(m_localeProviderPointer->getText("/scenes/entityCreation/general/alignment").data());
			ImGui::SetNextItemWidth(-1.0f);
			renderListBox(
				m_alignmentParameters,
				m_currentCreatureStatBlock.alignment,
				"##Alignment",
				m_localeProviderPointer->getTextList("/domain/alignment")
			);
		}
	}

	// Initiative and passive perception follow from other stats but stay overridable for special effects.
	void EntityCreationScene::renderDerivedOverridables() {
		{
			CombatTrackerLib::ScopedID initiativeBonusID{"InitiativeBonus"};
			CombatTrackerLib::renderOverridableInputInt(
				m_currentCreatureStatBlock.initiativeBonus,
				m_localeProviderPointer->getText("/scenes/entityCreation/general/initiativeBonus"),
				m_localeProviderPointer->getText("/scenes/entityCreation/general/resetInitiativeBonus"),
				ABILITY_MODIFIERS_LOWER_BOUND,
				ABILITY_MODIFIERS_UPPER_BOUND
			);
		}

		ImGui::Spacing();
		// Same thing here, with the bounds following from what a perception modifier can reach.
		CombatTrackerLib::ScopedID passivePerceptionID{"PassivePerception"};
		CombatTrackerLib::renderOverridableInputInt(
			m_currentCreatureStatBlock.passivePerception,
			m_localeProviderPointer->getText("/scenes/entityCreation/general/passivePerception"),
			m_localeProviderPointer->getText("/scenes/entityCreation/general/resetPassivePerception"),
			PASSIVE_PERCEPTION_BASE + ABILITY_MODIFIERS_LOWER_BOUND,
			PASSIVE_PERCEPTION_BASE + ABILITY_MODIFIERS_UPPER_BOUND
		);
	}

	void EntityCreationScene::renderArmorClassAndHitPoints() {

		const auto armorClassLabel{m_localeProviderPointer->getText("/scenes/entityCreation/general/armorClass")};
		const auto hitPointsLabel{m_localeProviderPointer->getText("/scenes/entityCreation/general/hitPoints")};
		if (CombatTrackerLib::ScopedTable achP{"##ACHP", 2, ImGuiTableFlags_SizingStretchProp}) {
			ImGui::TableSetupColumn("ACCol", ImGuiTableColumnFlags_WidthStretch, 0.5f);
			ImGui::TableSetupColumn("HPCol", ImGuiTableColumnFlags_WidthStretch, 0.5f);
			ImGui::TableNextRow();

			// AC inner table.
			ImGui::TableSetColumnIndex(0);
			if (CombatTrackerLib::ScopedTable acRow{"##ACRow", 2, ImGuiTableFlags_SizingStretchProp}) {
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
				CombatTrackerLib::renderBoundedInputInt(m_currentCreatureStatBlock.AC, "##AC");
			}

			// HP inner cell.
			ImGui::TableSetColumnIndex(1);
			if (CombatTrackerLib::ScopedTable hpRow{"##HPRow", 2, ImGuiTableFlags_SizingStretchProp}) {
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
				CombatTrackerLib::renderBoundedInputInt(m_currentCreatureStatBlock.HP, "##HP");
			}
		}
	}

	void EntityCreationScene::renderSpeedInfo() {

		const auto movementTypeNames{m_localeProviderPointer->getTextList("/domain/movementType")};

		if (CombatTrackerLib::ScopedTable
		        speedGrid{"SpeedGrid", static_cast<int>(movementTypeNames.size()), ImGuiTableFlags_SizingStretchSame}) {
			ImGui::TableNextRow();

			// An inline label plus the stepper buttons does not fit into a fifth of the column, so the labels go above the fields here too.
			// The names and the speeds are keyed by the same enumeration, so one index walks both.
			for (size_t i{}; i < movementTypeNames.size(); ++i) {
				CombatTrackerLib::ScopedID scopedID{static_cast<int>(i)};
				ImGui::TableSetColumnIndex(static_cast<int>(i));
				ImGui::TextUnformatted(movementTypeNames[i].data());
				ImGui::SetNextItemWidth(-1.0f);
				CombatTrackerLib::renderBoundedInputInt(m_currentCreatureStatBlock.speeds[i], "##Speed");
			}
		}
	}

	/*
	One reaction per damage type backs all three columns, so a box being ticked in one column is the same
	value the other two are reading. Ticking a box therefore replaces whatever was there, which is the
	behaviour the old three arrays needed explicit deduping to reach.
	*/
	void EntityCreationScene::renderDmgVulnResAndImm() {

		const auto damageTypeNames{m_localeProviderPointer->getTextList("/domain/damageType")};
		const auto renderReactionColumn{[this, damageTypeNames](
											int columnIndex,
											std::string_view header,
											std::string_view stableID,
											DamageReaction reaction
										) {
			ImGui::TableSetColumnIndex(columnIndex);
			ImGui::TextWrapped("%s", header.data());
			CombatTrackerLib::ScopedID scopedID{stableID};
			for (size_t i{}; i < damageTypeNames.size(); ++i) {
				bool hasReaction{m_currentCreatureStatBlock.damageReactions[i] == reaction};
				if (ImGui::Checkbox(damageTypeNames[i].data(), &hasReaction)) {
					m_currentCreatureStatBlock.damageReactions[i] = hasReaction ? reaction : DamageReaction::NONE;
				}
			}
		}};

		if (CombatTrackerLib::ScopedTable
		        dmgTri{"DmgTri", 3, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV}) {
			ImGui::TableNextRow();
			renderReactionColumn(
				0,
				m_localeProviderPointer->getText("/scenes/entityCreation/damage/vulnerabilities"),
				"Vulnerabilities",
				DamageReaction::VULNERABLE
			);
			renderReactionColumn(
				1,
				m_localeProviderPointer->getText("/scenes/entityCreation/damage/resistances"),
				"Resistances",
				DamageReaction::RESISTANT
			);
			renderReactionColumn(
				2,
				m_localeProviderPointer->getText("/scenes/entityCreation/damage/immunities"),
				"Immunities",
				DamageReaction::IMMUNE
			);
		}
	}

	// Specific function for sense rendering, just calls CombatTrackerLib::renderBoundedInputInt() for all senses.
	void EntityCreationScene::renderSenses() {
		const auto senseNames{m_localeProviderPointer->getTextList("/domain/sense")};
		if (CombatTrackerLib::ScopedTable sensesGrid{"SensesGrid", 4, ImGuiTableFlags_SizingStretchSame}) {
			ImGui::TableNextRow();
			for (size_t i{}; i < senseNames.size(); ++i) {
				CombatTrackerLib::ScopedID senseID{static_cast<int>(i)};
				ImGui::TableNextColumn();

				// Same layout as the speed grid, the label sits above its field.
				ImGui::TextUnformatted(senseNames[i].data());
				ImGui::SetNextItemWidth(-1.0f);
				CombatTrackerLib::renderBoundedInputInt(m_currentCreatureStatBlock.senses[i], "##Sense");
			}
		}
	}

	/*
	Actions are addables with two extras: the add popup also picks an action type, and the tooltip names
	that type alongside the description. Everything else is the same list, edit popup and add popup,
	so the shared helper does it and this only supplies the three differences.
	*/
	void EntityCreationScene::renderActions() {

		const auto actionNames{m_localeProviderPointer->getTextList("/domain/actionType")};

		// Static so they survive between frames while the add popup is open, and reset as it appears.
		static CombatTrackerLib::ListBoxParameters s_actionParameters{};
		static ActionType s_type{};

		renderAddables(
			m_currentCreatureStatBlock.actions,
			AddableLabels{
				m_localeProviderPointer->getText("/scenes/entityCreation/actions/header"),
				m_localeProviderPointer->getText("/scenes/entityCreation/actions/add"),
				m_localeProviderPointer->getText("/scenes/entityCreation/actions/edit"),
				m_localeProviderPointer->getText("/scenes/entityCreation/actions/name"),
				m_localeProviderPointer->getText("/scenes/entityCreation/actions/description"),
				m_localeProviderPointer->getText("/scenes/entityCreation/actions/finishCreation"),
				m_localeProviderPointer->getText("/scenes/entityCreation/actions/finishEditing"),
				m_localeProviderPointer->getText("/scenes/entityCreation/actions/delete")
			},
			[this, actionNames] {
				if (ImGui::IsWindowAppearing()) {
					s_actionParameters = CombatTrackerLib::ListBoxParameters{};
					s_type = {};
				}
				CombatTrackerLib::renderListBox(
					s_actionParameters,
					s_type,
					m_localeProviderPointer->getText("/scenes/entityCreation/actions/type").data(),
					actionNames
				);
			},
			[](const std::string& name, const std::string& description) {
				return CreatureAction{name, description, s_type};
			},
			[actionNames](const CreatureAction& action) {
				ImGui::SetTooltip(
					"%s: %s",
					actionNames[CombatTrackerLib::enumToSizeT(action.type)].data(),
					action.description.c_str()
				);
			}
		);
	}

	void EntityCreationScene::resetParametersAndOverrides() {
		m_currentCreatureStatBlock = CreatureStatBlock{};
		m_sizeListBoxParameters = CombatTrackerLib::ListBoxParameters{};
		m_creatureTypeParameters = CombatTrackerLib::ListBoxParameters{};
		m_alignmentParameters = CombatTrackerLib::ListBoxParameters{};
	}
}
