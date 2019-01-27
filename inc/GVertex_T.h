#pragma once

#include "glm.h"

namespace glw
{
  namespace engine
  {
    namespace buffers
    {
      struct GVertex_T
	    {
		    glm::vec3 position;
		    glm::vec3 color;
		    glm::vec3 normal;
		    glm::vec3 tangent;
		    glm::vec2 uv;
	    };
    }  
  } 
}

