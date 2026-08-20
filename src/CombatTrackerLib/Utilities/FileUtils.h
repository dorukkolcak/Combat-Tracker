#pragma once

#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <system_error>

namespace CombatTrackerLib {

	inline std::string readFileToString(const std::filesystem::path& filePath) {
		std::ifstream file(filePath);
		if (!file.is_open()) {
			std::cerr << " Can't open file: " << filePath.string() << "\n";
			return "";
		}

		std::ostringstream fileStream;
		fileStream << file.rdbuf();
		return fileStream.str();
	}

	// These two functions implement generic JSON reading/writing functionality so they are put inside the library section of the project.
	// They can be used to read/write any arbitrary JSON file.
	inline std::optional<nlohmann::json> readJSONFile(const std::filesystem::path& path) {
		std::ifstream file{ path };
		if (!file.is_open()) {
			return std::nullopt;
		}

		// Passing false as the last argument hands back a discarded value on malformed input instead of throwing an error.
		auto contents{ nlohmann::json::parse(file, nullptr, false, true) };
		if (contents.is_discarded()) {
			return std::nullopt;
		};
		return contents;
	}
	inline bool writeJSONFile(const std::filesystem::path& path, const nlohmann::json& contents) {

		// Create directory to write in if it doesn't exist.
		// If it already exists, create_directories returns false which we don't really care about.
		std::error_code errorCode{};
		std::filesystem::create_directories(path.parent_path(), errorCode);

		std::ofstream file{ path };
		if (!file.is_open()) {
			return false;
		}
		file << contents.dump(1, '\t');
		return true;
	}
}