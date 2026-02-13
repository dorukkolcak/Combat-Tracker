#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <cassert>
#include "Window.h"

namespace CombatTrackerLib {
	Window::Window(const WindowSpecification& specification) : INITIAL_WINDOW_SPECIFICATION(specification) {
		// Window is created for OpenGL 3.3 Core Profile.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		if (specification.isResizable) {
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		}
		if (specification.isVsyncEnabled) {
			glfwSwapInterval(1);
		}
		float mainScale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); 
		window = glfwCreateWindow((int) (mainScale * specification.width), (int) (mainScale * specification.height), specification.title.c_str(), NULL, NULL);
		assert(window != nullptr);
		glfwMakeContextCurrent(window);
	}
	Window::~Window() {
		destroy();
	}
	void Window::update() {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	void Window::destroy() {
		glfwDestroyWindow(window);
	}
	void Window::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}
	int Window::getWindowShouldClose() {
		return glfwWindowShouldClose(window);
	}
	GLFWwindow*& Window::getGLFWwindow() {
		return window;
	}
}