#pragma once

#include "CombatTrackerLib/Providers/DirectoryProvider.h"
#include "CombatTrackerLib/Utilities/GenericUtils.h"

#include <nlohmann/json.hpp>
#include <filesystem>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace CombatTrackerLib {

	// Struct to hold the entire list of available localization file information.
	// An application might need this large information to choose a language through the UI for example.
	struct LocaleInfo {
		std::string code{};
		std::string endonym{};
	};
	class LocaleProvider : public Unique<LocaleProvider> {
	public:
		//friend class LocaleProviderTest;

		LocaleProvider() = delete;
		LocaleProvider(const DirectoryProvider& directoryProvider, std::string_view localeCode);
		~LocaleProvider() = default;

		bool setLocale(std::string_view localeCode);
		std::string_view getText(std::string_view key) const;

		// Provide a span to string_view objects. So this is something like a view of views, lol.
		// This also has the advantage of preserving size information which is super nice.
		std::span<const std::string_view> getTextList(std::string_view keyPrefix) const;
		std::span<const LocaleInfo> getAvailableLocales() const;

	private:
		std::unordered_map<std::string, std::string_view, StringHash, std::equal_to<>> m_textViewMap{};

		// We hold a seperate map for lists as well. If not, we would have to use std::format.
		// Using std::format creates string temporaries so we allocate a lot of string temporaries per frame while iterating over a list.
		std::unordered_map<std::string, std::vector<std::string_view>, StringHash, std::equal_to<>> m_listViewMap{};
		std::vector<LocaleInfo> m_availableLocales{};
		std::optional<nlohmann::json> m_locale{}; // The views reference this locale.

		// The locale provider needs to remember the resource path directory.
		// So we hold a reference to the directory provider via a pointer.
		const DirectoryProvider* m_directoryProviderPointer{};

		void buildViewMaps(const nlohmann::json& node, const std::string& pointerPrefix);
		void buildAvailableLocales();
	};
}
