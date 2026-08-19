#pragma once

#include <cassert>
#include <type_traits>

namespace CombatTrackerLib {

	/*
	This is a base class that attempts to enforce the uniqueness/singleton-ish property of some objects.
	We do this using CRTF and a static variable that gets instantiated every time a new derived class is created.
	Now this is not a proper implementation of a singleton because I hate the global state singletons bring.
	The way we enforce the uniqueness property is via deleting move/copy constructor/assignment operators.
	And a static boolean flag that tracks whether an object of this type was instantiated.
	*/
	template <typename Derived>
	class Unique {
	public:
		Unique(const Unique& unique) = delete;
		Unique& operator=(const Unique& unique) = delete;

		Unique(Unique&& unique) noexcept = delete;
		Unique& operator=(Unique&& unique) noexcept = delete;

	protected:
		Unique() {
			static_assert(std::is_base_of_v<Unique, Derived>); // Evaluates to true if Unique is a base type of Derived.
			assert(!s_isAlive);
			s_isAlive = true;
		}
		~Unique() {
			s_isAlive = false;
		}

	private:
		inline static bool s_isAlive{ false };
	};
}
