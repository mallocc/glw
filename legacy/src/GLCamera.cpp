#include "GLCamera.h"

using gfx::engine::GLCamera;

void GLCamera::update(float dt, float friction)
{
	m_vel *= friction;
	m_pos += m_vel * dt;
	m_dir_vel *= friction;
	m_dir += m_dir_vel;
	m_up_vel *= friction;
	m_up += m_up_vel;
}

void GLCamera::apply_force(glm::vec3 force)
{
	m_vel += force;
}
void GLCamera::apply_force_foward()
{
	m_vel += m_dir;
}
void GLCamera::apply_force_foward(float mag)
{
	m_vel += m_dir * mag;
}
void GLCamera::apply_force_backward()
{
	m_vel -= m_dir;
}
void GLCamera::apply_force_right()
{
	m_vel += glm::cross(m_dir, m_up);
}
void GLCamera::apply_force_left()
{
	m_vel += -glm::cross(m_dir, m_up);
}

void GLCamera::yaw_left(float amount)
{
	m_dir = glm::quat(glm::vec3(0, 1, 0) * glm::radians(amount)) * m_dir;
}
void GLCamera::yaw_right(float amount)
{
	m_dir = glm::quat(glm::vec3(0, -1, 0) * glm::radians(amount)) * m_dir;
}
void GLCamera::pitch_up(float amount)
{
	glm::vec3 right = glm::cross(m_dir, m_up);
	m_up = glm::quat(right * glm::radians(amount)) * m_up;
	m_dir = glm::quat(right * glm::radians(amount)) * m_dir;
}
void GLCamera::pitch_down(float amount)
{
	glm::vec3 right = -glm::cross(m_dir, m_up);
	m_up = glm::quat(right * glm::radians(amount)) * m_up;
	m_dir = glm::quat(right * glm::radians(amount)) * m_dir;
}


void GLCamera::brake(float friction)
{
	m_vel *= friction;
}

glm::vec3 GLCamera::get_position()
{
	return m_pos;
}

glm::vec3 GLCamera::get_look_position()
{
	return m_pos + m_dir;
}

glm::vec3 GLCamera::get_up()
{
	return m_up;
}


// constructors

GLCamera::GLCamera() {}

GLCamera::GLCamera(glm::vec3 pos, glm::vec3 vel, glm::vec3 dir, glm::vec3 up)
{
	this->m_pos = pos;
	this->m_vel = vel;
	this->m_dir = glm::normalize(dir);
	this->m_up = up;
}