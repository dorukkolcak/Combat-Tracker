#pragma once

#include "../../CombatTrackerLib/Scene.h"

namespace CombatTrackerApp {
	class EncounterCreationScene : public CombatTrackerLib::Scene {
	public:
		EncounterCreationScene(CombatTrackerLib::App* app);
		virtual ~EncounterCreationScene();

		virtual void notifyScene(ImGuiIO& imguiIO) override;
		virtual void updateScene(double timeInterval) override;
		virtual void renderUI() override;
		virtual void renderScene() override;
	private:
	};
}