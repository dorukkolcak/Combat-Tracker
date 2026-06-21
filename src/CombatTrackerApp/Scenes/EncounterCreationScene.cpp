#include <vector>
#include <algorithm>

#include "EncounterCreationScene.h"
#include "MainMenuScene.h"
#include "Utils.h"
#include "../JSONUtils.h"
#include "../Entities/Creature/CreatureConstants.h"
#include "../Entities/Monster/MonsterConstants.h"

#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h" // std::string support

namespace CombatTrackerApp {
	EncounterCreationScene::EncounterCreationScene(CombatTrackerLib::App* app) :
		CombatTrackerLib::Scene{ app },
		monsterBuilder{},
		playerCharacterBuilder{},
		currentCreatureStatBlock{ monsterBuilder.getCreatureBlock() }, // Doesn't matter which builder is used here.
		currentPlayerStatBlock{ playerCharacterBuilder.getPlayerCharacterBlock() },
		currentMonsterCR{},
		currentMonsterXP{},
		monsterCRParameters{-1, -1, false},
		sizeListBoxParameters{ -1, -1, false },
		creatureTypeParameters{ -1, -1, false },
		alignmentParameters{ -1, -1, false },
		speciesParameters{ -1, -1, false },
		originParameters{ -1, -1, false },
		isInitiativeOverriden{false},
		isPassivePerceptionOverridden{ false },
		isXPOverridden{false},
		isPBOverridden{false},
		currentPopupName{}
	{
		encounterCreationDockFlags =
			ImGuiDockNodeFlags_NoResize |
			ImGuiDockNodeFlags_NoSplit;
		encounterCreationWindowFlags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;
		popupWindowFlags = ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse;
		for (const auto& ability : ABILITY_NAMES) {
			abilityModifiersOverride[ability.first] = false;
			savingThrowsOverride[ability.first] = false;
		}
		for (const auto& skill : SKILL_NAMES) {
			skillModifiersOverride[skill.first] = false;
		}
	}
	EncounterCreationScene::~EncounterCreationScene() {}

	void EncounterCreationScene::notifyScene(const ImGuiIO& imguiIO) {}
	void EncounterCreationScene::updateScene(double timeInterval) {}

	void EncounterCreationScene::renderUI() {

		renderHostWindow();
		renderMenuWindow();
		renderEntityBrowserWindow();
		renderMonsterBrowserWindow();
		renderPlayerCharacterBrowserWindow();
		renderEncounterBrowserWindow();
		renderEncounterManagerWindow();

	}
	void EncounterCreationScene::renderScene() {}

	void EncounterCreationScene::renderHostWindow() {

		using enum EncounterCreationWindows;

		// Set configurations of the host window of the scene on first render pass.
		if (!dockspaceInitializations.at(ENCOUNTER_CREATOR)) {
			mainViewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(mainViewport->WorkPos);
			ImGui::SetNextWindowSize(mainViewport->WorkSize);
			ImGui::SetNextWindowViewport(mainViewport->ID);
		}

		// Render host window.
		ImGui::Begin(
			MAPPED_WINDOW_NAMES.at(ENCOUNTER_CREATOR).c_str(),
			nullptr,
			encounterCreationWindowFlags
		);

		// Setup dockspace layout on first render pass.
		if (!dockspaceInitializations.at(ENCOUNTER_CREATOR)) {

			dockspaceIDs[ENCOUNTER_CREATOR] =
				ImGui::GetID(
					(MAPPED_WINDOW_NAMES.at(ENCOUNTER_CREATOR) + " - Dockspace")
					.c_str()
				);
			ImGui::DockBuilderRemoveNode(dockspaceIDs.at(ENCOUNTER_CREATOR));

			// Add dockspace nodes.
			ImGui::DockBuilderAddNode(dockspaceIDs.at(ENCOUNTER_CREATOR), ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspaceIDs.at(ENCOUNTER_CREATOR), mainViewport->WorkSize);

			// Configure dockspace nodes.
			ImGuiID leftID{};
			ImGui::DockBuilderSplitNode(
				dockspaceIDs.at(ENCOUNTER_CREATOR),
				ImGuiDir_Left,
				0.10f,
				&(dockspaceIDs[MENU]),
				&leftID
			);
			ImGui::DockBuilderSplitNode(leftID,
				ImGuiDir_Left,
				0.36f,
				&(dockspaceIDs[ENTITY_BROWSER]),
				&(dockspaceIDs[ENCOUNTER_MANAGER])
			);

			// Dock windows.
			ImGui::DockBuilderDockWindow(
				MAPPED_WINDOW_NAMES.at(ENTITY_BROWSER).c_str(),
				dockspaceIDs.at(ENTITY_BROWSER)
			);
			ImGui::DockBuilderDockWindow(
				MAPPED_WINDOW_NAMES.at(ENCOUNTER_MANAGER).c_str(),
				dockspaceIDs.at(ENCOUNTER_MANAGER)
			);
			ImGui::DockBuilderDockWindow(
				MAPPED_WINDOW_NAMES.at(MENU).c_str(),
				dockspaceIDs.at(MENU)
			);

			// Finish dockspace creation.
			ImGui::DockBuilderFinish(dockspaceIDs.at(ENCOUNTER_CREATOR));
			dockspaceInitializations.at(ENCOUNTER_CREATOR) = true;
		}
		// Render dockspace of the host window.
		ImGui::DockSpace(
			dockspaceIDs.at(ENCOUNTER_CREATOR),
			ImGui::GetWindowSize(),
			encounterCreationDockFlags
		);
		ImGui::End();
	}

