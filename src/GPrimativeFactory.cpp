#include "GPrimativeFactory.h"

#include "GVertex_T.h"


using glw::engine::buffers::GPrimativeFactory;
using glw::engine::buffers::GArrayVertex;
using glw::engine::buffers::GVertex_T;
using glw::engine::buffers::GArrayVec3;
using glw::engine::buffers::GArrayVec2;


namespace
{
  const char * TRG = "GPFT";
  const char * __CLASSNAME__ = "GPrimativeFactory";
  
  const float PI = 3.141596f;
  const float PI2 = PI * 2.0f;
  
  //Cube vertex data array
  const float CUBE_V_B[] = {
	  -1.0f,-1.0f,-1.0f,
	  -1.0f,-1.0f, 1.0f,
	  -1.0f, 1.0f, 1.0f,
	   1.0f, 1.0f,-1.0f,
	  -1.0f,-1.0f,-1.0f,
	  -1.0f, 1.0f,-1.0f,
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

GPrimativeFactory::GPrimativeFactory()
{
}


GPrimativeFactory::~GPrimativeFactory()
{
}

void GPrimativeFactory::cube(GArrayVec3& v)
{
  for (int i = 0; i < 36; i++)
	{
	  v.push_back(glm::vec3(CUBE_V_B[i * 3], CUBE_V_B[i * 3 + 1], CUBE_V_B[i * 3 + 2]));
	}
}
void GPrimativeFactory::cone(GArrayVec3& v, int k)
{
	float step = PI2 / float(k);
	float c = 0.0f, s = 0.0f;
	float len = 2.0f;
	//cone
	for (float a = 0; a <= PI2; a += step)
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
	for (float a = 0; a <= PI2; a += step)
	{
		c = cos(a - step);
		s = sin(a - step);
		v.push_back(glm::vec3(c, s, len));
		c = cos(a);
		s = sin(a);
		v.push_back(glm::vec3(c, s, len));
		v.push_back(glm::vec3(0.0f, 0.0f, len));
	}
}
void GPrimativeFactory::circle(GArrayVec3& v, int k)
{
	 float step = PI2 / float(k);
	 float c = 0.0f, s = 0.0f;
	 float len = 0.0f;
	 for (float a = 0; a <= PI2; a += step)
	 {
		 c = cos(a - step);
		 s = sin(a - step);
		 v.push_back(glm::vec3(c, s, len));
		 c = cos(a);
		 s = sin(a);
		 v.push_back(glm::vec3(c, s, len));
		 v.push_back(glm::vec3(0.0f, 0.0f, len));
	 }
}
void GPrimativeFactory::centeredCircle(GArrayVec3& v, int k)
{
	 float step = PI2 / float(k);
	 float c = 0.0f, s = 0.0f;
	 float len = 0.0f;
	 for (float a = 0; a <= PI2; a += step)
	 {
		 c = cos(a - step);
		 s = sin(a - step);
		 v.push_back(glm::vec3(c, s, len) / 2.0f + glm::vec3(0.5f,0.5f,0));
		 c = cos(a);
		 s = sin(a);
		 v.push_back(glm::vec3(c, s, len) / 2.0f + glm::vec3(0.5f, 0.5f, 0));
		 v.push_back(glm::vec3(0.0f, 0.0f, len) / 2.0f + glm::vec3(0.5f, 0.5f, 0));
	 }
}
void GPrimativeFactory::cylinder(GArrayVec3& v, int k, float len)
{
	glm::vec3 t1, t2;
	float step = PI2 / float(k);
	float Radius = 1., c = 0., s = 0.;
	for (float a = 0; a <= PI2; a += step)
	{
		v.push_back(glm::vec3(0., 0., 0));
		c = Radius * cos(a);
		s = Radius * sin(a);
		v.push_back(glm::vec3(c, s, 0));
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);
		v.push_back(glm::vec3(c, s, 0));
	}
	for (float a = 0; a > -PI2; a -= step)
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
	for (float a = 0; a <= PI2; a += step)
	{
		c = Radius * cos(a - step);
		s = Radius * sin(a - step);
		v.push_back(glm::vec3(c, s, len));
		c = Radius * cos(a);
		s = Radius * sin(a);
		v.push_back(glm::vec3(c, s, len));
		v.push_back(glm::vec3(0., 0., len));
	}
}
void GPrimativeFactory::sphere(GArrayVec3& v, int lats, int longs)
{
	float step_lats = PI2 / float(lats);
	float step_longs = PI2 / float(longs);
	float Radius = 1., x, y, z;
	for (float a = -PI; a <= PI; a += step_lats)
	{
		for (float b = -PI; b <= PI; b += step_longs)
		{
			v.push_back(polar_cart(a, b));
			v.push_back(polar_cart(a + step_lats, b));
			v.push_back(polar_cart(a + step_lats, b + step_longs));
			v.push_back(polar_cart(a + step_lats, b + step_longs));
			v.push_back(polar_cart(a, b + step_longs));
			v.push_back(polar_cart(a, b));
		}
	}
}
void GPrimativeFactory::sphereInvert(GArrayVec3& v, int lats, int longs)
{
	float step_lats = PI2 / float(lats);
	float step_longs = PI2 / float(longs);
	float Radius = 1., x, y, z;
	for (float a = 0; a <= PI2; a += step_lats)
	{
		for (float b = 0; b <= PI2; b += step_longs)
		{
			v.push_back(polar_cart(a, b));
			v.push_back(polar_cart(a, b + step_longs));
			v.push_back(polar_cart(a + step_lats, b + step_longs));
			v.push_back(polar_cart(a + step_lats, b + step_longs));
			v.push_back(polar_cart(a + step_lats, b));
			v.push_back(polar_cart(a, b));
		}
	}
}
void GPrimativeFactory::rect(GArrayVec3& v)
{
	v.push_back(glm::vec3(1, 0, 1));
	v.push_back(glm::vec3(1, 0, -1));
	v.push_back(glm::vec3(-1, 0, -1));
	v.push_back(glm::vec3(-1, 0, -1));
	v.push_back(glm::vec3(-1, 0, 1));
	v.push_back(glm::vec3(1, 0, 1));
}
void GPrimativeFactory::rects(GArrayVec3& v, int w, int h)
{
	glm::vec3
		s = glm::vec3(1 / (float)w, 0, 1 / (float)h),
		a = glm::vec3(1, 0, 1) * s,
		b = glm::vec3(1, 0, -1) * s,
		c = glm::vec3(-1, 0, -1) * s,
		d = glm::vec3(-1, 0, 1) * s;
	for (int x = 0; x < w; ++x)
	{
		for (int y = 0; y < h; ++y)
		{
			glm::vec3 t = s * glm::vec3(x, 0, y) - glm::vec3(0.5f, 0, 0.5f);
			v.push_back(a + t);
			v.push_back(b + t);
			v.push_back(c + t);
			v.push_back(c + t);
			v.push_back(d + t);
			v.push_back(a + t);
		}
	}
}     
void GPrimativeFactory::squareMesh(GArrayVec3& v, int w, int h)
{
	glm::vec3
		s = glm::vec3(1 / (float)w, 1 / (float)h, 0),
		a = glm::vec3(0, 0, 0) * s,
		b = glm::vec3(0, 1, 0) * s,
		c = glm::vec3(1, 1, 0) * s,
		d = glm::vec3(1, 0, 0) * s;
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			glm::vec3 t = s * glm::vec3(x, y, 0);
			v.push_back(a + t);
			v.push_back(b + t);
			v.push_back(c + t);
			v.push_back(c + t);
			v.push_back(d + t);
			v.push_back(a + t);
		}
	}
}
void GPrimativeFactory::squareMeshes(GArrayVec3& v, int n)
{
	glm::vec3
		a = glm::vec3(0, 0, 0),
		b = glm::vec3(0, 1, 0),
		c = glm::vec3(1, 1, 0),
		d = glm::vec3(1, 0, 0);
	for (int x = 0; x < n; ++x)
	{
		v.push_back(a);
		v.push_back(b);
		v.push_back(c);
		v.push_back(c);
		v.push_back(d);
		v.push_back(a);
	}
}  
void GPrimativeFactory::centeredSquareMesh(GArrayVec3& v, int w, int h)
{
  glm::vec3
    s = glm::vec3(1 / (float)w, 1 / (float)h, 0),
    a = glm::vec3(0, 0, 0) * s,
    b = glm::vec3(0, 1, 0) * s,
    c = glm::vec3(1, 1, 0) * s,
    d = glm::vec3(1, 0, 0) * s;
  for (int y = 0; y < h; ++y)
  {
    for (int x = 0; x < w; ++x)
    {
     glm::vec3 t = s * glm::vec3(x, y, 0) - glm::vec3(0.5f, 0.5f, 0);
     v.push_back(a + t);
     v.push_back(b + t);
     v.push_back(c + t);
     v.push_back(c + t);
     v.push_back(d + t);
     v.push_back(a + t);
    }
  }
}
void GPrimativeFactory::squareBorder(GArrayVec3& v, float weight, float aspectRatio)
{
  glm::vec3 a, b, c, d;

  // left
  a = glm::vec3(0, 0, 0);
  b = glm::vec3(0, 1, 0);
  c = glm::vec3(weight / aspectRatio, 1, 0);
  d = glm::vec3(weight / aspectRatio, 0, 0);

  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  v.push_back(c);
  v.push_back(d);
  v.push_back(a);

  //top
  a = glm::vec3(0, 1 - weight * aspectRatio, 0);
  b = glm::vec3(0, 1, 0);
  c = glm::vec3(1, 1, 0);
  d = glm::vec3(1, 1 - weight * aspectRatio, 0);

  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  v.push_back(c);
  v.push_back(d);
  v.push_back(a);

  //right
  a = glm::vec3(1 - weight / aspectRatio, 0, 0);
  b = glm::vec3(1 - weight / aspectRatio, 1, 0);
  c = glm::vec3(1, 1, 0);
  d = glm::vec3(1, 0, 0);

  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  v.push_back(c);
  v.push_back(d);
  v.push_back(a);

  //bottom
  a = glm::vec3(0, 0, 0);
  b = glm::vec3(0, weight * aspectRatio, 0);
  c = glm::vec3(1, weight * aspectRatio, 0);
  d = glm::vec3(1, 0, 0);

  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  v.push_back(c);
  v.push_back(d);
  v.push_back(a);
}


