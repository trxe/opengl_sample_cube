#pragma once
#include <filesystem>
#include <GL/glew.h>

namespace fs = std::filesystem;

struct Material {
	Material() {}
	Material(const fs::path& parent, const fs::path& filename_albedo, 
		const fs::path& filename_normal, 
		const fs::path& filename_displacement, 
		const fs::path& filename_occlusion);
	void set(const fs::path& parent, const fs::path& filename_albedo, 
		const fs::path& filename_normal, 
		const fs::path& filename_displacement, 
		const fs::path& filename_occlusion);
	void bind_and_use_textures(unsigned int program);

private:
	unsigned int albedo{};
	unsigned int normal{};
	unsigned int displacement{};
	unsigned int occlusion{};

	GLint albedo_slot{GL_TEXTURE0};
	GLint normal_slot{GL_TEXTURE1};
	GLint displacement_slot{GL_TEXTURE2};
	GLint occlusion_slot{GL_TEXTURE3};

	void init_texture(const fs::path& fp, unsigned int& id, GLint slot, const char* name);
};
