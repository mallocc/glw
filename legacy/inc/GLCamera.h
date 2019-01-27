#pragma once
#include "opengl.h"
#include "glm.h"

namespace gfx
{
	namespace engine
	{
		class GLCamera
		{
		public:
			void update(float dt, float friction);

			void apply_force(glm::vec3 force);
			void apply_force_foward();
			void apply_force_foward(float mag);
			void apply_force_backward();
			void apply_force_right();
			void apply_force_left();

			void yaw_left(float amount);
			void yaw_right(float amount);
			void pitch_up(float amount);
			void pitch_down(float amount);
			void roll_left(float amount);
			void roll_right(float amount);

			void brake(float friction);

			glm::vec3 get_position();

			glm::vec3 get_look_position();

			glm::vec3 get_up();


			// constructors

			GLCamera();

			GLCamera(glm::vec3 pos, glm::vec3 vel, glm::vec3 dir, glm::vec3 up);


			// member variables

			glm::vec3 m_pos, m_vel, m_dir, m_dir_vel, m_up, m_up_vel;

		};
	}
}
