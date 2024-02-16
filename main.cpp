#include <iostream>
#include <string>

#define TINYOBJLOADER_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.hpp"
#include "Utils.hpp"

static GLFWwindow* _window = nullptr;
static void glfw_callback_error(int error, const char* msg) {
	log_error(msg);
}
static void glfw_callback_window_size(GLFWwindow* window, int width, int height) {
	Renderer* renderer = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(_window));
	renderer->set_window_size(width, height);
	glfwSwapBuffers(_window);
	renderer->render();
}
static void glfw_callback_scroll(GLFWwindow* window, double xoffset, double yoffset) {
	Renderer* renderer = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(_window));
	renderer->input_scroll(xoffset, yoffset);
}
static void glfw_callback_key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Renderer* renderer = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(_window));
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		renderer->toggle_pause();
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
	_window = glfwCreateWindow(DEFAULT_RES_W, DEFAULT_RES_H, "Rotating Cube", NULL, NULL);
	if (!_window) {
		log_error("GLFW Window not created");
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(_window);

	if (glewInit() != GLEW_OK) {
		const GLubyte* err = glewGetErrorString(glewInit());
		log_error(err);
		glfwTerminate();
		exit(1);
	}

	try {
		Renderer renderer;
		glfwSetWindowUserPointer(_window, reinterpret_cast<void*>(&renderer));
		glfwSetWindowSizeCallback(_window, glfw_callback_window_size);
		glfwSetScrollCallback(_window, glfw_callback_scroll);
		glfwSetKeyCallback(_window, glfw_callback_key);
		renderer.add_object_from_fp(fs::current_path() / "data" / "box.obj");
		renderer.set_map_dir(fs::current_path() / "data" / "textures" / "Encrusted_Gems_002_SD" , "Encrusted_Gems_002");
		while (!glfwWindowShouldClose(_window)) {
			glfwPollEvents();
			renderer.render();
			glfwSwapBuffers(_window);
		}
	} catch (const std::exception& e) {
		log_error(e.what());
	}

	glfwDestroyWindow(_window);
	glfwTerminate();
	return 0;
}