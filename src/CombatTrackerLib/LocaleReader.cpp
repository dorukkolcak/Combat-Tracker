#include "CombatTrackerLib/LocaleReader.h"
#include "CombatTrackerLib/Utilities/FileUtils.h"

#include <iostream>
#include <format>

namespace CombatTrackerLib {
	LocaleReader::LocaleReader(std::string_view localeCode, std::filesystem::path* resourceDirectoryPathPointer) :
		m_resourceDirectoryPathPointer{resourceDirectoryPathPointer} 
	{
		const auto isLocaleSet{ setLocale(localeCode) };
		if (!isLocaleSet) {
			std::cerr << "Locale file couldn't be read." << std::endl;
		}
	}
	LocaleReader::LocaleReader(LocaleReader&& reader) noexcept {
		this->m_locale = std::move(reader.m_locale);
		this->m_resourceDirectoryPathPointer = reader.m_resourceDirectoryPathPointer;
		reader.m_locale = std::nullopt;
		reader.m_resourceDirectoryPathPointer = nullptr;
	}
	LocaleReader& LocaleReader::operator=(LocaleReader&& reader) noexcept {
		if (this == &reader) {
			return *this;
		}
		this->m_locale = std::move(reader.m_locale);
		this->m_resourceDirectoryPathPointer = reader.m_resourceDirectoryPathPointer;
		reader.m_locale = std::nullopt;
		reader.m_resourceDirectoryPathPointer = nullptr;
		return *this;
	}
	bool LocaleReader::setLocale(std::string_view localeCode) {
		const std::filesystem::path localePath{ (*m_resourceDirectoryPathPointer) / "assets/Locales" / std::format("{}.json", localeCode) };
		m_locale = readJSONFile(localePath);
		if (m_locale == std::nullopt) {
			return false;
		}
		return true;
	}
	std::string_view LocaleReader::getText(std::string_view key) const {
		return m_locale->at(key.data());
	}
	const std::vector<nlohmann::json>& LocaleReader::getList(std::string_view key) const {
		return m_locale->at(key.data()).get_ref<const std::vector<nlohmann::json>&>();
	}
}