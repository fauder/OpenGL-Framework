#pragma once

// Vendor GLAD Includes (Has to be included before GLFW!).
#include <Vendor/OpenGL/glad/glad.h>

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
