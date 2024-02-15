#include "Material.hpp"
#include <stb_image.h>
#include "Utils.hpp"

Material::Material(const fs::path& parent, const fs::path& filename_albedo, const fs::path& filename_normal, const fs::path& filename_displacement, const fs::path& filename_occlusion) {
	init_texture(parent / filename_albedo, albedo, albedo_slot, "albedo_map");
	init_texture(parent / filename_normal, normal, normal_slot, "normal_map");
	init_texture(parent / filename_displacement, displacement, displacement_slot, "displacement_map");
	init_texture(parent / filename_occlusion, occlusion, occlusion_slot, "occlusion_map");
}

void Material::init_texture(const fs::path& fp, unsigned int& id, GLint slot, const char* name) {
	glActiveTexture(slot);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
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
}

void Material::bind_and_use_textures(unsigned int program) {
	glUseProgram(program);

	glActiveTexture(albedo_slot);
	glBindTexture(GL_TEXTURE_2D, albedo);
	glUniform1i(glGetUniformLocation(program, "albedo_map"), 0);

	glActiveTexture(normal_slot);
	glBindTexture(GL_TEXTURE_2D, normal);
	glUniform1i(glGetUniformLocation(program, "normal_map"), 0);

	glActiveTexture(displacement_slot);
	glBindTexture(GL_TEXTURE_2D, displacement);
	glUniform1i(glGetUniformLocation(program, "displacement_map"), 0);

	glActiveTexture(occlusion_slot);
	glBindTexture(GL_TEXTURE_2D, occlusion);
	glUniform1i(glGetUniformLocation(program, "occlusion_map"), 0);

}
