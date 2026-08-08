#pragma once

#include <cstddef>
#include <type_traits>

namespace CombatTrackerLib {
	template<typename E> requires std::is_enum_v<E>
	inline std::size_t enumToSizeT(E enumerator) {
		return static_cast<std::size_t>(enumerator);
	}
}