// Project Includes.
#include "Shader.h"
#include "ShaderTypeInformation.h"

// std Includes.
#include <fstream>
#include <iostream>

namespace Framework
{
	Shader::Shader( const char* vertex_shader_file_path, const char* fragment_shader_file_path, const char* name )
		:
		name( name )
	{
		std::string vertex_shader_source = ReadShaderFromFile( vertex_shader_file_path, "VERTEX" );
		const auto  vertex_shader_id     = CompileShader( vertex_shader_source.c_str(), "VERTEX", GL_VERTEX_SHADER );

		std::string fragment_shader_source = ReadShaderFromFile( fragment_shader_file_path, "FRAGMENT" );
		const auto  fragment_shader_id     = CompileShader( fragment_shader_source.c_str(), "FRAGMENT", GL_FRAGMENT_SHADER );

		program_id = CreateProgramAndLinkShaders( vertex_shader_id, fragment_shader_id );
		
		ParseUniformData( uniform_info_map );

		GLCALL( glDeleteShader( vertex_shader_id ) );
		GLCALL( glDeleteShader( fragment_shader_id ) );
	}

	Shader::~Shader()
	{
		GLCALL( glDeleteProgram( program_id ) );
	}

	void Shader::Bind() const
	{
		GLCALL( glUseProgram( program_id ) );
	}

	void Shader::SetFloat( const std::string& uniform_name, const float value )
	{
		const auto& uniform_info = GetUniformInformation( uniform_name );
		ASSERT( uniform_info.type == GL_FLOAT );

		GLCALL( glUniform1f( uniform_info.location, value ) );
	}

	void Shader::SetInt( const std::string& uniform_name, const int value )
	{
		const auto& uniform_info = GetUniformInformation( uniform_name );
		ASSERT( uniform_info.type == GL_INT );

		GLCALL( glUniform1i( uniform_info.location, value ) );
	}

	void Shader::SetBool( const std::string& uniform_name, const bool value )
	{
		const auto& uniform_info = GetUniformInformation( uniform_name );
		ASSERT( uniform_info.type == GL_BOOL );

		GLCALL( glUniform1i( uniform_info.location, ( int )value ) );
	}

	void Shader::SetColor( const std::string& uniform_name, const Color3& value )
	{
		SetVector< 3 >( uniform_name, static_cast< const Vector3& >( value ) );
	}

	void Shader::SetColor( const std::string& uniform_name, const Color4& value )
	{
		SetVector< 4 >( uniform_name, static_cast< const Vector4& >( value ) );
	}

	const ShaderUniformInformation& Shader::GetUniformInformation( const std::string& uniform_name )
	{
	#ifdef _DEBUG
		try
		{
			return uniform_info_map.at( uniform_name );
		}
		catch( const std::exception& )
		{
			throw std::runtime_error( R"(ERROR::SHADER::UNIFORM::")" + std::string( uniform_name ) + R"("::DOES_NOT_EXIST)" );
		}
	#else
		return uniform_info_map[ uniform_name ];
	#endif // DEBUG
	}

	std::string Shader::ReadShaderFromFile( const char* file_path, const char* shader_type_string )
	{
		std::ifstream file;
		file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
		try
		{
			file.open( file_path );
			return std::string( ( std::istreambuf_iterator< char >( file ) ),
								( std::istreambuf_iterator< char >() ) );

		}
		catch( const std::ifstream::failure& e )
		{
			std::cout << "ERROR::SHADER::" << shader_type_string << "::FILE_NOT_SUCCESSFULLY_READ:\n\t" << e.what() << std::endl;
			std::terminate();
		}
	}

	unsigned int Shader::CompileShader( const char* shader_source, const char* shader_type_string, const GLenum shader_type )
	{
		GLClearError();
		const auto shader_id = glCreateShader( shader_type );
		ASSERT( GLLogCall( "glCreateShader", __FILE__, __LINE__ ) );

		GLCALL( glShaderSource( shader_id, 1, &shader_source, NULL ) );
		GLCALL( glCompileShader( shader_id ) );

		int success;
		static char infoLog[ 512 ];
		GLCALL( glGetShaderiv( shader_id, GL_COMPILE_STATUS, &success ) );

		if( !success )
		{
			GLCALL( glGetShaderInfoLog( shader_id, 512, NULL, infoLog ) );
			std::cout << "ERROR::SHADER::" << shader_type_string << "::COMPILATION_FAILED:\n\t" << infoLog << std::endl;
			GLCALL( glDeleteShader( shader_id ) );
			std::terminate();
		}

		return shader_id;
	}

	unsigned int Shader::CreateProgramAndLinkShaders( const unsigned int vertex_shader_id, const unsigned int fragment_shader_id )
	{
		GLClearError();
		const auto program_id = glCreateProgram();
		ASSERT( GLLogCall( "glCreateProgram", __FILE__, __LINE__ ) );

		GLCALL( glAttachShader( program_id, vertex_shader_id ) );
		GLCALL( glAttachShader( program_id, fragment_shader_id ) );
		GLCALL( glLinkProgram( program_id ) );

		int  success;
		static char infoLog[ 512 ];

		GLCALL( glGetProgramiv( program_id, GL_LINK_STATUS, &success ) );
		if( !success ) {
			GLCALL( glGetProgramInfoLog( program_id, 512, NULL, infoLog ) );
			std::cout << "ERROR::SHADER::LINKING_FAILED:\n\t" << infoLog << std::endl;
			std::terminate();
		}

		return program_id;
	}

	void Shader::ParseUniformData( std::unordered_map< std::string, ShaderUniformInformation >& uniform_information_map )
	{
		int active_uniform_count = 0;
		GLCALL( glGetProgramiv( program_id, GL_ACTIVE_UNIFORMS, &active_uniform_count ) );

		if( active_uniform_count == 0 )
			return;

		int uniform_name_max_length = 0;
		GLCALL( glGetProgramiv( program_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_name_max_length ) );
		std::string name( uniform_name_max_length, '?' );

		int offset = 0;
		for( int uniform_index = 0; uniform_index < active_uniform_count; uniform_index++ )
		{
			/*
			 * glGetActiveUniform has a parameter named "size", but its actually the size of the array. So for singular types like int, float, vec2, vec3 etc. the value returned is 1.
			 */
			int array_size_dontCare = 0, length = 0;
			GLenum type;
			glGetActiveUniform( program_id, uniform_index, uniform_name_max_length, &length, &array_size_dontCare, &type, name.data() );
			GLCALL( glGetActiveUniform( program_id, uniform_index, uniform_name_max_length, &length, &array_size_dontCare, &type, name.data() ) );

			const int size = GetSizeOfType( type );

			GLClearError();
			uniform_information_map[ name.data() ] = { glGetUniformLocation( program_id, name.data() ), size, offset, type };
			ASSERT( GLLogCall( "glGetUniformLocation", __FILE__, __LINE__ ) );

			offset += size;
		}
	}
}