void GPrimativeFactory::colourBuffer(GArrayVec3& c, glm::vec3 colour, int n)
{
	for (int i = 0; i < n; i++)
	{
	  c.push_back(colour);
  }
}

void GPrimativeFactory::randomColourBuffer(GArrayVec3& c, glm::vec3& max, int n)
{
	for (int i = 0; i < n; i++)
	{
	  c.push_back(glm::vec3(max.x*pg_randf(), max.y*pg_randf(), max.z*pg_randf()));
  }
}

void GPrimativeFactory::randomIntesityColourBuffer(GArrayVec3& c, glm::vec3& colour, int n)
{
	float f;
	for (int i = 0; i < n; i++)
	{
		f = pg_randf();
		c.push_back(glm::vec3(colour.x*f, colour.y*f, colour.z*f));
	}
}

void GPrimativeFactory::nullUVs(GArrayVec2& uv, int n)
{
	for (int i = 0; i < n; i++)
	{
		uv.push_back(glm::vec2());
  }
}

void GPrimativeFactory::polarUVs(GArrayVec2& uv, GArrayVec3& v)
{
  for (int i = 0; i < v.size(); i++)
	{
		uv.push_back(cart_polar(v[i]));
  }
}

void GPrimativeFactory::sphereicalUVs(GArrayVec2& uv, GArrayVec3& v)
{
	for (int i = 0; i < v.size(); i++)
	{
		uv.push_back(glm::vec2(
			  atan2(v[i].x, v[i].y) / glm::pi<float>() * 0.5f,
			  asin(v[i].z)          / glm::pi<float>() - 0.5f));
	}
}

