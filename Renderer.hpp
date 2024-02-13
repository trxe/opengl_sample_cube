#pragma once
#include <glm.hpp>
#include <filesystem>
#include <vector>
#include "Object.hpp"
#include "Camera.hpp"
namespace fs = std::filesystem;
using namespace glm;

namespace shaders {

static const GLchar* vert = R"(
#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 clip_mat;
uniform float time;

out VS_OUT {
    vec3 world_pos;
} vs_out;

void main() {
	vec4 world_pos_4 = world_mat * vec4(pos, 1.0);
	vs_out.world_pos = world_pos_4.xyz;
	gl_Position = clip_mat * view_mat * world_pos_4;
}
)";

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

static const GLchar* frag = R"(
#version 330 core
uniform vec3 cam_pos;
uniform vec3 light_pos;
uniform vec3 light_col;
uniform float light_intensity;
in vec3 albedo;
in vec3 normal;
in vec3 frag_world_pos;

out vec4 fragColor;

void main() {
	vec3 l = normalize(light_pos - frag_world_pos);
	vec3 primary = light_col * albedo * max(0.1, dot(normal, l)) * light_intensity;
	vec3 r = reflect(l, normal);
	vec3 v = normalize(cam_pos - frag_world_pos);
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

	void set_window_size(int w, int h) {
		camera.set_resolution(w, h);
	}
	void input_scroll(double xoffset, double yoffset) {
		camera.zoom(yoffset);
	}

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

	// Light
	vec3 light_pos{ 2.0, 4.0, 3.0 };
	vec3 light_col{ 1.0 };
	float light_intensity{ 1.0 };
};
