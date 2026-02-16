// These constants are used to style the UI.
// We will support dynamic theme changing later on so it is really important for these constants to be inline constexprs.
namespace CombatTrackerLib {

	inline constexpr ImVec2 WINDOW_PADDING(14, 12);
	inline constexpr ImVec2 FRAME_PADDING(10, 7);
	inline constexpr ImVec2 ITEM_SPACING(10, 9);
	inline constexpr ImVec2 ITEM_INNER_SPACING(8, 6);
	inline constexpr float WINDOW_ROUNDING { 10.0f };
	inline constexpr float CHILD_ROUNDING { 10.0f };
	inline constexpr float FRAME_ROUNDING { 8.0f };
	inline constexpr float POPUP_ROUNDING { 10.0f };
	inline constexpr float SCROLLBAR_ROUNDING { 10.0f };
	inline constexpr float GRAB_ROUNDING { 10.0f };
	inline constexpr float TAB_ROUNDING { 10.0f };
	inline constexpr float WINDOW_BORDER_SIZE { 1.0f };
	inline constexpr float FRAME_BORDER_SIZE { 1.0f };
	inline constexpr float TAB_BORDER_SIZE { 1.0f };
	inline constexpr float DOCKING_SEPARATOR_SIZE { 2.0f };

	namespace LightModeColors {

		inline constexpr ImVec4 TEXT_COLOR(87.0f / 255.0f, 74.0f / 255.0f, 63.0f / 255.0f, 1);
		inline constexpr ImVec4 WINDOW_BACKGROUND_COLOR(246.0f / 255.0f, 237.0f / 255.0f, 223.0f / 255.0f, 1);
		inline constexpr ImVec4 CHILD_WINDOW_BACKGROUND_COLOR(252.0f / 255.0f, 244.0f / 255.0f, 232.0f / 255.0f, 1);
		inline constexpr ImVec4 BORDER_COLOR(206.0f / 255.0f, 186.0f / 255.0f, 166.0f / 255.0f, 1);
		inline constexpr ImVec4 SCROLL_BAR_GRAB_COLOR(BORDER_COLOR.x, BORDER_COLOR.y, BORDER_COLOR.z, 220.0f / 255.0f);
		inline constexpr ImVec4 SCROLL_BAR_GRAB_ACTIVE_COLOR((BORDER_COLOR.x * 0.95f), (BORDER_COLOR.y * 0.95f), (BORDER_COLOR.z * 0.95f), 1);
		inline constexpr ImVec4 BAR_COLOR(223.0f / 255.0f, 207.0f / 255.0f, 186.0f / 255.0f, 1);
		inline constexpr ImVec4 FRAME_COLOR(1.0f, 250.f / 255.0f, 242.0f / 255.0f, 1);
		inline constexpr ImVec4 FRAME_BACKGROUND_HOVERED_COLOR(FRAME_COLOR.x, FRAME_COLOR.y - 0.02f, FRAME_COLOR.z - 0.03f, 1);
		inline constexpr ImVec4 FRAME_BACKGROUND_ACTIVE_COLOR(FRAME_COLOR.x, FRAME_COLOR.y - 0.04f, FRAME_COLOR.z - 0.06f, 1);
		inline constexpr ImVec4 SEPARATOR_COLOR(210.0f / 255.0f, 190.0f / 255.0f, 170.0f / 255.0f, 1);
		inline constexpr ImVec4 TAB_COLOR(223.0f / 255.0f, 207.0f / 255.0f, 186.0f / 255.0f, 1);
		inline constexpr ImVec4 TAB_UNFOCUSED_COLOR(223.0f / 255.0f, 207.0f / 255.0f, 186.0f / 255.0f, 220.0f / 255.0f);
		inline constexpr ImVec4 TAB_ACTIVE_COLOR(252.0f / 255.0f, 244.0f / 255.0f, 232.0f / 255.0f, 1);
		inline constexpr ImVec4 TAB_UNFOCUSED_ACTIVE_COLOR(TAB_ACTIVE_COLOR.x, TAB_ACTIVE_COLOR.y, TAB_ACTIVE_COLOR.z, 240.0f / 255.0f);
		inline constexpr ImVec4 DOCKING_PREVIEW_COLOR(200.0f / 255.0f, 85.0f / 255.0f, 84.0f / 255.0f, 60.0f / 255.0f);
		inline constexpr ImVec4 TEXT_DISABLED_COLOR(120.0f / 255.0f, 105.0f / 255.0f, 94.0f / 255.0f, 160.0f / 255.0f);
	}

	namespace DarkModeColors {

