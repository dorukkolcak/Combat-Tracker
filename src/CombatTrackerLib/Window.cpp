#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cassert>

#include "Window.h"

namespace CombatTrackerLib {
	Window::Window(const WindowSpecification& specification) : 
		windowSpecification{ specification } 
	{

		// Window is created for OpenGL 3.3 Core Profile.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		if (specification.isResizable) {
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		}
		float mainScale{ ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()) };
		window = glfwCreateWindow( static_cast<int> (mainScale * specification.width), static_cast<int> (mainScale * specification.height), specification.title.c_str(), NULL, NULL);
		assert(window != nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwMakeContextCurrent(window);
		if (specification.isVsyncEnabled) {
			glfwSwapInterval(1);
		}
	}

	// Once again should be left empty as we have a separate function for destroying the window.
	Window::~Window() {}
	void Window::update() {
		glfwSwapBuffers(window);
	}
	void Window::destroy() {
		glfwDestroyWindow(window);
	}
	int Window::getWindowShouldClose() {
		return glfwWindowShouldClose(window);
	}
	GLFWwindow*& Window::getGLFWwindow() {
		return window;
	}
}