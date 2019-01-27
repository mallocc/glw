#include "BezierLerper.h"

using gfx::lerp::BezierLerper;

glm::vec3 BezierLerper::lerpStepSmooth(float step)
{
	if (m_t < 1)
	{
		m_t += step;
		float t1 = smoothstep(0, 1, m_t);
		float ti = 1 - t1;
		float t2 = t1*t1;
		float t3 = t2 * t1;
		return pow(ti, 3) * m_start + 3 * pow(ti, 2)*m_t*m_c1 + 3 * (ti)*t2*m_c2 + t3*m_end;
	}
	stepWait(step);
	return m_end;
}

glm::vec3 BezierLerper::lerpStep(float step)
{
	if (m_t < 1)
	{
		m_t += step;
		float t1 = m_t;
		float ti = 1 - t1;
		float t2 = t1*t1;
		float t3 = t2 * t1;
		return pow(ti, 3) * m_start + 3 * pow(ti, 2)*m_t*m_c1 + 3 * (ti)*t2*m_c2 + t3*m_end;
	}
	stepWait(step);
	return m_end;
}


glm::vec3 BezierLerper::lerpStepSmooth()
{
	return lerpStepSmooth(m_step);
}

glm::vec3 BezierLerper::lerpStep()
{
	return lerpStep(m_step);
}

BezierLerper::BezierLerper() {}

BezierLerper::BezierLerper(glm::vec3 start, glm::vec3 end, glm::vec3 m_c1, glm::vec3 m_c2, float step)
{
	this->m_start = start;
	this->m_c1 = m_c1;
	this->m_c2 = m_c2;
	this->m_end = end;
	this->m_step = step;
}

BezierLerper::BezierLerper(glm::vec3 start, glm::vec3 end, glm::vec3 m_c1, glm::vec3 m_c2, float step, float wait)
{
	this->m_start = start;
	this->m_c1 = m_c1;
	this->m_c2 = m_c2;
	this->m_end = end;
	this->m_step = step;
	this->m_wait = wait;
}