		inline constexpr ImVec4 TEXT_COLOR(220.0f / 255.0f, 226.0f / 255.0f, 232.0f / 255.0f, 1);
		inline constexpr ImVec4 WINDOW_BACKGROUND_COLOR(12.0f / 255.0f, 15.0f / 255.0f, 19.0f / 255.0f, 1);
		inline constexpr ImVec4 CHILD_WINDOW_BACKGROUND_COLOR(16.0f / 255.0f, 20.0f / 255.0f, 25.0f / 255.0f, 1);
		inline constexpr ImVec4 BORDER_COLOR(42.0f / 255.0f, 50.0f / 255.0f, 60.0f / 255.0f, 1);
		inline constexpr ImVec4 SCROLL_BAR_GRAB_COLOR(BORDER_COLOR.x, BORDER_COLOR.y, BORDER_COLOR.z, 220.0f / 255.0f);
		inline constexpr ImVec4 SCROLL_BAR_GRAB_ACTIVE_COLOR((BORDER_COLOR.x * 0.92f), (BORDER_COLOR.y * 0.92f), (BORDER_COLOR.z * 0.92f), 1);
		inline constexpr ImVec4 BAR_COLOR(9.0f / 255.0f, 11.0f / 255.0f, 14.0f / 255.0f, 1);
		inline constexpr ImVec4 FRAME_COLOR(18.0f / 255.0f, 23.0f / 255.0f, 29.0f / 255.0f, 1);
		inline constexpr ImVec4 FRAME_BACKGROUND_HOVERED_COLOR(22.0f / 255.0f, 28.0f / 255.0f, 35.0f / 255.0f, 1);
		inline constexpr ImVec4 FRAME_BACKGROUND_ACTIVE_COLOR(26.0f / 255.0f, 33.0f / 255.0f, 41.0f / 255.0f, 1);
		inline constexpr ImVec4 SEPARATOR_COLOR(42.0f / 255.0f, 50.0f / 255.0f, 60.0f / 255.0f, 1);
		inline constexpr ImVec4 TAB_COLOR(9.0f / 255.0f, 11.0f / 255.0f, 14.0f / 255.0f, 1);
		inline constexpr ImVec4 TAB_UNFOCUSED_COLOR(9.0f / 255.0f, 11.0f / 255.0f, 14.0f / 255.0f, 220.0f / 255.0f);
		inline constexpr ImVec4 TAB_ACTIVE_COLOR(16.0f / 255.0f, 20.0f / 255.0f, 25.0f / 255.0f, 1);
		inline constexpr ImVec4 TAB_UNFOCUSED_ACTIVE_COLOR(TAB_ACTIVE_COLOR.x, TAB_ACTIVE_COLOR.y, TAB_ACTIVE_COLOR.z, 240.0f / 255.0f);
		inline constexpr ImVec4 DOCKING_PREVIEW_COLOR(200.0f / 255.0f, 85.0f / 255.0f, 84.0f / 255.0f, 70.0f / 255.0f);
		inline constexpr ImVec4 TEXT_DISABLED_COLOR(160.0f / 255.0f, 170.0f / 255.0f, 180.0f / 255.0f, 140.0f / 255.0f);
	}

	namespace CommonColors {

		inline constexpr ImVec4 BRICK_RED_COLOR(200.0f / 255.0f, 85.0f / 255.0f, 84.0f / 255.0f, 1);
		inline constexpr ImVec4 BRICK_RED_HOVER_COLOR(220.0f / 255.0f, 95.0f / 255.0f, 94.0f / 255.0f, 1);
		inline constexpr ImVec4 BRICK_RED_ACTIVE_COLOR(150.0f / 255.0f, 55.0f / 255.0f, 55.0f / 255.0f, 1);
		inline constexpr ImVec4 GOLD_COLOR(212.0f / 255.0f, 175.0f / 255.0f, 55.0f / 255.0f, 1);
		inline constexpr ImVec4 HEADER_COLOR(200.0f / 255.0f, 85.0f / 255.0f, 84.0f / 255.0f, 28.0f / 255.0f);
		inline constexpr ImVec4 HEADER_HOVERED_COLOR(200.0f / 255.0f, 85.0f / 255.0f, 84.0f / 255.0f, 52.0f / 255.0f);
		inline constexpr ImVec4 HEADER_ACTIVE_COLOR(200.0f / 255.0f, 85.0f / 255.0f, 84.0f / 255.0f, 78.0f / 255.0f);
		inline constexpr ImVec4 SEPARATOR_HOVERED_COLOR(200.0f / 255.0f, 85.0f / 255.0f, 84.0f / 255.0f, 190.0f / 255.0f);
		inline constexpr ImVec4 TEXT_SELECTED_BACKGROUND_COLOR(200.0f / 255.0f, 85.0f / 255.0f, 84.0f / 255.0f, 70.0f / 255.0f);
		inline constexpr ImVec4 TAB_HOVERED_COLOR(200.0f / 255.0f, 85.0f / 255.0f, 84.0f / 255.0f, 90.0f / 255.0f);
	}
}
