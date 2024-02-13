#pragma once
#include <glm.hpp>
#include <filesystem>
#include <vector>
#include "Object.hpp"
#include "Camera.hpp"
namespace fs = std::filesystem;
using namespace glm;

namespace shaders {

static const GLchar* vert = R"(
#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 clip_mat;
uniform float time;

void main() {
	gl_Position = clip_mat * view_mat * world_mat * vec4(pos, 1.0);
}
)";

static const GLchar* frag = R"(
#version 330 core
out vec4 fragColor;

void main() {
	fragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
)";

}

class Renderer {
public:
	Renderer();
	~Renderer();
	void render();
	void set_window_size(int w, int h) {
		camera.set_resolution(w, h);
	}

	// world translate/rotate/zoom callbacks
	void input_scroll(double xoffset, double yoffset) {
		camera.zoom(yoffset);
	}

	// view translate/rotate/zoom callbacks

private:
	Camera camera{};
	vec3 m_clear_color{0.0, 0.0, 0.0};
	std::vector<vec3> m_cpu_vertices{};
	std::vector<GLchar> m_cpu_indices{};

	GLuint m_shader_program_id;
	std::vector<Object> m_objects;
	unsigned int m_vao;
	unsigned int m_vbo; // vertex buffer object
	unsigned int m_ibo; // index buffer object
	unsigned int m_clip_uni;
	unsigned int m_view_uni;
	unsigned int m_world_uni;
	unsigned int m_time_uni;
};