void GPrimativeFactory::rectUVs(GArrayVec2& uv, GArrayVec3& v)
{
  for (int i = 0; i < v.size(); i += 6)
  {
    uv.push_back(glm::vec2(0, 0));
    uv.push_back(glm::vec2(1, 0));
    uv.push_back(glm::vec2(1, 1));
    uv.push_back(glm::vec2(1, 1));
    uv.push_back(glm::vec2(0, 1));
    uv.push_back(glm::vec2(0, 0));
  }
}

void GPrimativeFactory::squareMeshUVs(GArrayVec2& uv, int w, int h)
{
  glm::vec2
    s = glm::vec2(1 / (float)w, 1 / (float)h),
    a = glm::vec2(0, 0) * s,
    b = glm::vec2(0, 1) * s,
    c = glm::vec2(1, 1) * s,
    d = glm::vec2(1, 0) * s;

  for (int y = 0; y < h; ++y)
    for (int x = 0; x < w; ++x)
    {
      glm::vec2 t = s * glm::vec2(x, y);
      uv.push_back(a + t);
      uv.push_back(b + t);
      uv.push_back(c + t);
      uv.push_back(c + t);
      uv.push_back(d + t);
      uv.push_back(a + t);
    }
}

void GPrimativeFactory::normals(GArrayVec3& n, GArrayVec3& v)
{
	for (int i = 0; i < v.size(); i += 3)
	{
		glm::vec3 nm = glm::normalize(glm::cross(v[i + 1] - v[i], v[i + 2] - v[i]));
		for (int j = 0; j < 3; ++j)
		{
			n.push_back(nm);
	  }
	}
}