	// TODO: Add more options here. Currently only used as "Back to Main Menu" section.
	void EncounterCreationScene::renderMenuWindow() {
		ImGui::Begin(MAPPED_WINDOW_NAMES.at(EncounterCreationWindows::MENU).c_str());
		if (ImGui::Button("Return to Main Menu")) {
			applicationReference->queueSceneTransition(this, std::make_unique<MainMenuScene>(applicationReference));
		}
		ImGui::End();
	}
	void EncounterCreationScene::renderEntityBrowserWindow() {

		using enum EncounterCreationWindows;

		ImGui::Begin(MAPPED_WINDOW_NAMES.at(ENTITY_BROWSER).c_str());

		// Set configurations of the window of the scene on first render pass.
		if (!dockspaceInitializations.at(ENTITY_BROWSER)) {

			dockspaceIDs[ENTITY_BROWSER] =
				ImGui::GetID(
					(MAPPED_WINDOW_NAMES.at(ENTITY_BROWSER) + " - Dockspace")
					.c_str()
				);
			ImGui::DockBuilderRemoveNode(dockspaceIDs.at(ENTITY_BROWSER));

			// Add dockspace nodes.
			ImGui::DockBuilderAddNode(dockspaceIDs.at(ENTITY_BROWSER), ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspaceIDs.at(ENTITY_BROWSER), ImGui::GetWindowSize());

			// Dock windows.
			ImGui::DockBuilderDockWindow(
				MAPPED_WINDOW_NAMES.at(MONSTER_BROWSER).c_str(),
				dockspaceIDs.at(ENTITY_BROWSER)
			);
			ImGui::DockBuilderDockWindow(
				MAPPED_WINDOW_NAMES.at(PLAYER_CHARACTER_BROWSER).c_str(),
				dockspaceIDs.at(ENTITY_BROWSER)
			);
			ImGui::DockBuilderDockWindow(
				MAPPED_WINDOW_NAMES.at(ENCOUNTER_BROWSER).c_str(),
				dockspaceIDs.at(ENTITY_BROWSER)
			);

			// Finish dockspace creation.
			ImGui::DockBuilderFinish(dockspaceIDs.at(ENTITY_BROWSER));
			dockspaceInitializations.at(ENTITY_BROWSER) = true;
		}

		// Render dockspace of the entity browser window.
		ImGui::DockSpace(dockspaceIDs.at(ENTITY_BROWSER), ImGui::GetWindowSize());
		ImGui::End();
	}
	void EncounterCreationScene::renderMonsterBrowserWindow() {
		
		using enum EncounterCreationWindows;

		static bool editingModeMonster{ true };
		ImGui::Begin(MAPPED_WINDOW_NAMES.at(MONSTER_BROWSER).c_str());
		if (CombatTrackerLib::alignedButton("Create Monster", 0.5f)) {

			ImGui::OpenPopup("Monster Creation");
			editingModeMonster = false;
		}

		if (!dockspaceInitializations.at(MONSTER_BROWSER)) {
			initializeSubBrowserDockspace(MONSTER_BROWSER);
		}
		ImGui::DockSpace(dockspaceIDs.at(MONSTER_BROWSER), ImGui::GetWindowSize());

		/*
		IMPORTANT: The popup modal rendering code should be precisely within the rendering code of its parent window.
		Otherwise it won't be rendered at all, as it exists in a different ID context.
		See https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-about-the-id-stack-system.
		*/
		renderEntityCreationPopupModal(true, editingModeMonster);

		ImGui::End();

		/*
		TODO: Finalize UI design. Handle button for all monsters should be in the context of the window.
		With each button corresponding to a monster available in JSON.
		When this button is clicked, a popup should open with outlining three options:
		1) Delete Monster.
		2) Edit Monster.
		3) Add monster to current encounter.
		*/
		ImGui::Begin((MAPPED_WINDOW_NAMES.at(MONSTER_BROWSER) + " - User Created").c_str());

		ImGui::End();

		// TODO: Finalize UI design. Clicking on a compendium monster should add it to the current encounter.
		ImGui::Begin((MAPPED_WINDOW_NAMES.at(MONSTER_BROWSER) + " - Compendium").c_str());

		ImGui::End();
	}

	// TODO: Exact same comments for the monster browser apply to the player browser as well.
	void EncounterCreationScene::renderPlayerCharacterBrowserWindow() {
		
		using enum EncounterCreationWindows;

		static bool editingModePlayerCharacter{ true };
		ImGui::Begin(MAPPED_WINDOW_NAMES.at(PLAYER_CHARACTER_BROWSER).c_str());
		if (!dockspaceInitializations.at(PLAYER_CHARACTER_BROWSER)) {
			initializeSubBrowserDockspace(PLAYER_CHARACTER_BROWSER);
		}

		if (CombatTrackerLib::alignedButton("Create Player Character", 0.5f)) {
			ImGui::OpenPopup("Player Character Creation");
			editingModePlayerCharacter = false;
		}

		ImGui::DockSpace(dockspaceIDs.at(PLAYER_CHARACTER_BROWSER), ImGui::GetWindowSize());

		renderEntityCreationPopupModal(false, editingModePlayerCharacter);

		ImGui::End();

		ImGui::Begin((MAPPED_WINDOW_NAMES.at(PLAYER_CHARACTER_BROWSER) + " - User Created").c_str());

		ImGui::End();

		ImGui::Begin((MAPPED_WINDOW_NAMES.at(PLAYER_CHARACTER_BROWSER) + " - Compendium").c_str());

		ImGui::End();
	}

	// TODO: Finish this window when all monster, player character and encounter browser functionality is done.
	void EncounterCreationScene::renderEncounterBrowserWindow() {

		using enum EncounterCreationWindows;

		ImGui::Begin(MAPPED_WINDOW_NAMES.at(ENCOUNTER_BROWSER).c_str());
		if (!dockspaceInitializations.at(ENCOUNTER_BROWSER)) {
			initializeSubBrowserDockspace(ENCOUNTER_BROWSER);
		}
		ImGui::DockSpace(dockspaceIDs.at(ENCOUNTER_BROWSER), ImGui::GetWindowSize());
		ImGui::End();

		ImGui::Begin((MAPPED_WINDOW_NAMES.at(ENCOUNTER_BROWSER) + " - User Created").c_str());

		ImGui::End();

		ImGui::Begin((MAPPED_WINDOW_NAMES.at(ENCOUNTER_BROWSER) + " - Compendium").c_str());

		ImGui::End();

	}

	// TODO: Finish this window when all monster and player character functionality is done.
	void EncounterCreationScene::renderEncounterManagerWindow() {

		using enum EncounterCreationWindows;

		ImGui::Begin(MAPPED_WINDOW_NAMES.at(ENCOUNTER_MANAGER).c_str());
		ImGui::End();
	}

