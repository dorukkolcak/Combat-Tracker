#pragma once

#include "CombatTrackerLib/Scene.h"
#include "CombatTrackerLib/Texture2D.h"
#include "CombatTrackerLib/Utilities/UIUtils.h"
#include "CombatTrackerLib/ImGuiScopes.h"
#include "CombatTrackerLib/LocaleProvider.h"
#include "CombatTrackerLib/OptionsHandler.h"

#include <string_view>
#include <imgui_internal.h>

namespace CombatTrackerApp {
	class MainMenuScene : public CombatTrackerLib::Scene {
	public:
		explicit MainMenuScene(CombatTrackerLib::App* app);
		void notifyScene(const ImGuiIO& imguiIO) override;
		void updateScene(double timeInterval) override;
		void renderUI() override;
		void renderScene() override;
	private:
 
		inline static constexpr ImGuiDockNodeFlags DOCKSPACE_FLAGS{
			ImGuiDockNodeFlags_NoResize |
			ImGuiDockNodeFlags_NoSplit |
			ImGuiDockNodeFlags_NoWindowMenuButton
		};
		inline static constexpr ImGuiWindowFlags HOST_WINDOW_FLAGS{
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse
		};

		CombatTrackerLib::Texture2D m_titleTexture{};

		// Sort of a flag that overrides some options that are enforced on every window.
		// Used for the window that hosts our application art.
		ImGuiWindowClass m_titleWindowClass{};

		CombatTrackerLib::ListBoxParameters m_UIThemeParameters{};
		CombatTrackerLib::UIPreset m_chosenUIPreset{};

		// Which category the options panel is currently showing.
		CombatTrackerLib::OptionsCategory m_selectedOptionsCategory{};

		ImGuiID m_dockspaceID{};
		ImGuiID m_encounterCreationWindowID{};
		ImGuiID m_encounterRunningWindowID{};
		ImGuiID m_completedEncountersWindowID{};
		ImGuiID m_titleWindowID{};

		// Dockspace layout flag.
		bool m_isDockspaceInitialized{};
	};

}