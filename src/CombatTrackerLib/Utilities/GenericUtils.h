#pragma once

#include <cstddef>
#include <type_traits>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <optional>

#include "nlohmann/json.hpp"

namespace CombatTrackerLib {
	template<typename E> requires std::is_enum_v<E>
	inline std::size_t enumToSizeT(E enumerator) {
		return static_cast<std::size_t>(enumerator);
	}
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
		auto contents{ nlohmann::json::parse(file, nullptr, false) };
		if (contents.is_discarded()) {
			return std::nullopt;
		};
		return contents;
	}
	inline bool writeJSONFile(const std::filesystem::path& path, const nlohmann::json& contents) {
		std::ofstream file{ path };
		if (!file.is_open()) {
			return false;
		}
		file << contents.dump(1, '\t');
		return true;
	}
}