#include "PrimativeGenerator.h"
#include "Types.h"
#include "StringFormat.h"
#include "CLog.h"

using gfx::PrimativeGenerator;

namespace
{
	const char * CLASSNAME = "PrimativeGenerator";
}

// converts cartesian to polar
 glm::vec2 PrimativeGenerator::cart_polar(glm::vec3 v)
{
	float r = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	return glm::vec2(atan(v.y / v.x), acos(v.z / r));
}
// converts polar to cartesian
 glm::vec3 PrimativeGenerator::polar_cart(float theta, float phi)
{
	return glm::vec3(cos(theta)*cos(phi), cos(theta) * sin(phi), sin(theta));
}
//Returns random float
 float		PrimativeGenerator::pg_randf()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
 float PrimativeGenerator::Dot(glm::vec3 a, glm::vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

//shape generators non indexed triangles

 std::vector<glm::vec3>			PrimativeGenerator::subdivide(std::vector<glm::vec3> v)
{
	std::vector<glm::vec3> nv;
	for (int i = 0; i < v.size(); i += 3)
	{
		glm::vec3
			a = v[i],
			b = v[i + 1],
			c = v[i + 2],
			n = (a - b) / 2.0f + b;

		nv.push_back(n);
		nv.push_back(a);
		nv.push_back(b);

		nv.push_back(n);
		nv.push_back(b);
		nv.push_back(c);
	}
	return nv;
}

 std::vector<glm::vec3>			PrimativeGenerator::generate_cube()
{
	std::vector<glm::vec3> v;
	for (int i = 0; i < 36; i++)
		v.push_back(glm::vec3(CUBE_V_B[i * 3], CUBE_V_B[i * 3 + 1], CUBE_V_B[i * 3 + 2]));
	return v;
}
 std::vector<glm::vec3>			PrimativeGenerator::generate_cone(int k)
{
	std::vector<glm::vec3> v;
	float step = 2.0f * 3.141596f / float(k);
	float c = 0.0f, s = 0.0f;
	float len = 2.0f;
	//cone
	for (float a = 0; a <= (2.0f * 3.141596f); a += step)
	{
		v.push_back(glm::vec3());
		c = cos(a);
		s = sin(a);
		v.push_back(glm::vec3(c, s, len));
		c = cos(a - step);
		s = sin(a - step);
		v.push_back(glm::vec3(c, s, len));
	}
	//circle
	for (float a = 0; a <= (2.0f * 3.141596f); a += step)
	{
		c = cos(a - step);
		s = sin(a - step);
		v.push_back(glm::vec3(c, s, len));
		c = cos(a);
		s = sin(a);
		v.push_back(glm::vec3(c, s, len));
		v.push_back(glm::vec3(0.0f, 0.0f, len));
	}
	return v;
}

 std::vector<glm::vec3>			PrimativeGenerator::generate_circle(int k)
 {
	 std::vector<glm::vec3> v;
	 float step = 2.0f * 3.141596f / float(k);
	 float c = 0.0f, s = 0.0f;
	 float len = 0.0f;
	 for (float a = 0; a <= (2.0f * 3.141596f); a += step)
	 {
		 c = cos(a - step);
		 s = sin(a - step);
		 v.push_back(glm::vec3(c, s, len));
		 c = cos(a);
		 s = sin(a);
		 v.push_back(glm::vec3(c, s, len));
		 v.push_back(glm::vec3(0.0f, 0.0f, len));
	 }
	 return v;
 }

 std::vector<glm::vec3>			PrimativeGenerator::generate_centered_circle(int k)
 {
	 std::vector<glm::vec3> v;
	 float step = 2.0f * 3.141596f / float(k);
	 float c = 0.0f, s = 0.0f;
	 float len = 0.0f;
	 for (float a = 0; a <= (2.0f * 3.141596f); a += step)
	 {
		 c = cos(a - step);
		 s = sin(a - step);
		 v.push_back(glm::vec3(c, s, len) / 2.0f + glm::vec3(0.5f,0.5f,0));
		 c = cos(a);
		 s = sin(a);
		 v.push_back(glm::vec3(c, s, len) / 2.0f + glm::vec3(0.5f, 0.5f, 0));
		 v.push_back(glm::vec3(0.0f, 0.0f, len) / 2.0f + glm::vec3(0.5f, 0.5f, 0));
	 }
	 return v;
 }

 std::vector<glm::vec3>			PrimativeGenerator::generate_cylinder(int k, float len)
{
	std::vector<glm::vec3> v;
	glm::vec3 t1, t2;
	float step = 2. * 3.141596 / float(k);
	float Radius = 1., c = 0., s = 0.;
	for (float a = 0; a <= (2. * 3.141596); a += step)
	{
		v.push_back(glm::vec3(0., 0., 0));
		c = Radius * cos(a);
		s = Radius * sin(a);
		v.push_back(glm::vec3(c, s, 0));
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);
		v.push_back(glm::vec3(c, s, 0));
	}
	for (float a = 0; a > -(2. * 3.141596); a -= step)
	{
		c = Radius * cos(a);
		s = Radius * sin(a);
		t1 = glm::vec3(c, s, 0);
		t2 = glm::vec3(c, s, len);
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);

		v.push_back(t1);
		v.push_back(t2);
		v.push_back(glm::vec3(c, s, len));
		v.push_back(glm::vec3(c, s, len));
		v.push_back(glm::vec3(c, s, 0));
		v.push_back(t1);
	}
	for (float a = 0; a <= (2. * 3.141596); a += step)
	{
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);
		v.push_back(glm::vec3(c, s, len));
		c = Radius * cos(a);
		s = Radius * sin(a);
		v.push_back(glm::vec3(c, s, len));
		v.push_back(glm::vec3(0., 0., len));
	}
	return v;
}
 std::vector<glm::vec3>			PrimativeGenerator::generate_sphere(int lats, int longs)
{
	std::vector<glm::vec3> v;
	float step_lats = glm::radians(360.0f) / float(lats);
	float step_longs = glm::radians(360.0f) / float(longs);
	float Radius = 1., x, y, z;
	for (float a = -glm::radians(180.0f); a <= glm::radians(180.0f); a += step_lats)
		for (float b = -glm::radians(180.0f); b <= glm::radians(180.0f); b += step_longs)
		{
			v.push_back(polar_cart(a, b));
			v.push_back(polar_cart(a + step_lats, b));
			v.push_back(polar_cart(a + step_lats, b + step_longs));
			v.push_back(polar_cart(a + step_lats, b + step_longs));
			v.push_back(polar_cart(a, b + step_longs));
			v.push_back(polar_cart(a, b));
		}
	return v;
}
 std::vector<glm::vec3>			PrimativeGenerator::generate_sphere_invert(int lats, int longs)
{
	std::vector<glm::vec3> v;
	float step_lats = glm::radians(360.0f) / float(lats);
	float step_longs = glm::radians(360.0f) / float(longs);
	float Radius = 1., x, y, z;
	for (float a = 0; a <= glm::radians(360.0f); a += step_lats)
		for (float b = 0; b <= glm::radians(360.0f); b += step_longs)
		{
			v.push_back(polar_cart(a, b));
			v.push_back(polar_cart(a, b + step_longs));
			v.push_back(polar_cart(a + step_lats, b + step_longs));
			v.push_back(polar_cart(a + step_lats, b + step_longs));
			v.push_back(polar_cart(a + step_lats, b));
			v.push_back(polar_cart(a, b));
		}
	return v;
}
 std::vector<glm::vec3>			PrimativeGenerator::generate_rect()
{
	std::vector<glm::vec3> n;
	n.push_back(glm::vec3(1, 0, 1));
	n.push_back(glm::vec3(1, 0, -1));
	n.push_back(glm::vec3(-1, 0, -1));
	n.push_back(glm::vec3(-1, 0, -1));
	n.push_back(glm::vec3(-1, 0, 1));
	n.push_back(glm::vec3(1, 0, 1));
	return n;
}
 std::vector<glm::vec3>			PrimativeGenerator::generate_rects(int w, int h)
{
	glm::vec3
		s = glm::vec3(1 / (float)w, 0, 1 / (float)h),
		a = glm::vec3(1, 0, 1) * s,
		b = glm::vec3(1, 0, -1) * s,
		c = glm::vec3(-1, 0, -1) * s,
		d = glm::vec3(-1, 0, 1) * s;
	std::vector<glm::vec3> n;
	for (int x = 0; x < w; ++x)
		for (int y = 0; y < h; ++y)
		{
			glm::vec3 t = s * glm::vec3(x, 0, y) - glm::vec3(0.5f, 0, 0.5f);
			n.push_back(a + t);
			n.push_back(b + t);
			n.push_back(c + t);
			n.push_back(c + t);
			n.push_back(d + t);
			n.push_back(a + t);
		}
	return n;
}
 std::vector<glm::vec2>			PrimativeGenerator::generate_uv_rects(int w, int h)
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
 std::vector<glm::vec2>			PrimativeGenerator::generate_null_uvs(int n)
{
	std::vector<glm::vec2> uv;
	for (int i = 0; i < n; i++)
		uv.push_back(glm::vec2());
	return uv;
}
// generates normals from every triangle
 std::vector<glm::vec3>			PrimativeGenerator::generate_normals(std::vector<glm::vec3> v)
{
	std::vector<glm::vec3> n;
	for (int i = 0; i < v.size(); i += 3)
	{
		glm::vec3 nm = glm::normalize(glm::cross(v[i + 1] - v[i], v[i + 2] - v[i]));
		for (int j = 0; j < 3; ++j)
			n.push_back(nm);
	}
	return n;
}
 std::vector<glm::vec3>			PrimativeGenerator::generate_map_heights_from_uvs(std::vector<glm::vec3> v, std::vector<glm::vec3> n, std::vector<glm::vec2> uv, alib::ImageData_T * image, float k)
{
	std::vector<glm::vec3> V;
	for (int i = 0; i < v.size(); i += 3)
	{
		glm::vec3 nm = glm::normalize(glm::cross(v[i + 1] - v[i], v[i + 2] - v[i]));
		for (int j = 0; j < 3; ++j)
		{
			glm::vec2 uvt = uv[i + j];
			int x = uvt.x * image->w;
			int y = uvt.y * image->h;
			int c = x * image->n + (y * image->n * image->w);
			c = glm::clamp(c, 0, image->w*image->h*image->n - 2);
			float r = (image->data[c]) / (256.0f);
			r = (r - 1.0f) * k;
			glm::vec3 nv = nm * r + v[i + j];
			V.push_back(nv);
		}
	}
	return V;
}
// generates a second normal (tangent) for every normal (used for normal mapping)
 std::vector<glm::vec3>			PrimativeGenerator::generate_tangents(std::vector<glm::vec3> v)
{
	std::vector<glm::vec3> n;
	for (int i = 0; i < v.size(); i += 6)
	{
		glm::vec3 nm = glm::normalize(v[i + 2] - v[i]);
		for (int j = 0; j < 6; ++j)
			n.push_back(nm);
	}
	return n;
}
// generates uvs from converted carts to polar
 std::vector<glm::vec2>			PrimativeGenerator::generate_polar_uvs(std::vector<glm::vec3> v)
{
	std::vector<glm::vec2> uv;
	for (int i = 0; i < v.size(); i++)
		uv.push_back(cart_polar(v[i]));
	return uv;
}
 std::vector<glm::vec2>			PrimativeGenerator::generate_sphereical_uvs(std::vector<glm::vec3> v)
{
	std::vector<glm::vec2> uv;
	for (int i = 0; i < v.size(); i++)
	{
		uv.push_back(glm::vec2(
			atan2(v[i].x, v[i].y) / glm::pi<float>() * 0.5f,
			asin(v[i].z) / glm::pi<float>() - .5f
		));
	}
	return uv;
}
 std::vector<glm::vec2>			PrimativeGenerator::generate_repeated_rect_uvs(std::vector<glm::vec3> v)
{
	std::vector<glm::vec2> uv;
	for (int i = 0; i < v.size(); i += 6)
	{
		uv.push_back(glm::vec2(0, 0));
		uv.push_back(glm::vec2(1, 0));
		uv.push_back(glm::vec2(1, 1));
		uv.push_back(glm::vec2(1, 1));
		uv.push_back(glm::vec2(0, 1));
		uv.push_back(glm::vec2(0, 0));
	}
	return uv;
}
// generates a single color buffer
 std::vector<glm::vec3>			PrimativeGenerator::generate_colour_buffer(glm::vec3 colour, int n)
{
	std::vector<glm::vec3> v;
	for (int i = 0; i < n; i++)
		v.push_back(colour);
	return v;
}
 std::vector<glm::vec3>			PrimativeGenerator::generate_terrain_sphere(std::vector<glm::vec3> v, std::vector<glm::vec3> m)
{
	std::vector<glm::vec3> V;
	for (int i = 0; i < v.size(); i += 3)
	{
		for (int j = 0; j < 3; ++j)
		{
			glm::vec2 uvt = glm::vec2((atan2(v[i + j].x, v[i + j].y) / 3.1415926f + 1.0f) * 0.5, (asin(v[i + j].z) / 3.1415926 + 0.5));
			float height = m[i + j].y;
			glm::vec3 nm = glm::normalize(v[i + j]);
			glm::vec3 nv = nm * height + v[i + j];
			V.push_back(nv);
		}
	}
	return V;
}
 std::vector<glm::vec3>			PrimativeGenerator::generate_centered_square_mesh(int w, int h)
{
	 glm::vec3
		 s = glm::vec3(1 / (float)w, 1 / (float)h, 0),
		 a = glm::vec3(0, 0, 0) * s,
		 b = glm::vec3(0, 1, 0) * s,
		 c = glm::vec3(1, 1, 0) * s,
		 d = glm::vec3(1, 0, 0) * s;
	 std::vector<glm::vec3> n;

	 for (int y = 0; y < h; ++y)
		 for (int x = 0; x < w; ++x)
		 {
			 glm::vec3 t = s * glm::vec3(x, y, 0) - glm::vec3(0.5f, 0.5f, 0);
			 n.push_back(a + t);
			 n.push_back(b + t);
			 n.push_back(c + t);
			 n.push_back(c + t);
			 n.push_back(d + t);
			 n.push_back(a + t);
		 }
	 return n;
}
 std::vector<glm::vec3>			PrimativeGenerator::generate_square_mesh(int w, int h)
{
	glm::vec3
		s = glm::vec3(1 / (float)w, 1 / (float)h, 0),
		a = glm::vec3(0, 0, 0) * s,
		b = glm::vec3(0, 1, 0) * s,
		c = glm::vec3(1, 1, 0) * s,
		d = glm::vec3(1, 0, 0) * s;
	std::vector<glm::vec3> n;

	for (int y = 0; y < h; ++y)
		for (int x = 0; x < w; ++x)
		{
			glm::vec3 t = s * glm::vec3(x, y, 0);
			n.push_back(a + t);
			n.push_back(b + t);
			n.push_back(c + t);
			n.push_back(c + t);
			n.push_back(d + t);
			n.push_back(a + t);
		}
	return n;
}

