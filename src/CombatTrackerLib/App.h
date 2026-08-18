#pragma once

#include "CombatTrackerLib/Window.h"
#include "CombatTrackerLib/Scene.h"
#include "CombatTrackerLib/SceneManager.h"
#include "CombatTrackerLib/Themes.h"
#include "CombatTrackerLib/LocaleProvider.h"
#include "CombatTrackerLib/OptionsHandler.h"

#include <array>
#include <string>
#include <memory>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace CombatTrackerLib {
	class Scene;
	class App {
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
		App() = default;
		explicit App(Specification specification);
		
		// An App object exists as a singleton so it should be non-copiable and non-movable.
		App(const App& other) = delete;
		App& operator=(const App& other)  = delete;

		App(App&& other) = delete;
		App& operator=(App& other) = delete;

		~App();

		void run();
		void signalApplicationClose();
		
		static const std::filesystem::path& getResourceDirectory();
		static const std::filesystem::path& getUserDataDirectory(std::string_view applicationName);

		const LocaleProvider& getLocaleProvider() const;

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
		void setUIPreset(UIPreset newPreset);
		void setLanguage(std::string_view localeCode);

	private:
		Specification m_specification{};
		OptionsHandler m_optionsHandler{};
		std::unique_ptr<Window> m_windowPointer{};

		SceneManager m_sceneManager{};

		LocaleProvider m_localeProvider{};

		bool m_isCloseSignalled{}; 
		bool m_isUIPresetApplied{};

		// Both of the functions below should use NRVO so no expensive copies are made.
		
		// Finds the path for read only files that ship alongside the application, such as textures and shaders.
		// This is derived from the executable's own location instead of the working directory.
		// Falls back to the working directory if the platform call fails.
		static std::filesystem::path findResourceDirectory();

		// Finds the path for writable per user location for files the application creates.
		// The application name is a parameter so the library half keeps no knowledge of a particular application.
		// Returns an empty path when the location cannot be determined, which callers should treat as a failure to save.
		static std::filesystem::path findUserDataDirectory(std::string_view applicationName);
	};
}