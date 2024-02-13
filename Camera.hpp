#pragma once 
#include <glm.hpp>

using namespace glm;

class Camera {
public:
	Camera();
	Camera(const vec3& eye, const vec3& at, const vec3& up, float fovy, const ivec2& resolution);
	const mat4& clip_matrix() { return m_clip_matrix; }
	const mat4& view_matrix() { return m_view_matrix; }
	void set_resolution(int w, int h);
	float aspect_ratio() {
		return (float)resolution.x / (float)resolution.y;
	}
	void zoom(float amount, float scale = 0.1f);

private:
	vec3 eye{ 2.0, 4.0, 6.0 };
	vec3 at{ 0.0, 0.0, 0.0 };
	vec3 up{ 0.0, 1.0, 0.0 };
	ivec2 resolution{1920, 1080};
	float near{ 0.01f };
	float far{ 100.0f };
	float fovy{glm::radians(45.0f)};

	mat4 m_view_matrix{};
	mat4 m_clip_matrix{};

};