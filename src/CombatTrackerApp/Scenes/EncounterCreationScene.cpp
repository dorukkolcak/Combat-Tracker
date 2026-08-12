#include "CombatTrackerApp/Scenes/EncounterCreationScene.h"
#include "CombatTrackerApp/Scenes/MainMenuScene.h"
#include "CombatTrackerApp/Scenes/MonsterCreationScene.h"
#include "CombatTrackerApp/Scenes/PlayerCharacterCreationScene.h"
#include "CombatTrackerLib/Utilities/GenericUtils.h"

#include <imgui_internal.h>
#include <imgui_stdlib.h>

namespace CombatTrackerApp {
	EncounterCreationScene::EncounterCreationScene(CombatTrackerLib::App* app) :
		CombatTrackerLib::Scene{ app },
		m_monsterSource{ EntitySource::ALL },
		m_playerCharacterSource{ EntitySource::ALL },
		m_selectedMonsterIndex{ -1 },
		m_selectedPlayerCharacterIndex{ -1 },
		m_isdockspaceInitialized{ false }
	{}

	void EncounterCreationScene::notifyScene(const ImGuiIO&) {}
	void EncounterCreationScene::updateScene(double) {}

	void EncounterCreationScene::renderUI() {

		// The menu bar is rendered inside the host window and the three browsers inside the entity browser's tab bar.
		// So only the three top level windows are submitted from here.
		renderHostWindow();
		renderEntityBrowserHostWindow();
		renderEncounterManagerWindow();

	}
	void EncounterCreationScene::renderScene() {}

	std::string EncounterCreationScene::composeWindowName(WindowType windowType, std::string_view stableID) const {
		const auto& windowNames{ m_applicationPointer->getLocaleReader().getList(std::string{ WINDOW_NAMES_KEY }) };
		return std::format("{}###{}", windowNames[CombatTrackerLib::enumToSizeT(windowType)].get_ref<const std::string&>(), stableID);
	}

