#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <span>
#include <functional>
#include <optional>
#include <filesystem>
#include <string_view>
#include <nlohmann/json.hpp>

namespace CombatTrackerLib {

	// Struct to hold the entire list of available localization file information.
	// An application might need this large information to choose a language through the UI for example.
	struct LocaleInfo {
		std::string code{};
		std::string endonym{};
	};
	class LocaleProvider {
	public:
		friend class LocaleProviderTest;

		LocaleProvider() = default;
		LocaleProvider(std::string_view localeCode, const std::filesystem::path* resourceDirectoryPathPointer);

		LocaleProvider(const LocaleProvider& reader) = delete;
		LocaleProvider& operator=(const LocaleProvider& reader) = delete;

		LocaleProvider(LocaleProvider&& reader) noexcept = delete;
		LocaleProvider& operator=(LocaleProvider&& reader) noexcept = delete;

		~LocaleProvider() = default;
		
		bool setLocale(std::string_view localeCode);

		std::string_view getText(std::string_view key) const;

		// Provide a span to string_view objects. So this is something like a view of views, lol.
		// This also has the advantage of preserving size information which is super nice.
		std::span<const std::string_view> getTextList(std::string_view keyPrefix) const;

		std::span<const LocaleInfo> getAvailableLocales() const;
	private:

		void buildViewMaps(const nlohmann::json& node, const std::string& pointerPrefix);
		void buildAvailableLocales();

		/*
		See https://stackoverflow.com/questions/34596768/stdunordered-mapfind-using-a-type-different-than-the-key-type
		and https://medium.com/@simontoth/daily-bit-e-of-c-heterogeneous-lookup-in-unordered-containers-42e98bb6cd79
		For a lookup in unordered map, the type of the key must exactly match the type used as a template argument.
		For the string case, this causes a temporary string to get created which results in a heap allocation.
		Since this map will be indexed tens of times each frame, this results in A LOT of heap allocations.
		We use something called heterogeneous lookup which was provided to unordered_map's in C++ 20.
		This allows us to index unordered_map using a different key type.
		In our case this is std::string_view which doesn't cause heap allocations.
		To do this we provide something called a transparent hasher and a transparent equality comparator (StringHash struct).
		Which is responsible for hashing the provided string using different template type arguments.
		This mechanism is only used with lookups via find(). So [], at(), and other lookup mechanisms shouldn't be used.
		All in all, the use of unordered_map here allows us to only create the locale structure once (or on change) 
		and index the values using a string_view, resulting in no allocations per lookup. 
		Since we do a lot of lookups in each frame this drastically improves performance.
		*/
		struct StringHash {
			using is_transparent = void;
			std::size_t operator()(std::string_view key) const {
				return std::hash<std::string_view>{}(key);
			}
		};
		std::unordered_map<
			std::string,
			std::string_view,
			StringHash,
			std::equal_to<>
		> m_textViewMap{};
		
		// We hold a seperate map for lists as well. If not, we would have to use std::format.
		// Using std::format creates string temporaries so we allocate a lot of string temporaries per frame while iterating over a list.
		std::unordered_map<
			std::string,
			std::vector<std::string_view>,
			StringHash,
			std::equal_to<>
		> m_listViewMap{};

		std::optional<nlohmann::json> m_locale{};
		std::vector<LocaleInfo> m_availableLocales{};
		const std::filesystem::path* m_resourceDirectoryPathPointer{};
	};
}