#pragma once

#include <string>

// Pay attention to the ordering here.
#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace CombatTrackerLib {

	// The App class also needs to see the struct definition so we don't nest this struct inside Window's definition.
	struct WindowSpecification {
		std::string title{};

		unsigned int width{};
		unsigned int height{};

		bool isResizable{};
		bool isVsyncEnabled{};
	};

	class Window {
	public:
		explicit Window(const WindowSpecification& specification);
		~Window();

		void update();
		void destroy();
		int getWindowShouldClose();
		GLFWwindow*& getGLFWwindow();
	private:
		WindowSpecification m_windowSpecification{};
		GLFWwindow* m_window{};
	};
}