#pragma once

// Project Includes.
#include "Color.hpp"
#include "Graphics.h"

namespace Framework
{
	class Texture
	{
	public:
		Texture( const char* file_path, GLenum color_format );
		~Texture();

		void Bind() const;
		void Unbind() const;
		void ActivateAndBind( GLenum texture_unit ) const;

		inline int Width() const { return width; }
		inline int Height() const { return height; }
		inline int ChannelsInFile() const { return channels_in_file; }

	public:
		static const Color DEFAULT_BORDER_COLOR;

	private:
		unsigned int id;
		int width;
		int height;
		int channels_in_file;
		unsigned char* local_buffer;
	};
}
