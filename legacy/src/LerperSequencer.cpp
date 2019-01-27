#include "LerperSequencer.h"

using gfx::lerp::LerperSequencer;

void LerperSequencer::addLerper(Lerper lerper)
{
	m_sequence.push_back(lerper);
}
void LerperSequencer::addLerper(glm::vec3 next, float step, float wait)
{
	m_sequence.push_back(Lerper(m_sequence.back().getEnd(), next, step, wait));
}
void LerperSequencer::addLerper(glm::vec3 next)
{
	m_sequence.push_back(Lerper(m_sequence.back().getEnd(), next, m_sequence.front().getStep(), m_sequence.front().getWait()));
}
void LerperSequencer::addLerper(glm::vec3 next, float step)
{
	m_sequence.push_back(Lerper(m_sequence.back().getEnd(), next, step, m_sequence.front().getWait()));
}
void LerperSequencer::addLerper(float wait, glm::vec3 next)
{
	m_sequence.push_back(Lerper(m_sequence.back().getEnd(), next, m_sequence.front().getStep(), wait));
}

void LerperSequencer::reset()
{
	m_currentLerper = 0;
	for (Lerper l : m_sequence)
		l.reset();
}

bool LerperSequencer::isFinished()
{
	return m_sequence.size() > 0 && m_sequence[m_sequence.size() - 1].isFinished();
}

glm::vec3 LerperSequencer::lerpStep(float step)
{
	if (m_sequence.size() > 0)
	{
		if (m_sequence[m_currentLerper].isFinished())
		{
			if (m_currentLerper == m_sequence.size() - 1)
				return m_sequence[m_currentLerper].getEnd();
			else
				m_currentLerper++;
		}
		return m_sequence[m_currentLerper].lerpStep(step);
	}
	return glm::vec3();
}

glm::vec3 LerperSequencer::lerpStepSmooth(float step)
{
	if (m_sequence.size() > 0)
	{
		if (m_sequence[m_currentLerper].isFinished())
		{
			if (m_currentLerper == m_sequence.size() - 1)
				return m_sequence[m_currentLerper].getEnd();
			else
				m_currentLerper++;
		}
		return m_sequence[m_currentLerper].lerpStepSmooth(step);
	}
	return glm::vec3();
}

glm::vec3 LerperSequencer::lerpStepSmooth()
{
	if (m_sequence.size() > 0)
	{
		if (m_sequence[m_currentLerper].isFinished())
		{
			if (m_currentLerper == m_sequence.size() - 1)
				return m_sequence[m_currentLerper].getEnd();
			else
				m_currentLerper++;
		}
		return m_sequence[m_currentLerper].lerpStepSmooth();
	}
	return glm::vec3();
}

glm::vec3 LerperSequencer::lerpStep()
{
	if (m_sequence.size() > 0)
	{
		if (m_sequence[m_currentLerper].isFinished())
		{
			if (m_currentLerper == m_sequence.size() - 1)
				return m_sequence[m_currentLerper].getEnd();
			else
				m_currentLerper++;
		}
		return m_sequence[m_currentLerper].lerpStep();
	}
	return glm::vec3();
}

void LerperSequencer::next()
{
	if (m_sequence.size() > 0)
		if (m_currentLerper < m_sequence.size() - 1)
			m_currentLerper++;
}

LerperSequencer::LerperSequencer() {}

LerperSequencer::LerperSequencer(glm::vec3 start, glm::vec3 first, float step, float wait)
{
	addLerper(Lerper(start, first, step, wait));
}

LerperSequencer::LerperSequencer(std::vector<gfx::lerp::Lerper> sequence)
{
	this->m_sequence = sequence;
}