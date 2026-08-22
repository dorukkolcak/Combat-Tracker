#pragma once

#include "CombatTrackerLib/Utilities/GenericUtils.h"

#include <filesystem>
#include <string_view>

namespace CombatTrackerLib {
	class DirectoryProvider : public Unique<DirectoryProvider> {
	public:
		DirectoryProvider() = delete;
		explicit DirectoryProvider(std::string_view applicationName);

		[[nodiscard]] const std::filesystem::path& getResourceDirectory() const;
		[[nodiscard]] const std::filesystem::path& getUserDataDirectory() const;

	private:
		std::filesystem::path m_resourceDirectory{};
		std::filesystem::path m_userDataDirectory{};

	private:
		// Both of the functions below should use NRVO so no expensive copies are made.

		// Finds the path for read only files that ship alongside the application, such as textures and shaders.
		// This is derived from the executable's own location instead of the working directory.
		// Falls back to the working directory if the platform call fails.
		static std::filesystem::path findResourceDirectory();

		// Finds the path for writable per user location for files the application creates.
		// The application name is a parameter so the library half keeps no knowledge of a particular application.
		// Returns an empty path when the location cannot be determined, which callers should treat as a failure to save.
		static const std::filesystem::path findUserDataDirectory(std::string_view applicationName);
	};
}
