#include "imgui.h"
#include "Styling.h"

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
	inline void setupLayout() {

		ImGuiStyle& style{ ImGui::GetStyle() };

		// Padding, spacing, rounding and sizing.
		style.WindowPadding = WINDOW_PADDING;
		style.FramePadding = FRAME_PADDING;
		style.ItemSpacing = ITEM_SPACING;
		style.ItemInnerSpacing = ITEM_INNER_SPACING;

		style.WindowBorderSize = WINDOW_BORDER_SIZE;
		style.FrameBorderSize = FRAME_BORDER_SIZE;
		style.TabBorderSize = TAB_BORDER_SIZE;
		style.DockingSeparatorSize = DOCKING_SEPARATOR_SIZE;
	}
	inline void createImguiLightTheme() {

		ImGuiStyle& style{ ImGui::GetStyle() };
		ImGui::StyleColorsLight();

		ImVec4* colors{ style.Colors };

		// Text coloring.
		colors[ImGuiCol_Text] = LightModeColors::TEXT_COLOR;
		colors[ImGuiCol_TextDisabled] = LightModeColors::TEXT_DISABLED_COLOR;

		// Window coloring.
		colors[ImGuiCol_WindowBg] = LightModeColors::WINDOW_BACKGROUND_COLOR;
		colors[ImGuiCol_DockingEmptyBg] = LightModeColors::WINDOW_BACKGROUND_COLOR;
		colors[ImGuiCol_DockingPreview] = LightModeColors::DOCKING_PREVIEW_COLOR;
		colors[ImGuiCol_ChildBg] = LightModeColors::CHILD_WINDOW_BACKGROUND_COLOR;
		colors[ImGuiCol_PopupBg] = LightModeColors::CHILD_WINDOW_BACKGROUND_COLOR;

		// Border coloring.
		colors[ImGuiCol_Border] = LightModeColors::BORDER_COLOR;

		// Bar coloring.
		colors[ImGuiCol_TitleBg] = LightModeColors::BAR_COLOR;
		colors[ImGuiCol_TitleBgActive] = LightModeColors::BAR_COLOR;
		colors[ImGuiCol_MenuBarBg] = LightModeColors::BAR_COLOR;
		colors[ImGuiCol_ScrollbarBg] = LightModeColors::BAR_COLOR;
		colors[ImGuiCol_ScrollbarGrab] = LightModeColors::SCROLL_BAR_GRAB_COLOR;
		colors[ImGuiCol_ScrollbarGrabHovered] = LightModeColors::BORDER_COLOR;
		colors[ImGuiCol_ScrollbarGrabActive] = LightModeColors::SCROLL_BAR_GRAB_ACTIVE_COLOR;

		// Frame coloring.
		colors[ImGuiCol_FrameBg] = LightModeColors::FRAME_COLOR;
		colors[ImGuiCol_FrameBgHovered] = LightModeColors::FRAME_BACKGROUND_HOVERED_COLOR;
		colors[ImGuiCol_FrameBgActive] = LightModeColors::FRAME_BACKGROUND_ACTIVE_COLOR;

		// Separator coloring.
		colors[ImGuiCol_Separator] = LightModeColors::SEPARATOR_COLOR;

		// Tab coloring.
		colors[ImGuiCol_Tab] = LightModeColors::TAB_COLOR;
		colors[ImGuiCol_TabActive] = LightModeColors::TAB_ACTIVE_COLOR;
		colors[ImGuiCol_TabUnfocused] = LightModeColors::TAB_UNFOCUSED_COLOR;
		colors[ImGuiCol_TabUnfocusedActive] = LightModeColors::TAB_UNFOCUSED_ACTIVE_COLOR;

	}
	inline void createImguiDarkTheme() {

		ImGuiStyle& style{ ImGui::GetStyle() };
		ImGui::StyleColorsDark();

		ImVec4* colors{ style.Colors };

		// Text coloring.
		colors[ImGuiCol_Text] = DarkModeColors::TEXT_COLOR;
		colors[ImGuiCol_TextDisabled] = DarkModeColors::TEXT_DISABLED_COLOR;

		// Window coloring.
		colors[ImGuiCol_WindowBg] = DarkModeColors::WINDOW_BACKGROUND_COLOR;
		colors[ImGuiCol_DockingEmptyBg] = DarkModeColors::WINDOW_BACKGROUND_COLOR;
		colors[ImGuiCol_DockingPreview] = DarkModeColors::DOCKING_PREVIEW_COLOR;
		colors[ImGuiCol_ChildBg] = DarkModeColors::CHILD_WINDOW_BACKGROUND_COLOR;
		colors[ImGuiCol_PopupBg] = DarkModeColors::CHILD_WINDOW_BACKGROUND_COLOR;

		// Border coloring.
		colors[ImGuiCol_Border] = DarkModeColors::BORDER_COLOR;

		// Bar coloring.
		colors[ImGuiCol_TitleBg] = DarkModeColors::BAR_COLOR;
		colors[ImGuiCol_TitleBgActive] = DarkModeColors::BAR_COLOR;
		colors[ImGuiCol_MenuBarBg] = DarkModeColors::BAR_COLOR;
		colors[ImGuiCol_ScrollbarBg] = DarkModeColors::BAR_COLOR;
		colors[ImGuiCol_ScrollbarGrab] = DarkModeColors::SCROLL_BAR_GRAB_COLOR;
		colors[ImGuiCol_ScrollbarGrabHovered] = DarkModeColors::BORDER_COLOR;
		colors[ImGuiCol_ScrollbarGrabActive] = DarkModeColors::SCROLL_BAR_GRAB_ACTIVE_COLOR;

		// Frame coloring.
		colors[ImGuiCol_FrameBg] = DarkModeColors::FRAME_COLOR;
		colors[ImGuiCol_FrameBgHovered] = DarkModeColors::FRAME_BACKGROUND_HOVERED_COLOR;
		colors[ImGuiCol_FrameBgActive] = DarkModeColors::FRAME_BACKGROUND_ACTIVE_COLOR;

		// Separator coloring.
		colors[ImGuiCol_Separator] = DarkModeColors::SEPARATOR_COLOR;

		// Tab coloring.
		colors[ImGuiCol_Tab] = DarkModeColors::TAB_COLOR;
		colors[ImGuiCol_TabActive] = DarkModeColors::TAB_ACTIVE_COLOR;
		colors[ImGuiCol_TabUnfocused] = DarkModeColors::TAB_UNFOCUSED_COLOR;
		colors[ImGuiCol_TabUnfocusedActive] = DarkModeColors::TAB_UNFOCUSED_ACTIVE_COLOR;

	}
	inline void initializeCommonColors() {

		ImGuiStyle& style{ ImGui::GetStyle() };
		ImVec4* colors{ style.Colors };
		// Checkmark and slider coloring.
		colors[ImGuiCol_CheckMark] = CommonColors::BRICK_RED_COLOR;
		colors[ImGuiCol_SliderGrab] = CommonColors::GOLD_COLOR;
		colors[ImGuiCol_SliderGrabActive] = CommonColors::GOLD_COLOR;

		// Selectable and tree coloring.
		colors[ImGuiCol_Header] = CommonColors::HEADER_COLOR;
		colors[ImGuiCol_HeaderHovered] = CommonColors::HEADER_HOVERED_COLOR;
		colors[ImGuiCol_HeaderActive] = CommonColors::HEADER_ACTIVE_COLOR;

		colors[ImGuiCol_TabHovered] = CommonColors::TAB_HOVERED_COLOR;
		colors[ImGuiCol_SeparatorHovered] = CommonColors::SEPARATOR_HOVERED_COLOR;
		colors[ImGuiCol_SeparatorActive] = CommonColors::BRICK_RED_COLOR;
		colors[ImGuiCol_TextSelectedBg] = CommonColors::TEXT_SELECTED_BACKGROUND_COLOR;
		// Button coloring.
		colors[ImGuiCol_Button] = CommonColors::BRICK_RED_COLOR;
		colors[ImGuiCol_ButtonHovered] = CommonColors::BRICK_RED_HOVER_COLOR;
		colors[ImGuiCol_ButtonActive] = CommonColors::BRICK_RED_ACTIVE_COLOR;
	}
}