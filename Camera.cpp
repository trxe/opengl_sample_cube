#include "Camera.hpp"
#include <gtc/matrix_transform.hpp> 

Camera::Camera() {
	m_view_matrix = glm::lookAt(eye, at, up);
	m_clip_matrix = glm::perspective(fovy, aspect_ratio(), near, far);
}

Camera::Camera(const vec3& eye, const vec3& at, const vec3& up, float fovy, const ivec2& resolution) :
	eye(eye), at(at), up(up), fovy(fovy), resolution(resolution) {
	m_view_matrix = glm::lookAt(eye, at, up);
	m_clip_matrix = glm::perspective(fovy, aspect_ratio(), near, far);
}

void Camera::set_resolution(int w, int h) {
	resolution = { w, h };
	m_clip_matrix = glm::perspective(fovy, aspect_ratio(), near, far);
}

void Camera::zoom(float amount, float scale) {
	auto dir = (at - eye) * amount * scale;
	eye += dir;
	m_view_matrix = glm::lookAt(eye, at, up);
}