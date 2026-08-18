#include "CombatTrackerLib/Utilities/GenericUtils.h"

#include <gtest/gtest.h>

namespace CombatTrackerLib {
	TEST(BoundedIntTest, TestBoundClamp) {
		BoundedInt<0, 5> val{};
		val = -5;
		EXPECT_EQ(0, val.get());
		val = 10;
		EXPECT_EQ(5, val.get());
	}
}
