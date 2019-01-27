#include "ImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using alib::ImageLoader;

// loads an image into a gl texture
GLuint ImageLoader::loadTextureFromImage(const char *fname)
{
	int w, h, n;
	unsigned char *data = stbi_load(fname, &w, &h, &n, STBI_rgb_alpha);
	if (data == NULL) {
		fprintf(stderr, "Image not loaded: %s\n", fname);
		const char *error = stbi_failure_reason();
		fprintf(stderr, "Failure reason %s\n", error);
		return GL_TEXTURE0;
	}

	if (n == 1)
	{
		unsigned char *data2 = new unsigned char[w*h * 3];
		for (int i = 0; i < w*h; ++i)
			for (int j = 0; j < 3; ++j)
				data2[i * 3 + j] = data[i];
		delete data;
		data = data2;
	}


	GLuint tex = 1;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	// set the texture wrapping parameters
	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	delete data;
	return tex;
}

// loads an image into a gl texture
GLuint ImageLoader::load_texture_blank()
{
	int w = 1, h = 1;
	unsigned char *data = new unsigned char[3 * w * h];
	for (int i = 0; i < 3 * w*h; ++i)
		data[i] = 0;

	GLuint tex = 1;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	// set the texture wrapping parameters
	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	delete data;
	return tex;
}

GLuint ImageLoader::load_texture_uniform(int r, int g, int b)
{
	int w = 1, h = 1;
	unsigned char *data = new unsigned char[3 * w * h];
	for (int i = 0; i < 3 * w*h; i += 3)
	{
		data[i] = r;
		data[i+1] = g;
		data[i+2] = b;
	}

	GLuint tex = 1;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	// set the texture wrapping parameters
	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	delete data;
	return tex;
}

alib::ImageData_T ImageLoader::get_data(const char * filename)
{
	int w, h, n;
	unsigned char *data = stbi_load(filename, &w, &h, &n, 0);
	return{data, w,h,n};
}

alib::ImageData_T ImageLoader::get_data_png(const char * filename)
{
	int w, h, n;
	unsigned char *data = stbi_load(filename, &w, &h, &n, STBI_rgb_alpha);
	return{ data, w,h,n };
}

alib::ImageData_T * ImageLoader::createImageData_T(unsigned char * data, int w, int h, int n, alib::ImageData_T * imageData_t)
{
	if (imageData_t == nullptr)
		imageData_t = new alib::ImageData_T();
	imageData_t->data = data;
	imageData_t->w = w;
	imageData_t->h = h;
	imageData_t->n = n;
	return imageData_t;
}
