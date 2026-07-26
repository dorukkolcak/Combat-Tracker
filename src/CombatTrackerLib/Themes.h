#pragma once

#include "imgui.h"

#include <unordered_map>
#include <string>

namespace CombatTrackerLib {

	enum class UIPreset {
		NONE, // Used mostly for value-initialization.
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
	};
	inline const std::unordered_map<UIPreset, std::string> PRESET_NAMES{
		{UIPreset::DARK, "Dark"},
		{UIPreset::LIGHT, "Light"},
		{UIPreset::HIGH_CONTRAST, "High Contrast"},
		{UIPreset::ARCANE, "Arcane"},
		{UIPreset::FOREST, "Forest"},
		{UIPreset::INFERNAL, "Infernal"},
		{UIPreset::GLACIAL, "Glacial"},
		{UIPreset::CELESTIAL, "Celestial"},
		{UIPreset::NECROTIC, "Necrotic"},
		{UIPreset::BLOOD_MOON, "Blood Moon"},
		{UIPreset::FEYWILD, "Feywild"},
		{UIPreset::STORM, "Storm"},
		{UIPreset::SANDS, "Sands"},
		{UIPreset::MOONLIT, "Moonlit"},
	};
	struct UITheme {
	public:
		UIPreset preset{};
		ImGuiStyle config{};
		void setUITheme(UIPreset newPreset);
	private:

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
