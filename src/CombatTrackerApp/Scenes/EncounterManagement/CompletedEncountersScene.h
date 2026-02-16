#pragma once

#include "../../../CombatTrackerLib/Scene.h"

namespace CombatTrackerApp {
	class CompletedEncountersScene : public CombatTrackerLib::Scene {
	public:
		CompletedEncountersScene(CombatTrackerLib::App* app);
		virtual ~CompletedEncountersScene();

		virtual void notifyScene(ImGuiIO& imguiIO) override;
		virtual void updateScene(double timeInterval) override;
		virtual void renderUI() override;
		virtual void renderScene() override;
	private:
	};
}