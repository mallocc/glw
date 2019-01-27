#include "CameraSequencer.h"

using gfx::lerp::CameraSequencer;

void CameraSequencer::addLerper(Lerper position, Lerper direction, Lerper up, Lerper other)
{
	m_sequence.push_back(position);
	m_directions.push_back(direction);
	m_ups.push_back(up);
	m_others.push_back(other);
}

void CameraSequencer::addLerper(glm::vec3 nextPos, glm::vec3 nextDir, glm::vec3 nextUp, float step, float wait)
{
	addLerper(Lerper(m_sequence.back().getEnd(), nextPos, step, wait), Lerper(m_directions.back().getEnd(), nextDir, step, wait), Lerper(m_ups.back().getEnd(), nextUp, step, wait), Lerper(m_others.back().getEnd(), m_ups.back().getEnd(), step, wait));
}

void CameraSequencer::addLerper(glm::vec3 nextPos, glm::vec3 nextDir, glm::vec3 nextUp, glm::vec3 nextOther, float step, float wait)
{
	addLerper(Lerper(m_sequence.back().getEnd(), nextPos, step, wait), Lerper(m_directions.back().getEnd(), nextDir, step, wait), Lerper(m_ups.back().getEnd(), nextUp, step, wait), Lerper(m_others.back().getEnd(), nextOther, step, wait));
}

void CameraSequencer::reset()
{
	m_currentLerper = 0;
	for (int i = 0; i < m_sequence.size(); ++i)
		m_sequence[i].reset();
	for (int i = 0; i < m_directions.size(); ++i)
		m_directions[i].reset();
	for (int i = 0; i < m_ups.size(); ++i)
		m_ups[i].reset();
	for (int i = 0; i < m_others.size(); ++i)
		m_others[i].reset();
	if (m_sequence.size() > 0)
	{
		m_currentPosition = m_sequence[m_currentLerper].getStart();
		m_currentDirection = m_directions[m_currentLerper].getStart();
		m_currentUp = m_ups[m_currentLerper].getStart();
		m_currentOther = m_others[m_currentLerper].getStart();
	}
}

bool CameraSequencer::lerpStep(float step)
{
	bool finished = false;
	if (m_sequence.size() > 0)
	{
		if (m_sequence[m_currentLerper].isFinished())
		{
			if (m_currentLerper == m_sequence.size() - 1)
			{
				m_currentPosition = m_sequence[m_currentLerper].getEnd();
				m_currentDirection = m_directions[m_currentLerper].getEnd();
				m_currentUp = m_ups[m_currentLerper].getEnd();
				m_currentOther = m_others[m_currentLerper].getEnd();
			}
			else
				m_currentLerper++;
			finished = true;
		}
		else {
			m_currentPosition = m_sequence[m_currentLerper].lerpStep(step);
			m_currentDirection = m_directions[m_currentLerper].lerpStep(step);
			m_currentUp = m_ups[m_currentLerper].lerpStep(step);
			m_currentOther = m_others[m_currentLerper].lerpStep(step);
		}
	}
	else
		m_currentPosition = m_currentDirection = m_currentUp = m_currentOther = glm::vec3();
	return finished;
}

bool CameraSequencer::lerpStepSmooth(float step)
{
	bool finished = false;
	if (m_sequence.size() > 0)
	{
		if (m_sequence[m_currentLerper].isFinished())
		{
			if (m_currentLerper == m_sequence.size() - 1)
			{
				m_currentPosition = m_sequence[m_currentLerper].getEnd();
				m_currentDirection = m_directions[m_currentLerper].getEnd();
				m_currentUp = m_ups[m_currentLerper].getEnd();
				m_currentOther = m_others[m_currentLerper].getEnd();
			}
			else
				m_currentLerper++;
			finished = true;
		}
		else {
			m_currentPosition = m_sequence[m_currentLerper].lerpStepSmooth(step);
			m_currentDirection = m_directions[m_currentLerper].lerpStepSmooth(step);
			m_currentUp = m_ups[m_currentLerper].lerpStepSmooth(step);
			m_currentOther = m_others[m_currentLerper].lerpStepSmooth(step);
		}
	}
	else
		m_currentPosition = m_currentDirection = m_currentUp = m_currentOther = glm::vec3();
	return finished;
}

bool CameraSequencer::lerpStep()
{
	bool finished = false;
	if (m_sequence.size() > 0)
	{
		if (m_sequence[m_currentLerper].isFinished())
		{
			if (m_currentLerper == m_sequence.size() - 1)
			{
				m_currentPosition = m_sequence[m_currentLerper].getEnd();
				m_currentDirection = m_directions[m_currentLerper].getEnd();
				m_currentUp = m_ups[m_currentLerper].getEnd();
				m_currentOther = m_others[m_currentLerper].getEnd();
			}
			else
				m_currentLerper++;
			finished = true;
		}
		else {
			m_currentPosition = m_sequence[m_currentLerper].lerpStep();
			m_currentDirection = m_directions[m_currentLerper].lerpStep();
			m_currentUp = m_ups[m_currentLerper].lerpStep();
			m_currentOther = m_others[m_currentLerper].lerpStep();
		}
	}
	else
		m_currentPosition = m_currentDirection = m_currentUp = m_currentOther = glm::vec3();
	return finished;
}

bool CameraSequencer::lerpStepSmooth()
{
	bool finished = false;
	if (m_sequence.size() > 0)
	{
		if (m_sequence[m_currentLerper].isFinished())
		{
			if (m_currentLerper == m_sequence.size() - 1)
			{
				m_currentPosition = m_sequence[m_currentLerper].getEnd();
				m_currentDirection = m_directions[m_currentLerper].getEnd();
				m_currentUp = m_ups[m_currentLerper].getEnd();
				m_currentOther = m_others[m_currentLerper].getEnd();
			}
			else
				m_currentLerper++;
			finished = true;
		}
		else {
			m_currentPosition = m_sequence[m_currentLerper].lerpStepSmooth();
			m_currentDirection = m_directions[m_currentLerper].lerpStepSmooth();
			m_currentUp = m_ups[m_currentLerper].lerpStepSmooth();
			m_currentOther = m_others[m_currentLerper].lerpStepSmooth();
		}
	}
	else
		m_currentPosition = m_currentDirection = m_currentUp = m_currentOther = glm::vec3();
	return finished;
}

CameraSequencer::CameraSequencer() : LerperSequencer() {}

CameraSequencer::CameraSequencer(
	glm::vec3 startPos, glm::vec3 endPos, glm::vec3 startDir, glm::vec3 endDir, glm::vec3 startUp, glm::vec3 endUp, glm::vec3 startOther, glm::vec3 endOther, float step, float wait)
{
	addLerper(Lerper(startPos, endPos, step, wait), Lerper(startDir, endDir, step, wait), Lerper(startUp, endUp, step, wait), Lerper(startOther, endOther, step, wait));
}