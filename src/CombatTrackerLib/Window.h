#pragma once

#include <string_view>

// Pay attention to the ordering here.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace CombatTrackerLib {
	class Window {
	public:
		Window() = default;
		Window(int width, int height, std::string_view applicationName);

		// A Window object exists as a singleton so it should be non-copiable and non-movable.
		Window(const Window& other) = delete;
		Window& operator=(const Window& other) = delete;

		Window(Window&& other) = delete;
		Window& operator=(Window&& other) = delete;

		~Window();

		void setVsync(bool enabled);
		void setFullscreen(bool enabled);
		void setResizable(bool enabled);

		void update();
	private:
		GLFWwindow* m_window{};
		int m_currentWidth{};
		int m_currentHeight{};
		int m_currentXPos{};
		int m_currentYPos{};
	};
}