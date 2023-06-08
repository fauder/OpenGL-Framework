// Project Includes.
#include "Texture.h"

// stb Includes.
#include "Vendor/stb_image.h"

Texture::Texture( const char* file_path, GLenum color_format )
	: id( 0 )
{
	 local_buffer = stbi_load( file_path, &width, &height, &channels_in_file, 0 );
	 if( local_buffer )
	 {
		 // Create OpenGL stuff.
		 std::cerr << "Successfully read from the file.\n";
	 }
	 else
		 std::cerr << "ERROR::TEXTURE::COULD_NOT_READ_FROM_FILE!\n";
}

Texture::~Texture()
{
}