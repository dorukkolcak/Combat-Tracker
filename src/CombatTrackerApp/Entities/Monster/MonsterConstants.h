#pragma once

#include <algorithm>
#include <array>
#include <string_view>

namespace CombatTrackerApp {

	// This struct is only used for UI purposes so it is put here.
	struct ChallengeRating {
		double value{};
		std::string_view label{};
		int XP{};
		int proficiencyBonus{};
	};

	// These should be ordered for UI purposes.
	// We process them in sequential order for monster creation widgets.
	inline constexpr std::array<ChallengeRating, 34> CHALLENGE_RATINGS {{
		{0.0,   "0",   10,     2},
		{0.125, "1/8", 25,     2},
		{0.25,  "1/4", 50,     2},
		{0.5,   "1/2", 100,    2},
		{1.0,   "1",   200,    2},
		{2.0,   "2",   450,    2},
		{3.0,   "3",   700,    2},
		{4.0,   "4",   1100,   2},
		{5.0,   "5",   1800,   3},
		{6.0,   "6",   2300,   3},
		{7.0,   "7",   2900,   3},
		{8.0,   "8",   3900,   3},
		{9.0,   "9",   5000,   4},
		{10.0,  "10",  5900,   4},
		{11.0,  "11",  7200,   4},
		{12.0,  "12",  8400,   4},
		{13.0,  "13",  10000,  5},
		{14.0,  "14",  11500,  5},
		{15.0,  "15",  13000,  5},
		{16.0,  "16",  15000,  5},
		{17.0,  "17",  18000,  6},
		{18.0,  "18",  20000,  6},
		{19.0,  "19",  22000,  6},
		{20.0,  "20",  25000,  6},
		{21.0,  "21",  33000,  7},
		{22.0,  "22",  41000,  7},
		{23.0,  "23",  50000,  7},
		{24.0,  "24",  62000,  7},
		{25.0,  "25",  75000,  8},
		{26.0,  "26",  90000,  8},
		{27.0,  "27",  105000, 8},
		{28.0,  "28",  120000, 8},
		{29.0,  "29",  135000, 9},
		{30.0,  "30",  155000, 9}
	}};
	constexpr const ChallengeRating& findChallengeRating(double CR) {

		// This overload of std::ranges::find projects values in CHALLENGE_RATINGS to ChallengeRating::value, which is the underlying CR value.
		const auto entry{ std::ranges::find(CHALLENGE_RATINGS, CR, &ChallengeRating::value) };
		if (entry != CHALLENGE_RATINGS.end()) {
			return *entry;
		}
		return CHALLENGE_RATINGS.front();
	}
}
