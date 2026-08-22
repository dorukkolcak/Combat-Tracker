#include "CombatTrackerLib/Utilities/GenericUtils.h"

#include <gtest/gtest.h>

namespace CombatTrackerLib {
	TEST(OverridableIntTest, TestOverride) {
		OverridableInt val{};
		EXPECT_FALSE(val.isOverridden());
		val.setOverride(5);
		EXPECT_TRUE(val.isOverridden());
		EXPECT_EQ(5, val.get());
		val.setDefault(10);
		val.clearOverride();
		EXPECT_FALSE(val.isOverridden());
		EXPECT_EQ(10, val.get());
	}

	TEST(OverridableIntTest, TestDefault) {
		OverridableInt val{};
		EXPECT_EQ(0, val.getDefault());
		EXPECT_EQ(val.getDefault(), val.get());
		val.setDefault(20);
		EXPECT_EQ(20, val.getDefault());
		val.setOverride(5);
		EXPECT_EQ(5, val.get());
		EXPECT_EQ(20, val.getDefault());
	}
}
