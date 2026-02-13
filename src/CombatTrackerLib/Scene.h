#pragma once
#include "App.h"

class App;

namespace CombatTrackerLib {
	class Scene {
	public:
		Scene(App* app);
		//virtual ~Scene();

		//void virtual renderScene();
		//void virtual updateScene(double timeInterval);
		//void virtual notifyScene();
	private:
		// We keep this to queue scene events.
		App* applicationReference;
	};
}