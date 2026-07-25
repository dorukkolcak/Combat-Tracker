#pragma once

#include "../../CombatTrackerLib/Scene.h"

// Used as a sort of "omni-present" Scene at the top layer that will be constantly running.
namespace CombatTrackerApp {
	class OverlayScene : public CombatTrackerLib::Scene {
	public:
		explicit OverlayScene(CombatTrackerLib::App* app);

		void notifyScene(const ImGuiIO& imguiIO) override;
		void updateScene(double timeInterval) override;
		void renderUI() override;
		void renderScene() override;
	};
}