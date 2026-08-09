#include "CombatTrackerLib/Window.h"

#include <imgui_impl_glfw.h>
#include <cassert>

namespace CombatTrackerLib {
	Window::Window(WindowSpecification* specification, std::string_view applicationName) : 
		m_specificationPointer{ specification } 
	{

		// Window is created for OpenGL 3.3 Core Profile.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		if (m_specificationPointer->isResizable) {
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		}
		auto mainScale{ ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()) };
		m_window = glfwCreateWindow(
			static_cast<int> (mainScale * static_cast<float>(m_specificationPointer->width)),
			static_cast<int> (mainScale * static_cast<float>(m_specificationPointer->height)),
			applicationName.data(),
			nullptr,
			nullptr
		);
		assert(m_window != nullptr);
		glfwSetWindowUserPointer(m_window, this);
		glfwMakeContextCurrent(m_window);
		if (m_specificationPointer->isVsyncEnabled) {
			glfwSwapInterval(1);
		}
	}
	Window::~Window() {
		glfwDestroyWindow(m_window);
	}
	void Window::update() {
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}