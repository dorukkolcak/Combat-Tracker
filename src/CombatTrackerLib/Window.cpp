#include "CombatTrackerLib/Window.h"

#include <imgui_impl_glfw.h>
#include <cassert>

namespace CombatTrackerLib {
	Window::Window(int initialWidth, int initialHeight, std::string_view applicationName) : 
		Unique(),
		m_currentWidth{ initialWidth },
		m_currentHeight{ initialHeight }
	{

		// Window is created for OpenGL 3.3 Core Profile.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		auto mainScale{ ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()) };
		m_window = glfwCreateWindow(
			static_cast<int> (mainScale * static_cast<float>(m_currentWidth)),
			static_cast<int> (mainScale * static_cast<float>(m_currentHeight)),
			applicationName.data(),
			nullptr,
			nullptr
		);
		assert(m_window != nullptr);
		glfwSetWindowUserPointer(m_window, this);
		glfwMakeContextCurrent(m_window);
		glfwSetFramebufferSizeCallback(m_window, []([[maybe_unused]] GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
		});
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {

			// The window size callback records window resize in fullscreens as well 
			// which we don't want for remembering previos window dimensions/locations.
			// So we check whether the window is fullscreen first.
			if (glfwGetWindowMonitor(window) != nullptr) {
				return;
			}
			auto* const userPointer{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };
			userPointer->m_currentWidth = width;
			userPointer->m_currentHeight = height;
		});
		glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int xpos, int ypos) {

			// Same fullscreen consideration here.
			if (glfwGetWindowMonitor(window) != nullptr) {
				return;
			}
			auto* const userPointer{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };
			userPointer->m_currentXPos = xpos;
			userPointer->m_currentYPos = ypos;
		});
		glfwGetWindowPos(m_window, &m_currentXPos, &m_currentYPos);
		glfwGetWindowSize(m_window, &m_currentWidth, &m_currentHeight);
	}
	Window::~Window() {
		glfwDestroyWindow(m_window);
	}
	void Window::setVsync(bool enabled) {
		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
	}
	void Window::setFullscreen(bool enabled) {
		auto* monitor{ glfwGetPrimaryMonitor() };
		const auto* glfwVideoMode{ glfwGetVideoMode(monitor) };
		if (enabled) {
			glfwSetWindowMonitor(m_window, monitor, 0, 0, glfwVideoMode->width, glfwVideoMode->height, glfwVideoMode->refreshRate);
		}
		else {
			glfwSetWindowMonitor(m_window, nullptr, m_currentXPos, m_currentYPos, m_currentWidth, m_currentHeight, GLFW_DONT_CARE);
		}
	}
	void Window::setResizable(bool enabled) {
		if (enabled) {
			glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, GLFW_TRUE);
		}
		else {
			glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, GLFW_FALSE);
		}
	}
	void Window::update() {
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}