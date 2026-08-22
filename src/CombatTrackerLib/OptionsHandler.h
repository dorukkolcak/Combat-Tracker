#pragma once

#include "CombatTrackerLib/Utilities/GenericUtils.h"

#include <nlohmann/json.hpp>

#include <any>
#include <array>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace CombatTrackerLib {
	class App;
	enum class OptionsCategory {
		GENERAL,    // Related to language, etc.
		APPEARANCE, // Related to themes, fonts, vsync, fullscreen, etc.
		COUNT
	};

	class OptionsHandler : public Unique<OptionsHandler> {
	public:
		friend class OptionsHandlerProxy;

	public:
		OptionsHandler() = delete;
		OptionsHandler(
			const std::filesystem::path& userDataDirectory,
			std::array<std::string, enumToSizeT(OptionsCategory::COUNT)> categoryKeys,
			nlohmann::json defaultOptions
		);
		~OptionsHandler();
		template <typename T> T getOption(OptionsCategory category, std::string_view option) const;
		template <typename T> bool setOption(OptionsCategory category, std::string_view option, T value);
		void registerImmediateOptionChangeCallback(
			OptionsCategory category,
			const std::string& option,
			std::function<void()> callback
		);
		void registerDeferredOptionChangeCallback(
			OptionsCategory category,
			const std::string& option,
			std::function<void()> callback
		);

	private:
		std::map<std::pair<OptionsCategory, std::string>, std::function<void()>> m_immediateCallbacks{};
		std::map<std::pair<OptionsCategory, std::string>, std::function<void()>> m_deferredCallbacks{};
		std::map<std::pair<OptionsCategory, std::string>, bool> m_deferredCallbackStatuses{};
		std::filesystem::path m_optionsFilePath{};
		std::array<std::string, enumToSizeT(OptionsCategory::COUNT)> m_categoryKeys{};
		nlohmann::json m_defaultOptions{};
		nlohmann::json m_options{};

	private:
		bool validateOption(OptionsCategory category, std::string_view option) const;
		void invokeDeferredCallbacks();
	};

	// See https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Friendship_and_the_Attorney-Client.
	// This is a C++ idiom for granting friendship to only specific parts of a class, not the entire contents.
	// invokeDeferredCallbacks() is kind of a dangerous function so only App should be able to call it.
	class OptionsHandlerProxy {
	private:
		friend class App;

	private:
		static void invokeDeferredCallbacks(OptionsHandler& handler);
	};

	template <typename T> T OptionsHandler::getOption(OptionsCategory category, std::string_view option) const {
		return validateOption(category, option)
		           ? m_options[m_categoryKeys[enumToSizeT(category)]][option].template get<T>()
		           : T{};
	}

	template <typename T> bool OptionsHandler::setOption(OptionsCategory category, std::string_view option, T value) {
		if (validateOption(category, option)) {

			const auto pair{std::pair(category, std::string{option})};
			if (m_immediateCallbacks.contains(pair)) {
				m_immediateCallbacks[pair]();
			}
			if (m_deferredCallbacks.contains(pair)) {
				m_deferredCallbackStatuses[pair] = true;
			}
			m_options[m_categoryKeys[enumToSizeT(category)]][option] = value;
			return true;
		}
		return false;
	}
}
