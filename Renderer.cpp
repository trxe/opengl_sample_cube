#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include "Renderer.hpp"
#include "Utils.hpp"

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
		if (!success) {
			char log[255];
			glGetShaderInfoLog(frag_shader, 255, NULL, log);
			log_error(log);
		}

		GLint geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom_shader, 1, &shaders::geom, NULL);
		glCompileShader(geom_shader);
		glGetShaderiv(geom_shader, GL_COMPILE_STATUS, &success);
		glGetUniformiv(geom_shader, m_clip_uni, &success);
		if (!success) {
			char log[255];
			glGetShaderInfoLog(geom_shader, 255, NULL, log);
			log_error(log);
		}

		GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vert_shader, 1, &shaders::vert, NULL);
		glCompileShader(vert_shader);
		glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
		glGetUniformiv(vert_shader, m_clip_uni, &success);
		if (!success) {
			char log[255];
			glGetShaderInfoLog(vert_shader, 255, NULL, log);
			log_error(log);
		}

		m_shader_program_id = glCreateProgram();
		glAttachShader(m_shader_program_id, vert_shader);
		glAttachShader(m_shader_program_id, geom_shader);
		glAttachShader(m_shader_program_id, frag_shader);
		glLinkProgram(m_shader_program_id);
		glGetProgramiv(m_shader_program_id, GL_LINK_STATUS, &success);
		if (!success) {
			char log[255];
			glGetProgramInfoLog(m_shader_program_id, 255, NULL, log);
			log_error(log);
		}

		m_clip_uni = glGetUniformLocation(m_shader_program_id, "clip_mat");
		m_view_uni = glGetUniformLocation(m_shader_program_id, "view_mat");
		m_world_uni = glGetUniformLocation(m_shader_program_id, "world_mat");
		m_time_uni = glGetUniformLocation(m_shader_program_id, "time"); 
		m_cam_pos_uni = glGetUniformLocation(m_shader_program_id, "cam_pos"); 
		m_light_pos_uni = glGetUniformLocation(m_shader_program_id, "light_pos"); 
		m_light_col_uni = glGetUniformLocation(m_shader_program_id, "light_col"); 
		m_light_intensity_uni = glGetUniformLocation(m_shader_program_id, "light_intensity"); 

		glDeleteShader(frag_shader);
		glDeleteShader(geom_shader);
		glDeleteShader(vert_shader);
	}

	glEnable(GL_DEPTH_TEST);
	m_objects.emplace_back(fs::current_path() / "data" / "monkey.obj");

}

Renderer::~Renderer() {
	glDeleteProgram(m_shader_program_id);
}

void Renderer::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_shader_program_id);
	float time_ns = static_cast<float>(glfwGetTime());
	glUniform1f(m_time_uni, time_ns);
	glUniformMatrix4fv(m_clip_uni, 1, GL_FALSE, &camera.clip_matrix()[0][0]);
	glUniformMatrix4fv(m_view_uni, 1, GL_FALSE, &camera.view_matrix()[0][0]);
	glUniform3fv(m_cam_pos_uni, 1, &camera.cam_pos()[0]);
	glUniform3fv(m_light_pos_uni, 1, &light_pos[0]);
	glUniform3fv(m_light_col_uni, 1, &light_col[0]);
	glUniform1fv(m_light_intensity_uni, 1, &light_intensity);
	for (auto& obj : m_objects) {
		obj.world_mat = glm::rotate(obj.world_mat, glm::radians(1.0f / 60.0f * 0.2f), vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(m_world_uni, 1, GL_FALSE, &obj.world_mat[0][0]);
		obj.gl_bind_and_draw();
	}
}
