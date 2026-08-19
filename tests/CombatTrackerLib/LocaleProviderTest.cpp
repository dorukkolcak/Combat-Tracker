/*
#include "tests/CombatTrackerLib/LocaleProviderTest.h"
#include "CombatTrackerLib/Providers/DirectoryProvider.h"

#include <filesystem>
#include <system_error>
#include <ranges>
#include <algorithm>

namespace CombatTrackerLib {
	void LocaleProviderTest::SetUp() {

		// Find all localization files and set up in memory structure for each one.
		static const DirectoryProvider s_directoryProvider{ "CombatTrackerLibTests" };
		std::error_code errorCode{};
		for (const auto& item : std::filesystem::directory_iterator{ s_directoryProvider.getResourceDirectory() / "assets/Locales", errorCode }) {
			if (item.is_regular_file(errorCode) && item.path().extension() == ".json") {
				std::string localeCode{ item.path().stem().string() };
				m_locales.emplace_back(
					localeCode,
					std::make_unique<LocaleProvider>(localeCode, &s_directoryProvider.getResourceDirectory())
				);
			}
		}
		ASSERT_GE(m_locales.size(), MIN_NUMBER_OF_LOCALES) << "At least two locale files needed to compare." << std::endl;
	}
	TEST_F(LocaleProviderTest, TestLocaleFilesLoad) {
		for (const auto& locale : m_locales) {
			const auto areEntriesAvailable{ !getTextViewMap(*locale.provider).empty() && !getListViewMap(*locale.provider).empty() };
			EXPECT_TRUE(areEntriesAvailable) << "No entries were read for locale " << locale.code << std::endl;
		}
	}
	TEST_F(LocaleProviderTest, TestLocaleFilesSharedTextKeys) {
		const auto& referenceLocale{ m_locales.begin() };
		const auto& referenceLocaleProvider{ *referenceLocale->provider };
		const auto& referenceLocaleCode{ referenceLocale->code };

		for (const auto& locale : m_locales | std::views::drop(1)) { // Drop the first locale from the iteration order.
			const auto& localeProvider{ *locale.provider };
			EXPECT_EQ(getTextViewMap(localeProvider).size(), getTextViewMap(referenceLocaleProvider).size())
				<< "Locale " << locale.code << " has different number of text keys than " << referenceLocaleCode << std::endl;
			for (const auto& textKeyValuePair : getTextViewMap(localeProvider)) {
				EXPECT_NE(
					getTextViewMap(referenceLocaleProvider).end(),
					getTextViewMap(referenceLocaleProvider).find(textKeyValuePair.first)
				) << "Locale " << referenceLocaleCode << " is missing the text key " << textKeyValuePair.first << std::endl;
			}
		}
	}
	TEST_F(LocaleProviderTest, TestLocaleFilesSharedListKeys) {
		const auto& referenceLocale{ m_locales.begin() };
		const auto& referenceLocaleProvider{ *referenceLocale->provider };
		const auto& referenceLocaleCode{ referenceLocale->code };

		for (const auto& locale : m_locales | std::views::drop(1)) {
			const auto& localeProvider{ *locale.provider };
			EXPECT_EQ(getListViewMap(localeProvider).size(), getListViewMap(referenceLocaleProvider).size())
				<< "Locale " << locale.code << " has different number of list keys than " << referenceLocaleCode << std::endl;
			for (const auto& listKeyValuePair : getListViewMap(localeProvider)) {
				EXPECT_NE(
					getListViewMap(referenceLocaleProvider).end(),
					getListViewMap(referenceLocaleProvider).find(listKeyValuePair.first)
				) << "Locale " << referenceLocaleCode << " is missing the list key " << listKeyValuePair.first << std::endl;
			}
		}
	}
	TEST_F(LocaleProviderTest, TestLocaleFilesSharedListLengths) {
		const auto& referenceLocale{ m_locales.begin() };
		const auto& referenceLocaleProvider{ *referenceLocale->provider };
		const auto& referenceLocaleCode{ referenceLocale->code };

		for (const auto& locale : m_locales | std::views::drop(1)) {
			const auto& localeProvider{ *locale.provider };
			for (const auto& listKeyValuePair : getListViewMap(localeProvider)) {
				const auto referenceEntry{ getListViewMap(referenceLocaleProvider).find(listKeyValuePair.first) };

				// This shouldn't happen because of the LocaleFilesShareTheSameListKeys test case.
				if (referenceEntry == getListViewMap(referenceLocaleProvider).end()) {
					continue;
				}
				EXPECT_EQ(listKeyValuePair.second.size(), referenceEntry->second.size())
					<< "The list " << listKeyValuePair.first << " has a different length in locale "
					<< locale.code << " than in " << referenceLocaleCode << std::endl;
			}
		}
	}
	TEST_F(LocaleProviderTest, TestGetTextReturnValueForKnownKeys) {
		for (const auto& locale : m_locales) {
			for (const auto& textKeyValuePair : getTextViewMap(*locale.provider)) {
				EXPECT_EQ(textKeyValuePair.second, locale.provider->getText(textKeyValuePair.first))
					<< "Locale " << locale.code << " did not serve the stored value of the text key "
					<< textKeyValuePair.first << std::endl;
			}
		}
	}
	TEST_F(LocaleProviderTest, TestGetTextReturnValueForMissingKeys) {
		for (const auto& locale : m_locales) {
			EXPECT_EQ(MISSING_KEY, locale.provider->getText(MISSING_KEY))
				<< "Locale " << locale.code << " did not hand back the key of a missing text entry." << std::endl;
		}
	}
	TEST_F(LocaleProviderTest, TestGetTextListReturnValueForKnownKeys) {
		for (const auto& locale : m_locales) {
			for (const auto& listKeyValuePair : getListViewMap(*locale.provider)) {
				EXPECT_TRUE(std::ranges::equal(listKeyValuePair.second, locale.provider->getTextList(listKeyValuePair.first)))
					<< "Locale " << locale.code << " did not serve the stored entries of the list key "
					<< listKeyValuePair.first << std::endl;
			}
		}
	}
	TEST_F(LocaleProviderTest, TestGetTextListReturnValueForMissingKeys) {
		for (const auto& locale : m_locales) {
			EXPECT_TRUE(locale.provider->getTextList(MISSING_KEY).empty())
				<< "Locale " << locale.code << " handed back a non empty span for a missing list entry." << std::endl;
		}
	}
	TEST_F(LocaleProviderTest, TestSetLocaleForUnknownLocale) {
		auto& localeProvider{ *m_locales.begin()->provider };

		EXPECT_FALSE(localeProvider.setLocale(UNKNOWN_LOCALE_CODE));
		EXPECT_TRUE(getTextViewMap(localeProvider).empty())
			<< "Text views into the previous locale file survived a failed locale switch." << std::endl;
		EXPECT_TRUE(getListViewMap(localeProvider).empty())
			<< "List views into the previous locale file survived a failed locale switch." << std::endl;
	}
	TEST_F(LocaleProviderTest, TestSetLocaleWithoutAResourceDirectory) {
		LocaleProvider localeProvider{};

		EXPECT_FALSE(localeProvider.setLocale(m_locales.begin()->code))
			<< "A locale was read without a resource directory to read it from." << std::endl;
	}
	TEST_F(LocaleProviderTest, TestSetLocaleReplacePreviousLocale) {
		auto& localeProvider{ *m_locales.begin()->provider };
		const auto& replacementLocale{ *(m_locales.begin() + 1) };

		ASSERT_TRUE(localeProvider.setLocale(replacementLocale.code));

		const auto& textKeyValuePair{ *getTextViewMap(*replacementLocale.provider).begin() };
		EXPECT_EQ(textKeyValuePair.second, localeProvider.getText(textKeyValuePair.first))
			<< "The locale provider did not properly change after switching to " << replacementLocale.code << std::endl;
	}
}
*/