// std::vector<glm::vec3> *			PrimativeGenerator::generate_terrain(std::vector<glm::vec3> * v, int w, int h, float variation, float flattness)
//{
//	float m = variation / (float)max(w, h);
//	for (int y = 0; y < h; ++y)
//		for (int x = 0; x < w; ++x)
//		{
//			int index = (x + y * w) * 6;
//			float heightSum = stb_perlin_noise3(x * m, y * m, 0, m, m) * flattness;
//			(*v)[index].y += heightSum;
//			(*v)[index + 5].y += heightSum;
//			if (x > 0)
//				(*v)[index - 2].y += heightSum;
//			if (y > 0)
//			{
//				(*v)[index - w * 6 + 1].y += heightSum;
//				if (x > 0)
//				{
//					(*v)[index - (w + 1) * 6 + 2].y += heightSum;
//					(*v)[index - (w + 1) * 6 + 3].y += heightSum;
//				}
//			}
//		}
//	return v;
//}
 std::vector<glm::vec3>			PrimativeGenerator::generate_terrian_colour(std::vector<glm::vec3> v, float max_height)
{
	std::vector<glm::vec3> c;
	float f, h;
	for (int i = 0; i < v.size(); i++)
	{
		f = pg_randf();
		h = v[i].y / max_height;
		c.push_back(glm::vec3(0.2f, h * 2 + 0.35f, 0.1f) + f * glm::vec3(1, 1, 1) * 0.05f + glm::clamp(h * h * h * 10000.0f, 0.0f, 1.0f) * glm::vec3(1, 1, 1));
	}
	return c;
}
 std::vector<glm::vec3>			PrimativeGenerator::generate_water_colour(std::vector<glm::vec3> v)
{
	std::vector<glm::vec3> c;
	float f, h;
	for (int i = 0; i < v.size(); i++)
	{
		f = pg_randf();
		h = v[i].y;
		c.push_back((glm::vec3(0, 0, 1) * (0.5f + glm::clamp(h * 1000.0f, 0.0f, 1.0f)) + glm::vec3(0.5, 0.5, 0.5) * 0.3f + glm::vec3(0, 1, 0) * 0.1f));
	}
	return c;
}
 std::vector<glm::vec3>			PrimativeGenerator::pre_rotate(std::vector<glm::vec3> v, glm::vec3 rotate)
{
	for (glm::vec3 V : v)
		V = glm::quat(rotate) * V;
	return v;
}

 std::vector<glm::vec3>			PrimativeGenerator::generate_square_border(float weight, float aspectRatio)
 {
	 std::vector<glm::vec3> n;

	 glm::vec3 a, b, c, d;

	 // left
	 a = glm::vec3(0, 0, 0);
	 b = glm::vec3(0, 1, 0);
	 c = glm::vec3(weight / aspectRatio, 1, 0);
	 d = glm::vec3(weight / aspectRatio, 0, 0);

	 n.push_back(a);
	 n.push_back(b);
	 n.push_back(c);
	 n.push_back(c);
	 n.push_back(d);
	 n.push_back(a);

	 //top
	 a = glm::vec3(0, 1- weight* aspectRatio, 0);
	 b = glm::vec3(0, 1, 0);
	 c = glm::vec3(1, 1, 0);
	 d = glm::vec3(1, 1- weight* aspectRatio, 0);

	 n.push_back(a);
	 n.push_back(b);
	 n.push_back(c);
	 n.push_back(c);
	 n.push_back(d);
	 n.push_back(a);

	 //right
	 a = glm::vec3(1- weight/ aspectRatio, 0, 0);
	 b = glm::vec3(1- weight/ aspectRatio, 1, 0);
	 c = glm::vec3(1, 1, 0);
	 d = glm::vec3(1, 0, 0);

	 n.push_back(a);
	 n.push_back(b);
	 n.push_back(c);
	 n.push_back(c);
	 n.push_back(d);
	 n.push_back(a);

	 //bottom
	 a = glm::vec3(0, 0, 0);
	 b = glm::vec3(0, weight * aspectRatio, 0);
	 c = glm::vec3(1, weight * aspectRatio, 0);
	 d = glm::vec3(1, 0, 0);

	 n.push_back(a);
	 n.push_back(b);
	 n.push_back(c);
	 n.push_back(c);
	 n.push_back(d);
	 n.push_back(a);

	 return n;
 }

