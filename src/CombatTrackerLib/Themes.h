#pragma once

#include "imgui.h"

#include <array>
#include <cstddef>
#include <unordered_map>
#include <string_view>

namespace CombatTrackerLib {

	enum class UIPreset {
		DARK,
		LIGHT,
		HIGH_CONTRAST,
		ARCANE,
		FOREST,
		INFERNAL,
		GLACIAL,
		CELESTIAL,
		NECROTIC,
		BLOOD_MOON,
		FEYWILD,
		STORM,
		SANDS,
		MOONLIT,
		COUNT,
	};

	// Notice double brace initialization for using initializer list.
	inline constexpr std::array<std::string_view, static_cast<std::size_t>(UIPreset::COUNT)> PRESET_NAMES { {
		"Dark",
		"Light",
		"High Contrast",
		"Arcane",
		"Forest",
		"Infernal",
		"Glacial",
		"Celestial",
		"Necrotic",
		"Blood Moon",
		"Feywild",
		"Storm",
		"Sands",
		"Moonlit"
	} };
	struct UITheme {
	public:
		UIPreset preset{};
		void setUITheme(UIPreset newPreset);
	private:

		// The preset member defaults to the first enumerator, so without this the very first call looks like a switch to the theme we are already on.
		bool m_isPresetApplied{};

		// Helper to simplify color representation.
		static constexpr ImVec4 rgba(float red, float green, float blue, float alpha = 255.0f) {
			return ImVec4(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
		}
		static std::unordered_map<UIPreset, ImGuiStyle> buildPresetConfigs();
		
		/*
		The preset styles are created via lambdas that return a ImGuiStyle object.
		Since the unordered_map is const, the lambdas are called only once.
		So we don't create unnecessary temporaries when switching between presets.
		We forward declare this variable and define it outside the class so that it can see the rgba definition.
		*/
		static const std::unordered_map<UIPreset, ImGuiStyle> PRESET_TO_CONFIG;
	};

	/*
	Defined out of the class body because rgba is only declared, not yet defined, while the class is still being parsed.
	Which leaves the constexpr color constants below with nothing to evaluate.
	Also the lambdas are immediately invoked using IIFE (parentheses with empty parameter list at the end of the lambda expression.)
	*/
	inline const std::unordered_map<UIPreset, ImGuiStyle> UITheme::PRESET_TO_CONFIG = UITheme::buildPresetConfigs();
		
}
