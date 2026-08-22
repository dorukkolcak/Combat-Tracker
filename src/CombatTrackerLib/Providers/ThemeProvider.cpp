#include "CombatTrackerLib/Providers/ThemeProvider.h"

#include "CombatTrackerLib/Providers/DirectoryProvider.h"
#include "CombatTrackerLib/Utilities/FileUtils.h"

namespace CombatTrackerLib {
	ThemeProvider::ThemeProvider(const DirectoryProvider& directoryProvider) {
		std::error_code errorCode{};
		const std::filesystem::path themePath{directoryProvider.getResourceDirectory() / "assets/Themes"};
		for (const auto& entry : std::filesystem::directory_iterator(themePath, errorCode)) {
			const auto themeFile{readJSONFile(entry.path())};
			if (themeFile == std::nullopt) {
				continue;
			}
			const auto themeName{entry.path().stem().string()};
			mapThemeToStyle(themeName, *themeFile);
		}
	}

	/*
	The theme provider is responsible for returning the style, not actually setting the theme.
	This is for a couple of reasons:
	1) Theme setting requires an active ImGui context. The context management should be done solely by the App class.
	2) This class would be hard to test as it will be coupled with the instantiation of an ImGui context.
	3) The application class can instantiate the provider before booting up an ImGui context 
	as it is the root class managing lifetimes, contexts, etc.
	*/
	const ImGuiStyle* ThemeProvider::getStyle(std::string_view themeName) const {
		const auto theme{m_themeToStyle.find(themeName)};
		if (theme == m_themeToStyle.end()) {
			return nullptr;
		}
		return &theme->second;
	}

	void ThemeProvider::mapThemeToStyle(const std::string& themeName, const nlohmann::json& fileContents) {

		// This call does the initial insertion, this is why we also pass a string reference here instead of string_view.
		auto& style{m_themeToStyle[themeName]};
		const auto baseNode{fileContents.find("base")};
		if (baseNode != fileContents.end() && baseNode->is_string()) {
			const auto& baseName{baseNode->get_ref<const nlohmann::json::string_t&>()};
			if (baseName == "dark") {
				ImGui::StyleColorsDark(&style);
			} else if (baseName == "light") {
				ImGui::StyleColorsLight(&style);
			}
		}

		const auto colorNodes{fileContents.find("colors")};
		if (colorNodes == fileContents.end()) {
			return;
		}
		for (int i{}; i < ImGuiCol_COUNT; ++i) {
			const auto* const colorName{ImGui::GetStyleColorName(i)};
			const auto color{colorNodes->find(colorName)};
			if (color != colorNodes->end() && color->is_array() && color->size() == 4) {
				style.Colors[i] = color->get<ImVec4>();
			}
		}
	}
}
