// Project Includes.
#include "Texture.h"

// stb Includes.
#include "Vendor/stb_image.h"

namespace Framework
{
	/* Static member variable initializations. */
	const Framework::Color Texture::DEFAULT_BORDER_COLOR{ 1.0f, 1.0f, 0.0f, 1.0f };

	Texture::Texture( const char* file_path, GLenum color_format )
		:
		id( 0 )
	{
		stbi_set_flip_vertically_on_load( 1 );

		local_buffer = stbi_load( file_path, &width, &height, &channels_in_file, 0 );
		if( local_buffer )
		{
			GLCALL( glGenTextures( 1, &id ) );
			Bind();

			GLCALL( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER ) );
			GLCALL( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER ) );
			GLCALL( glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, DEFAULT_BORDER_COLOR.Data() ) );

			GLCALL( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR ) );
			GLCALL( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );

			GLCALL( glTexImage2D( GL_TEXTURE_2D, 0, color_format, width, height, 0, color_format, GL_UNSIGNED_BYTE, local_buffer ) );
			GLCALL( glGenerateMipmap( GL_TEXTURE_2D ) );
		}
		else
			std::cerr << "ERROR::TEXTURE::COULD_NOT_READ_FROM_FILE!\n";

		stbi_image_free( local_buffer );
	}

	Texture::~Texture()
	{
		GLCALL( glDeleteTextures( 1, &id ) );
	}

	void Texture::Bind() const
	{
		GLCALL( glBindTexture( GL_TEXTURE_2D, id ) );
	}

	void Texture::Unbind() const
	{
		GLCALL( glBindTexture( GL_TEXTURE_2D, 0 ) );
	}

	void Texture::ActivateAndBind( GLenum texture_unit ) const
	{
		GLCALL( glActiveTexture( texture_unit ) );
		Bind();
	}
}
