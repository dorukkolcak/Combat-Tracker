#pragma once

#include "imgui.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

namespace CombatTrackerLib {

	// Taken from https://github.com/ocornut/imgui/discussions/3862.
	// I would love to make the label's type std::string_view but the Imgui API requires to pass const char* as an argument.
	inline bool alignedButton(const char* label, float alignmentFactor) {

		ImGuiStyle& style{ ImGui::GetStyle() };
		float buttonSize{ ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f };
		float avail{ ImGui::GetContentRegionAvail().x };

		float off{ (avail - buttonSize) * alignmentFactor };
		if (off > 0.0f) {
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
		}
		return ImGui::Button(label);
	}
	inline std::string readFile(const std::filesystem::path& filePath) {

		std::ifstream file(filePath);
		if (!file.is_open()) {
			std::cerr << " Can't open file: " << filePath.string() << "\n";
			return "";
		}

		std::ostringstream fileStream;
		fileStream << file.rdbuf();
		file.close();
		return std::move(fileStream.str());
	}
}