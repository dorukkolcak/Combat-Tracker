#pragma once

#include "CombatTrackerLib/Utilities/GenericUtils.h"
#include "CombatTrackerLib/Utilities/Unique.h"

#include <array>
#include <vector>
#include <string>
#include <optional>
#include <filesystem>
#include <string_view>
#include <nlohmann/json.hpp>
#include <functional>

namespace CombatTrackerLib {
	enum class OptionsCategory {
		GENERAL, // Related to language, etc.
		APPEARANCE, // Related to themes, fonts, vsync, fullscreen, etc.
		COUNT
	};
	class OptionsHandler : public Unique<OptionsHandler> {
	public:
		OptionsHandler() = delete;
 
		OptionsHandler(
			const std::filesystem::path& userDataDirectory,
			std::array<std::string, enumToSizeT(OptionsCategory::COUNT)> categoryKeys,
			nlohmann::json defaultOptions
		);

		~OptionsHandler();

		template<typename T>
		T getOption(OptionsCategory category, std::string_view option) const;

		template<typename T>
		bool setOption(OptionsCategory category, std::string_view option, T value);

	private:

		std::filesystem::path m_optionsFilePath{};
		std::array<std::string, enumToSizeT(OptionsCategory::COUNT)> m_categoryKeys{};
		nlohmann::json m_defaultOptions{};
		nlohmann::json m_options{};

		bool validateOption(OptionsCategory category, std::string_view option) const;
	};

	template <typename T>
	T OptionsHandler::getOption(OptionsCategory category, std::string_view option) const {
		return validateOption(category, option) ? m_options[m_categoryKeys[enumToSizeT(category)]][option].get<T>() : T{};
	}
	template <typename T>
	bool OptionsHandler::setOption(OptionsCategory category, std::string_view option, T value) {
		if (validateOption(category, option)) {
			m_options[m_categoryKeys[enumToSizeT(category)]][option] = value;
			return true;
		}
		return false;
	}
}
