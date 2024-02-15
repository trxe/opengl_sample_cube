#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
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
		glDeleteShader(vert_shader);
	}

	glEnable(GL_DEPTH_TEST);

}

Renderer::~Renderer() {
	glDeleteProgram(m_shader_program_id);
}

void Renderer::add_albedo_map(const fs::path& fp) {
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_albedo);
	glBindTexture(GL_TEXTURE_2D, m_albedo);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); 
	unsigned char* data = stbi_load(fp.string().c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		log_error(std::format("failed to load texture {}", fp.string()));
	}
	stbi_image_free(data);

	glUseProgram(m_shader_program_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_albedo);
	glUniform1i(glGetUniformLocation(m_shader_program_id, "albedo_map"), 0);
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
		obj.world_mat = glm::rotate(obj.world_mat, glm::radians(rot_speed / 60.0f), vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(m_world_uni, 1, GL_FALSE, &obj.world_mat[0][0]);
		obj.gl_bind_and_draw();
	}
}