	void EncounterCreationScene::renderHostWindow() {

		using enum WindowType;

		const auto& localeReader{ m_applicationPointer->getLocaleReader() };
		const auto encounterCreatorWindowName{ composeWindowName(ENCOUNTER_CREATOR, "encounterCreator") };
		const auto entityBrowserWindowName{ composeWindowName(ENTITY_BROWSER, "entityBrowser") };
		const auto encounterManagerWindowName{ composeWindowName(ENCOUNTER_MANAGER, "encounterManager") };

		// Set configurations of the host window of the scene.
		// This has to happen on every pass and not just the initial one, otherwise the host stays at its old size once the OS window is resized.
		const auto viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		// Setup dockspace layout on first render pass.
		if (!m_isdockspaceInitialized) {

			m_dockspaceID = ImGui::GetID("Encounter Creator - Dockspace");
			ImGui::DockBuilderRemoveNode(m_dockspaceID);

			// Add dockspace nodes.
			ImGui::DockBuilderAddNode(m_dockspaceID, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(m_dockspaceID, viewport->WorkSize);

			// Configure dockspace nodes.
			ImGui::DockBuilderSplitNode(
				m_dockspaceID,
				ImGuiDir_Left,
				0.38f,
				&m_entityBrowserWindowID,
				&m_encounterManagerWindowID
			);

			// Dock windows.
			ImGui::DockBuilderDockWindow(entityBrowserWindowName.c_str(), m_entityBrowserWindowID);
			ImGui::DockBuilderDockWindow(encounterManagerWindowName.c_str(), m_encounterManagerWindowID);

			// Finish dockspace creation.
			ImGui::DockBuilderFinish(m_dockspaceID);
			m_isdockspaceInitialized = true;
		}

		// Render host window. Like the other host windows, it is just for scaffolding.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
		ImGui::Begin(
			encounterCreatorWindowName.c_str(),
			nullptr,
			ENCOUNTER_CREATION_WINDOW_FLAGS
		);
		ImGui::PopStyleVar(3);

		// The menu bar of the host window. This should grant access to functionality such as returning to the main menu.
		if (ImGui::BeginMenuBar()) {
			const auto& windowNames{ localeReader.getList(std::string{ WINDOW_NAMES_KEY }) };
			if (ImGui::BeginMenu(windowNames[CombatTrackerLib::enumToSizeT(MENU)].get_ref<const std::string&>().c_str())) {
				if (ImGui::MenuItem(localeReader.getText("/scenes/encounterCreation/returnToMainMenu").data())) {
					m_applicationPointer->queueSceneTransition(this, std::make_unique<MainMenuScene>(m_applicationPointer));
				}
				// TODO: Expand the options from here.
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::DockSpace(
			m_dockspaceID,
			ImGui::GetContentRegionAvail(),
			DOCKSPACE_FLAGS
		);
		ImGui::End();
	}
	void EncounterCreationScene::renderEntityBrowserHostWindow() {

		using enum WindowType;

		const auto& localeReader{ m_applicationPointer->getLocaleReader() };
		const auto& windowNames{ localeReader.getList(std::string{ WINDOW_NAMES_KEY }) };
		const auto entityBrowserWindowName{ composeWindowName(ENTITY_BROWSER, "entityBrowser") };

		// This window hosts each entity's seperate subbrowser window.
		// These subbrowser windows are accessed via a tab bar.
		ImGui::Begin(entityBrowserWindowName.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar);

		// Tabs of the window host other windows for each entity (Monster and PlayerCharacter).
		if (ImGui::BeginTabBar("##EntityBrowserTabs")) {
			if (ImGui::BeginTabItem(windowNames[CombatTrackerLib::enumToSizeT(MONSTER_BROWSER)].get_ref<const std::string&>().c_str())) {
				renderEntityBrowserWindow<MonsterCreationScene>(
					localeReader.getText("/scenes/monsterCreation/entityLabel"),
					MONSTER_BROWSER_COLUMNS_KEY,
					m_monsterSource,
					m_monsterSearchQuery,
					m_selectedMonsterIndex
				);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem(windowNames[CombatTrackerLib::enumToSizeT(PLAYER_CHARACTER_BROWSER)].get_ref<const std::string&>().c_str())) {
				renderEntityBrowserWindow<PlayerCharacterCreationScene>(
					localeReader.getText("/scenes/playerCharacterCreation/entityLabel"),
					PLAYER_CHARACTER_BROWSER_COLUMNS_KEY,
					m_playerCharacterSource,
					m_playerCharacterSearchQuery,
					m_selectedPlayerCharacterIndex
				);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
	void EncounterCreationScene::renderBrowserFilters(EntitySource& source, std::string& searchQuery, const char* label) {

		const auto& localeReader{ m_applicationPointer->getLocaleReader() };

		// Each tab passes its own label so that the widgets below don't collide in the ID stack.
		ImGui::PushID(label);
		ImGui::AlignTextToFramePadding();
		ImGui::TextUnformatted(localeReader.getText("/scenes/encounterCreation/filters/source").data());
		ImGui::SameLine();
		if (ImGui::RadioButton(localeReader.getText("/scenes/encounterCreation/filters/all").data(), source == EntitySource::ALL)) {
			source = EntitySource::ALL;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton(localeReader.getText("/scenes/encounterCreation/filters/userCreated").data(), source == EntitySource::USER_CREATED)) {
			source = EntitySource::USER_CREATED;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton(localeReader.getText("/scenes/encounterCreation/filters/compendium").data(), source == EntitySource::COMPENDIUM)) {
			source = EntitySource::COMPENDIUM;
		}

		ImGui::SetNextItemWidth(-1.0f); // The search field spans the tab so it grows with the browser instead of sitting at ImGui's default item width.
		ImGui::InputTextWithHint("##Search", localeReader.getText("/scenes/encounterCreation/filters/searchHint").data(), &searchQuery);
		ImGui::PopID();
	}

	// TODO: Finish this window when all monster and player character functionality is done.
	// This window is rendered as the "right" node of the dockspace.
	void EncounterCreationScene::renderEncounterManagerWindow() {

		using enum WindowType;

		ImGui::Begin(composeWindowName(ENCOUNTER_MANAGER, "encounterManager").c_str());
		ImGui::End();
	}
}
