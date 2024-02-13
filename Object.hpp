#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <filesystem>
#include <fstream>
#include <format>
#include <vector>
#include <tiny_obj_loader.h>

using namespace glm;
namespace fs = std::filesystem;

struct Object {
	Object(const fs::path& obj_fp);
	Object(std::vector<vec3>& verts, std::vector<GLchar>& v_indices);
	
	std::vector<vec3> vertices;
	std::vector<GLchar> v_indices;
	std::vector<vec3> normals;
	std::vector<GLchar> n_indices;
	mat4 world_mat{1.0};

	unsigned int m_vao;
	unsigned int m_vbo_pos;
	unsigned int m_vbo_norm;
	unsigned int m_ibo;

	void gl_bind_and_draw();

	~Object();

private:
	void gl_instantiate();
};
