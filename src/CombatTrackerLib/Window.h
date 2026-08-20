#pragma once

#include "CombatTrackerLib/Utilities/GenericUtils.h"

#include <string_view>

// Pay attention to the ordering here.
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

namespace CombatTrackerLib {
	class Window : public Unique<Window> {
	public:
		Window() = delete;
		Window(
			int initialWidth,
			int initialHeight,
			std::string_view applicationName
		);

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