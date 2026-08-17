#pragma once

#include <cstddef>
#include <type_traits>
#include <algorithm>
#include <optional>

namespace CombatTrackerLib {
	template<typename E> requires std::is_enum_v<E>
	constexpr std::size_t enumToSizeT(E enumerator) {
		return static_cast<std::size_t>(enumerator);
	}

	// A wrapper over int that defines lower and upper bounds.
	// This is particularly useful for ability scores/modifiers which have set bounds.
	template <int LOWER_BOUND, int UPPER_BOUND> requires (LOWER_BOUND <= UPPER_BOUND)
	class BoundedInt {
	public:
		constexpr BoundedInt() = default;
		constexpr BoundedInt(int value) : m_value{ std::clamp(value, LOWER_BOUND, UPPER_BOUND) } {}
		constexpr BoundedInt& operator=(int value) {
			m_value = std::clamp(value, LOWER_BOUND, UPPER_BOUND);
			return *this;
		}
		constexpr int get() const {
			return m_value;
		}
		static constexpr int getLowerBound() {
			return LOWER_BOUND;
		}
		static constexpr int getUpperBound() {
			return UPPER_BOUND;
		}
	private:
		int m_value{ std::clamp(0, LOWER_BOUND, UPPER_BOUND) };
	};

	/*
	An integer that is normally computed from some formula, but can also be set by hand.
	This is to essentially communicate that a value has been overridden to the saved data format.
	For example, we might calculate an ability modifier from its ability score.
	But the user might decide to override that value for some reason (ie. item effects).
	*/
	class OverridableInt {
	public:
		constexpr OverridableInt() = default;
		constexpr explicit OverridableInt(int defaultValue) : m_defaultValue{ defaultValue } {}

		// Written by whatever computes the value, and kept fresh even while an override is in place.
		constexpr OverridableInt& setDefault(int value) {
			m_defaultValue = value;
			return *this;
		}

		// Written by hand. Holding a value is what makes the integer overridden,
		// so the two can never disagree the way a separate flag could.
		constexpr OverridableInt& setOverride(int value) {
			m_override = value;
			return *this;
		}
		constexpr OverridableInt& clearOverride() {
			m_override.reset();
			return *this;
		}
		constexpr int get() const {
			return m_override.value_or(m_defaultValue);
		}
		constexpr int getDefault() const {
			return m_defaultValue;
		}
		constexpr bool isOverridden() const {
			return m_override.has_value();
		}
	private:
		int m_defaultValue{};
		std::optional<int> m_override{};
	};
}