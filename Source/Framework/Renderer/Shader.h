#pragma once

// Vendor GLAD Includes (Has to be included before GLFW!).
#include "Vendor/OpenGL/glad/glad.h"

// Framework Includes.
#include "Math/Color.hpp"
#include "Math/Concepts.h"
#include "Math/Matrix.hpp"
#include "Math/Vector.hpp"

#include "Renderer/Graphics.h"
#include "Renderer/ShaderUniformInformation.h"

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

		template< Concepts::Arithmetic Coordinate, std::size_t Size >
			requires Concepts::NonZero< Size >
		void SetVector( const std::string& uniform_name, const Math::Vector< Coordinate, Size >& value )
		{
			const auto& uniform_info = GetUniformInformation( uniform_name );

			if constexpr( std::is_same_v< Coordinate, float > )
			{
				if constexpr( Size == 2 )
				{
					ASSERT_DEBUG_ONLY( uniform_info.type == GL_FLOAT_VEC2 );

					GLCALL( glUniform2fv( uniform_info.location, 1, value.Data() ) );
				}
				if constexpr( Size == 3 )
				{
					ASSERT_DEBUG_ONLY( uniform_info.type == GL_FLOAT_VEC3 );

					GLCALL( glUniform3fv( uniform_info.location, 1, value.Data() ) );
				}
				if constexpr( Size == 4 )
				{
					ASSERT_DEBUG_ONLY( uniform_info.type == GL_FLOAT_VEC4 );

					GLCALL( glUniform4fv( uniform_info.location, 1, value.Data() ) );
				}
			}

			if constexpr( std::is_same_v< Coordinate, int > )
			{
				if constexpr( Size == 2 )
				{
					ASSERT_DEBUG_ONLY( uniform_info.type == GL_INT_VEC2 );

					GLCALL( glUniform2iv( uniform_info.location, 1, value.Data() ) );
				}
				if constexpr( Size == 3 )
				{
					ASSERT_DEBUG_ONLY( uniform_info.type == GL_INT_VEC3 );

					GLCALL( glUniform3iv( uniform_info.location, 1, value.Data() ) );
				}
				if constexpr( Size == 4 )
				{
					ASSERT_DEBUG_ONLY( uniform_info.type == GL_INT_VEC4 );

					GLCALL( glUniform4iv( uniform_info.location, 1, value.Data() ) );
				}
			}

			if constexpr( std::is_same_v< Coordinate, unsigned int > )
			{
				if constexpr( Size == 2 )
				{
					ASSERT_DEBUG_ONLY( uniform_info.type == GL_UNSIGNED_INT_VEC2 );

					GLCALL( glUniform2uiv( uniform_info.location, 1, value.Data() ) );
				}
				if constexpr( Size == 3 )
				{
					ASSERT_DEBUG_ONLY( uniform_info.type == GL_UNSIGNED_INT_VEC3 );

					GLCALL( glUniform3uiv( uniform_info.location, 1, value.Data() ) );
				}
				if constexpr( Size == 4 )
				{
					ASSERT_DEBUG_ONLY( uniform_info.type == GL_UNSIGNED_INT_VEC4 );

					GLCALL( glUniform4uiv( uniform_info.location, 1, value.Data() ) );
				}
			}

			if constexpr( std::is_same_v< Coordinate, bool > )
			{
				if constexpr( Size == 2 )
				{
					ASSERT_DEBUG_ONLY( uniform_info.type == GL_BOOL_VEC2 );

					GLCALL( glUniform2i( uniform_info.location, value.X(), value.Y() ) );
				}
				if constexpr( Size == 3 )
				{
					ASSERT_DEBUG_ONLY( uniform_info.type == GL_BOOL_VEC3 );

					GLCALL( glUniform3i( uniform_info.location, value.X(), value.Y(), value.Z() ) );
				}
				if constexpr( Size == 4 )
				{
					ASSERT_DEBUG_ONLY( uniform_info.type == GL_BOOL_VEC4 );

					GLCALL( glUniform4i( uniform_info.location, value.X(), value.Y(), value.Z(), value.W() ) );
				}
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
