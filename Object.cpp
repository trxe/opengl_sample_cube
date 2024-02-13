#include "Object.hpp"

Object::~Object() {
	glDeleteBuffers(1, &m_ibo);
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void Object::gl_instantiate() {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, v_indices.size(), v_indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Object::gl_bind_and_draw() {
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, v_indices.size(), GL_UNSIGNED_BYTE, NULL);
}

Object::Object(std::vector<vec3>& verts, std::vector<GLchar>& v_indices) {
	this->vertices = std::move(verts);
	this->v_indices = std::move(v_indices);
	gl_instantiate();
}

Object::Object(const fs::path& obj_fp) {
	if (!fs::exists(obj_fp) || obj_fp.extension().string() != ".obj") {
		throw std::runtime_error(std::format("Invalid obj file: {}", obj_fp.string()));
	}
	std::ifstream input(obj_fp);
	tinyobj::attrib_t mesh_attributes;
	std::vector<tinyobj::shape_t> mesh_shapes;
	std::vector<tinyobj::material_t> mesh_materials;
	std::string err;
	if (!tinyobj::LoadObj(&mesh_attributes, &mesh_shapes, &mesh_materials, &err, &input)) {
		throw std::runtime_error(std::format("Error loading obj file: {}", obj_fp.string()));
	}
	mesh_shapes.front().mesh.indices.front().vertex_index;
	auto& in_verts = mesh_attributes.vertices;
	for (size_t i = 0; i < in_verts.size(); i += 3) {
		this->vertices.emplace_back(in_verts[i], in_verts[i + 1], in_verts[i + 2]);
	}
	auto& in_norms = mesh_attributes.normals;
	for (size_t i = 0; i < in_norms.size(); i += 3) {
		this->normals.emplace_back(in_norms[i], in_norms[i + 1], in_norms[i + 2]);
	}
	for (auto& shape : mesh_shapes) {
		for (auto& index_set : shape.mesh.indices) {
			this->v_indices.push_back(index_set.vertex_index);
			this->n_indices.push_back(index_set.normal_index);
		}
	}
	gl_instantiate();
}