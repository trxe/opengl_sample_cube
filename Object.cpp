#include "Object.hpp"

Object::~Object() {
	glDeleteBuffers(1, &m_ibo);
	glDeleteBuffers(1, &m_vbo_pos);
	glDeleteVertexArrays(1, &m_vao);
}

void Object::gl_instantiate() {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo_pos);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &m_vbo_norm);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_norm);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &m_vbo_uv);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * uvs.size(), uvs.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Object::gl_bind_and_draw() {
	glBindVertexArray(m_vao);
	// glDrawElements(GL_TRIANGLES, vertices.size() , GL_UNSIGNED_BYTE, NULL);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

Object::Object(std::vector<vec3>&& verts, std::vector<vec3>&& normals, std::vector<vec2>&& uvs) {
	this->vertices = std::move(verts);
	this->normals = std::move(normals);
	this->uvs = std::move(uvs);
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

	auto& in_verts = mesh_attributes.vertices;
	auto& in_norms = mesh_attributes.normals;
	auto& in_uvs = mesh_attributes.texcoords;
	for (auto& shape : mesh_shapes) {
		for (auto& index_set : shape.mesh.indices) {
			auto vidx = index_set.vertex_index * 3;
			auto nidx = index_set.normal_index * 3;
			auto uidx = index_set.texcoord_index * 2;
			this->vertices.emplace_back(in_verts[vidx], in_verts[vidx + 1], in_verts[vidx + 2]);
			this->normals.emplace_back(in_norms[nidx], in_norms[nidx + 1], in_norms[nidx + 2]);
			this->uvs.emplace_back(in_uvs[uidx], in_uvs[uidx + 1]);
		}
	}
	gl_instantiate();
}