#pragma once

#include <string_view>

// Pay attention to the ordering here.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace CombatTrackerLib {
	struct WindowSpecification {
		unsigned int width{};
		unsigned int height{};
		bool isResizable{};
		bool isVsyncEnabled{};
	};
	class Window {
	public:
		Window() = default;
		Window(WindowSpecification* specification, std::string_view applicationName);

		// A Window object exists as a singleton so it should be non-copiable and non-movable.
		Window(const Window& other) = delete;
		Window& operator=(const Window& other) = delete;

		Window(Window&& other) = delete;
		Window& operator=(Window&& other) = delete;

		~Window();

		void update();
	private:
		WindowSpecification* m_specificationPointer{};
		GLFWwindow* m_window{};
	};
}