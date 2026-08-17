#pragma once

#include "CombatTrackerLib/Texture2D.h"
#include "CombatTrackerLib/Utilities/GenericUtils.h"
#include "CombatTrackerLib/ImGuiScopes.h"

#include <imgui.h>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <optional>
#include <span>
#include <string_view>
#include <type_traits>

namespace CombatTrackerLib {

	/*
	The state a listbox keeps between frames.
	isHighlighted says whether the cursor is over a row right now, and highlightedIndex says which one,
	so a caller can preview whatever the cursor is over. It is recomputed every frame, 
	so it goes back to false as soon as the cursor leaves the listbox.
	*/
	struct ListBoxParameters {
		std::size_t chosenIndex{};
		std::size_t highlightedIndex{};
		bool isHighlighted{};
	};

	// Pins a popup to the middle of the viewport at a fraction of its size, so it neither drifts nor
	// outgrows the window when the viewport is resized.
	inline void centerNextWindow(float widthFactor, float heightFactor) {
		const auto viewport{ ImGui::GetMainViewport() };
		ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Always, ImVec2{ 0.5f, 0.5f });
		ImGui::SetNextWindowSize(
			ImVec2{ viewport->Size.x * widthFactor, viewport->Size.y * heightFactor },
			ImGuiCond_Always
		);
	}

	// Generic helper for single selectable listbox layouts.
	// It reports the row clicked this frame, leaving what that means to the caller.
	inline std::optional<std::size_t> renderListBox(
		ListBoxParameters& parameters,
		std::string_view listBoxLabel,
		std::span<const std::string_view> names // A view over a vector of const std::string_view elements. Basically same idea as string_view.
	) {
		parameters.isHighlighted = false;
		std::optional<std::size_t> clickedIndex{};
		const float height{ ImGui::GetTextLineHeightWithSpacing() * 8.0f };

		if (ScopedListBox listBox{ listBoxLabel, ImVec2{ 0, height } }) {
			for (std::size_t i{}; i < names.size(); ++i) {
				const bool isSelected{ i == parameters.chosenIndex };
				if (ImGui::Selectable(names[i].data(), isSelected)) {
					parameters.chosenIndex = i;
					clickedIndex = i;
				}
				if (ImGui::IsItemHovered()) {
					parameters.isHighlighted = true;
					parameters.highlightedIndex = i;
				}
			}
		}
		return clickedIndex;
	}

	// The enumeration case, where a row position is an enumerator and the names are in that same order.
	template <typename T> requires std::is_enum_v<T> // Use C++20 concepts to guarentee the passed type is an enum type.
	void renderListBox(
		ListBoxParameters& parameters,
		T& type,
		std::string_view listBoxLabel,
		std::span<const std::string_view> names
	) {
		assert(names.size() == enumToSizeT(T::COUNT));
		const auto clickedIndex{ renderListBox(parameters, listBoxLabel, names) };
		if (clickedIndex != std::nullopt) {
			type = static_cast<T>(*clickedIndex);
		}
	}

	/*
	Simple helper for rendering input ints that can be overriden by an user provided value, also providing a lower/upper bound.
	The computed default lives inside the integer itself, so this only shows what it currently reads as.
	Typing a value overrides it, and the reset button hands the field back to whatever recomputes the default.
	*/
	inline void renderOverridableInputInt(
		OverridableInt& overridableInt,
		std::string_view label,
		std::string_view overrideLabel,
		int lowerBound,
		int upperBound
	) {
		ImGui::TextUnformatted(label.data());
		ImGui::SetNextItemWidth(-1.0f);

		// ImGui writes through an int pointer, so the widget edits a copy that is handed back on change.
		int value{ overridableInt.get() };
		if (ImGui::InputInt("##Value", &value)) {
			overridableInt.setOverride(std::clamp(value, lowerBound, upperBound));
		}
		if (ImGui::Button(overrideLabel.data(), ImVec2{ -1.0f, 0.0f })) {
			overridableInt.clearOverride();
		}
	}

	// Analogous parallel to renderOverridableInputInt for BoundedInt.
	template <int LOWER_BOUND, int UPPER_BOUND>
	void renderBoundedInputInt(BoundedInt<LOWER_BOUND, UPPER_BOUND>& boundedInt, std::string_view label) {
		int value{ boundedInt.get() };
		if (ImGui::InputInt(label.data(), &value)) {
			boundedInt = value;
		}
	}

	// Simple helper method for rendering multi-selectable parameters. 
	// This function uses a table call so we pass a non-localized id parameter to construct the table.
	// Otherwise a localized value will cause us to create a different table depending on the language.
	template <typename M>
	void renderCheckboxGrid(
		M& items,
		std::span<const std::string_view> names,
		std::string_view stableID,
		std::string_view label,
		int columnCount
	) {
		ImGui::TextWrapped("%s", label.data());
		if (ScopedTable table{ stableID, columnCount, ImGuiTableFlags_SizingStretchSame }) {

			// The whole table is drawn and the flags sit in an array of the same length, so one index walks both plain arrays.
			for (std::size_t i{}; i < names.size(); ++i) {
				ImGui::TableNextColumn();
				ImGui::Checkbox(names[i].data(), &items[i]);
			}
		}
	}

	// Very simple helper for rendering information texts as bullet points.
	inline void renderBulletList(std::span<const std::string_view> bullets) {
		for (const auto& bullet : bullets) {
			ImGui::Bullet(); 
			ImGui::TextWrapped("%s", bullet.data());
		}
	}

	// Taken from https://github.com/ocornut/imgui/discussions/3862.
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
		const auto width{ static_cast<float>(texture.getWidth()) };
		const auto height{ static_cast<float>(texture.getHeight()) };
		const auto scaleX{ availableSize.x / width };
		const auto scaleY{ availableSize.y / height };

		// The scale is defaulted to 1.0 in case the region size is larger than the image size.
		const auto finalScale{ std::min(std::min(scaleX, scaleY), 1.0f) };
		const auto displaySize{ ImVec2(width * finalScale, height * finalScale) };

		// Center the cursor position so that the image renders at the center of the content region.
		auto cursorPos{ ImGui::GetCursorPos() };
		cursorPos.x += std::max(0.0f, (availableSize.x - displaySize.x) * 0.5f);
		cursorPos.y += std::max(0.0f, (availableSize.y - displaySize.y) * 0.5f);
		ImGui::SetCursorPos(cursorPos);

		// The tint is multiplied with the texel in the ImGui shader, so a white texture comes out as exactly the tint colour. 
		ImGui::ImageWithBg(static_cast<ImTextureID>(texture.getID()), displaySize,
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