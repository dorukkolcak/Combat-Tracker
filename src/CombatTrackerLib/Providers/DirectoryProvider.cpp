#include "CombatTrackerLib/Providers/DirectoryProvider.h"

#if defined(_WIN32)

// Windows.h defines min and max as macros, which collides with the std::min/std::max calls elsewhere in the library.
// NOMINMAX suppresses them and WIN32_LEAN_AND_MEAN drops the parts of the header we have no use for.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <string>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <cstdint>
#include <limits.h>
#include <vector>
#elif defined(__linux__)
// read_symlink needs nothing other than <filesystem>. So no extra headers needed.
#else
#error "Unsupported platform"
#endif

#include <cstdlib>
#include <system_error>

namespace CombatTrackerLib {
	DirectoryProvider::DirectoryProvider(std::string_view applicationName)
		: Unique(), m_resourceDirectory{ findResourceDirectory() },
		  m_userDataDirectory{ findUserDataDirectory(applicationName) } {}
	const std::filesystem::path&
	DirectoryProvider::getResourceDirectory() const {
		return m_resourceDirectory;
	}
	const std::filesystem::path&
	DirectoryProvider::getUserDataDirectory() const {
		return m_userDataDirectory;
	}
	std::filesystem::path DirectoryProvider::findResourceDirectory() {
		std::error_code errorCode{};
#if defined(_WIN32)

		/*
			Read the module filename to a wide character string buffer.
			GetModuleFileNameW is a Windows API function that retrieves the fully qualified path for the file
			containing the specified module loaded by the current process, using wide-character (Unicode) strings.
			https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamew
			*/
		std::wstring buffer(
			MAX_PATH, L'\0'
		); //wstring has a initializer list constructor so we use direct brace initialization.
		auto written{ GetModuleFileNameW(
			nullptr, buffer.data(), static_cast<DWORD>(buffer.size())
		) };

		// According to the API specification, this implies that the string buffer is too small to hold the path.
		// So we double the size of the buffer and try to read the path again.
		while (written != 0 && written == buffer.size()) {
			buffer.resize(buffer.size() * 2);
			written = GetModuleFileNameW(
				nullptr, buffer.data(), static_cast<DWORD>(buffer.size())
			);
		}

		// This implies that the function has failed so we return a fallback which is just the current working directory.
		if (written == 0) {
			return std::filesystem::current_path(errorCode
			); // Again like JSON reading functionality, we use an error code to not throw an exception.
		}

		// Return the parent path so that the application using the library can place resources in the appropriate, high-level directory.
		buffer.resize(written);
		return std::filesystem::path{ buffer }.parent_path();
#elif defined(__APPLE__)

		// _NSGetExecutablePath is a low-level C function used on Apple platforms (macOS, iOS)
		// to retrieve the file system path of the currently running executable.
		// See: https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/dyld.3.html.
		std::vector<char> buffer(PATH_MAX);

		/*
			_NSGetExecutablePath requires us to pass a uint32_t to its argument.
			This function also returns 0 if it is successful, -1 if the buffer is not large enough.
			As a side effect, this function also set the bufsize pointer to the required size.
			So we now the required buffer size on failure immediately.
			*/
		auto size{ static_cast<std::uint32_t>(buffer.size()) };
		if (_NSGetExecutablePath(buffer.data(), &size) == -1) {

			// Resize the buffer to the appropriate final size and retry.
			// If this fails, there is some other issue so we return a default.
			buffer.resize(size);
			if (_NSGetExecutablePath(buffer.data(), &size) != 0) {
				return std::filesystem::current_path(errorCode);
			}
		}

		// According to _NSGetExecutablePath specification, the returned path can still contain symlinks and relative segments.
		// This is resolved by calling std::filesystem::canonical which converts the path to the canonical absolute path.
		const auto executablePath{
			std::filesystem::canonical(buffer.data(), errorCode)
		};
		if (errorCode) {
			return std::filesystem::path{ buffer.data() }.parent_path();
		}
		return executablePath.parent_path();
#elif defined(__linux__)

		// On Linux, the OS maintains a symbolic link to the currently running executable file.
		// We read this symbolic link to acquire te resource directory.
		const auto executableDirectory{
			std::filesystem::read_symlink("/proc/self/exe", errorCode)
		};
		if (errorCode) {
			return std::filesystem::current_path(errorCode);
		}
		return executableDirectory.parent_path();
#endif
	}
	const std::filesystem::path
	DirectoryProvider::findUserDataDirectory(std::string_view applicationName) {
		if (applicationName.empty()) {
			return {};
		}
#if defined(_WIN32)

		// Get the value of the "APPDATA" environment variable.
		// The reason we don't use std::getenv() here is because it doesn't work with wide characters, particularly a windows issue.
		const auto required{ GetEnvironmentVariableW(L"APPDATA", nullptr, 0) };
		if (required == 0) {
			return {};
		}
		std::wstring applicationData(required, L'\0');
		const auto written{ GetEnvironmentVariableW(
			L"APPDATA", applicationData.data(), required
		) };
		if (written == 0 || written >= required) {
			return {};
		}
		applicationData.resize(written);
		return std::filesystem::path{ applicationData } / applicationName;
#elif defined(__APPLE__)
		const auto* home{ std::getenv("HOME") };
		if (home == nullptr || home[0] == '\0') {
			return {};
		}
		return std::filesystem::path{ home } / "Library" /
			   "Application Support" / applicationName;
#elif defined(__linux__)

		// The XDG base directory specification says to use the default whenever the variable is unset or empty.
		// See https://specifications.freedesktop.org/basedir/latest/.
		const auto* dataHome{ std::getenv("XDG_DATA_HOME") };

		// XDG_DATA_HOME is set, use its value.
		if (dataHome != nullptr && dataHome[0] != '\0') {
			return std::filesystem::path{ dataHome } / applicationName;
		}

		// The default is equal to $HOME/.local/share
		// See https://specifications.freedesktop.org/basedir/latest/.
		const char* home{ std::getenv("HOME") };
		if (home == nullptr || home[0] == '\0') {
			return {};
		}
		return std::filesystem::path{ home } / ".local" / "share" /
			   applicationName;
#endif
	}
}
