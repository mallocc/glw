#pragma once

#include "glm.h"
#include <minmax.h>
#include <vector>
#include "LerperSequencer.h"

namespace gfx
{
	namespace lerp
	{

		struct CameraSequencer : LerperSequencer
		{
		public:
			void addLerper(Lerper position, Lerper direction, Lerper up, Lerper other);

			void addLerper(glm::vec3 nextPos, glm::vec3 nextDir, glm::vec3 nextUp, float step, float wait);

			void addLerper(glm::vec3 nextPos, glm::vec3 nextDir, glm::vec3 nextUp, glm::vec3 nextOther, float step, float wait);

			void reset();

			bool lerpStep(float step);

			bool lerpStepSmooth(float step);

			bool lerpStep();

			bool lerpStepSmooth();
			

			// constructors

			CameraSequencer();
			CameraSequencer(
				glm::vec3 startPos, glm::vec3 endPos, glm::vec3 startDir, glm::vec3 endDir, glm::vec3 startUp, glm::vec3 endUp, glm::vec3 startOther, glm::vec3 endOther, float step, float wait);


			// member variables

			std::vector<Lerper> m_directions;
			std::vector<Lerper> m_ups;
			std::vector<Lerper> m_others;
			glm::vec3 m_currentPosition, m_currentDirection, m_currentUp, m_currentOther;

		};
	}
}