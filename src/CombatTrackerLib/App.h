#pragma once

#include "Window.h"

#include <memory>

namespace CombatTrackerLib {
	struct AppSpecification {
		std::string applicationName;
		CombatTrackerLib::WindowSpecification mainWindowSpecification;
	};
	class App {
	public:
		App(const AppSpecification& applicationSpecification);
		~App();
		void run();
		void stop();
	private:
		AppSpecification applicationSpecification;
		std::unique_ptr<Window> mainWindow;
	};
}