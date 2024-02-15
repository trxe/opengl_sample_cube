#pragma once
#include <glm.hpp>
#include <filesystem>
#include <vector>
#include "Object.hpp"
#include "Camera.hpp"
#include "Material.hpp"
namespace fs = std::filesystem;
using namespace glm;

namespace shaders {

static const GLchar* vert = R"(
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 texcoord;

uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 clip_mat;
uniform float time;

out vec3 world_pos;
out vec3 world_norm;
out vec2 uv;

void main() {
	vec4 world_pos_4 = world_mat * vec4(pos, 1.0);
	world_pos = world_pos_4.xyz;
	world_norm = (world_mat * vec4(norm, 1.0)).xyz;
	uv = texcoord;
	gl_Position = clip_mat * view_mat * world_pos_4;
}
)";

/*
static const GLchar* geom = R"(
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
	vec3 world_pos;
} gs_in[];

out vec3 albedo;
out vec3 normal;
out vec3 frag_world_pos;

vec3 get_normal() {
	vec3 a = gs_in[2].world_pos - gs_in[1].world_pos;
	vec3 b = gs_in[0].world_pos - gs_in[1].world_pos;
	return normalize(cross(a, b));
}

void main() {    
	vec3 face_normal = get_normal();

	gl_Position = gl_in[0].gl_Position; 
	frag_world_pos = gs_in[0].world_pos;
	normal = face_normal;
	albedo = face_normal * 0.5f + 0.5f;
    EmitVertex();

	gl_Position = gl_in[1].gl_Position; 
	frag_world_pos = gs_in[1].world_pos;
	normal = face_normal;
	albedo = face_normal * 0.5f + 0.5f;
    EmitVertex();

	gl_Position = gl_in[2].gl_Position; 
	frag_world_pos = gs_in[2].world_pos;
	normal = face_normal;
	albedo = face_normal * 0.5f + 0.5f;
    EmitVertex();

	EndPrimitive();
} 
)";
*/

static const GLchar* frag = R"(
#version 330 core

uniform vec3 cam_pos;
uniform vec3 light_pos;
uniform vec3 light_col;
uniform float light_intensity;
uniform sampler2D albedo_map;
uniform sampler2D normal_map;
uniform sampler2D displacement_map;
uniform sampler2D occlusion_map;

in vec3 world_pos;
in vec3 world_norm;
in vec2 uv;

out vec4 fragColor;

void main() {
	vec3 albedo = texture(displacem, uv).rgb;
	vec3 l = normalize(light_pos - world_pos);
	vec3 primary = light_col * albedo * max(0.1, dot(world_norm, l)) * light_intensity;
	vec3 r = reflect(l, world_norm);
	vec3 v = normalize(cam_pos - world_pos);
	vec3 secondary = light_col * pow(max(0.0, dot(r, v)), 256.0) * light_intensity;
	fragColor = vec4(primary + secondary, 1.0);
}
)";

}

class Renderer {
public:
	Renderer();
	~Renderer();
	void render();

	void toggle_pause() {
		if (rot_speed < 0.0001f) rot_speed = 1.0f;
		else rot_speed = 0.0f;
	}

	void add_object_from_fp(const fs::path& fp) {
		m_objects.emplace_back(fp);
	}
	void set_window_size(int w, int h) {
		camera.set_resolution(w, h);
	}
	void input_scroll(double xoffset, double yoffset) {
		camera.zoom(yoffset);
	}

	void set_map_dir(const fs::path& dir, const char* prefix);

private:
	Camera camera{};
	vec3 m_clear_color{0.0, 0.0, 0.0};
	std::vector<vec3> m_cpu_vertices{};
	std::vector<GLchar> m_cpu_indices{};

	GLuint m_shader_program_id;
	std::vector<Object> m_objects;
	unsigned int m_clip_uni;
	unsigned int m_view_uni;
	unsigned int m_world_uni;
	unsigned int m_time_uni;
	unsigned int m_cam_pos_uni;
	unsigned int m_light_pos_uni;
	unsigned int m_light_col_uni;
	unsigned int m_light_intensity_uni;

	unsigned int m_albedo;
	unsigned int m_normal;
	Material m_material;

	// Light
	vec3 light_pos{ 2.0, 4.0, 3.0 };
	vec3 light_col{ 1.0 };
	float light_intensity{ 1.0f };
	float rot_speed{ 1.0f };
};
