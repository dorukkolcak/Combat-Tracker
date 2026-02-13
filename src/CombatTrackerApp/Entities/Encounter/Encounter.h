#pragma once

#include <vector>
#include <memory>
#include <map>

#include "../PlayerCharacter/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Turn/Turn.h"

class Turn;

namespace CombatTrackerApp {
	class Encounter {
	private:
		std::uint32_t ID;
		bool isCompleted;
		std::uint64_t totalXPReward;
		/*
		Multiple encounters can own Creatures, so use shared pointers. 
		Only encounters will actually own instances of Creatures, as they are created solely for encounters. 
		Using a unique_ptr might also be fine as there is only one encounter active at a time and instances of Creatures are unique.
		For encounter creation, we will just save them in a database/JSON format anyway so this entity is only for running encounters.
		*/
		std::vector<std::unique_ptr<Monster>> monsters;
		std::vector<std::unique_ptr<PlayerCharacter>> players;
		// This is the initial mapping of initiatives so it contains every single creature.
		std::map<std::uint32_t, std::uint16_t> mappedInitiatives; //Maps initiatives by creature ID, which is unique by definition.
		// An encounter is the sole owner of its turns.
		std::vector<std::unique_ptr<Turn>> turns;
	public:
		Encounter(std::vector<std::unique_ptr<Monster>> encounterMonsters, std::vector<std::unique_ptr<PlayerCharacter>> encounterPlayers);
		~Encounter();

		void setInitiatives(std::map<std::uint32_t, std::uint16_t> initiatives);
		void setAsCompleted();
		void addTurn(std::unique_ptr<Turn> turn);
	};
}