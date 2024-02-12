#pragma once
#include <glm.hpp>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;
using namespace glm;

namespace shaders {

static const char* vert = R"(
#version 330 core
layout (location = 0) in vec3 pos;

void main() {
	gl_Position = vec4(0.5 * pos.xy, pos.z, 1.0);
}
)";

static const char* frag = R"(
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
	void render();
	void import_obj(const fs::path& obj_fp);
private:
	vec3 m_clear_color{1.0, 0.0, 0.0};
	GLuint m_shader_program_id;
	unsigned int m_vao;
	unsigned int m_vbo;
	std::vector<vec3> m_cpu_vertices;
};
