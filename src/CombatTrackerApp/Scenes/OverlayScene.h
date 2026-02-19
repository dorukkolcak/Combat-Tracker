#pragma once

#include "../../CombatTrackerLib/Scene.h"

// Used as a sort of "omni-present" Scene at the top layer that will be constantly running.
namespace CombatTrackerApp {
	class OverlayScene : public CombatTrackerLib::Scene {
	public:
		OverlayScene(CombatTrackerLib::App* app);
		virtual ~OverlayScene();

		virtual void notifyScene(ImGuiIO& imguiIO) override;
		virtual void updateScene(double timeInterval) override;
		virtual void renderUI() override;
		virtual void renderScene() override;
	private:
		// UI IDs.
		ImGuiID overlayDockspaceID{};
		ImGuiID generalID{};
		ImGuiID viewID{};
		// Dockspace layout flag.
		bool isOverlayDockspaceInitialized;
	};
}