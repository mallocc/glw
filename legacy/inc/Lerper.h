#pragma once

#include "glm.h"
#include <minmax.h>
#include <vector>


namespace gfx
{
	namespace lerp
	{
		class Lerper
		{
		public:
			void reset();

			bool isFinished();

			glm::vec3 lerpStepSmooth(float step);

			glm::vec3 lerpStep(float step);

			glm::vec3 lerpStepSmooth();

			glm::vec3 lerpStep();

			glm::vec3 getStart();

			glm::vec3 getEnd();

			float getStep();

			float getWait();

			float clamp(float x, float lowerlimit, float upperlimit);
			float smoothstep(float edge0, float edge1, float x);

			void stepWait(float step);
			

			Lerper();
			Lerper(glm::vec3 start, glm::vec3 end, float step);
			Lerper(glm::vec3 start, glm::vec3 end, float step, float wait);


			glm::vec3 m_start, m_end;
			float m_step = 0;
			float m_t = 0;
			float m_wait = 0;
			bool m_finished = 0;

		};
	}
}
