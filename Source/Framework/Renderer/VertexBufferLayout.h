#pragma once

// Framework Includes.
#include "Renderer/Graphics.h"
#include "Renderer/ShaderTypeInformation.h"

// std Includes.
#include <vector>

namespace Framework
{
	struct VertexAttribute
	{
		int count;
		GLenum type;
		GLboolean normalized;

		inline unsigned int GetSizeOfType() const { return Framework::GetSizeOfType( type ); }
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
				stride += count * Framework::GetSizeOfType( GL_FLOAT );
			}
			if constexpr( std::is_same_v< T, int > )
			{
				attribute_list.push_back( { count, GL_INT, GL_FALSE } );
				stride += count * Framework::GetSizeOfType( GL_INT );
			}
			if constexpr( std::is_same_v< T, unsigned char > )
			{
				attribute_list.push_back( { count, GL_BYTE, GL_TRUE } );
				stride += count * Framework::GetSizeOfType( GL_BYTE );
			}
		}

	private:
		std::vector< VertexAttribute > attribute_list;
		unsigned int stride;
	};
}
