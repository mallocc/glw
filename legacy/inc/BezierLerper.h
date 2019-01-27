#pragma once

#include "glm.h"
#include <minmax.h>
#include <vector>
#include "Lerper.h"

namespace gfx
{
	namespace lerp
	{
		class BezierLerper : gfx::lerp::Lerper
		{
		public:
			glm::vec3 lerpStepSmooth(float step);
			glm::vec3 lerpStep(float step);
			glm::vec3 lerpStepSmooth();
			glm::vec3 lerpStep();


			// constructors

			BezierLerper();
			BezierLerper(glm::vec3 start, glm::vec3 end, glm::vec3 m_c1, glm::vec3 m_c2, float step);
			BezierLerper(glm::vec3 start, glm::vec3 end, glm::vec3 m_c1, glm::vec3 m_c2, float step, float wait);


			// member variables

			glm::vec3 m_c1, m_c2;

		};
	}
}