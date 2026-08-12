#include "CombatTrackerLib/LocaleProvider.h"
#include "CombatTrackerLib/Utilities/FileUtils.h"

#include <iostream>
#include <format>

namespace CombatTrackerLib {
	LocaleProvider::LocaleProvider(std::string_view localeCode, std::filesystem::path* resourceDirectoryPathPointer) :
		m_resourceDirectoryPathPointer{resourceDirectoryPathPointer}
	{
		const auto isLocaleSet{ setLocale(localeCode) };
		if (!isLocaleSet) {
			std::cerr << "Locale file couldn't be read." << std::endl;
		}
	}

	// One thing to note about this function is that we don't need to clear the view map in success.
	// As the localization files should have matching keys so a new view map will just overwrite the existing one.
	bool LocaleProvider::setLocale(std::string_view localeCode) {
		if (m_resourceDirectoryPathPointer == nullptr) {
			return false;
		}
		const std::filesystem::path localePath{ (*m_resourceDirectoryPathPointer) / "assets/Locales" / std::format("{}.json", localeCode) };
		m_locale = readJSONFile(localePath);
		if (m_locale == std::nullopt) {
			m_viewMap.clear();
			return false;
		}

		/*
		Since we check for std::nullopt, we can access std::optional safely using ->.
		See https://json.nlohmann.me/api/basic_json/flatten/.
		Flattening allows us to access nested types easily, which is most of the localization file.
		Array values are accessed via indicies (ie array/0, array/1). We can just use enumToSizeT() to access them (see GenericUtils).
		We can't really get the array itself easily with this method, which should be fine. 
		As the caller can create their own loop and pass the JSON pointer key to accessor function using std::format().
		*/
		m_locale = m_locale->flatten();
		for (const auto& item : m_locale->items()) {
			m_viewMap[item.key()] = item.value().get_ref<const nlohmann::json::string_t&>();
		}
		return true;
	}
	std::string_view LocaleProvider::getText(std::string_view key) const {
		const auto entry{ m_viewMap.find(key) };
		if (entry == m_viewMap.end()) {
			return key; // If this happens, there is something wrong. So this is mostly a debug statement.
		}
		return entry->second;
	}
}