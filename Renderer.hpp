#pragma once
#include <glm.hpp>
#include <filesystem>
#include <vector>
#include "Object.hpp"
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
	float aspect_ratio() {
		return (float)m_window_size.r / (float)m_window_size.g;
	}
	void set_window_size(int w, int h) {
		m_window_size = { w, h };
	}

	// world translate/rotate/zoom callbacks

	// view translate/rotate/zoom callbacks

private:
	ivec2 m_window_size{1920, 1080};
	vec3 m_clear_color{0.0, 0.0, 0.0};
	std::vector<vec3> m_cpu_vertices{};
	std::vector<GLchar> m_cpu_indices{};
	mat4 m_clip_matrix{};
	mat4 m_view_matrix{};

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