	// Helper used for creating the user-compendium split windows. 
	void EncounterCreationScene::initializeSubBrowserDockspace(EncounterCreationWindows windowType) {

		dockspaceIDs[windowType] =
			ImGui::GetID(
				(MAPPED_WINDOW_NAMES.at(windowType) + "- Dockspace")
				.c_str()
			);

		const auto& id{ dockspaceIDs.at(windowType) };

		ImGui::DockBuilderRemoveNode(id);

		// Add dockspace nodes.
		ImGui::DockBuilderAddNode(id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(id, ImGui::GetWindowSize());

		ImGui::DockBuilderDockWindow((MAPPED_WINDOW_NAMES.at(windowType) + " - User Created").c_str(), id);
		ImGui::DockBuilderDockWindow((MAPPED_WINDOW_NAMES.at(windowType) + " - Compendium").c_str(), id);

		ImGui::DockBuilderFinish(id);
		dockspaceInitializations.at(windowType) = true;
	}

	// This is used for both adding monsters and players AND for editing them as well. 
	// So this is a pretty chunky function.
	void EncounterCreationScene::renderEntityCreationPopupModal(bool isMonster, bool editMode) {

		// Set popup configs.
		if (isMonster) {
			if (editMode) {
				currentPopupName = "Monster Editing";
			}
			else {
				currentPopupName = "Monster Creation";
			}
		}
		else {
			if (editMode) {
				currentPopupName = "Player Character Editing";
			}
			else {
				currentPopupName = "Player Character Creation";
			}
		}
		const ImVec2& center{ ImGui::GetMainViewport()->GetCenter() };
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		const ImVec2& viewportSize{ ImGui::GetMainViewport()->Size };
		ImGui::SetNextWindowSize(ImVec2{ viewportSize.x * 0.92f, viewportSize.y * 0.92f });

		// Render the monolithic popup.
		if (ImGui::BeginPopupModal(currentPopupName.c_str(), NULL, popupWindowFlags)) {

			// Scrollable region for the entire popup modal.
			const float footerHeight{ ImGui::GetFrameHeightWithSpacing() * 2.0f };
			ImGui::BeginChild("EntityManagementScroll", ImVec2{ 0, -footerHeight });

			if (CombatTrackerLib::alignedButton("Close Window", 0.99f)) {
				ImGui::CloseCurrentPopup();
			}
			
			// TODO: Properly setup initial values of the entity that is being created/edited.
			if (ImGui::IsWindowAppearing()) {
				if (editMode) {
					if (isMonster) {

					}
					else {

					}
				}
				else {
					if (isMonster) {
						monsterBuilder.reset();
						currentCreatureStatBlock = monsterBuilder.getCreatureBlock();
					}
					else {
						playerCharacterBuilder.reset();
						currentCreatureStatBlock = playerCharacterBuilder.getCreatureBlock();
						currentPlayerStatBlock = playerCharacterBuilder.getPlayerCharacterBlock();
					}
				}

			};

			// Top UI elements, which include are laid out as follow:
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
				if (isMonster) {
					ImGui::SeparatorText("Monster Specific Data");
					renderMonsterSpecificInfo();
				}
				else {
					ImGui::SeparatorText("Classes and Subclasses");
					renderPlayerClassAndSubclasses();
				}
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
				// We take most of the available content height, leaving a bit for separators/buttons.
				const float availY{ ImGui::GetContentRegionAvail().y };
				const float childHeight{ std::max(MIN_CHILD_HEIGHT, availY * 0.46f) };

				// Saving throw data as a separate collapsable header.
				ImGui::TableSetColumnIndex(0);
				if (ImGui::CollapsingHeader("Saving Throws", ImGuiTreeNodeFlags_DefaultOpen)) {

					// Saving Throw proficiency and expertise.
					ImGui::BeginChild("##SaveProfandExp", ImVec2{ 0.0f, childHeight }, true);
					renderProficiencyAndExpertiseCluster(
						"Saving Throw Proficiencies",
						"Saving Throw Expertises",
						currentCreatureStatBlock.savingThrowProficiencies,
						currentCreatureStatBlock.savingThrowExpertise,
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
						currentCreatureStatBlock.skillProficiencies,
						currentCreatureStatBlock.skillExpertise,
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
				currentCreatureStatBlock.conditionImmunities,
				CONDITTON_NAMES, 
				"Condition Immunities"
			);

			ImGui::Spacing();
			ImGui::SeparatorText("Senses & Languages");
			renderMutuallyExclusiveCheckboxCluster(
				currentCreatureStatBlock.languages,
				LANGUAGE_NAMES, 
				"Languages"
			);
			renderSenses();

			// Some parts of the player character specific info goes here.
			ImGui::Spacing();
			if (!isMonster) {
				ImGui::SeparatorText("Player Character Specific Data");
				renderPlayerCharacterSpecificInfo();
			}
			ImGui::Spacing();
			ImGui::SeparatorText("Actions, Traits, Equipment");
			renderActions(isMonster);
			renderAddables(currentCreatureStatBlock.traits, TRAIT_LABEL);
			renderAddables(currentCreatureStatBlock.equipment, EQUIPMENT_LABEL);

			// End scrollable region.
			ImGui::EndChild();

			// TODO: Different "finalization" buttons based upon the context of the popup modal.
			ImGui::Separator();
			if (isMonster) {
				if (editMode) {
					if (CombatTrackerLib::alignedButton("Update Monster", 0.5f)) {

					}
				}
				else {
					if (CombatTrackerLib::alignedButton("Complete Monster Creation", 0.5f)) {
		
					}
				}
			}
			else {
				if (editMode) {
					if (CombatTrackerLib::alignedButton("Update Player Character", 0.5f)) {

					}
				}
				else {
					if (CombatTrackerLib::alignedButton("Complete Player Character Creation", 0.5f)) {

					}
				}
			}
			ImGui::EndPopup();
		}
	}

	// Generic helper for single selectable listbox layouts.
	// Used for creature type, alignment, size, monster CR, action type during action creation.
	template <typename T, typename M>
	void EncounterCreationScene::renderListBox(
		ListBoxParameters& parameters,
		T& type,
		const char* listBoxLabel,
		const M& items
	) {
		int counter{};
		const float height{ ImGui::GetTextLineHeightWithSpacing() * 8.0f };
		if (ImGui::BeginListBox(listBoxLabel, ImVec2{ 0, height })) {
			for (const auto& item : items) {
				const bool isSelected{ counter == parameters.chosenIndex };
				if (ImGui::Selectable(item.second.c_str(), isSelected)) {
					parameters.chosenIndex = counter;
					type = item.first;
				}
				if (parameters.isHighlighted && ImGui::IsItemHovered()) {
					parameters.highlightedIndex = counter;
				}
				++counter;
			}
			ImGui::EndListBox();
		}
	}
	
	// Simple helper method used for user input of HP, AC, movement speed and sense distances.
	void EncounterCreationScene::renderNonOverridableInputInt(int& value, const char* label) {
		if (ImGui::InputInt(label, &value)) {
			value = std::max(0, value);
		}
	}

	/*
	A slightly more complicated helper used for user input of monster XP and PB.
	The common thing about these two types is that they set mapped values.
	Various other overridable user input can also be put in this format.
	But mapping these types is meaningless as their mapping is quite simple.
	Example: Initiative Bonus: Dexterity Modifier, Passive Perception: Perception Modifier + 10
	*/
	template <typename T, typename M>
	void EncounterCreationScene::renderOverriddableInputInt (
		int& value,
		bool& overrideFlag,
		const char* label,
		const char* overrideLabel,
		const M& items,
		const int lowerBound,
		const int upperBound,
		const T key
	) {
		if (!overrideFlag) {
			value = items.at(key);
		}
		if (ImGui::InputInt(label, &value)) {
			value = std::min(std::max(lowerBound, value), upperBound);
			overrideFlag = true;
		}
		ImGui::SameLine();
		if (ImGui::Button(overrideLabel)) {
			overrideFlag = false;
		}
	}
	/*
	Used for rendering skill and saving throw proficiencies and expertises for both monsters and player characters.
	The two clusters have a simple deduping logic between them. 
	We can't have proficiency and expertise of the same "thing" at the same time.
	*/
	template <typename M, typename N>
	void EncounterCreationScene::renderProficiencyAndExpertiseCluster (
		const char* proficiencyLabel,
		const char* expertiseLabel,
		M& proficiencyItems,
		M& expertiseItems,
		const N& itemNames
	) {
		if (ImGui::BeginTable("ProfExpTable", 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV)) {

			ImGui::TableSetupColumn("Left");
			ImGui::TableSetupColumn("Right");
			ImGui::TableNextRow();

			// Proficiency checkboxes.
			ImGui::TableSetColumnIndex(0);
			ImGui::TextWrapped("%s", proficiencyLabel);
			for (const auto& item : itemNames) {
				if (ImGui::Checkbox((item.second + "##" + PROFICIENCY_SUFFIX).c_str(), &proficiencyItems.at(item.first))) {
					if (proficiencyItems.at(item.first) && expertiseItems.at(item.first)) {
						expertiseItems.at(item.first) = false;
					}
				}
			}

			// Expertise checkboxes.
			ImGui::TableSetColumnIndex(1);
			ImGui::TextWrapped("%s", expertiseLabel);
			for (const auto& item : itemNames) {
				if (ImGui::Checkbox((item.second + "##" + EXPERTISE_SUFFIX).c_str(), &expertiseItems.at(item.first))) {
					if (expertiseItems.at(item.first) && proficiencyItems.at(item.first)) {
						proficiencyItems.at(item.first) = false;
					}
				}
			}
			ImGui::EndTable();
		}
	}
	void EncounterCreationScene::renderAbilityScoresAndModifiers () {
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
			for (auto& item : currentCreatureStatBlock.abilityScores) {
				ImGui::TableNextRow();

				// Compute and show default ability score modifiers.
				if (!abilityModifiersOverride.at(item.first)) {
					currentCreatureStatBlock.abilityScoreModifiers.at(item.first) = Creature::findAbilityScoreModifier(item.second);
				}

				ImGui::TableSetColumnIndex(0);
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
					&currentCreatureStatBlock.abilityScoreModifiers.at(item.first))
					) {
					currentCreatureStatBlock.abilityScoreModifiers.at(item.first) = 
						std::max(std::min(currentCreatureStatBlock.abilityScoreModifiers.at(item.first), 
							ABILITY_MODIFIERS_UPPER_BOUND), ABILITY_MODIFIERS_LOWER_BOUND);
					abilityModifiersOverride.at(item.first) = true;
				}

				ImGui::TableSetColumnIndex(3);
				if (ImGui::Button(("Reset " + ABILITY_NAMES.at(item.first) + " Modifier").c_str())) {
					abilityModifiersOverride.at(item.first) = false;
				}
			}
			ImGui::EndTable();
		}
	}
	void EncounterCreationScene::renderSavingThrowModifiers () {
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
				if (!savingThrowsOverride.at(item.first)) {
					if (currentCreatureStatBlock.savingThrowExpertise.at(item.first)) {
						currentCreatureStatBlock.savingThrowModifiers.at(item.first) = 
							currentCreatureStatBlock.abilityScoreModifiers.at(item.first) + 
							2 * currentCreatureStatBlock.proficiencyBonus;
					}
					else if (currentCreatureStatBlock.savingThrowProficiencies.at(item.first)) {
						currentCreatureStatBlock.savingThrowModifiers.at(item.first) = 
							currentCreatureStatBlock.abilityScoreModifiers.at(item.first) + 
							currentCreatureStatBlock.proficiencyBonus;
					}
					else {
						currentCreatureStatBlock.savingThrowModifiers.at(item.first) =
							currentCreatureStatBlock.abilityScoreModifiers.at(item.first);
					}
				}
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted(item.second.c_str());

				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1);
				if (ImGui::InputInt(("##" + item.second + "Saving Throw Modifiers").c_str(), 
					&currentCreatureStatBlock.savingThrowModifiers.at(item.first)
				)) {

					// Setup no bounds for saving throw modifiers as their values can get quite large, low depending upon expertise and proficiency.
					// The "largeness" depends on PB. As the PB can be overridden to be quite large, the modifiers can bu quite large.
					currentCreatureStatBlock.savingThrowModifiers.at(item.first) = currentCreatureStatBlock.savingThrowModifiers.at(item.first);
					savingThrowsOverride.at(item.first) = true;
				}

				ImGui::TableSetColumnIndex(2);
				if (ImGui::Button(("Reset " + item.second + " Saving Throw Modifier").c_str())) {
					savingThrowsOverride.at(item.first) = false;
				}
			}
			ImGui::EndTable();
		}
	}
	void EncounterCreationScene::renderSkillModifiers () {

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
				if (!skillModifiersOverride.at(item.first)) {
					if (currentCreatureStatBlock.skillExpertise.at(item.first)) {
						currentCreatureStatBlock.skillModifiers.at(item.first) =
							currentCreatureStatBlock.abilityScoreModifiers.at(MAPPED_SKILLS.at(item.first)) + 
							2 * currentCreatureStatBlock.proficiencyBonus;
					}
					else if (currentCreatureStatBlock.skillProficiencies.at(item.first)) {
						currentCreatureStatBlock.skillModifiers.at(item.first) =
							currentCreatureStatBlock.abilityScoreModifiers.at(MAPPED_SKILLS.at(item.first)) 
							+ currentCreatureStatBlock.proficiencyBonus;
					}
					else {
						currentCreatureStatBlock.skillModifiers.at(item.first) =
							currentCreatureStatBlock.abilityScoreModifiers.at(MAPPED_SKILLS.at(item.first));
					}
				}

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted(item.second.c_str());

				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1);
				if (ImGui::InputInt(("##" + item.second + "Skill Modifier").c_str(), 
					&currentCreatureStatBlock.skillModifiers.at(item.first)
				)) {

					// Exact same reasoning as saving throw modifiers is applicable to here.
					currentCreatureStatBlock.skillModifiers.at(item.first) = currentCreatureStatBlock.skillModifiers.at(item.first);
					skillModifiersOverride.at(item.first) = true;
				}

				ImGui::TableSetColumnIndex(2);
				if (ImGui::Button(("Reset " + item.second + " Skill Modifier").c_str())) {
					skillModifiersOverride.at(item.first) = false;
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
	void EncounterCreationScene::renderGeneralCreatureInfo() {

		if (ImGui::BeginTable("##GeneralCreatureInfo_NameRow", 2, ImGuiTableFlags_SizingStretchProp)) {
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 70.0f);
			ImGui::TableSetupColumn("Field", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted("Name");

			ImGui::TableSetColumnIndex(1);
			ImGui::SetNextItemWidth(-1.0f);
			ImGui::InputText("##Name", &currentCreatureStatBlock.name);

			ImGui::EndTable();
		}

		ImGui::Spacing();

		if (ImGui::BeginTable("##GeneralCreatureInfo_MainSplit", 2, ImGuiTableFlags_SizingStretchProp)) {

			// Give right side a reasonable fixed width so it never spills into the Entity Specific column.
			const float avail{ ImGui::GetContentRegionAvail().x };
			const float rightW{ std::min(300.0f, std::max(240.0f, avail * 0.30f)) };

			ImGui::TableSetupColumn("Left", ImGuiTableColumnFlags_WidthStretch, 1.0f);
			ImGui::TableSetupColumn("Right", ImGuiTableColumnFlags_WidthFixed, rightW);

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			if (ImGui::BeginTable("##GeneralCreatureInfo_Listboxes", 3, ImGuiTableFlags_SizingStretchProp)) {
				ImGui::TableSetupColumn("SizeCol", ImGuiTableColumnFlags_WidthStretch, 0.33f);
				ImGui::TableSetupColumn("TypeCol", ImGuiTableColumnFlags_WidthStretch, 0.34f);
				ImGui::TableSetupColumn("AlignmentCol", ImGuiTableColumnFlags_WidthStretch, 0.33f);

				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted("Size");
				renderListBox(sizeListBoxParameters, currentCreatureStatBlock.size, "##Size", SIZE_NAMES);

				ImGui::TableSetColumnIndex(1);
				ImGui::TextUnformatted("Creature Type");
				renderListBox(creatureTypeParameters, currentCreatureStatBlock.type, "##CreatureType", CREATURE_TYPE_NAMES);

				ImGui::TableSetColumnIndex(2);
				ImGui::TextUnformatted("Alignment");
				renderListBox(alignmentParameters, currentCreatureStatBlock.alignment, "##Alignment", ALIGNMENT_NAMES);

				ImGui::EndTable();
			}

			ImGui::TableSetColumnIndex(1);

			if (!isInitiativeOverriden) {
				currentCreatureStatBlock.initiativeBonus = currentCreatureStatBlock.abilityScoreModifiers.at(Ability::DEXTERITY);
			}

			// Hide the label of the input field. Instead use text field for the label for better layout.
			ImGui::TextUnformatted("Initiative Bonus");
			ImGui::SetNextItemWidth(-1.0f);
			if (ImGui::InputInt("##InitiativeBonus", &currentCreatureStatBlock.initiativeBonus, 0, 0)) {
				currentCreatureStatBlock.initiativeBonus =
					std::min(std::max(ABILITY_SCORE_LOWER_BOUND, currentCreatureStatBlock.initiativeBonus),
						ABILITY_MODIFIERS_UPPER_BOUND);
				isInitiativeOverriden = true;
			}
			ImGui::SetNextItemWidth(-1.0f);
			if (ImGui::Button("Reset Initiative Bonus", ImVec2{ -1.0f, 0.0f })) {
				isInitiativeOverriden = false;
			}

			ImGui::Spacing();
			if (!isPassivePerceptionOverridden) {
				currentCreatureStatBlock.passivePerception = currentCreatureStatBlock.skillModifiers.at(Skill::PERCEPTION) + 10;
			}

			// Same thing here.
			ImGui::TextUnformatted("Passive Perception");
			ImGui::SetNextItemWidth(-1.0f);
			if (ImGui::InputInt("##PassivePerception", &currentCreatureStatBlock.passivePerception, 0, 0)) {
				currentCreatureStatBlock.passivePerception =
					std::min(std::max(10 + ABILITY_MODIFIERS_UPPER_BOUND, currentCreatureStatBlock.passivePerception),
						10 + ABILITY_MODIFIERS_LOWER_BOUND);
				isPassivePerceptionOverridden = true;
			}
			ImGui::SetNextItemWidth(-1.0f);
			if (ImGui::Button("Reset Passive Perception", ImVec2{ -1.0f, 0.0f })) {
				isPassivePerceptionOverridden = false;
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
				ImGui::TableSetupColumn("ACLabel", ImGuiTableColumnFlags_WidthFixed, 30.0f);
				ImGui::TableSetupColumn("ACField", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted("AC");

				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1.0f);
				renderNonOverridableInputInt(currentCreatureStatBlock.AC, "##AC");

				ImGui::EndTable();
			}

			// HP inner cell.
			ImGui::TableSetColumnIndex(1);
			if (ImGui::BeginTable("##HPRow", 2, ImGuiTableFlags_SizingStretchProp)) {
				ImGui::TableSetupColumn("HPLabel", ImGuiTableColumnFlags_WidthFixed, 30.0f);
				ImGui::TableSetupColumn("HPField", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted("HP");

				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1.0f);
				renderNonOverridableInputInt(currentCreatureStatBlock.HP, "##HP");

				ImGui::EndTable();
			}
			ImGui::EndTable();
		}
	}

	// Monster specific info includes CR, XP and PB.
	// Even though PB isn't specific to monsters, how it is computed is different for monsters and player characters.
	void EncounterCreationScene::renderMonsterSpecificInfo () {
		if (ImGui::BeginTable("MonsterSpecificGrid", 1, ImGuiTableFlags_SizingStretchSame)) {

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			renderListBox(monsterCRParameters, currentMonsterCR, "CR", CR_NAMES);

			ImGui::Spacing();
			renderOverriddableInputInt(currentMonsterXP, isXPOverridden, "XP", "Reset XP", MAPPED_CR_TO_XP, 0, INT_MAX, currentMonsterCR);

			ImGui::Spacing();
			renderOverriddableInputInt(currentCreatureStatBlock.proficiencyBonus, isPBOverridden, "PB", "Reset PB", MAPPED_CR_TO_PB, PB_LOWER_BOUND, INT_MAX, currentMonsterCR);

			ImGui::EndTable();
		}
	}
	void EncounterCreationScene::renderSpeedInfo () {
		if (ImGui::BeginTable("SpeedGrid", 5, ImGuiTableFlags_SizingStretchSame)) {
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			renderNonOverridableInputInt(currentCreatureStatBlock.walkSpeed, "Walk");

			ImGui::TableSetColumnIndex(1);
			renderNonOverridableInputInt(currentCreatureStatBlock.climbingSpeed, "Climb");

			ImGui::TableSetColumnIndex(2);
			renderNonOverridableInputInt(currentCreatureStatBlock.swimmingSpeed, "Swim");

			ImGui::TableSetColumnIndex(3);
			renderNonOverridableInputInt(currentCreatureStatBlock.burrowSpeed, "Burrow");

			ImGui::TableSetColumnIndex(4);
			renderNonOverridableInputInt(currentCreatureStatBlock.flySpeed, "Fly");

			ImGui::EndTable();
		}
	}
	void EncounterCreationScene::renderDmgVulnResAndImm () {
		if (ImGui::BeginTable("DmgTri", 3, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV)) {
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::TextWrapped("Vulnerabilities");
			for (const auto& item : DAMAGE_TYPE_NAMES) {
				if (ImGui::Checkbox((item.second + " ##Vuln").c_str(), 
					&currentCreatureStatBlock.damageVulnerabilities.at(item.first)
				)) {

					// Triple simple deduping logic.
					if (currentCreatureStatBlock.damageVulnerabilities.at(item.first) 
						&& currentCreatureStatBlock.damageResistances.at(item.first)) {
						currentCreatureStatBlock.damageResistances.at(item.first) = false;
					}
					if (currentCreatureStatBlock.damageVulnerabilities.at(item.first) 
						&& currentCreatureStatBlock.damageImmunities.at(item.first)) {
						currentCreatureStatBlock.damageImmunities.at(item.first) = false;
					}
				}
			}

			ImGui::TableSetColumnIndex(1);
			ImGui::TextWrapped("Resistances");
			for (const auto& item : DAMAGE_TYPE_NAMES) {
				if (ImGui::Checkbox((item.second + " ##Res").c_str(), 
					&currentCreatureStatBlock.damageResistances.at(item.first)
				)) {

					// Exact same logic here.
					if (currentCreatureStatBlock.damageVulnerabilities.at(item.first) 
						&& currentCreatureStatBlock.damageResistances.at(item.first)) {
						currentCreatureStatBlock.damageVulnerabilities.at(item.first) = false;
					}
					if (currentCreatureStatBlock.damageResistances.at(item.first) 
						&& currentCreatureStatBlock.damageImmunities.at(item.first)) {
						currentCreatureStatBlock.damageImmunities.at(item.first) = false;
					}
				}
			}

			ImGui::TableSetColumnIndex(2);
			ImGui::TextWrapped("Immunities");
			for (auto& item : DAMAGE_TYPE_NAMES) {
				if (ImGui::Checkbox((item.second + " ##Imm").c_str(), 
					&currentCreatureStatBlock.damageImmunities.at(item.first)
				)) {

					// Same here.
					if (currentCreatureStatBlock.damageVulnerabilities.at(item.first) 
						&& currentCreatureStatBlock.damageImmunities.at(item.first)) {
						currentCreatureStatBlock.damageVulnerabilities.at(item.first) = false;
					}
					if (currentCreatureStatBlock.damageResistances.at(item.first) 
						&& currentCreatureStatBlock.damageImmunities.at(item.first)) {
						currentCreatureStatBlock.damageResistances.at(item.first) = false;
					}
				}
			}
			ImGui::EndTable();
		}
	}

	// Simple helper method for rendering multi-selectable creature parameters.
	// Used in condition immunities and languages.
	template <typename M, typename N>
	void EncounterCreationScene::renderMutuallyExclusiveCheckboxCluster(M& items, const N& names, const char* label) {
		ImGui::TextWrapped("%s", label);
		if (ImGui::BeginTable(label, 3, ImGuiTableFlags_SizingStretchSame)) {
			int counter{};
			for (const auto& item : names) {
				ImGui::TableNextColumn();
				ImGui::Checkbox(item.second.c_str(), &items.at(item.first));
				++counter;
			}
			ImGui::EndTable();
		}
	}

	// Specific function for sense rendering, just calls renderNonOverridableInputInt() for all senses.
	void EncounterCreationScene::renderSenses() {
		if (ImGui::BeginTable("SensesGrid", 4, ImGuiTableFlags_SizingStretchSame)) {
			ImGui::TableNextRow();
			for (auto& item : currentCreatureStatBlock.senses) {
				ImGui::TableNextColumn();
				renderNonOverridableInputInt(item.second, SENSE_NAMES.at(item.first).c_str());
			}
			ImGui::EndTable();
		}
	}

	// Chunky helper used for rendering traits and gear items.
	template <typename I>
	void EncounterCreationScene::renderAddables(std::vector<I>& currentItems, const std::string& label) {
		ImGui::TextWrapped("%s", label.c_str());

		// It is fine for this to be actually static as we clear it on each render pass anyways.
		// Furthermore, this vector is only used in this function and nowhere else.
		static std::vector<size_t> markedForDeletion{};
		for (size_t i{}; i < currentItems.size(); ++i) {
			ImGui::TableNextColumn();

			// Cache the string to prevent recomputing it.
			std::string popupName{ ("Edit " + label + "##" + std::to_string(i)).c_str() };

			// Push a separate id to the ID stack for each item.
			ImGui::PushID(static_cast<int> (i));

			// We should check separate IO events explicitly instead of generically checking for any IO events for these buttons.
			// Unlike most of the widgets. This is because we need a tooltip for each button.
			ImGui::Button(currentItems[i].name.c_str());
			ImGui::SameLine();
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				ImGui::SetTooltip("%s", currentItems[i].description.c_str());
			}
			if (ImGui::IsItemClicked()) {
				ImGui::OpenPopup(popupName.c_str());
			}

			// Setup configs of the edit popup of items.
			const ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2{ 0.5f, 0.5f });
			const ImVec2 vp{ ImGui::GetMainViewport()->Size };
			ImGui::SetNextWindowSize(ImVec2{ vp.x * 0.70f, vp.y * 0.70f }, ImGuiCond_Appearing);

			// Popup modal for editing/deleting items.				
			if (ImGui::BeginPopupModal(popupName.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {

				// The layouting is quite similar to the big entity creation popup modal.
				ImGui::BeginChild("EditAddableScroll", ImVec2{ 0, -ImGui::GetFrameHeightWithSpacing() * 2.0f }, true);
				ImGui::InputText((label + " Name").c_str(), &currentItems[i].name);
				ImGui::InputText((label + " Description").c_str(), &currentItems[i].description);
				ImGui::EndChild();

				ImGui::Separator();
				if (CombatTrackerLib::alignedButton(("Finish " + label + " Editing").c_str(), 0.5f)) {
					ImGui::CloseCurrentPopup();
				}
				if (CombatTrackerLib::alignedButton(("Delete " + label).c_str(), 0.5f)) {
					markedForDeletion.push_back(i);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}

		// Delete marked items safely, starting from the largest index to the smallest index.
		std::sort(markedForDeletion.begin(), markedForDeletion.end());
		for (size_t i = markedForDeletion.size(); i-- > 0;) {
			currentItems.erase(currentItems.begin() + markedForDeletion[i]);
		}
		markedForDeletion.clear();

		// Add item section.
		ImGui::Spacing();
		if (ImGui::Button(("Add " + label).c_str())) {
			ImGui::OpenPopup(label.c_str());
		}

		// Setup configs of the add item popup.
		ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2{ 0.5f, 0.5f });
		ImVec2 vp{ ImGui::GetMainViewport()->Size };
		ImGui::SetNextWindowSize(ImVec2{ vp.x * 0.70f, vp.y * 0.70f }, ImGuiCond_Appearing);
		if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
			ImGui::BeginChild("AddAddableScroll", ImVec2{ 0, -ImGui::GetFrameHeightWithSpacing() * 2.0f }, true);

			// Again it is fine for these to be static, we only use them here and reset them after adding the item.
			static std::string name{};
			static std::string description{};
			ImGui::InputText((label + " Name").c_str(), &name);
			ImGui::InputText((label + " Description").c_str(), &description);
			ImGui::EndChild();

			// Button and cleanup section.
			ImGui::Separator();
			if (CombatTrackerLib::alignedButton(("Finish " + label + " Creation").c_str(), 0.5f) &&
				!name.empty() && !description.empty()) {
				I item(name, description);
				currentItems.push_back(item);
				name.clear();
				description.clear();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	// Really similar to renderAddables() just adds actionType as a separate parameter. 
	void EncounterCreationScene::renderActions(bool isMonster) {
		ImGui::TextWrapped("Actions");
		static std::vector<size_t> actionsMarkedForDeletion{};
		for (size_t i{}; i < currentCreatureStatBlock.actions.size(); ++i) {
			ImGui::TableNextColumn();
			ImGui::PushID(static_cast<int> (i));

			ImGui::SetNextItemWidth(-1.0f);
			ImGui::Button(currentCreatureStatBlock.actions[i].name.c_str());
			ImGui::SameLine();
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				ImGui::SetTooltip("%s: %s", ACTION_NAMES.at(currentCreatureStatBlock.actions[i].type).c_str(), currentCreatureStatBlock.actions[i].description.c_str());
			}
			if (ImGui::IsItemClicked()) {
				ImGui::OpenPopup(("Edit Action##" + std::to_string(i)).c_str());
			}

			ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2{ 0.5f, 0.5f });
			ImVec2 vp{ ImGui::GetMainViewport()->Size };
			ImGui::SetNextWindowSize(ImVec2{ vp.x * 0.70f, vp.y * 0.70f }, ImGuiCond_Appearing);
			if (ImGui::BeginPopupModal(("Edit Action##" + std::to_string(i)).c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
				ImGui::BeginChild("EditActionScroll", ImVec2{ 0, -ImGui::GetFrameHeightWithSpacing() * 2.0f }, true);
				ImGui::InputText("Action Name", &currentCreatureStatBlock.actions[i].name);
				ImGui::InputText("Action Description", &currentCreatureStatBlock.actions[i].description);
				ImGui::EndChild();

				ImGui::Separator();
				if (CombatTrackerLib::alignedButton("Finish Action Editing", 0.5f)) {
					ImGui::CloseCurrentPopup();
				}
				if (CombatTrackerLib::alignedButton("Delete Action", 0.5f)) {
					actionsMarkedForDeletion.push_back(i);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}

		std::sort(actionsMarkedForDeletion.begin(), actionsMarkedForDeletion.end());
		for (size_t i{ actionsMarkedForDeletion.size() }; i-- > 0;) {
			currentCreatureStatBlock.actions.erase(currentCreatureStatBlock.actions.begin() + actionsMarkedForDeletion[i]);
		}
		actionsMarkedForDeletion.clear();
		
		// Action adding popup modal.
		ImGui::Spacing();
		if (ImGui::Button("Add Action")) {
			ImGui::OpenPopup("Action");
		}
		const ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2{ 0.5f, 0.5f });
		const ImVec2 vp{ ImGui::GetMainViewport()->Size };
		ImGui::SetNextWindowSize(ImVec2(vp.x * 0.70f, vp.y * 0.70f), ImGuiCond_Appearing);
		if (ImGui::BeginPopupModal("Action", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
			ImGui::BeginChild("##AddActionScroll", ImVec2{ 0, -ImGui::GetFrameHeightWithSpacing() * 2.0f }, true);

			// Again, fine for these to be static as they are reset after each addition of actions.
			static ListBoxParameters actionParameters{ -1, -1, false };
			static ActionType type{};

			// Important logic to prevent PCs from creating Legendary Actions and Lair Actions.
			if (isMonster) {
				renderListBox(actionParameters, type, "Action Type", ACTION_NAMES);
			}
			else {
				renderListBox(actionParameters, type, "Action Type", PLAYER_ACTION_NAMES);
			}

			static std::string name{};
			static std::string description{};
			ImGui::InputText("Action Name", &name);
			ImGui::InputText("Action Description", &description);
			ImGui::EndChild();

			ImGui::Separator();
			if (CombatTrackerLib::alignedButton("Finish Action Creation", 0.5f) &&
				!name.empty() && !description.empty() && type != ActionType::NO_ACTION) {
				CreatureAction action{ name, description, type };
				currentCreatureStatBlock.actions.push_back(action);
				actionParameters = { -1, -1, false };
				type = { ActionType::NO_ACTION };
				name.clear();
				description.clear();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	void EncounterCreationScene::renderPlayerCharacterSpecificInfo() {
		renderListBox(speciesParameters, currentPlayerStatBlock.species, "Species", SPECIES_NAMES);
		renderListBox(originParameters, currentPlayerStatBlock.origin, "Origin", ORIGIN_NAMES);
		renderMutuallyExclusiveCheckboxCluster(currentPlayerStatBlock.weaponProficiencies, WEAPON_TYPE_NAMES, "Weapon Proficiencies");
		renderMutuallyExclusiveCheckboxCluster(currentPlayerStatBlock.armorProficiencies, ARMOR_TYPE_NAMES, "Armor Proficiencies");
		renderMutuallyExclusiveCheckboxCluster(currentPlayerStatBlock.toolProficiencies, TOOL_NAMES, "Tool Proficiencies");
		renderMutuallyExclusiveCheckboxCluster(currentPlayerStatBlock.weaponMasteries, WEAPON_MASTERY_NAMES, "Weapon Masteries");
	}
	void EncounterCreationScene::renderPlayerClassAndSubclasses() {
		if (ImGui::BeginTable("Classes and Subclasses", 1, ImGuiTableFlags_SizingStretchSame)) {

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			for (const auto& item : currentPlayerStatBlock.levels) {
				if (item.second == 0) {
					continue;
				}
				ImGui::TextWrapped("%s %d", CLASS_NAMES.at(item.first).c_str(), item.second);
			}
			for (const auto& item : currentPlayerStatBlock.subclasses) {
				ImGui::TextWrapped("%s", SUBCLASS_NAMES.at(item.second).c_str() );
			}
			if (CombatTrackerLib::alignedButton("Configure Classes and Subclasses", 0.5f)) {
				ImGui::OpenPopup("Configuration");
			}
			const ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2{ 0.5f, 0.5f });
			const ImVec2 viewportSize{ ImGui::GetMainViewport()->Size };
			ImGui::SetNextWindowSize(ImVec2{ viewportSize.x * 0.92f, viewportSize.y * 0.92f });
			if (ImGui::BeginPopupModal("Configuration", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
				ImGui::BeginChild("EditClassScroll", ImVec2{ 0, -ImGui::GetFrameHeightWithSpacing() * 2.0f }, true);

				
				ImGui::TextWrapped("Classes");
				for (const auto& item : CLASS_NAMES) {
					renderNonOverridableInputInt(currentPlayerStatBlock.levels.at(item.first), item.second.c_str());
				}

				static PlayerClass currentClass{};
				ImGui::SeparatorText("Subclasses");
				for (const auto& item : SUBCLASS_NAMES) {
					if (currentClass != MAPPED_SUBCLASSES.at(item.first)) {
						ImGui::SeparatorText(CLASS_NAMES.at(MAPPED_SUBCLASSES.at(item.first)).c_str());
					}
					if (currentPlayerStatBlock.levels.at(MAPPED_SUBCLASSES.at(item.first)) < SUBCLASS_LEVEL_THRESHOLD) {
						chosenSubclasses.at(item.first) = false;
						currentPlayerStatBlock.subclasses.erase(MAPPED_SUBCLASSES.at(item.first));
					}
					if (ImGui::Checkbox(item.second.c_str(), &chosenSubclasses.at(item.first))) {
						hasCorrespondingSubclass.at(MAPPED_SUBCLASSES.at(item.first)) = true;
						currentPlayerStatBlock.subclasses[MAPPED_SUBCLASSES.at(item.first)] = item.first;
						for (const auto& secondItem : MAPPED_CLASSES.at(MAPPED_SUBCLASSES.at(item.first))) {
							if ((chosenSubclasses.at(secondItem) && secondItem != item.first)
								|| currentPlayerStatBlock.levels.at(MAPPED_SUBCLASSES.at(secondItem)) < SUBCLASS_LEVEL_THRESHOLD )  
							{
								chosenSubclasses.at(secondItem) = false;
							}
						}
					}
					currentClass = MAPPED_SUBCLASSES.at(item.first);
				}
				ImGui::EndChild();

				if (CombatTrackerLib::alignedButton("Finish Class and Subclass Configuration", 0.5f)) {

					// Lengthy but necessary validation.
					int sum{};
					bool isValid{ false };
					for (auto& item : currentPlayerStatBlock.levels) {
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

	// Call this after creating/completing editing of Monsters/Player Characters.
	void EncounterCreationScene::resetParametersAndOverrides() {
		monsterCRParameters = { -1, -1, false };
		sizeListBoxParameters = { -1, -1, false };
		creatureTypeParameters = { -1, -1, false };
		alignmentParameters = { -1, -1, false };
		speciesParameters = { -1, -1, false };
		originParameters = { -1, -1, false };
		isInitiativeOverriden = false;
		isPassivePerceptionOverridden = false;
		isXPOverridden = false;
		isPBOverridden = false;
		for (const auto& ability : ABILITY_NAMES) {
			abilityModifiersOverride[ability.first] = false;
			savingThrowsOverride[ability.first] = false;
		}
		for (const auto& skill : SKILL_NAMES) {
			skillModifiersOverride[skill.first] = false;
		}
	}
}