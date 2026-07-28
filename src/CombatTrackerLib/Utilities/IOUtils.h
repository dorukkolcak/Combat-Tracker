#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

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
}