// generates a random color buffer where max is the cap color
 std::vector<glm::vec3>			PrimativeGenerator::random_colour_buffer(glm::vec3 max, int n)
{
	std::vector<glm::vec3> v;
	for (int i = 0; i < n; i++)
		v.push_back(glm::vec3(max.x*pg_randf(), max.y*pg_randf(), max.z*pg_randf()));
	return v;
}
// generates a single color buffer of random intensities
 std::vector<glm::vec3>			PrimativeGenerator::random_intesity_colour_buffer(glm::vec3 colour, int n)
{
	std::vector<glm::vec3> v;
	float f;
	for (int i = 0; i < n; i++)
	{
		f = pg_randf();
		v.push_back(glm::vec3(colour.x*f, colour.y*f, colour.z*f));
	}
	return v;
}


 std::vector<glm::vec3>			PrimativeGenerator::load_model(const char * filename)
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector< glm::vec3 > vertices;

	std::string obj_err =
		tinyobj::LoadObj(shapes, materials, filename, NULL);

	CINFO(alib::StringFormat("model loaded with errors:\n%0").arg(obj_err).str());
		
	for (int i = 0; i < shapes.size(); i++)
		for (int j = 0; j < shapes[i].mesh.indices.size(); j++)
			vertices.push_back(glm::vec3(
				shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3],
				shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3 + 1],
				shapes[i].mesh.positions[shapes[i].mesh.indices[j] * 3 + 2]
			));
	return vertices;
}


