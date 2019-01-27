#pragma once
#include "opengl.h"
#include "glm.h"
#include <math.h>  
#include <minmax.h>
#include "ImageLoader.h"

//#define STB_PERLIN_IMPLEMENTATION
//#include <stb_perlin.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Types.h"
#include "ImageLoader.h"

#define GEN_NORMS 0x1
#define GEN_TANGS 0x2
#define GEN_UVS_POLAR 0x4
#define GEN_UVS_RECTS 0x8
#define GEN_UVS_SPHERE 0x80
#define GEN_MAP_HEIGHTS 0x100
#define GEN_ALL (GEN_NORMS | GEN_TANGS | GEN_UVS_POLAR)
#define GEN_COLOR 0x10
#define GEN_COLOR_RAND 0x20
#define GEN_COLOR_RAND_I 0x40
#define GEN_DEFAULT (GEN_NORMS | GEN_COLOR)
#define GEN_SQUAREUVS 0x200

namespace gfx
{
	namespace
	{
		//Cube vertex data array
		GLfloat CUBE_V_B[] = {
			-1.0f,-1.0f,-1.0f, // triangle 1 : begin
			-1.0f,-1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, // triangle 1 : end
			1.0f, 1.0f,-1.0f, // triangle 2 : begin
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f, // triangle 2 : end
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f
		};
	}

	typedef std::vector<Vertex_T> VertexData;

	class PrimativeGenerator
	{
		public:
			// converts cartesian to polar
			static glm::vec2 cart_polar(glm::vec3 v);
			// converts polar to cartesian
			static glm::vec3 polar_cart(float theta, float phi);
			//Returns random float
			static float		pg_randf();
			static float Dot(glm::vec3 a, glm::vec3 b);
				
			///shape generators non indexed triangles

			static std::vector<glm::vec3>			subdivide(std::vector<glm::vec3> v);

			static std::vector<glm::vec3>			generate_cube();
			static std::vector<glm::vec3>			generate_cone(int k);
			static std::vector<glm::vec3>			generate_circle(int k);
			static std::vector<glm::vec3>			generate_centered_circle(int k);
			static std::vector<glm::vec3>			generate_cylinder(int k, float len);
			static std::vector<glm::vec3>			generate_sphere(int lats, int longs);
			static std::vector<glm::vec3>			generate_sphere_invert(int lats, int longs);
			static std::vector<glm::vec3>			generate_rect();
			static std::vector<glm::vec3>			generate_rects(int w, int h);
			static std::vector<glm::vec2>			generate_uv_rects(int w, int h);
			static std::vector<glm::vec2>			generate_null_uvs(int n);
			// generates normals from every triangle
			static std::vector<glm::vec3>			generate_normals(std::vector<glm::vec3> v);
			static std::vector<glm::vec3>			generate_map_heights_from_uvs(std::vector<glm::vec3> v, std::vector<glm::vec3> n, std::vector<glm::vec2> uv, alib::ImageData_T * image, float k);
			// generates a second normal (tangent) for every normal (used for normal mapping)
			static std::vector<glm::vec3>			generate_tangents(std::vector<glm::vec3> v);
			// generates uvs from converted carts to polar
			static std::vector<glm::vec2>			generate_polar_uvs(std::vector<glm::vec3> v);
			static std::vector<glm::vec2>			generate_sphereical_uvs(std::vector<glm::vec3> v);
			static std::vector<glm::vec2>			generate_repeated_rect_uvs(std::vector<glm::vec3> v);
			// generates a single color buffer
			static std::vector<glm::vec3>			generate_colour_buffer(glm::vec3 colour, int n);
			static std::vector<glm::vec3>			generate_terrain_sphere(std::vector<glm::vec3> v, std::vector<glm::vec3> m);
			static std::vector<glm::vec3>			generate_centered_square_mesh(int w, int h);
			static std::vector<glm::vec3>			generate_square_mesh(int w, int h);
			static std::vector<glm::vec3> *			generate_terrain(std::vector<glm::vec3> * v, int w, int h, float variation, float flattness);
			static std::vector<glm::vec3>			generate_terrian_colour(std::vector<glm::vec3> v, float max_height);
			static std::vector<glm::vec3>			generate_water_colour(std::vector<glm::vec3> v);
			static std::vector<glm::vec3>			pre_rotate(std::vector<glm::vec3> v, glm::vec3 rotate);
			static std::vector<glm::vec3>			generate_square_border(float weight, float aspectRatio);

			// generates a random color buffer where max is the cap color
			static std::vector<glm::vec3>			random_colour_buffer(glm::vec3 max, int n);
			// generates a single color buffer of random intensities
			static std::vector<glm::vec3>			random_intesity_colour_buffer(glm::vec3 colour, int n);


			static std::vector<glm::vec2> generate_square_mesh_uvs(int w, int h)
			{
				glm::vec2
					s = glm::vec2(1 / (float)w, 1 / (float)h),
					a = glm::vec2(0, 0) * s,
					b = glm::vec2(0, 1) * s,
					c = glm::vec2(1, 1) * s,
					d = glm::vec2(1, 0) * s;
				std::vector<glm::vec2> n;

				for (int y = 0; y < h; ++y)
					for (int x = 0; x < w; ++x)
					{
						glm::vec2 t = s * glm::vec2(x, y);
						n.push_back(a + t);
						n.push_back(b + t);
						n.push_back(c + t);
						n.push_back(c + t);
						n.push_back(d + t);
						n.push_back(a + t);
					}
				return n;
			}
			static std::vector<glm::vec3> generate_square_meshes(int N)
			{
				glm::vec3
					a = glm::vec3(0, 0, 0),
					b = glm::vec3(0, 1, 0),
					c = glm::vec3(1, 1, 0),
					d = glm::vec3(1, 0, 0);
				std::vector<glm::vec3> n;
				for (int x = 0; x < N; ++x)
				{
					n.push_back(a);
					n.push_back(b);
					n.push_back(c);
					n.push_back(c);
					n.push_back(d);
					n.push_back(a);
				}
				return n;
			}

			static std::vector<glm::vec3>			load_model(const char * filename);

			// creates a vector of Vertices to pass to Obj
			static VertexData				pack_object(
				std::vector<glm::vec3> * v,
				unsigned int flags,
				glm::vec3 color
			);
			static VertexData				pack_object(
				std::vector<glm::vec3> * v,
				unsigned int flags,
				glm::vec3 color,
				alib::ImageData_T * image,
				float k
			);
			// normal packer of custom properties
			static VertexData				pack_object(
				std::vector<glm::vec3> * v,
				std::vector<glm::vec3> * c,
				std::vector<glm::vec3> * n,
				std::vector<glm::vec2> * uv,
				std::vector<glm::vec3> * t
			);
	};
}