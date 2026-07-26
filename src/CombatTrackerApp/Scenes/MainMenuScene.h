#pragma once

#include <unordered_map>
#include <string_view>

#include "../../CombatTrackerLib/Scene.h"
#include "../../CombatTrackerLib/Texture2D.h"
#include "../../CombatTrackerLib/Utilities/UIUtils.h"

namespace CombatTrackerApp {
	class MainMenuScene : public CombatTrackerLib::Scene {
	public:
		explicit MainMenuScene(CombatTrackerLib::App* app);
		void notifyScene(const ImGuiIO& imguiIO) override;
		void updateScene(double timeInterval) override;
		void renderUI() override;
		void renderScene() override;

	private:

		enum OptionsCategory {
			Appearance,
			//...
		};

		CombatTrackerLib::Texture2D m_titleTexture;

		// Sort of a flag that overrides some options that are enforced on every window.
		// Used for the window that hosts our application art.
		ImGuiWindowClass m_mainMenuWindowClass{};

		inline static const std::unordered_map<OptionsCategory, std::string_view> OPTIONS_CATEGORY_NAMES{ 
			{OptionsCategory::Appearance, "Appearance"},
			//...
		};

		CombatTrackerLib::ListBoxParameters m_UIThemeParameters{};
		CombatTrackerLib::UIPreset m_chosenUIPreset{};

		// Which category the options panel is currently showing.
		OptionsCategory m_selectedOptionsCategory{};

		// Flags.
		ImGuiDockNodeFlags m_mainMenuDockspaceFlags{};
		ImGuiWindowFlags m_hostWindowFlags{};

		// UI IDs.
		ImGuiID m_mainMenuDockspaceID{};
		ImGuiID m_encounterCreationWindowID{};
		ImGuiID m_encounterRunningWindowID{};
		ImGuiID m_completedEncountersWindowID{};
		ImGuiID m_mainMenuTitleWindowID{};

		// Dockspace layout flag.
		bool m_isMainMenuDockspaceInitialized{};
		
	};

}