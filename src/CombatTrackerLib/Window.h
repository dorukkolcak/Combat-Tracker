#pragma once

#include <cstdint>
#include <string>

// Pay attention to the ordering here.
#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace CombatTrackerLib {
	struct WindowSpecification {
		std::string title{};

		std::uint16_t width{};
		std::uint16_t height{};

		bool isResizable{};
		bool isVsyncEnabled{};
	};

	class Window {
	public:
		Window(const WindowSpecification& specification);
		~Window();

		void update();
		void destroy();
		int getWindowShouldClose();
		GLFWwindow*& getGLFWwindow();
	private:
		WindowSpecification windowSpecification{};
		GLFWwindow* window{};
	};
}