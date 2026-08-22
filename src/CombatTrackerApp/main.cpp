#include "CombatTrackerApp/Scenes/MainMenuScene.h"
#include "CombatTrackerApp/Scenes/OverlayScene.h"
#include "CombatTrackerLib/App.h"
#include "CombatTrackerLib/OptionsHandler.h"
#include "CombatTrackerLib/Utilities/GenericUtils.h"

#include <nlohmann/json.hpp>

#include <array>
#include <string>

namespace CombatTrackerApp {
	const std::array<std::string, CombatTrackerLib::enumToSizeT(CombatTrackerLib::OptionsCategory::COUNT)>
		OPTION_CATEGORY_KEYS{"general", "appearance"};
	const nlohmann::json DEFAULT_OPTIONS = nlohmann::json::object(
		{{OPTION_CATEGORY_KEYS[CombatTrackerLib::enumToSizeT(CombatTrackerLib::OptionsCategory::GENERAL)],
	      nlohmann::json::object({{"language", "en"}})},
	     {OPTION_CATEGORY_KEYS[CombatTrackerLib::enumToSizeT(CombatTrackerLib::OptionsCategory::APPEARANCE)],
	      nlohmann::json::object(
			  {{"theme", "Arcane"}, {"vsyncEnabled", true}, {"fullscreenEnabled", false}, {"isResizable", true}}
		  )}}
	);
}

int main() {
	CombatTrackerLib::App::Specification
		appSpec{"CombatTracker", CombatTrackerApp::OPTION_CATEGORY_KEYS, CombatTrackerApp::DEFAULT_OPTIONS, 1920, 1080};
	CombatTrackerLib::App application(std::move(appSpec));
	application.getSceneManager().queueAddScene(std::make_unique<CombatTrackerApp::MainMenuScene>(&application));
	application.getSceneManager().queueAddScene(std::make_unique<CombatTrackerApp::OverlayScene>(&application));
	application.run();
}
