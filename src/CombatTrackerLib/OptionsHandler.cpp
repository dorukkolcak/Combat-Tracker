#include "CombatTrackerLib/OptionsHandler.h"
#include "CombatTrackerLib/Utilities/FileUtils.h"

#include <algorithm>

namespace CombatTrackerLib {
	OptionsHandler::OptionsHandler(
		const std::filesystem::path& userDataDirectory,
		std::array<std::string, enumToSizeT(OptionsCategory::COUNT)> categoryKeys,
		nlohmann::json defaultOptions
	) :	Unique(),
		m_optionsFilePath(userDataDirectory / "options.json"),
		m_categoryKeys{ std::move(categoryKeys) },
		m_defaultOptions(std::move(defaultOptions)),
		m_options(m_defaultOptions)
	{
		// See https://json.nlohmann.me/api/basic_json/merge_patch/.
		// Essentially what we do here is we patch the default options with the read options file
		// for each existing key in the original document.
		m_options.merge_patch(readJSONFile(m_optionsFilePath).value_or(nlohmann::json::object()));
	}

	// This might be a massive mistake, implies that the options file itself is written back to only on application close.
	// So upon a crash, we lose all the previous options. But I really like the management of this.
	OptionsHandler::~OptionsHandler() {
		writeJSONFile(m_optionsFilePath, m_options);
	}
	bool OptionsHandler::validateOption(OptionsCategory category, std::string_view option) const {
		const auto& categoryKey{ m_categoryKeys[enumToSizeT(category)] };
		if (!m_defaultOptions[categoryKey].contains(option)) {
			return false;
		}
		return true;
	}
	void OptionsHandler::registerImmediateOptionChangeCallback(
		OptionsCategory category,
		const std::string& option,
		std::function<void()> callback
	) {
		m_immediateCallbacks[{category, option}] = callback;
	}
	void OptionsHandler::registerDeferredOptionChangeCallback(
		OptionsCategory category,
		const std::string& option,
		std::function<void()> callback
	) {
		const auto pair{ std::pair{category, option} };
		m_deferredCallbacks[pair] = callback;
		m_deferredCallbackStatuses[pair] = false;
	}
	void OptionsHandler::invokeDeferredCallbacks() {
		for (const auto& callbackPair : m_deferredCallbacks) {
			callbackPair.second();
			m_deferredCallbackStatuses[callbackPair.first] = false;
		}
	}
	void OptionsHandlerProxy::invokeDeferredCallbacks(OptionsHandler& handler) {
		handler.invokeDeferredCallbacks();
	}
}
