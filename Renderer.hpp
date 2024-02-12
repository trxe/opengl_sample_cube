#pragma once
#include <glm.hpp>
#include <filesystem>
namespace fs = std::filesystem;
using namespace glm;

namespace shaders {

static const char* frag = R"(
#version 300 core
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
	// GLuint m_shader_program_id;
};
