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
	/*
		GLint success = GL_FALSE;
		GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag_shader, 1, &shaders::frag, NULL);
		glCompileShader(frag_shader);

		glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
		CHECK_SHADER_SUCCESS(success, frag_shader, "Shader Compilation");
		GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vert_shader, 1, &shaders::vert, NULL);
		glCompileShader(vert_shader);
		m_shader_program_id = glCreateProgram();
		glAttachShader(m_shader_program_id, frag_shader);
		glLinkProgram(m_shader_program_id);
		glGetProgramiv(m_shader_program_id, GL_LINK_STATUS, &success);
		CHECK_SHADER_SUCCESS(success, frag_shader, "Linking");
		glDeleteShader(frag_shader);
	*/
	}
}

void Renderer::render() {
	glClear(GL_COLOR_BUFFER_BIT);
}