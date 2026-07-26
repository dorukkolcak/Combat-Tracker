#pragma once

#include "imgui.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Texture2D.h"

namespace CombatTrackerLib {

	// Taken from https://github.com/ocornut/imgui/discussions/3862.
	// I would love to make the label's type std::string_view but the Imgui API requires to pass const char* as an argument.
	inline bool alignedButton(const char* label, float alignmentFactor) {

		auto& style{ ImGui::GetStyle() };
		auto buttonSize{ ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f };
		auto avail{ ImGui::GetContentRegionAvail().x };

		auto off{ (avail - buttonSize) * alignmentFactor };
		if (off > 0.0f) {
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
		}
		return ImGui::Button(label);
	}
	inline void centeredImage(const Texture2D& texture, const ImVec4& tint = ImVec4(1.0f, 1.0f, 1.0f, 1.0f)) {

		// Scale down the image size in case the available container is smaller than the texture itself.
		// This scaling should be done uniformly.
		const auto availableSize{ ImGui::GetContentRegionAvail() };
		const auto scaleX{ availableSize.x / texture.getTextureWidth() };
		const auto scaleY{ availableSize.y / texture.getTextureHeight() };

		// The scale is defaulted to 1.0 in case the region size is larger than the image size.
		const auto finalScale{ std::min(std::min(scaleX, scaleY), 1.0f) };
		const auto displaySize{ ImVec2(texture.getTextureWidth() * finalScale, texture.getTextureHeight() * finalScale) };

		// Center the cursor position so that the image renders at the center of the content region.
		auto cursorPos{ ImGui::GetCursorPos() };
		cursorPos.x += std::max(0.0, (availableSize.x - displaySize.x) * 0.5);
		cursorPos.y += std::max(0.0, (availableSize.y - displaySize.y) * 0.5);
		ImGui::SetCursorPos(cursorPos);

		// The tint is multiplied with the texel in the ImGui shader, so a white texture comes out as exactly the tint colour. 
		ImGui::ImageWithBg(static_cast<ImTextureID>(texture.getTextureID()), displaySize,
			ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tint);
	}
	inline std::string readFileToString(const std::filesystem::path& filePath) {

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