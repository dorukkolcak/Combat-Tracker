#include <gtest/gtest.h>

#include "Themes.h"

/*
The theme listbox draws one row per entry of this table and turns the row position back into a preset.
So a table shorter than the enum would leave blank rows that select a preset the user can't see.
Applying a theme is not covered here, as that writes into a live ImGui context.
*/
TEST(ThemesTests, EveryPresetHasAName) {
	for (const auto& presetName : CombatTrackerLib::PRESET_NAMES) {
		EXPECT_FALSE(presetName.empty());
	}
}

TEST(ThemesTests, PresetNamesCoverEveryEnumerator) {
	EXPECT_EQ(CombatTrackerLib::PRESET_NAMES.size(), static_cast<std::size_t>(CombatTrackerLib::UIPreset::COUNT));
}
