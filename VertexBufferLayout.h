#pragma once

// GLAD Includes.
#include <glad/glad.h>

// std Includes.
#include <vector>

// Project Includes.
#include "Graphics.h"

namespace Framework
{
	struct VertexAttribute
	{
		int count;
		GLenum type;
		GLboolean normalized;

		inline unsigned int GetSizeOfType() const { return GetSizeOfType( type ); }

		static unsigned int GetSizeOfType( GLenum type )
		{
			switch( type )
			{
				case GL_FLOAT:	return 4;
				case GL_INT:	return 4;
				case GL_BYTE:	return 1;

				default: throw std::logic_error( "VertexAttribute::GetSizeOfType: Unknown type encountered." );
					return 0;
			}
		}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout()
			:
			stride( 0 )
		{}

		inline unsigned int GetStride() const { return stride; };
		inline std::vector< VertexAttribute > GetAttributes() const { return attribute_list; }

		template< typename T >
		void Push( int count )
		{
			if constexpr( std::is_same_v< T, float > )
			{
				attribute_list.push_back( { count, GL_FLOAT, GL_FALSE } );
				stride += count * VertexAttribute::GetSizeOfType( GL_FLOAT );
			}
			if constexpr( std::is_same_v< T, int > )
			{
				attribute_list.push_back( { count, GL_INT, GL_FALSE } );
				stride += count * VertexAttribute::GetSizeOfType( GL_INT );
			}
			if constexpr( std::is_same_v< T, unsigned char > )
			{
				attribute_list.push_back( { count, GL_BYTE, GL_TRUE } );
				stride += count * VertexAttribute::GetSizeOfType( GL_BYTE );
			}
		}

	private:
		std::vector< VertexAttribute > attribute_list;
		unsigned int stride;
	};
}
