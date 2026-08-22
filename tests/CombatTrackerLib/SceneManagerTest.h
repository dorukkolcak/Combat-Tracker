#pragma once

#include "CombatTrackerLib/Scene.h"
#include "CombatTrackerLib/SceneManager.h"

#include <gtest/gtest.h>

#include <cstddef>
#include <memory>

namespace CombatTrackerLib {

	// A stub scene whose only purposes is to act as a data placeholder for SceneManager to operate on.
	class StubScene : public Scene {
	public:
		void notifyScene(const ImGuiIO&) override {}

		void updateScene(double) override {}

		void renderUI() override {}

		void renderScene() override {}
	};

	class SceneManagerTest : public testing::Test {
	protected:
		void SetUp() override;

		Scene* queueStubScene();

		SceneManager m_sceneManager{};
		Scene* m_sceneOneReference{};
		Scene* m_sceneTwoReference{};
	};
}
