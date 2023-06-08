#pragma once

// Project Includes.
#include "Graphics.h"

class Texture
{
public:
	Texture( const char* file_path, GLenum color_format );
	~Texture();
private:
	unsigned int id;
	int width;
	int height;
	int channels_in_file;
	unsigned char* local_buffer;
};
