#pragma once

#include "glm.h"
#include <minmax.h>
#include <vector>
#include "Lerper.h"

using gfx::lerp::Lerper;

namespace gfx
{
	namespace lerp
	{
		struct LerperSequencer
		{
		public:
			void addLerper(Lerper lerper);
			void addLerper(glm::vec3 next, float step, float wait);
			void addLerper(glm::vec3 next);
			void addLerper(glm::vec3 next, float step);
			void addLerper(float wait, glm::vec3 next);

			void reset();

			bool isFinished();

			glm::vec3 lerpStep(float step);

			glm::vec3 lerpStepSmooth(float step);

			glm::vec3 lerpStepSmooth();

			glm::vec3 lerpStep();

			void next();

			LerperSequencer();
			LerperSequencer(glm::vec3 start, glm::vec3 first, float step, float wait);
			LerperSequencer(std::vector<gfx::lerp::Lerper> sequence);

			std::vector<gfx::lerp::Lerper> m_sequence;
			int m_currentLerper = 0;
		};
	}
}