void GPrimativeFactory::tangents(GArrayVec3& t, GArrayVec3& v)
{
	for (int i = 0; i < v.size(); i += 6)
	{
		glm::vec3 nm = glm::normalize(v[i + 2] - v[i]);
		for (int j = 0; j < 6; ++j)
			t.push_back(nm);
	}
}

void GPrimativeFactory::preRotate(GArrayVec3& v, glm::vec3& rotate)
{
	for (glm::vec3& V : v)
	{
	  V = glm::quat(rotate) * V;
  }
}


// creates a vector of Vertices to pass to Obj
void GPrimativeFactory::packObject(GArrayVertex& o, GArrayVec3& v)
{
  GArrayVec2 uv;
  packObject(o, v, uv);
}

void GPrimativeFactory::packObject(GArrayVertex& o, GArrayVec3& v, GArrayVec2& uv)
{
  if(o.size() > 0)
  {
    o.clear();
  }

  GArrayVec3 c, n, t;
  glm::vec3 color = glm::vec3(0.5f);

  if(uv.size() == 0)
  {
    randomColourBuffer(c, color, v.size());
  }
  else
  {
    colourBuffer(c, glm::vec3(0), v.size());
  }
  normals(n, v);
  tangents(t, v);

  packObject(o, v, c, n, t, uv);
}

void GPrimativeFactory::packObject(GArrayVertex& o, GArrayVec3& v, 
                                   GArrayVec3& c, GArrayVec3& n, 
                                   GArrayVec3& t, GArrayVec2& uv)
{
  for (int i = 0; i < v.size(); ++i)
	{
		GVertex_T vert;
		
		if (v.size() != 0)
		{
			vert.position = v[i];
		}
		else
		{
			vert.position = glm::vec3();
		}
		
		if (c.size() != 0)
		{
		  vert.color = c[i];
		}
		else
		{
			vert.color = glm::vec3();
		}
		
		if (n.size() != 0)
		{
		  vert.normal = n[i];
		}
		else
		{
			vert.normal = glm::vec3();
		}
		
		if (uv.size() != 0)
		{
			vert.uv = uv[i];
		}
		else
		{
			vert.uv = glm::vec2();
		}	
		
		if (t.size() != 0)
		{
			vert.tangent = t[i];
		}
		else
		{
			vert.tangent = glm::vec3();
		}
		
		o.push_back(vert);
	}
}
       
//Returns random float
float	GPrimativeFactory::pg_randf()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);                            
}
// converts polar to cartesian
glm::vec3 GPrimativeFactory::polar_cart(float theta, float phi)
{
	return glm::vec3(cos(theta)*cos(phi), cos(theta) * sin(phi), sin(theta));
}
// converts cartesian to polar
glm::vec2 GPrimativeFactory::cart_polar(glm::vec3& v)
{
	float r = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	return glm::vec2(atan(v.y / v.x), acos(v.z / r));
}
