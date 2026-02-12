#pragma once

#include <cstdint>
#include <string>
// Pay attention to the ordering here.
#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace CombatTrackerLib {
	struct WindowSpecification {
		std::uint16_t width;
		std::uint16_t height;
		std::string title;
		bool isResizable;
	};

	class Window {
	public:
		Window(const WindowSpecification& specification);
		~Window();

		void destroy();
		int getWindowShouldClose();
		GLFWwindow*& getGLFWwindow();
	private:
		const WindowSpecification INITIAL_WINDOW_SPECIFICATION;
		GLFWwindow* window;

		void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
	};
}