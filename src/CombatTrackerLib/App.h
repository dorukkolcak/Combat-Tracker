#pragma once

#include "CombatTrackerLib/Window.h"
#include "CombatTrackerLib/Scene.h"
#include "CombatTrackerLib/SceneManager.h"
#include "CombatTrackerLib/Themes.h"
#include "CombatTrackerLib/LocaleProvider.h"

#include <string>
#include <memory>
#include <filesystem>

namespace CombatTrackerLib {
	class Scene;
	class App {
	public:
		struct Specification {
			UIPreset uiPreset{};
			std::string locale{};
			CombatTrackerLib::WindowSpecification windowSpecification{};
			std::string name{};
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

		void setUIPreset(UIPreset newPreset);
		UIPreset getUIPreset() const;
		
		const std::filesystem::path& getResourceDirectory() const;
		const std::filesystem::path& getUserDataDirectory() const;
		const LocaleProvider& getLocaleProvider() const;
		SceneManager& getSceneManager();

	private:
		Specification m_specfication{};
		std::unique_ptr<Window> m_windowPointer{};

		SceneManager m_sceneManager{};

		std::filesystem::path m_resourceDirectory{};
		std::filesystem::path m_userDataDirectory{};

		LocaleProvider m_localeReader{};

		bool m_isCloseSignalled{}; 
		bool m_isUIPresetApplied{};

		// Both of the functions below should use NRVO so no expensive copies are made.
		
		// Finds the path for read only files that ship alongside the application, such as textures and shaders.
		// This is derived from the executable's own location instead of the working directory.
		// Falls back to the working directory if the platform call fails.
		std::filesystem::path findResourceDirectory();

		// Finds the path for writable per user location for files the application creates.
		// The application name is a parameter so the library half keeps no knowledge of a particular application.
		// Returns an empty path when the location cannot be determined, which callers should treat as a failure to save.
		std::filesystem::path findUserDataDirectory(std::string_view applicationName);
	};
}