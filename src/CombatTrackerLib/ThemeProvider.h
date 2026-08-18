#pragma once

#include "CombatTrackerLib/Utilities/GenericUtils.h"

#include <string>
#include <functional>
#include <string_view>
#include <filesystem>
#include <imgui.h>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <ranges>

namespace nlohmann {

	// Make it so that json nodes serialize to ImVec4 when read.
	// This is a template specialization of the adl_serializer struct, which belongs to the nlohmann namespace.
	// So the specialization itself needs to belong to the same namespace.
	template<>
	struct adl_serializer<ImVec4> {
		static void from_json(const json& node, ImVec4& color) {
			color = ImVec4{
				node.at(0).get<float>(),
				node.at(1).get<float>(),
				node.at(2).get<float>(),
				node.at(3).get<float>()
			};
		}
	};
}

namespace CombatTrackerLib {
	class ThemeProvider {
	public:
		ThemeProvider() = default;

		ThemeProvider(const ThemeProvider& provider) = delete;
		ThemeProvider& operator=(const ThemeProvider& provider) = delete;

		ThemeProvider(ThemeProvider&& provider) noexcept = delete;
		ThemeProvider& operator=(ThemeProvider&& provider) noexcept = delete;

		~ThemeProvider() = default;

		// This is the actual constructor of the class. This is because the theme provider needs to be initialized
		// after initializing the ImGui context which we can't achieve via constructor member initializer list.
		void init(std::string_view initialThemeName, const std::filesystem::path* resourceDirectoryPathPointer);

		// This can be static as well but I prefer to have ThemeProvider as a singleton object where theme operations are handled.
		void changeTheme(std::string_view themeName);

		auto getThemeNames() const {
			return m_themeToStyle | std::views::keys;
		}
	private:
		std::unordered_map<
			std::string,
			ImGuiStyle,
			StringHash,
			std::equal_to<>
		> m_themeToStyle{};

		void mapThemeToStyle(const std::string& themeName, const nlohmann::json& fileContents);
	};
}