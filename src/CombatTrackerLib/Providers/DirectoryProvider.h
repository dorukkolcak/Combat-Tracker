#pragma once

#include <filesystem>
#include <string_view>

namespace CombatTrackerLib {
	class DirectoryProvider {
	public:
		DirectoryProvider() = default;
		explicit DirectoryProvider(std::string_view applicationName);

		DirectoryProvider(const DirectoryProvider& provider) = delete;
		DirectoryProvider& operator=(const DirectoryProvider& provider) = delete;

		DirectoryProvider(DirectoryProvider&& provider) noexcept = delete;
		DirectoryProvider& operator=(DirectoryProvider&& provider) noexcept = delete;

		~DirectoryProvider() = default;

		const std::filesystem::path& getResourceDirectory() const;
		const std::filesystem::path& getUserDataDirectory() const;

	private:

		std::filesystem::path m_resourceDirectory{};
		std::filesystem::path m_userDataDirectory{};

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
