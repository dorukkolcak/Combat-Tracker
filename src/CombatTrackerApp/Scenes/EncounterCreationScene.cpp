#include "EncounterCreationScene.h"
#include "MainMenuScene.h"
#include "../Entities/Creature/CreatureConstants.h"
#include "../Entities/Creature/Creature.h"
#include "../Entities/Monster/MonsterConstants.h"

#include "imgui_internal.h"
#include "Imgui/misc/cpp/imgui_stdlib.h" // std::string support


namespace CombatTrackerApp {
	EncounterCreationScene::EncounterCreationScene(CombatTrackerLib::App* app) :
		CombatTrackerLib::Scene(app)
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
	}
	EncounterCreationScene::~EncounterCreationScene() {}

	void EncounterCreationScene::notifyScene(ImGuiIO& imguiIO) {}
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
			ImGuiID leftID;
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
		ImGui::Begin(MAPPED_WINDOW_NAMES.at(MENU).c_str());
		if (ImGui::Button("Return to Main Menu")) {
			applicationReference->queueSceneTransition(this, std::make_unique<MainMenuScene>(applicationReference));
		}
		ImGui::End();
	}
	void EncounterCreationScene::renderEntityBrowserWindow() {

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

		ImGui::Begin(MAPPED_WINDOW_NAMES.at(MONSTER_BROWSER).c_str());
		if (CombatTrackerLib::Scene::centeredButton("Create Monster")) {

			ImGui::OpenPopup("Monster Creation");
		}
		if (!dockspaceInitializations.at(MONSTER_BROWSER)) {
			initializeSubBrowserDockspace(MONSTER_BROWSER);
		}
		ImGui::DockSpace(dockspaceIDs.at(MONSTER_BROWSER), ImGui::GetWindowSize());

		/*
		IMPORTANT: The popup modal rendering code should be precisely within the rendering code of its parent window.
		Otherwise it won't be rendered at all, as it exists in a different ID context.
		See https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-about-the-id-stack-system.
		TODO: Finish the popup modal UI and integrate JSON saving functionality.
		*/
		renderMonsterCreationPopupModal();

		ImGui::End();

		// TODO: Finalize UI design after "backend" stuff is done.
		ImGui::Begin((MAPPED_WINDOW_NAMES.at(MONSTER_BROWSER) + " - User Created").c_str());

		ImGui::End();

		ImGui::Begin((MAPPED_WINDOW_NAMES.at(MONSTER_BROWSER) + " - Compendium").c_str());

		ImGui::End();
	}
	void EncounterCreationScene::renderPlayerCharacterBrowserWindow() {
		// Player character browser window.
		ImGui::Begin(MAPPED_WINDOW_NAMES.at(PLAYER_CHARACTER_BROWSER).c_str());
		if (!dockspaceInitializations.at(PLAYER_CHARACTER_BROWSER)) {
			initializeSubBrowserDockspace(PLAYER_CHARACTER_BROWSER);
		}

		if (CombatTrackerLib::Scene::centeredButton("Create Player Character")) {
			ImGui::BeginPopupModal("Player Character Creation");
		}

		ImGui::DockSpace(dockspaceIDs.at(PLAYER_CHARACTER_BROWSER), ImGui::GetWindowSize());

		// Same exact thing applies to this popup modal as well.
		// TODO: Finish the popup modal UI and integrate JSON saving functionality.
		if (ImGui::BeginPopupModal("Player Character Creation", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {

			ImGui::EndPopup();;
		}

		ImGui::End();

		// TODO: Finalize UI design after "backend" stuff is done.
		ImGui::Begin((MAPPED_WINDOW_NAMES.at(PLAYER_CHARACTER_BROWSER) + " - User Created").c_str());

		ImGui::End();

		ImGui::Begin((MAPPED_WINDOW_NAMES.at(PLAYER_CHARACTER_BROWSER) + " - Compendium").c_str());

		ImGui::End();
	}
	void EncounterCreationScene::renderEncounterBrowserWindow() {

		ImGui::Begin(MAPPED_WINDOW_NAMES.at(ENCOUNTER_BROWSER).c_str());
		if (!dockspaceInitializations.at(ENCOUNTER_BROWSER)) {
			initializeSubBrowserDockspace(ENCOUNTER_BROWSER);
		}
		ImGui::DockSpace(dockspaceIDs.at(ENCOUNTER_BROWSER), ImGui::GetWindowSize());
		ImGui::End();

		// TODO: Finalize UI design after "backend" stuff is done.
		ImGui::Begin((MAPPED_WINDOW_NAMES.at(ENCOUNTER_BROWSER) + " - User Created").c_str());

		ImGui::End();

		ImGui::Begin((MAPPED_WINDOW_NAMES.at(ENCOUNTER_BROWSER) + " - Compendium").c_str());

		ImGui::End();

	}
	void EncounterCreationScene::renderEncounterManagerWindow() {

		// TODO: Encounter manager window. Finalize UI design after "backend" stuff is done.
		ImGui::Begin(MAPPED_WINDOW_NAMES.at(ENCOUNTER_MANAGER).c_str());
		ImGui::End();
	}
	void EncounterCreationScene::initializeSubBrowserDockspace(EncounterCreationWindows windowType) {

		dockspaceIDs[windowType] =
			ImGui::GetID(
				(MAPPED_WINDOW_NAMES.at(windowType) + "- Dockspace")
				.c_str()
			);

		auto& id = dockspaceIDs.at(windowType);

		ImGui::DockBuilderRemoveNode(id);

		// Add dockspace nodes.
		ImGui::DockBuilderAddNode(id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(id, ImGui::GetWindowSize());

		ImGui::DockBuilderDockWindow((MAPPED_WINDOW_NAMES.at(windowType) + " - User Created").c_str(), id);
		ImGui::DockBuilderDockWindow((MAPPED_WINDOW_NAMES.at(windowType) + " - Compendium").c_str(), id);

		ImGui::DockBuilderFinish(id);
		dockspaceInitializations.at(windowType) = true;
	}

	void EncounterCreationScene::renderMonsterCreationPopupModal() {

		// Set popup configs. 
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size);
		if (ImGui::BeginPopupModal("Monster Creation", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {

			static std::string name;
			ImGui::InputText("Name", &name);

			static ListBoxParameters monsterListBoxParameters(-1, -1, false);
			static Size monsterSize{ Size::NO_SIZE };
			renderListBox(monsterListBoxParameters, monsterSize, "Size", SIZE_NAMES);

			ImGui::SameLine();
			static ListBoxParameters monsterCreatureTypeParameters(-1, -1, false);
			static CreatureType monsterType{ CreatureType::NO_TYPE };
			renderListBox(monsterCreatureTypeParameters, monsterType, "Creature Type", CREATURE_TYPE_NAMES);

			ImGui::SameLine();
			static ListBoxParameters monsterAlignmentParameters{ -1, -1, false };
			static Alignment monsterAlignment{ Alignment::UNALIGNED };
			renderListBox(monsterAlignmentParameters, monsterAlignment, "Alignment", ALIGNMENT_NAMES);

			ImGui::SameLine();
			static ListBoxParameters monsterCRParameters{ -1, -1, false };
			static double monsterCR{};
			renderListBox(monsterCRParameters, monsterCR, "CR", CR_NAMES);

			ImGui::SameLine();
			static int monsterXP{};
			static bool isXPOverridden{ false };
			renderOveriddableInputInt(monsterXP, isXPOverridden, "XP", "Reset XP Override", MAPPED_CR_TO_XP, 0, INT_MAX, monsterCR)
				;
			static int monsterAC{};
			renderNonOverridableInputInt(monsterAC, "AC");
			ImGui::SameLine(); static int monsterHP{};
			renderNonOverridableInputInt(monsterHP, "HP");

			ImGui::SameLine(); static int monsterPB{};
			static bool isPBOverridden{ false };
			renderOveriddableInputInt(monsterPB, isPBOverridden, "Proficiency Bonus", "Reset PB Override", MAPPED_CR_TO_PB, -5, 10, monsterCR);

			static int monsterWalkSpeed{};
			renderNonOverridableInputInt(monsterWalkSpeed, "Walk Speed");

			ImGui::SameLine(); static int monsterClimbingSpeed{};
			renderNonOverridableInputInt(monsterClimbingSpeed, "Climbing Speed");

			ImGui::SameLine(); static int monsterSwimSpeed{};
			renderNonOverridableInputInt(monsterSwimSpeed, "Swim Speed");

			ImGui::SameLine(); static int monsterBurrowSpeed{};
			renderNonOverridableInputInt(monsterBurrowSpeed, "Burrow Speed");

			ImGui::SameLine(); static int monsterFlySpeed{};
			renderNonOverridableInputInt(monsterFlySpeed, "Fly Speed");

			static std::unordered_map<Ability, int> monsterAbilityScores{
				{Ability::STRENGTH, 10},
				{Ability::DEXTERITY, 10},
				{Ability::CONSTITUTION, 10},
				{Ability::WISDOM, 10},
				{Ability::INTELLIGENCE, 10},
				{Ability::CHARISMA, 10} 
			};
			static std::unordered_map<Ability, int> monsterAbilityModifiers{
				{Ability::STRENGTH, 0},
				{Ability::DEXTERITY, 0},
				{Ability::CONSTITUTION, 0},
				{Ability::WISDOM, 0},
				{Ability::INTELLIGENCE, 0},
				{Ability::CHARISMA, 0} };
			static std::unordered_map<Ability, bool> abilityModifiersOverridden
			{ {Ability::STRENGTH, false},
				{Ability::DEXTERITY, false},
				{Ability::CONSTITUTION, false},
				{Ability::WISDOM, false}, {Ability::INTELLIGENCE, false},
				{Ability::CHARISMA, false} 
			};
			renderAbilityScoresAndModifiers(monsterAbilityScores, monsterAbilityModifiers, abilityModifiersOverridden);

			static std::unordered_map<Ability, bool> proficientSavingThrows{
				{Ability::STRENGTH, false},
				{Ability::DEXTERITY, false},
				{Ability::CONSTITUTION, false},
				{Ability::WISDOM, false},
				{Ability::INTELLIGENCE, false},
				{Ability::CHARISMA, false} 
			};
			static std::unordered_map<Ability, bool> expertiseSavingThrows{
				{Ability::STRENGTH, false},
				{Ability::DEXTERITY, false},
				{Ability::CONSTITUTION, false},
				{Ability::WISDOM, false}, {Ability::INTELLIGENCE, false},
				{Ability::CHARISMA, false} 
			};
			static std::string savingThrowProficiencySuffix{ " Proficiency" };
			static std::string savingThrowExpertiseSuffix{ " Expertise" };
			renderProficiencyAndExpertiseCluster(
				"Saving Throw Proficiencies",
				savingThrowProficiencySuffix,
				"Saving Throw Expertises",
				savingThrowExpertiseSuffix,
				proficientSavingThrows,
				expertiseSavingThrows,
				ABILITY_NAMES
			);

			static std::unordered_map<Ability, int> monsterSavingThrowModifiers{
				{Ability::STRENGTH, 0},
				{Ability::DEXTERITY, 0},
				{Ability::CONSTITUTION, 0},
				{Ability::WISDOM, 0}, {Ability::INTELLIGENCE, 0},
				{Ability::CHARISMA, 0} 
			};
			static std::unordered_map<Ability, bool> monsterSavingThrowsOverride{
				{Ability::STRENGTH, false},
				{Ability::DEXTERITY, false},
				{Ability::CONSTITUTION, false},
				{Ability::WISDOM, false},
				{Ability::INTELLIGENCE, false},
				{Ability::CHARISMA, false}
			};
			renderSavingThrowModifiers(
				monsterSavingThrowModifiers,
				monsterSavingThrowsOverride,
				proficientSavingThrows,
				expertiseSavingThrows,
				monsterAbilityModifiers,
				monsterPB);

			static std::unordered_map<Skill, bool> proficientSkills{
				{Skill::ATHLETICS, false},
				{Skill::ACROBATICS, false},
				{Skill::SLEIGHT_OF_HAND, false},
				{Skill::STEALTH, false}, {Skill::ARCANA, false},
				{Skill::HISTORY, false}, {Skill::INVESTIGATION, false},
				{Skill::NATURE, false},
				{Skill::RELIGION, false},
				{Skill::ANIMAL_HANDLING, false},
				{Skill::INSIGHT, false},
				{Skill::MEDICINE, false},
				{Skill::PERCEPTION, false},
				{Skill::SURVIVAL, false},
				{Skill::DECEPTION, false},
				{Skill::INTIMIDATION, false},
				{Skill::PERFORMANCE, false},
				{Skill::PERSUASION, false} 
			};
			static std::unordered_map<Skill, bool> expertiseSkills{
				{Skill::ATHLETICS, false},
				{Skill::ACROBATICS, false},
				{Skill::SLEIGHT_OF_HAND, false},
				{Skill::STEALTH, false},
				{Skill::ARCANA, false},
				{Skill::HISTORY, false},
				{Skill::INVESTIGATION, false},
				{Skill::NATURE, false},
				{Skill::RELIGION, false},
				{Skill::ANIMAL_HANDLING, false},
				{Skill::INSIGHT, false},
				{Skill::MEDICINE, false},
				{Skill::PERCEPTION, false},
				{Skill::SURVIVAL, false},
				{Skill::DECEPTION, false},
				{Skill::INTIMIDATION, false},
				{Skill::PERFORMANCE, false},
				{Skill::PERSUASION, false}
			};
			static std::string skillProficiencySuffix{ " Proficiency" };
			static std::string skillExpertiseSuffix{ " Expertise" };
			renderProficiencyAndExpertiseCluster(
				"Skill Proficiencies",
				skillProficiencySuffix,
				"Skill Expertises",
				skillExpertiseSuffix,
				proficientSkills,
				expertiseSkills,
				SKILL_NAMES
			);

			static std::unordered_map<Skill, int> monsterSkillModifiers{
				{Skill::ATHLETICS, 0},
				{Skill::ACROBATICS, 0},
				{Skill::SLEIGHT_OF_HAND, 0},
				{Skill::STEALTH, 0},
				{Skill::ARCANA, 0},
				{Skill::HISTORY, 0},
				{Skill::INVESTIGATION, 0},
				{Skill::NATURE, 0},
				{Skill::RELIGION, 0},
				{Skill::ANIMAL_HANDLING, 0},
				{Skill::INSIGHT, 0},
				{Skill::MEDICINE, 0},
				{Skill::PERCEPTION, 0},
				{Skill::SURVIVAL, 0},
				{Skill::DECEPTION, 0},
				{Skill::INTIMIDATION, 0},
				{Skill::PERFORMANCE, 0},
				{Skill::PERSUASION, 0} 
			};
			static std::unordered_map<Skill, bool> monsterSkillModifiersOverride{
				{Skill::ATHLETICS, false},
				{Skill::ACROBATICS, false},
				{Skill::SLEIGHT_OF_HAND, false},
				{Skill::STEALTH, false},
				{Skill::ARCANA, false},
				{Skill::HISTORY, false},
				{Skill::INVESTIGATION, false},
				{Skill::NATURE, false},
				{Skill::RELIGION, false},
				{Skill::ANIMAL_HANDLING, false},
				{Skill::INSIGHT, false},
				{Skill::MEDICINE, false},
				{Skill::PERCEPTION, false},
				{Skill::SURVIVAL, false},
				{Skill::DECEPTION, false},
				{Skill::INTIMIDATION, false},
				{Skill::PERFORMANCE, false},
				{Skill::PERSUASION, false}
			};
			renderSkillModifiers(
				monsterSkillModifiers,
				monsterSkillModifiersOverride,
				proficientSkills, expertiseSkills,
				monsterAbilityModifiers,
				monsterPB
			);

			// TODO: Damage vulnerabilities, resistances and immunities. 

			// TODO: Condition immunities. 

			// TODO: Senses. 
 
			//TODO: Languages.

			// TODO: Adding actions. 

			// TODO: Adding traits. 

			// TODO: Adding equipment. 

			// TODO: Validation of monster stats and saving functionality. 
			if (centeredButton("Complete Monster Creation")) {

			}

			// TODO: Close Button. 
			ImGui::EndPopup();
		}
	}
	template <typename T, typename MapType>
	void EncounterCreationScene::renderListBox(ListBoxParameters& parameters, T& type, const char* listBoxLabel, const MapType& items) {
		int counter{};
		ImGui::BeginListBox(listBoxLabel);
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
	void EncounterCreationScene::renderNonOverridableInputInt(int& value, const char* label) {
		if (ImGui::InputInt(label, &value)) {
			value = std::max(0, value);
		}
	}
	template <typename T, typename MapType>
	void EncounterCreationScene::renderOveriddableInputInt(
		int& value,
		bool& overrideFlag,
		const char* label,
		const char* overrideLabel,
		const MapType& items,
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
	// TODO: Fix the UI layouting of this.
	template <typename MapType, typename NameMap>
	void EncounterCreationScene::renderProficiencyAndExpertiseCluster(
		const char* proficiencyLabel,
		std::string& proficiencyLabelSuffix,
		const char* expertiseLabel,
		std::string& expertiseLabelSuffix,
		MapType& proficiencyItems,
		MapType& expertiseItems,
		const NameMap& itemNames
	) {
		ImGui::TextWrapped(proficiencyLabel);
		for (auto& item : itemNames) {
			if (ImGui::Checkbox((item.second + proficiencyLabelSuffix).c_str(), &proficiencyItems.at(item.first))) {
				if (proficiencyItems.at(item.first) && expertiseItems.at(item.first)) {
					expertiseItems.at(item.first) = false;
				}
			}
		}

		ImGui::TextWrapped(expertiseLabel);
		for (auto& item : itemNames) {
			if (ImGui::Checkbox((item.second + expertiseLabelSuffix).c_str(), &expertiseItems.at(item.first))) {
				if (expertiseItems.at(item.first) && proficiencyItems.at(item.first)) {
					proficiencyItems.at(item.first) = false;
				}
			}
		}
	}
	void EncounterCreationScene::renderAbilityScoresAndModifiers(
		std::unordered_map<Ability, int>& abilityScores,
		std::unordered_map<Ability, int>& abilityModifiers,
		std::unordered_map<Ability, bool>& abilityScoresOverride
	) {
		for (auto& item : abilityScores) {
			if (!abilityScoresOverride.at(item.first)) {
				abilityModifiers.at(item.first) = Creature::findAbilityScoreModifier(item.second);
			}
			if (ImGui::InputInt((ABILITY_NAMES.at(item.first) + " Score").c_str(), &item.second)) {
				item.second = std::max(std::min(item.second, ABILITY_SCORE_UPPER_BOUND), 0);
			}
			ImGui::SameLine();
			if (ImGui::InputInt((ABILITY_NAMES.at(item.first) + " Modifier").c_str(), &abilityModifiers.at(item.first))) {
				abilityModifiers.at(item.first) = std::max(std::min(abilityModifiers.at(item.first), 10), -5);
				abilityScoresOverride.at(item.first) = true;
			}
			ImGui::SameLine();
			if (ImGui::Button(("Reset " + ABILITY_NAMES.at(item.first) + " Modifier Override").c_str())) {
				abilityScoresOverride.at(item.first) = false;
			}
		}
	}
	void EncounterCreationScene::renderSavingThrowModifiers(
		std::unordered_map<Ability, int>& savingThrowModifiers,
		std::unordered_map<Ability, bool>& savingThrowModifiersOverride,
		std::unordered_map<Ability, bool>& proficientSavingThrows,
		std::unordered_map<Ability, bool>& expertiseSavingThrows,
		std::unordered_map<Ability, int>& abilityModifiers,
		const int PB)
	{
		ImGui::TextWrapped("Saving Throw Modifiers");
		for (auto& item : ABILITY_NAMES) {
			if (!savingThrowModifiersOverride.at(item.first)) {
				if (expertiseSavingThrows.at(item.first)) {
					savingThrowModifiers.at(item.first) =
						abilityModifiers.at(item.first) + 2 * PB;
				}
				else if (proficientSavingThrows.at(item.first)) {
					savingThrowModifiers.at(item.first) =
						abilityModifiers.at(item.first) + PB;
				}
				else {
					savingThrowModifiers.at(item.first) = abilityModifiers.at(item.first);
				}
			}
			if (ImGui::InputInt((item.second + " Saving Throw Modifier").c_str(), &savingThrowModifiers.at(item.first))) {
				savingThrowModifiers.at(item.first) = std::max(std::min(savingThrowModifiers.at(item.first), 10), -5);
				savingThrowModifiersOverride.at(item.first) = true;
			}
			ImGui::SameLine();
			if (ImGui::Button(("Reset " + item.second + " Saving Throw Modifier Override").c_str())) {
				savingThrowModifiersOverride.at(item.first) = false;

			}
		}
	}
	void EncounterCreationScene::renderSkillModifiers(
		std::unordered_map<Skill, int>& skillModifiers,
		std::unordered_map<Skill, bool>& skillModifiersOverride,
		std::unordered_map<Skill, bool>& proficientSkills,
		std::unordered_map<Skill, bool>& expertiseSkills,
		std::unordered_map<Ability, int>& abilityModifiers,
		const int PB
	) {
		ImGui::TextWrapped("Skill Modifiers");
		for (auto& item : SKILL_NAMES) {
			if (!skillModifiersOverride.at(item.first)) {
				if (expertiseSkills.at(item.first)) {
					skillModifiers.at(item.first) =
						abilityModifiers.at(MAPPED_SKILLS.at(item.first)) + 2 * PB;
				}
				else if (proficientSkills.at(item.first)) {
					skillModifiers.at(item.first) =
						abilityModifiers.at(MAPPED_SKILLS.at(item.first)) + PB;
				}
				else {
					skillModifiers.at(item.first) = abilityModifiers.at(MAPPED_SKILLS.at(item.first)) + PB;
				}
			}
			if (ImGui::InputInt((item.second + " Skill Modifier").c_str(), &skillModifiers.at(item.first))) {
				skillModifiers.at(item.first) = std::max(std::min(skillModifiers.at(item.first), 10), -5);
				skillModifiersOverride.at(item.first) = true;
			}
			ImGui::SameLine();
			if (ImGui::Button(("Reset " + item.second + " Skill Modifier Override").c_str())) {
				skillModifiersOverride.at(item.first) = false;
			}
		}
	}
}