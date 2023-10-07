#pragma once

// Framework Includes.
#include "Math/Color.hpp"

#include "Renderer/Graphics.h"

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
		static constexpr Color4 DefaultBorderColor() { return Color4{ 1.0f, 1.0f, 0.0f, 1.0f }; }

	private:
		unsigned int id;
		int width;
		int height;
		int channels_in_file;
	};
}
