#pragma once

#include "../../CombatTrackerLib/Scene.h"

// Used as a sort of "omni-present" Scene at the top layer that will be constantly running.
namespace CombatTrackerApp {
	class OverlayScene : public CombatTrackerLib::Scene {
	public:
		explicit OverlayScene(CombatTrackerLib::App* app);
		virtual ~OverlayScene();

		virtual void notifyScene(const ImGuiIO& imguiIO) override;
		virtual void updateScene(double timeInterval) override;
		virtual void renderUI() override;
		virtual void renderScene() override;
	private:

		// UI IDs.
		ImGuiID m_overlayDockspaceID{};
		ImGuiID m_generalID{};
		ImGuiID m_viewID{};

		// Dockspace layout flag.
		bool m_isOverlayDockspaceInitialized{};
	};
}