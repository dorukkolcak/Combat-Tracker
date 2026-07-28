#include "imgui_impl_glfw.h"
#include <cassert>

#include "Window.h"

namespace CombatTrackerLib {
	Window::Window(const WindowSpecification& specification) : 
		m_windowSpecification{ specification } 
	{

		// Window is created for OpenGL 3.3 Core Profile.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		if (specification.isResizable) {
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		}
		float mainScale{ ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()) };
		m_window = glfwCreateWindow( static_cast<int> (mainScale * specification.width), static_cast<int> (mainScale * specification.height), specification.title.data(), NULL, NULL);
		assert(m_window != nullptr);
		glfwSetWindowUserPointer(m_window, this);
		glfwMakeContextCurrent(m_window);
		if (specification.isVsyncEnabled) {
			glfwSwapInterval(1);
		}
	}
	void Window::update() {
		glfwSwapBuffers(m_window);
	}
	void Window::destroy() {
		glfwDestroyWindow(m_window);
	}
	int Window::getWindowShouldClose() {
		return glfwWindowShouldClose(m_window);
	}
	GLFWwindow* Window::getGLFWwindow() {
		return m_window;
	}
}