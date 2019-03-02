#pragma once

#include "glm.h"

#include "GVertex_T.h"

#include <vector>

using glw::engine::buffers::GVertex_T;

//TODO comment

namespace glw
{
  namespace engine
  {
    namespace buffers
    {
    	typedef std::vector<GVertex_T> GArrayVertex;
    	typedef std::vector<glm::vec3> GArrayVec3;
    	typedef std::vector<glm::vec2> GArrayVec2;
    	
      class GPrimativeFactory
      {
      public:
        
        GPrimativeFactory();
        
        ~GPrimativeFactory();
        
        ///shape generators non indexed triangles
        
        static void cube(GArrayVec3& v);
			  static void	cone(GArrayVec3& v, int k);
			  static void	circle(GArrayVec3& v, int k);
			  static void	centeredCircle(GArrayVec3& v, int k);
        static void	cylinder(GArrayVec3& v, int k, float len);
			  static void	sphere(GArrayVec3& v, int lats, int longs);
			  static void	sphereInvert(GArrayVec3& v, int lats, int longs);
			  static void	rect(GArrayVec3& v);
			  static void	rects(GArrayVec3& v, int w, int h);     
        static void squareMesh(GArrayVec3& v, int w, int h);
        static void squareMeshes(GArrayVec3& v, int n);  
        static void	centeredSquareMesh(GArrayVec3& v, int w, int h);
        static void squareBorder(GArrayVec3& v, float weight, float aspectRatio);			
			
			  static void colourBuffer(GArrayVec3& c, glm::vec3 colour, int n);
			  static void randomColourBuffer(GArrayVec3& c, glm::vec3& max, int n);
 			  static void randomIntesityColourBuffer(GArrayVec3& c, glm::vec3& colour, int n);
			
			  static void nullUVs(GArrayVec2& uv, int n);
        static void polarUVs(GArrayVec2& uv, GArrayVec3& v);
        static void sphereicalUVs(GArrayVec2& uv, GArrayVec3& v);
        static void rectUVs(GArrayVec2& uv, GArrayVec3& v);
        static void squareMeshUVs(GArrayVec2& uv, int w, int h);
        static void squareMeshUVsGUI(GArrayVec2& uv, int w, int h);
        
        static void normals(GArrayVec3& n, GArrayVec3& v);
        static void tangents(GArrayVec3& t, GArrayVec3& v);
			
			  static void preRotate(GArrayVec3& v, glm::vec3& rotate);
			
				// creates a vector of Vertices to pass to Obj
			  static void packObject(GArrayVertex& o, GArrayVec3& v);
        static void packObject(GArrayVertex& o, GArrayVec3& v, GArrayVec2& uv);
        static void packObject(GArrayVertex& o, GArrayVec3& v, GArrayVec3& n);
			  static void packObject(GArrayVertex& o, GArrayVec3& v, 
			                         GArrayVec3& c, GArrayVec3& n, 
			                         GArrayVec3& t, GArrayVec2& uv);
			  
      private:
        
			  //Returns random float
			  static float pg_randf();
			  // converts polar to cartesian
        static glm::vec3 polar_cart(float theta, float phi);
			  // converts cartesian to polar
        static glm::vec2 cart_polar(glm::vec3& v);
      };
    }
  }
}
