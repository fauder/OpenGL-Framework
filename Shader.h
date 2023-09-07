#pragma once

// GLAD Includes.
#include <glad/glad.h>

// Project Includes.
#include "Color.hpp"
#include "Concepts.h"
#include "Graphics.h"
#include "Matrix.hpp"
#include "Vector.hpp"

// std Includes.
#include <string>
#include <unordered_map>

namespace Framework
{
	class Shader
	{
	public:
		Shader( const char* vertex_shader_file_path, const char* fragment_shader_file_path );
		~Shader();

		void Bind() const;

		/* Only accept square matrices for now. If there a use-case for non-square matrices come up, I'll refactor. */
		template< unsigned int Size > requires Concepts::NonZero< Size >
		void SetMatrix( const char* name, const Math::Matrix< float, Size, Size >& value )
		{
			if constexpr( Size == 2U )
			{
				GLCALL( glUniformMatrix2fv( GetUniformLocation( name ), 1, GL_TRUE, value.Data() ) );
			}
			if constexpr( Size == 3U )
			{
				GLCALL( glUniformMatrix3fv( GetUniformLocation( name ), 1, GL_TRUE, value.Data() ) );
			}
			if constexpr( Size == 4U )
			{
				GLCALL( glUniformMatrix4fv( GetUniformLocation( name ), 1, GL_TRUE, value.Data() ) );
			}
		}
		void SetFloat( const char* name, const float value );
		void SetInt( const char* name, const int value );
		void SetBool( const char* name, const bool value );
		template< unsigned int Size > requires Concepts::NonZero< Size >
		void SetVector( const char* name, const Math::Vector< float, Size >& value )
		{
			if constexpr( Size == 2 )
			{
				GLCALL( glUniform2fv( GetUniformLocation( name ), Size, value.Data() ) );
			}
			if constexpr( Size == 3 )
			{
				GLCALL( glUniform3fv( GetUniformLocation( name ), Size, value.Data() ) );
			}
			if constexpr( Size == 4 )
			{
				GLCALL( glUniform4fv( GetUniformLocation( name ), Size, value.Data() ) );
			}
		}
		void SetColor( const char* name, const Color4& value );

		inline const std::unordered_map< std::string, int >& GetUniformInformation() const
		{
			return uniform_location_map;
		}

	private:
		int GetUniformLocation( const char* name );
		static std::string ReadShaderFromFile( const char* file_path, const char* shader_type_string );
		static unsigned int CompileShader( const char* shader_source, const char* shader_type_string, const GLenum shader_type );
		static unsigned int CreateProgramAndLinkShaders( const unsigned int vertex_shader_id, const unsigned int fragment_shader_id );
		void ParseUniformData( std::unordered_map< std::string, int >& uniform_location_map );

	private:
		GLuint program_id;
		std::unordered_map< std::string, int > uniform_location_map;
	};
}
