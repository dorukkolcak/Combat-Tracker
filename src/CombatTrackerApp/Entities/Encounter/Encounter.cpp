#include "Encounter.h"

namespace CombatTrackerApp {

	// TODO: Find a way for ID generation.
	Encounter::Encounter(std::vector<std::unique_ptr<Monster>> encounterMonsters, std::vector<std::unique_ptr<PlayerCharacter>> encounterPlayers) :
		monsters{ std::move(encounterMonsters) },
		players{ std::move(encounterPlayers) },
		isCompleted{false},
		totalXPReward{ 0 },
		ID{-1}
		{
		for (size_t i{}; i < monsters.size(); ++i) {
			totalXPReward += (monsters[i])->getXP();
		}
	}
	Encounter::~Encounter() {}
	void Encounter::setInitiatives(std::map<int, int> initiatives) {
		mappedInitiatives = std::move(initiatives);
	}
	void Encounter::setAsCompleted() {
		isCompleted = true;
	}
	void Encounter::addTurn(std::unique_ptr<Turn> turn) {
		turns.push_back(std::move(turn));
	}
}