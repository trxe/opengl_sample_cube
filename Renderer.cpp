#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.hpp"
#include "Utils.hpp"

#define CHECK_SHADER_SUCCESS(success, id, type) { \
	if (!success) { \
		char log[255]; \
		glGetShaderInfoLog(id, 255, NULL, log); \
		log_error(log); \
	} \
}

Renderer::Renderer() {

	{
		auto& c = m_clear_color;
		glClearColor(c.r, c.g, c.b, 1.0);
	}

	{
		GLint success = GL_FALSE;
		GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag_shader, 1, &shaders::frag, NULL);
		glCompileShader(frag_shader);
		glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
		CHECK_SHADER_SUCCESS(success, frag_shader, "Shader Compilation");

		GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vert_shader, 1, &shaders::vert, NULL);
		glCompileShader(vert_shader);
		glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
		CHECK_SHADER_SUCCESS(success, vert_shader, "Shader Compilation");

		m_shader_program_id = glCreateProgram();
		glAttachShader(m_shader_program_id, frag_shader);
		glLinkProgram(m_shader_program_id);
		glGetProgramiv(m_shader_program_id, GL_LINK_STATUS, &success);
		CHECK_SHADER_SUCCESS(success, frag_shader, "Linking");
		glDeleteShader(frag_shader);
	}

	{
		m_cpu_vertices = { vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0) };
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * m_cpu_vertices.size(), m_cpu_vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void Renderer::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(m_shader_program_id);
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, m_cpu_vertices.size());

}