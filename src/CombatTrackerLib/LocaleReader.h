#pragma once

#include <string>
#include <vector>
#include <optional>
#include <filesystem>
#include <string_view>
#include <nlohmann/json.hpp>

namespace CombatTrackerLib {
	class LocaleReader {
	public:
		LocaleReader() = default;
		LocaleReader(std::string_view localeCode, std::filesystem::path* resourceDirectoryPathPointer);

		LocaleReader(const LocaleReader& reader) = delete;
		LocaleReader& operator=(const LocaleReader& reader) = delete;

		LocaleReader(LocaleReader&& reader) noexcept;
		LocaleReader& operator=(LocaleReader&& reader) noexcept;

		~LocaleReader() = default;
		
		bool setLocale(std::string_view localeCode);

		std::string_view getText(const std::string& key) const;
		const nlohmann::json::array_t& getList(const std::string& key) const;
	private:
		std::optional<nlohmann::json> m_locale{};
		std::filesystem::path* m_resourceDirectoryPathPointer{};
	};
}