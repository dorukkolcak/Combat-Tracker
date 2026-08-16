#pragma once

#include "CombatTrackerLib/LocaleProvider.h"

#include <gtest/gtest.h>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace CombatTrackerLib {
	class LocaleProviderTest : public testing::Test {
	protected:
		struct Locale {
			std::string code{};

			// LocaleProvider has deleted move/copy constructor/assignment operator so we indirectly do these operations via unique_ptrs.
			// Normally for the application side we should only have one locale provider.
			// But for easy testing, we use multiple. Kind of a weird workaround but it works.
			std::unique_ptr<LocaleProvider> provider{}; 
		};

		void SetUp() override;
		
		/*
		Normally, we would use friend classes to provide access to private member variables here.
		We actually still do this in the base fixture class. 
		However, we can't do this for TEST_F calls which create derived classes from the fixture classes
		Since derived classes don't inherit friendship, they can't access private members.
		So we provide this protected member function to grant member variable access.
		*/
		const auto& getTextViewMap(const LocaleProvider& provider) const {
			return provider.m_textViewMap;
		}
		const auto& getListViewMap(const LocaleProvider& provider) const {
			return provider.m_listViewMap;
		}

		std::vector<Locale> m_locales{};
		inline static constexpr std::size_t MIN_NUMBER_OF_LOCALES{ 2 };
	};
}
