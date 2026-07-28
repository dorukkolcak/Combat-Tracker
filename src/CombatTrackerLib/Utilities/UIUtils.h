#pragma once

#include "imgui.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <string_view>
#include <type_traits>

#include "Texture2D.h"

namespace CombatTrackerLib {

	struct ListBoxParameters {
		int chosenIndex{};
		int highlightedIndex{};
		bool isHighlighted{};
	};

	// Generic helper for single selectable listbox layouts.
	template <typename T> requires std::is_enum_v<T> // Use C++20 concepts to guarentee the passed type is an enum type.
	void renderListBox(
		ListBoxParameters& parameters,
		T& type,
		const char* listBoxLabel,
		const std::array<std::string_view, static_cast<size_t>(T::COUNT)>& names // We can't really check whether the enum type has a COUNT enumerator but this will only occur if I am stupid.
	) {
		const float height{ ImGui::GetTextLineHeightWithSpacing() * 8.0f };
		if (ImGui::BeginListBox(listBoxLabel, ImVec2{ 0, height })) {

			// Every entry is drawn, so the row index doubles as the index that the parameters keep track of.
			for (std::size_t i{}; i < names.size(); ++i) {
				const int rowIndex{ static_cast<int>(i) };
				const bool isSelected{ rowIndex == parameters.chosenIndex };
				if (ImGui::Selectable(names[i].data(), isSelected)) {
					parameters.chosenIndex = rowIndex;
					type = static_cast<T>(i);
				}
				if (parameters.isHighlighted && ImGui::IsItemHovered()) {
					parameters.highlightedIndex = rowIndex;
				}
			}
			ImGui::EndListBox();
		}
	}

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

	// Vertical space to reserve at the bottom of a window or popup for a separator plus rowCount rows of full height widgets.
	inline float reservedFooterHeight(int rowCount) {
		return ImGui::GetFrameHeightWithSpacing() * static_cast<float>(rowCount) + ImGui::GetStyle().ItemSpacing.y * 2.0f;
	}

	// The width a button auto sizes to for the given label.
	inline float buttonWidth(const char* label) {
		return ImGui::CalcTextSize(label).x + ImGui::GetStyle().FramePadding.x * 2.0f;
	}

	// The same alignment math alignedButton() does, but for a row of widgets whose total width is known up front.
	inline void alignCursorForWidth(float width, float alignmentFactor) {
		const auto off{ (ImGui::GetContentRegionAvail().x - width) * alignmentFactor };
		if (off > 0.0f) {
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
		}
	}
}