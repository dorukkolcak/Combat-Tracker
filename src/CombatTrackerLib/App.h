#pragma once

#include "CombatTrackerLib/OptionsHandler.h"
#include "CombatTrackerLib/Providers/DirectoryProvider.h"
#include "CombatTrackerLib/Providers/LocaleProvider.h"
#include "CombatTrackerLib/Providers/ThemeProvider.h"
#include "CombatTrackerLib/Scene.h"
#include "CombatTrackerLib/SceneManager.h"
#include "CombatTrackerLib/Utilities/GenericUtils.h"
#include "CombatTrackerLib/Window.h"

#include <nlohmann/json.hpp>

#include <array>
#include <filesystem>
#include <memory>
#include <string>

namespace CombatTrackerLib {
	class Scene;
	class OptionsHandler;

	class App : public Unique<App> {
	public:
		struct Specification {
			std::string name{};

			// These are moved during construction so they should be referenced via the options handler.
			std::array<std::string, enumToSizeT(OptionsCategory::COUNT)> optionCategoryKeys{};
			nlohmann::json defaultOptions{};

			// These parameters don't really make sense if the window is set to be fullscreen.
			// I think what happens in that case is we immediately trigger a windowResize callback,
			// which sets the window size accordingly.
			int initialWindowWidth{};
			int initialWindowHeight{};
		};

	public:
		App() = delete;
		explicit App(Specification specification);

		~App();

		void run();
		void signalApplicationClose();

		const DirectoryProvider& getDirectoryProvider() const;
		const LocaleProvider& getLocaleProvider() const;
		const ThemeProvider& getThemeProvider() const;

		// This should be modifiable to handle scene operations.
		SceneManager& getSceneManager();

		/*
		This should be modifiable to change options from the application side.
		TODO: So I kind of dislike exposing this for a number of reasons:
		1) A change in option requires a change in two parts: 
		A change in the internal memory structure of OptionsHandler 
		And a change in some specific application state that greatly differs between options.
		Exposing the handler solves the first change but doesn't solve the second change.
		For the second change I have to write a lot of wrappers for every single possible option.
		Which is really bad in my opinion. We will eventually solve this via the observer pattern
		and submitting callbacks to options.
		*/
		OptionsHandler& getOptionsHandler();

		/*
		IMPORTANT TODO: This should be changed once we do observer based event handling for non-imgui side of the application.
		And relatedly observer pattern for the option handler as well.
		A change in an option should ideally call a callback that modifies the necessary application state
		As well as the in-memory option structure. This is a very big rewrite by the way.
		*/
		void setWindowVsync(bool enabled);
		void setWindowFullscreen(bool enabled);
		void setWindowResize(bool enabled);
		void setTheme(const std::string& themeName);
		void setLanguage(std::string_view localeCode);

	private:
		// The order of construction of these members is quite important by the way.
		SceneManager m_sceneManager{};
		Specification m_specification{};
		DirectoryProvider m_directoryProvider; // Deleted default constructor.
		OptionsHandler m_optionsHandler;       // Deleted default constructor.
		LocaleProvider m_localeProvider;       // Deleted default constructor
		ThemeProvider m_themeProvider;         // Deleted default constructor.

		std::unique_ptr<Window> m_windowPointer{};

		bool m_isCloseSignalled{};
		bool m_isUIPresetApplied{};
	};
}
