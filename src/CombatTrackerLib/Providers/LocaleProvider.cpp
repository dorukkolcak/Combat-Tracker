#include "CombatTrackerLib/Providers/LocaleProvider.h"
#include "CombatTrackerLib/Utilities/FileUtils.h"

#include <iostream>
#include <format>
#include <algorithm>

namespace CombatTrackerLib {
	LocaleProvider::LocaleProvider(std::string_view localeCode, const std::filesystem::path* resourceDirectoryPathPointer) :
		m_resourceDirectoryPathPointer{resourceDirectoryPathPointer}
	{
		buildAvailableLocales();
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
			m_textViewMap.clear();
			m_listViewMap.clear();
			return false;
		}

		// Since we check for std::nullopt, we can access std::optional safely using ->.
		buildViewMaps(*m_locale, std::string{});
		return true;
	}

	/*
	This is a recursive function that builds maps as follows:
	If the underlying json node is of type string, it corresponds to a text. 
	So we just get a string reference to the item's value and store it in the text view map.
	If the underlying type is an array, it is a list. 
	So we get the string_view vector that corresponds to the list, clear it and insert elements sequentially, similar to the string case.
	As for the final case, the type being an object, it corresponds to a nested type.
	So we have to recursively reach the nested values. We do this by iterating over the nested item's items.
	*/
	void LocaleProvider::buildViewMaps(const nlohmann::json& node, const std::string& pointerPrefix) {
		for (const auto& item : node.items()) {
			const auto pointer{ std::format("{}/{}", pointerPrefix, item.key()) };

			if (item.value().is_object()) {
				buildViewMaps(item.value(), pointer);
			}
			else if (item.value().is_string()) {
				m_textViewMap[pointer] = item.value().get_ref<const nlohmann::json::string_t&>();
			}
			else if (item.value().is_array()) {
				auto& list{ m_listViewMap[pointer] };
				list.clear();
				for (const auto& element : item.value()) {
					list.push_back(element.get_ref<const nlohmann::json::string_t&>());
				}
			}
		}
	}
	void LocaleProvider::buildAvailableLocales() {
		if (m_resourceDirectoryPathPointer == nullptr) {
			return;
		}

		// Iterate through every file in the locale directory and read their filename and endonym key.
		// This causes us to read one of the files twice in construction which is fine I guess.
		std::error_code errorCode{};
		const std::filesystem::path localeDirectory{ (*m_resourceDirectoryPathPointer) / "assets/Locales" };
		for (const auto& entry : std::filesystem::directory_iterator{ localeDirectory, errorCode }) {
			if (!entry.is_regular_file(errorCode) || entry.path().extension() != ".json") {
				continue;
			}
			const auto contents{ readJSONFile(entry.path()) };
			if (contents == std::nullopt || !contents->contains("endonym")) {
				continue;
			}

			// Since entry and contents have local scope, we can't use string_view here.
			// Which should be fine as we call this function once throughout the entire application lifecycle.
			// So no string-heap allocation in hot paths.
			m_availableLocales.emplace_back(
				entry.path().stem().string(),
				(*contents)["endonym"].get<std::string>()
			);
		}
	}
	std::span<const LocaleInfo> LocaleProvider::getAvailableLocales() const {
		return m_availableLocales;
	}
	std::string_view LocaleProvider::getText(std::string_view key) const {
		const auto entry{ m_textViewMap.find(key) };
		if (entry == m_textViewMap.end()) {
			return key; // If this happens, there is something wrong. So this is mostly a debug statement.
		}
		return entry->second;
	}
	std::span<const std::string_view> LocaleProvider::getTextList(std::string_view keyPrefix) const {
		const auto entry{ m_listViewMap.find(keyPrefix) };
		if (entry == m_listViewMap.end()) {
			return {};
		}
		return entry->second;
	}
}