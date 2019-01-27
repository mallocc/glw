#pragma once

#include "glm.h"


namespace glw
{
  namespace engine
  {
    class GCamera
    {
    public:
      
      GCamera();
      
			GCamera(glm::vec3 pos, glm::vec3 vel, glm::vec3 dir, glm::vec3 up);
      
      ~GCamera();
      
			void update(float dt = 1.0f, float friction = 1.0f);

			void applyForce(glm::vec3 force);
			void applyForceForward(float mag = 1.0f);
			void applyForceBackward(float mag = 1.0f);
			void applyForceRight(float mag = 1.0f);
			void applyForceLeft(float mag = 1.0f);
			void applyForceUp(float mag = 1.0f);
			void applyForceDown(float mag = 1.0f);

			void yawLeft(float deg);
			void yawRight(float deg);
			void pitchUp(float deg);
			void pitchDown(float deg);
			void rollLeft(float deg);
			void rollRight(float deg);

			void brake(float friction);

			glm::vec3 getPosition();
			glm::vec3 getLookPosition();
			glm::vec3 getUp();
      
    private:
			// member variables
			glm::vec3 m_pos;
			glm::vec3 m_vel;
			glm::vec3 m_dir;
			glm::vec3 m_dirVel;
			glm::vec3 m_up;
			glm::vec3 m_upVel;
    };
  }
}
