#pragma once

// GLAD Includes.
#include <glad/glad.h>

// Project Includes.
#include "Color.hpp"
#include "Concepts.h"
#include "Graphics.h"
#include "Matrix.hpp"
#include "ShaderUniformInformation.h"
#include "Vector.hpp"

// std Includes.
#include <string>
#include <unordered_map>

namespace Framework
{
	class Shader
	{
	public:
		Shader( const char* vertex_shader_file_path, const char* fragment_shader_file_path, const char* name );
		~Shader();

		void Bind() const;

		void SetFloat( const std::string& uniform_name, const float value );
		void SetInt( const std::string& uniform_name, const int value );
		void SetBool( const std::string& uniform_name, const bool value );

		template< unsigned int Size > requires Concepts::NonZero< Size >
		void SetVector( const std::string& uniform_name, const Math::Vector< float, Size >& value )
		{
			const auto& uniform_info = GetUniformInformation( uniform_name );

			if constexpr( Size == 2 )
			{
				ASSERT( uniform_info.type == GL_FLOAT_VEC2 );

				GLCALL( glUniform2fv( uniform_info.location, 1, value.Data() ) );
			}
			if constexpr( Size == 3 )
			{
				ASSERT( uniform_info.type == GL_FLOAT_VEC3 );

				GLCALL( glUniform3fv( uniform_info.location, 1, value.Data() ) );
			}
			if constexpr( Size == 4 )
			{
				ASSERT( uniform_info.type == GL_FLOAT_VEC4 );

				GLCALL( glUniform4fv( uniform_info.location, 1, value.Data() ) );
			}
		}

		void SetColor( const std::string& uniform_name, const Color3& value );
		void SetColor( const std::string& uniform_name, const Color4& value );

		/* Only accept square matrices for now. If there a use-case for non-square matrices come up, I'll refactor. */
		template< unsigned int Size > requires Concepts::NonZero< Size >
		void SetMatrix( const std::string& uniform_name, const Math::Matrix< float, Size, Size >& value )
		{
			if constexpr( Size == 2U )
			{
				GLCALL( glUniformMatrix2fv( GetUniformInformation( uniform_name ).location, 1, GL_TRUE, value.Data() ) );
			}
			if constexpr( Size == 3U )
			{
				GLCALL( glUniformMatrix3fv( GetUniformInformation( uniform_name ).location, 1, GL_TRUE, value.Data() ) );
			}
			if constexpr( Size == 4U )
			{
				GLCALL( glUniformMatrix4fv( GetUniformInformation( uniform_name ).location, 1, GL_TRUE, value.Data() ) );
			}
		}

		void SetTextureSampler1D( const std::string& uniform_name, const int value );
		void SetTextureSampler2D( const std::string& uniform_name, const int value );
		void SetTextureSampler3D( const std::string& uniform_name, const int value );

		inline const ShaderUniformInformation& GetUniformInformation( const std::string& uniform_name );
		inline const std::unordered_map< std::string, ShaderUniformInformation >& GetUniformInformations() const { return uniform_info_map; }

		inline const std::string& Name() const { return name; }

	private:
		static std::string ReadShaderFromFile( const char* file_path, const char* shader_type_string );
		static unsigned int CompileShader( const char* shader_source, const char* shader_type_string, const GLenum shader_type );
		static unsigned int CreateProgramAndLinkShaders( const unsigned int vertex_shader_id, const unsigned int fragment_shader_id );
		void ParseUniformData( std::unordered_map< std::string, ShaderUniformInformation >& uniform_information_map );

	private:
		std::string name;
		GLuint program_id;
		std::unordered_map< std::string, ShaderUniformInformation > uniform_info_map;
	};
}
