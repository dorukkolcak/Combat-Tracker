#pragma once

#include "CombatTrackerApp/Entities/Creature.h"

#include <string_view>
#include <array>
#include <algorithm>

namespace CombatTrackerApp {

	struct ChallengeRating {
		double value{};
		std::string_view label{};
		int XP{};
	};

	
	// Exact same idea as the creature side, only applied to XP currently (Can possibly extend it).
	class MonsterDerivedStats {
	public:
		constexpr int getXP() const {
			return m_XP;
		}
	private:
		friend class Monster;

		// The XP is derived from the challenge rating table rather than saved, as the CR fully determines it.
		int m_XP{};
	};
	struct MonsterStatBlock {
		double CR{};
		MonsterDerivedStats derived{};
	};

	// These should be ordered for UI purposes.
	// We process them in sequential order for monster creation widgets.
	inline constexpr std::array<ChallengeRating, 34> CHALLENGE_RATINGS{ {
		{0.0,   "0",   10},
		{0.125, "1/8", 25},
		{0.25,  "1/4", 50},
		{0.5,   "1/2", 100},
		{1.0,   "1",   200},
		{2.0,   "2",   450},
		{3.0,   "3",   700},
		{4.0,   "4",   1100},
		{5.0,   "5",   1800},
		{6.0,   "6",   2300},
		{7.0,   "7",   2900},
		{8.0,   "8",   3900},
		{9.0,   "9",   5000},
		{10.0,  "10",  5900},
		{11.0,  "11",  7200},
		{12.0,  "12",  8400},
		{13.0,  "13",  10000},
		{14.0,  "14",  11500},
		{15.0,  "15",  13000},
		{16.0,  "16",  15000},
		{17.0,  "17",  18000},
		{18.0,  "18",  20000},
		{19.0,  "19",  22000},
		{20.0,  "20",  25000},
		{21.0,  "21",  33000},
		{22.0,  "22",  41000},
		{23.0,  "23",  50000},
		{24.0,  "24",  62000},
		{25.0,  "25",  75000},
		{26.0,  "26",  90000},
		{27.0,  "27",  105000},
		{28.0,  "28",  120000},
		{29.0,  "29",  135000},
		{30.0,  "30",  155000}
	} };

	class Monster : public Creature {
	public:
		friend class MonsterTest;

		// Same consideration as Creature constructor, should only be used for encounter running.
		// The XP is not passed in, as the challenge rating already determines it.
		Monster(CreatureStatBlock statBlock, double creatureCR);

		Monster(const Monster& monster) = delete;
		Monster& operator=(const Monster& monster) = delete;

		Monster(Monster&& monster) noexcept = default;
		Monster& operator=(Monster&& monster) noexcept = default;

		~Monster() = default;

		double getCR() const;
		int getXP() const;

		// For a monster the proficiency tier is the challenge rating itself.
		static constexpr void resolveDerived(CreatureStatBlock& statBlock, MonsterStatBlock& monsterStatBlock) {
			Creature::resolveDerived(statBlock, monsterStatBlock.CR);
			monsterStatBlock.derived.m_XP = findXP(monsterStatBlock.CR);
		}
		
		// The only extra condition for validation is that the challenge rating maps to a valid XP value.
		static constexpr bool isValid(const CreatureStatBlock& statBlock, const MonsterStatBlock& monsterStatBlock) {
			return Creature::isValid(statBlock)
				&& std::ranges::find(CHALLENGE_RATINGS, monsterStatBlock.CR, &ChallengeRating::value) != CHALLENGE_RATINGS.end();
		}
	private:

		MonsterStatBlock m_statBlockMonsterPortion{};

		// Even though this is a linear search over CHALLENGE_RATINGS, it is always a constant time operation as the array has fixed size.
		static constexpr int findXP(double CR) {
			auto entry{ std::ranges::find(CHALLENGE_RATINGS, CR, &ChallengeRating::value) };
			if (entry != CHALLENGE_RATINGS.end()) {
				return entry->XP;
			}
			else {
				return CHALLENGE_RATINGS.front().XP;
			}
		}
	};
}
