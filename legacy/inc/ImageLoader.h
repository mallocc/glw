#pragma once

#include "opengl.h"
#include "Types.h"

namespace alib
{
	struct ImageData_T
	{
		unsigned char * data;
		int w, h, n;
	};

	class ImageLoader
	{
	public:
		static GLuint loadTextureFromImage(const char *fname);
		static GLuint load_texture_blank();
		static GLuint load_texture_uniform(int r, int g, int b);
		static alib::ImageData_T get_data(const char * filename);
		static alib::ImageData_T * createImageData_T(unsigned char * data, int w, int h, int n, alib::ImageData_T * imageData_t);
		static alib::ImageData_T get_data_png(const char * filename);
	};
}

