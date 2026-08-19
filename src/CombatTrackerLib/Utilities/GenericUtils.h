#pragma once

#include <cstddef>
#include <type_traits>
#include <algorithm>
#include <optional>
#include <string_view>
#include <nlohmann/json.hpp>

namespace CombatTrackerLib {

	/*
	See https://stackoverflow.com/questions/34596768/stdunordered-mapfind-using-a-type-different-than-the-key-type
	and https://medium.com/@simontoth/daily-bit-e-of-c-heterogeneous-lookup-in-unordered-containers-42e98bb6cd79
	For a lookup in unordered map, the type of the key must exactly match the type used as a template argument.
	For the string case, this causes a temporary string to get created which results in a heap allocation.
	Since this map will be indexed tens of times each frame, this results in A LOT of heap allocations.
	We use something called heterogeneous lookup which was provided to unordered_map's in C++ 20.
	This allows us to index unordered_map using a different key type.
	In our case this is std::string_view which doesn't cause heap allocations.
	To do this we provide something called a transparent hasher and a transparent equality comparator (StringHash struct).
	Which is responsible for hashing the provided string using different template type arguments.
	This mechanism is only used with lookups via find(). So [], at(), and other lookup mechanisms shouldn't be used.
	All in all, the use of unordered_map here allows us to only create the locale structure once (or on change)
	and index the values using a string_view, resulting in no allocations per lookup.
	Since we do a lot of lookups in each frame this drastically improves performance.
	This is also used elsewhere (ThemeProvider).
	*/
	struct StringHash {
		using is_transparent = void;
		std::size_t operator()(std::string_view key) const {
			return std::hash<std::string_view>{}(key);
		}
	};

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
	template <int LOWER_BOUND, int UPPER_BOUND>
	void from_json(const nlohmann::json& contents, BoundedInt<LOWER_BOUND, UPPER_BOUND>& boundedInt) {
		boundedInt = contents.get<int>();
	}
	template <int LOWER_BOUND, int UPPER_BOUND>
	void to_json(nlohmann::json& contents, const BoundedInt<LOWER_BOUND, UPPER_BOUND>& boundedInt) {
		contents = boundedInt.get();
	}

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

	// Since we don't really know how the default is computed from the json file,
	// We reset the override and set the default elsewhere.
	inline void from_json(const nlohmann::json& contents, OverridableInt& overridableInt) {
		if (contents.is_null()) {
			overridableInt.clearOverride();
		}
		else {
			overridableInt.setOverride(contents.get<int>());
		}
	}

	// If overridden we should hold the value. Otherwise we can derive the value from other values.
	inline void to_json(nlohmann::json& contents, const OverridableInt& overridableInt) {
		if (overridableInt.isOverridden()) {
			contents = overridableInt.get();
		}
		else {
			contents = nullptr;
		}
	}
}