// creates a vector of Vertices to pass to Obj
 gfx::VertexData				PrimativeGenerator::pack_object(
	std::vector<glm::vec3> * v,
	unsigned int flags,
	glm::vec3 color
)
{
	std::vector<gfx::Vertex_T> object;
	std::vector<glm::vec3> n, c, t, nv = *v;
	std::vector<glm::vec2> uv;

	if (flags == NULL)
		flags = GEN_DEFAULT;

	if ((flags & GEN_NORMS) == GEN_NORMS)
		n = generate_normals(nv);
	if ((flags & GEN_COLOR) == GEN_COLOR)
		c = generate_colour_buffer(color, nv.size());
	if ((flags & GEN_COLOR_RAND) == GEN_COLOR_RAND)
		c = random_colour_buffer(color, nv.size());
	if ((flags & GEN_COLOR_RAND_I) == GEN_COLOR_RAND_I)
		c = random_intesity_colour_buffer(color, nv.size());
	if ((flags & GEN_UVS_POLAR) == GEN_UVS_POLAR)
		uv = generate_polar_uvs(nv);
	if ((flags & GEN_UVS_RECTS) == GEN_UVS_RECTS)
		uv = generate_repeated_rect_uvs(nv);
	if ((flags & GEN_UVS_SPHERE) == GEN_UVS_SPHERE)
		uv = generate_sphereical_uvs(nv);
	else
		uv = generate_null_uvs(nv.size());
	if ((flags & GEN_TANGS) == GEN_TANGS)
		t = generate_tangents(nv);
	if ((flags & GEN_SQUAREUVS) == GEN_SQUAREUVS)
		uv = generate_square_mesh_uvs(16, 16);

	for (int i = 0; i < nv.size(); ++i)
	{
		gfx::Vertex_T vert;
		if (nv.size() != 0)
			vert.position = nv[i];
		else
			vert.position = glm::vec3();
		if (c.size() != 0)
			vert.color = c[i];
		else
			vert.color = glm::vec3();
		if (n.size() != 0)
			vert.normal = n[i];
		else
			vert.normal = glm::vec3();
		if (uv.size() != 0)
			vert.uv = uv[i];
		else
			vert.uv = glm::vec2();
		if (t.size() != 0)
			vert.tangent = t[i];
		else
			vert.tangent = glm::vec3();
		object.push_back(vert);
	}
	return object;
}
 gfx::VertexData				PrimativeGenerator::pack_object(
	std::vector<glm::vec3> * v,
	unsigned int flags,
	glm::vec3 color,
	 alib::ImageData_T * image,
	float k
)
{
	std::vector<gfx::Vertex_T> object;
	std::vector<glm::vec3> n, c, t, nv = *v;
	std::vector<glm::vec2> uv;

	if (flags == NULL)
		flags = GEN_DEFAULT;


	if ((flags & GEN_NORMS) == GEN_NORMS)
		n = generate_normals(nv);

	if ((flags & GEN_MAP_HEIGHTS) == GEN_MAP_HEIGHTS)
		nv = generate_map_heights_from_uvs(nv, n, uv, image, k);

	if ((flags & GEN_NORMS) == GEN_NORMS)
		n = generate_normals(nv);
	if ((flags & GEN_COLOR) == GEN_COLOR)
		c = generate_colour_buffer(color, nv.size());
	if ((flags & GEN_COLOR_RAND) == GEN_COLOR_RAND)
		c = random_colour_buffer(color, nv.size());
	if ((flags & GEN_COLOR_RAND_I) == GEN_COLOR_RAND_I)
		c = random_intesity_colour_buffer(color, nv.size());
	if ((flags & GEN_UVS_POLAR) == GEN_UVS_POLAR)
		uv = generate_polar_uvs(nv);
	if ((flags & GEN_UVS_RECTS) == GEN_UVS_RECTS)
		uv = generate_repeated_rect_uvs(nv);
	if ((flags & GEN_UVS_SPHERE) == GEN_UVS_SPHERE)
		uv = generate_sphereical_uvs(nv);
	if ((flags & GEN_TANGS) == GEN_TANGS)
		t = generate_tangents(nv);
	if ((flags & GEN_SQUAREUVS) == GEN_SQUAREUVS)
		uv = generate_square_mesh_uvs(16, 16);

	for (int i = 0; i < nv.size(); ++i)
	{
		gfx::Vertex_T vert;
		if (nv.size() != 0)
			vert.position = nv[i];
		else
			vert.position = glm::vec3();
		if (c.size() != 0)
			vert.color = c[i];
		else
			vert.color = glm::vec3();
		if (n.size() != 0)
			vert.normal = n[i];
		else
			vert.normal = glm::vec3();
		if (uv.size() != 0)
			vert.uv = uv[i];
		else
			vert.uv = glm::vec2();
		if (t.size() != 0)
			vert.tangent = t[i];
		else
			vert.tangent = glm::vec3();
		object.push_back(vert);
	}
	return object;
}
// normal packer of custom properties
 gfx::VertexData			PrimativeGenerator::pack_object(
	std::vector<glm::vec3> * v,
	std::vector<glm::vec3> * c,
	std::vector<glm::vec3> * n,
	std::vector<glm::vec2> * uv,
	std::vector<glm::vec3> * t
)
{
	std::vector<gfx::Vertex_T> object;
	for (int i = 0; i < v->size(); ++i)
	{
		gfx::Vertex_T vert;
		if (v != NULL)
			vert.position = (*v)[i];
		else
			vert.position = glm::vec3();
		if (c != NULL)
			vert.color = (*c)[i];
		else
			vert.color = glm::vec3();
		if (n != NULL)
			vert.normal = (*n)[i];
		else
			vert.normal = glm::vec3();
		if (uv != NULL)
			vert.uv = (*uv)[i];
		else
			vert.uv = glm::vec2();
		if (t != NULL)
			vert.tangent = (*t)[i];
		else
			vert.tangent = glm::vec3();
		object.push_back(vert);
	}
	return object;
}
