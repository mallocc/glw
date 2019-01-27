#include "GCamera.h"


using glw::engine::GCamera;


namespace
{
  const char * TRG = "GCAM";
  const char * __CLASSNAME__ = "GCamera";
}


GCamera::GCamera()
{
}

GCamera::GCamera(glm::vec3 pos, glm::vec3 vel, glm::vec3 dir, glm::vec3 up)
    :
    m_pos(pos),
    m_vel(vel),
    m_dir(dir),
    m_up(up)
{
}

GCamera::~GCamera()
{
}

void GCamera::update(float dt, float friction)
{
  m_vel *= friction;
	m_pos += m_vel * dt;
	m_dirVel *= friction;
	m_dir += m_dirVel;
	m_upVel *= friction;
	m_up += m_upVel;
}

void GCamera::applyForce(glm::vec3 force)
{
	m_vel += force;
}

void GCamera::applyForceForward(float mag)
{
	m_vel += m_dir * mag;
}

void GCamera::applyForceBackward(float mag)
{
	m_vel -= m_dir * mag;
}

void GCamera::applyForceRight(float mag)
{
	m_vel += glm::cross(m_dir, m_up) * mag;
}

void GCamera::applyForceLeft(float mag)
{
	m_vel -= glm::cross(m_dir, m_up) * mag;
}

void GCamera::applyForceUp(float mag)
{
  m_vel += m_up * mag;
}

void GCamera::applyForceDown(float mag)
{
  m_vel -= m_up * mag;
}

void GCamera::yawLeft(float deg)
{
	m_dir = glm::quat(glm::vec3(0, 1, 0) * glm::radians(deg)) * m_dir;
}

void GCamera::yawRight(float deg)
{
	m_dir = glm::quat(glm::vec3(0, -1, 0) * glm::radians(deg)) * m_dir;
}

void GCamera::pitchUp(float deg)
{
	glm::vec3 right = glm::cross(m_dir, m_up);
	//m_up = glm::quat(right * glm::radians(deg)) * m_up;
	m_dir = glm::quat(right * glm::radians(deg)) * m_dir;
}

void GCamera::pitchDown(float deg)
{
	glm::vec3 right = -glm::cross(m_dir, m_up);
	//m_up = glm::quat(right * glm::radians(deg)) * m_up;
	m_dir = glm::quat(right * glm::radians(deg)) * m_dir;
}

void GCamera::brake(float friction)
{
	m_vel *= friction;
}

glm::vec3 GCamera::getPosition()
{
  return m_pos;
}

glm::vec3 GCamera::getLookPosition()
{
  return m_pos + m_dir;
}

glm::vec3 GCamera::getUp()
{
  return m_up;
}
