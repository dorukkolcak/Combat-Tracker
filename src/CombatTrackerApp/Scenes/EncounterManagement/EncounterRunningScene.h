#pragma once

#include "../../../CombatTrackerLib/Scene.h"

namespace CombatTrackerApp {
	class EncounterRunningScene : public CombatTrackerLib::Scene {
	public:
		EncounterRunningScene(CombatTrackerLib::App* app);
		virtual ~EncounterRunningScene();

		virtual void notifyScene(ImGuiIO& imguiIO) override;
		virtual void updateScene(double timeInterval) override;
		virtual void renderUI() override;
		virtual void renderScene() override;
	private:
	};
}