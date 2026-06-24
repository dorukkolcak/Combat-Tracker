#pragma once

#include <vector>
#include <memory>
#include <map>

#include "../PlayerCharacter/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Turn/Turn.h"



namespace CombatTrackerApp {
	
	// Forward declaration to prevent linker errors.
	class Turn;
	class Encounter {
	public:

		// The arguments are moved so pass them by value.
		Encounter(
			std::vector<std::unique_ptr<Monster>> encounterMonsters, 
			std::vector<std::unique_ptr<PlayerCharacter>> encounterPlayers
		);
		~Encounter();

		void setInitiatives(std::map<int, int> initiatives);
		void setAsCompleted();
		void addTurn(std::unique_ptr<Turn> turn);

	private:
		static int inline currentIDValue{ 1 };
		int m_ID{};
		bool m_isCompleted{};
		int m_totalXPReward{};

		/*
		Multiple encounters can own Creatures, so use shared pointers. 
		Only encounters will actually own instances of Creatures, as they are created solely for encounters. 
		Using a unique_ptr might also be fine as there is only one encounter active at a time and instances of Creatures are unique.
		For encounter creation, we will just save them in a database/JSON format anyway so this entity is only for running encounters.
		*/
		std::vector<std::unique_ptr<Monster>> m_monsters{};
		std::vector<std::unique_ptr<PlayerCharacter>> m_players{};

		// This is the initial mapping of initiatives so it contains every single creature.
		std::map<int, int> m_mappedInitiatives{}; //Maps initiatives by creature ID, which is unique by definition.

		// An encounter is the sole owner of its turns.
		std::vector<std::unique_ptr<Turn>> m_turns{};
	};
}