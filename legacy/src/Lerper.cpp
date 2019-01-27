#include "Lerper.h"

using gfx::lerp::Lerper;

void Lerper::reset()
{
	m_finished = 0;
	m_t = 0;
}

bool Lerper::isFinished()
{
	return m_finished;
}

glm::vec3 Lerper::lerpStepSmooth(float step)
{
	if (m_t < 1)
	{
		m_t += step;
		return m_start + (m_end - m_start) * smoothstep(0, 1, m_t);
	}
	stepWait(step);
	return m_end;
}

glm::vec3 Lerper::lerpStep(float step)
{
	if (m_t < 1)
	{
		m_t += step;
		return m_start + (m_end - m_start) * m_t;
	}
	stepWait(step);
	return m_end;
}

glm::vec3 Lerper::lerpStepSmooth()
{
	return lerpStepSmooth(m_step);
}

glm::vec3 Lerper::lerpStep()
{
	return lerpStep(m_step);
}

glm::vec3 Lerper::getStart()
{
	return m_start;
}

glm::vec3 Lerper::getEnd()
{
	return m_end;
}

float Lerper::getStep()
{
	return m_step;
}

float Lerper::getWait()
{
	return m_wait;
}

float Lerper::clamp(float x, float lowerlimit, float upperlimit) {
	if (x < lowerlimit)
		x = lowerlimit;
	if (x > upperlimit)
		x = upperlimit;
	return x;
}
float Lerper::smoothstep(float edge0, float edge1, float x) {
	// Scale, bias and saturate x to 0..1 range
	x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	// Evaluate polynomial
	return x * x * (3 - 2 * x);
}

void Lerper::stepWait(float step)
{
	if (m_t < 1 + m_wait)
		m_t += step;
	else
	{
		m_t = 1 + m_wait;
		m_finished = 1;
	}
}


Lerper::Lerper() {}

Lerper::Lerper(glm::vec3 start, glm::vec3 end, float step)
{
	this->m_start = start;
	this->m_end = end;
	this->m_step = step;
}
Lerper::Lerper(glm::vec3 start, glm::vec3 end, float step, float wait)
{
	this->m_start = start;
	this->m_end = end;
	this->m_step = step;
	this->m_wait = wait;
}