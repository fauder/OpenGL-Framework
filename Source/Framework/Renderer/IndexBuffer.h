#pragma once

// GLAD Includes.
#include <glad/glad.h> // TODO: Move this from under C:\Sys or whatever it is in, into the project and get rid of the reference to this external directory.

namespace Framework
{
	class IndexBuffer
	{
	public:
		IndexBuffer( const void* data, const unsigned int count );
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned int Count() const { return count; }

	private:
		GLuint index_buffer_object_id;
		unsigned int count;
	};
}
