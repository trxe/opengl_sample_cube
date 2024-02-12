#include <iostream>
#include <string>

#include <GLFW/glfw3.h>
#include "Renderer.hpp"
#include "Utils.hpp"

static GLFWwindow* _window = nullptr;
static void glfw_callback_error(int error, const char* msg) {
	log_error(msg);
}

int main() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSetErrorCallback(glfw_callback_error);
	if (!glfwInit()) {
		log_error("GLFW failed to initialize");
		exit(1);
	}
	_window = glfwCreateWindow(1920, 1080, "Rotating Cube", NULL, NULL);
	if (!_window) {
		log_error("GLFW Window not created");
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(_window);

	Renderer renderer;
	while (!glfwWindowShouldClose(_window)) {
		glfwPollEvents();
		glfwSwapBuffers(_window);
		renderer.render();
		// TODO
	}
	glfwDestroyWindow(_window);
	glfwTerminate();
	return 0;
}