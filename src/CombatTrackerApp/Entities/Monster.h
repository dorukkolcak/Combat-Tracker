#pragma once

#include "CombatTrackerApp/Entities/Creature.h"

#include <string_view>
#include <array>
#include <algorithm>

namespace CombatTrackerApp {
	struct ChallengeRating {
		double value{};
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
		{0.0,   10},
		{0.125, 25},
		{0.25, 50},
		{0.5, 100},
		{1.0,   200},
		{2.0,   450},
		{3.0,   700},
		{4.0,   1100},
		{5.0,   1800},
		{6.0,   2300},
		{7.0,   2900},
		{8.0,   3900},
		{9.0,   5000},
		{10.0,  5900},
		{11.0,  7200},
		{12.0,  8400},
		{13.0,  10000},
		{14.0,  11500},
		{15.0,  13000},
		{16.0,  15000},
		{17.0,  18000},
		{18.0,  20000},
		{19.0,  22000},
		{20.0,  25000},
		{21.0,  33000},
		{22.0,  41000},
		{23.0,  50000},
		{24.0,  62000},
		{25.0,  75000},
		{26.0,  90000},
		{27.0,  105000},
		{28.0,  120000},
		{29.0,  135000},
		{30.0,  155000}
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
