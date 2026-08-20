#pragma once

#include "CombatTrackerLib/Utilities/GenericUtils.h"
#include "CombatTrackerLib/Providers/DirectoryProvider.h"

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
		// I don't think to_json is possible as we have no way of passing the name of the color to to_json.
	};
}

namespace CombatTrackerLib {
	class ThemeProvider : public Unique<ThemeProvider> {
	public:
		ThemeProvider() = delete;
		explicit ThemeProvider(const DirectoryProvider& directoryProvider);
		~ThemeProvider() = default;

		const ImGuiStyle* getStyle(std::string_view themeName) const;
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
