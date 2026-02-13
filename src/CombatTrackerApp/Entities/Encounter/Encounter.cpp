#include "Encounter.h"

namespace CombatTrackerApp {
	Encounter::Encounter(std::vector<std::unique_ptr<Monster>> encounterMonsters, std::vector<std::unique_ptr<PlayerCharacter>> encounterPlayers) :
		monsters(std::move(encounterMonsters)),
		players(std::move(encounterPlayers)),
		isCompleted(false),
		totalXPReward(0)
		{
		// TODO: Find a way for ID generation.
		for (int i = 0; i < monsters.size(); ++i) {
			totalXPReward += (monsters[i])->getXP();
		}
	}
	Encounter::~Encounter() {}
	void Encounter::setInitiatives(std::map<std::uint32_t, std::uint16_t> initiatives) {
		mappedInitiatives = std::move(initiatives);
	}
	void Encounter::setAsCompleted() {
		isCompleted = true;
	}
	void Encounter::addTurn(std::unique_ptr<Turn> turn) {
		turns.push_back(std::move(turn));
	}
}