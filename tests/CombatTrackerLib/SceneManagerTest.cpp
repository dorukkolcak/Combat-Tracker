#include "tests/CombatTrackerLib/SceneManagerTest.h"

namespace CombatTrackerLib {
	Scene* SceneManagerTest::queueStubScene() {
		auto scene{ std::make_unique<StubScene>() };

		// .get() returns a pointer to the own object.
		// So even if the pointer is moved, the reference will point to the same object.
		auto* sceneReference{ scene.get() };

		m_sceneManager.queueAddScene(std::move(scene));
		return sceneReference;
	}
	void SceneManagerTest::SetUp() {
		m_sceneOneReference = queueStubScene();
		m_sceneTwoReference = queueStubScene();
	}
	TEST_F(SceneManagerTest, TestQueueAddSceneDefer) { EXPECT_TRUE(m_sceneManager.getScenes().empty()); }
	TEST_F(SceneManagerTest, TestDequeueMoveToStack) {
		m_sceneManager.dequeueSceneOperations();
		const auto& scenes{ m_sceneManager.getScenes() };
		ASSERT_EQ(std::size_t{ 2 }, scenes.size());
		EXPECT_EQ(m_sceneOneReference, scenes[0].get());
		EXPECT_EQ(m_sceneTwoReference, scenes[1].get());
	}
	TEST_F(SceneManagerTest, TestDequeueAddOnTopOfTheStack) {
		m_sceneManager.dequeueSceneOperations();
		auto* sceneThreeReference{ queueStubScene() };
		m_sceneManager.dequeueSceneOperations();
		const auto& scenes{ m_sceneManager.getScenes() };
		ASSERT_EQ(std::size_t{ 3 }, scenes.size());
		EXPECT_EQ(sceneThreeReference, scenes[2].get());
	}
	TEST_F(SceneManagerTest, TestDequeueRemoveFromStack) {
		m_sceneManager.dequeueSceneOperations();
		m_sceneManager.queueDeleteScene(m_sceneOneReference);
		EXPECT_EQ(std::size_t{ 2 }, m_sceneManager.getScenes().size());
		m_sceneManager.dequeueSceneOperations();
		const auto& scenes{ m_sceneManager.getScenes() };
		ASSERT_EQ(std::size_t{ 1 }, scenes.size());
		EXPECT_EQ(m_sceneTwoReference, scenes[0].get());
	}
	TEST_F(SceneManagerTest, TestDequeueIgnoreDeletionOutsideStack) {
		m_sceneManager.dequeueSceneOperations();
		const auto foreignScene{ std::make_unique<StubScene>() };
		m_sceneManager.queueDeleteScene(foreignScene.get());
		m_sceneManager.dequeueSceneOperations();
		EXPECT_EQ(std::size_t{ 2 }, m_sceneManager.getScenes().size());
	}
	TEST_F(SceneManagerTest, TestDequeueReplaceTransitionedSceneInPlace) {
		m_sceneManager.dequeueSceneOperations();
		auto transitionScene{ std::make_unique<StubScene>() };
		auto* transitionSceneReference{ transitionScene.get() };
		m_sceneManager.queueSceneTransition(m_sceneOneReference, std::move(transitionScene));
		EXPECT_EQ(std::size_t{ 2 }, m_sceneManager.getScenes().size());
		m_sceneManager.dequeueSceneOperations();
		const auto& scenes{ m_sceneManager.getScenes() };
		ASSERT_EQ(std::size_t{ 2 }, scenes.size());
		EXPECT_EQ(transitionSceneReference, scenes[0].get());
		EXPECT_EQ(m_sceneTwoReference, scenes[1].get());
	}
}
