#include "CombatTrackerLib/SceneManager.h"

#include "CombatTrackerLib/Scene.h"

#include <algorithm>

namespace CombatTrackerLib {
	SceneManager::SceneManager() = default;
	SceneManager::~SceneManager() = default;

	void SceneManager::queueAddScene(std::unique_ptr<Scene> scene) {
		m_sceneAdditionQueue.push_back(std::move(scene));
	}

	void SceneManager::queueDeleteScene(Scene* sceneToBeDeleted) {
		m_sceneDeletionQueue.push_back(sceneToBeDeleted);
	}

	void SceneManager::queueSceneTransition(Scene* transitionedFrom, std::unique_ptr<Scene> transitionedTo) {
		m_sceneTransitionQueue.insert({transitionedFrom, std::move(transitionedTo)});
	}

	void SceneManager::dequeueSceneOperations() {

		// Handle scene transition.
		for (auto& transitionedTo : m_sceneTransitionQueue) {
			for (auto& transitionedFrom : m_sceneStack) {
				if (transitionedFrom.get() == transitionedTo.first) {
					transitionedFrom = std::move(transitionedTo.second);
					break;
				}
			}
		}
		m_sceneTransitionQueue.clear();

		// Handle scene deletion.
		for (auto& deletedScene : m_sceneDeletionQueue) {
			for (auto& scene : m_sceneStack) {
				if (scene.get() == deletedScene) {

					/*
					This is kinda dumb. The main reason we are using smart pointers in the first place is that they automatically manage memory.
					But I explicitly "delete" the memory associated with the smart pointer here.
					The thing is I don't like transferring the ownership of a new object to the "deleted" smart pointer either.
					As after deletion we might have less scenes than the previous state, aka less smart pointers.
					So regardless some smart pointers will just be left null with this design and I need to explicitly remove the null ones.
					*/
					scene.reset();
					break;
				}
			}
		}

		// Remove the deleted scene unique_ptrs from the sceneStack.
		m_sceneStack.erase(
			std::remove_if(
				m_sceneStack.begin(),
				m_sceneStack.end(),
				[](const std::unique_ptr<Scene>& scenePointer) { return scenePointer == nullptr; }
			),
			m_sceneStack.end()
		);
		m_sceneDeletionQueue.clear();

		// Handle sceneAddition
		for (auto& addedScene : m_sceneAdditionQueue) {
			m_sceneStack.push_back(std::move(addedScene));
		}
		m_sceneAdditionQueue.clear();
	}

	const std::vector<std::unique_ptr<Scene>>& SceneManager::getScenes() const {
		return m_sceneStack;
	}
}
