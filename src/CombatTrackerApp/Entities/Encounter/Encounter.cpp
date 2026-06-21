#include "Encounter.h"

namespace CombatTrackerApp {

	// TODO: Find a way for ID generation.
	Encounter::Encounter(
	std::vector<std::unique_ptr<Monster>> encounterMonsters, 
		std::vector<std::unique_ptr<PlayerCharacter>> encounterPlayers
	) :
		m_monsters{ std::move(encounterMonsters) },
		m_players{ std::move(encounterPlayers) },
		m_isCompleted{false},
		m_totalXPReward{ 0 },
		m_ID{-1}
		{
		for (size_t i{}; i < m_monsters.size(); ++i) {
			m_totalXPReward += (m_monsters[i])->getXP();
		}
	}
	Encounter::~Encounter() {}
	void Encounter::setInitiatives(std::map<int, int> initiatives) {
		m_mappedInitiatives = std::move(initiatives);
	}
	void Encounter::setAsCompleted() {
		m_isCompleted = true;
	}
	void Encounter::addTurn(std::unique_ptr<Turn> turn) {
		m_turns.push_back(std::move(turn));
	}
}