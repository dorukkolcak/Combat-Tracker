#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

namespace CombatTrackerLib {
	class Scene;
	class SceneManager {
	public:
		SceneManager();

		// Should exist as a singleton owned by the App class.
		// So the manager should be non-copiable/movable.
		SceneManager(const SceneManager& other) = delete;
		SceneManager& operator=(const SceneManager& other) = delete;
		SceneManager(SceneManager&& other) = delete;
		SceneManager& operator=(SceneManager&& other) = delete;

		~SceneManager();

		void queueAddScene(std::unique_ptr<Scene> scene);
		void queueDeleteScene(Scene* sceneToBeDeleted);
		void queueSceneTransition(Scene* transitionedFrom, std::unique_ptr<Scene> transitionedTo);

		/*
		So here is the rough idea of how scenes work:
		sceneStack maintains our active scenes at an instance.
		These scenes are laid out on the application from top to bottom, the first index being the "lowest level" scene.
		Scene additions, deletions and transitions happen at the last step of the application main loop, so after rendering.
		IMPORTANT CONSIDERATION:
		We should never transition from a scene that is going to be deleted.
		Otherwise, the raw pointer keys in sceneTransitionQueue will be dangling.
		QUEUEING SCENE ADDITIONS:
		To add a new scene on top, we just push_back to sceneAdditionQueue.
		I am currently only considering allowing addition to on top of the sceneStack but I might change this later.
		QUEUEING SCENE DELETIONS:
		To remove a scene, we first search the scene in the vector (passed by as a raw pointer that points to the object pointed by the unique_ptr)
		To find it we just dereference the pointers and check whether a unique_ptr points to the same object as the raw pointer.
		After finding the scene to be deleted, we keep a reference to it as a raw pointer in the sceneDeletionQueue.
		QUEUEING SCENE TRANSITIONS:
		We pass a raw pointer/unique_ptr pair to an unordered map:
		Our keys refer to scenes that are going to be transitioned FROM.
		The values refer to the scenes that are going to be transitioned TO.
		Each key and value is unique by the way, despite unordered_map's only condition being making the keys unique.
		So this is how we maintain the mapping of scene transitions.
		DEQUEUEING SCENE OPERATIONS:
		1) We first handle the sceneTransitionQueue by just moving unique_ptrs in the sceneStack that correspond to a key in sceneTransitionQueue,
		to the value (which is a unique_ptr) of the associated key. We can do this easily with std::move()
		2) Then we handle sceneDeletionQueue. We just erase the unique_ptrs in the sceneStack that correspond to some raw pointer in the sceneDeletionQueue.
		3) Finally we handle sceneAdditionQueue. We just transfer ownership of the object pointed by the elements of the sceneAdditionQueue to new unique_ptrs.
		Once again this can easily be done with std::move
		You might notice that scene transitions are simply deletion + addition consecutively.
		*/
		void dequeueSceneOperations();
		const std::vector<std::unique_ptr<Scene>>& getScenes() const;
	private:

		// I prefer using vector as sort of a stack because it is much better at resizing, compared to a pure stack data structure.
		std::vector<std::unique_ptr<Scene>> m_sceneStack{};
		std::vector<std::unique_ptr<Scene>> m_sceneAdditionQueue{};
		std::vector<Scene*> m_sceneDeletionQueue{};
		std::unordered_map<Scene*, std::unique_ptr<Scene>> m_sceneTransitionQueue{};
	};
}
