// Project Includes.
#include "Shader.h"

// std Includes.
#include <fstream>
#include <iostream>

Shader::Shader( const char* vertex_shader_file_name, const char* fragment_shader_file_name )
{
	std::string vertex_shader_code = ReadShaderFromFile( vertex_shader_file_name, "VERTEX" );
	const auto vertex_shader_id    = CompileShader( vertex_shader_code.c_str(), "VERTEX", GL_VERTEX_SHADER );

	std::string fragment_shader_code = ReadShaderFromFile( fragment_shader_file_name, "FRAGMENT" );
	const auto fragment_shader_id    = CompileShader( fragment_shader_code.c_str(), "FRAGMENT", GL_FRAGMENT_SHADER );

	program_id = CreateProgramAndLinkShaders( vertex_shader_id, fragment_shader_id );

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

void Shader::SetFloat( const char* name, const float value )
{
	GLCALL( glUniform1f( GetUniformLocation( name ), value ) );
}

void Shader::SetInt( const char* name, const int value )
{
	GLCALL( glUniform1i( GetUniformLocation( name ), value ) );
}

void Shader::SetBool( const char* name, const bool value )
{
	GLCALL( glUniform1i( GetUniformLocation( name ), ( int )value ) );
}

void Shader::SetColor( const char* name, const Color& value )
{
	GLCALL( glUniform4fv( GetUniformLocation( name ), 1, value.Data() ) );
}

int Shader::GetUniformLocation( const char* name )
{
	if( layout_location_map.count( name ) == 1 )
		return layout_location_map[ name ];
	else
	{
		GLClearError();
		const int value = glGetUniformLocation( program_id, name );
		GLLogCall( "glGetUniformLocation", __FILE__, __LINE__ );
		layout_location_map[ name ] = value;
		return value;
	}
}

std::string Shader::ReadShaderFromFile( const char* fileName, const char* shader_type_string )
{
	std::ifstream file;
	file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
	try
	{
		file.open( fileName );
		return std::string( ( std::istreambuf_iterator< char >( file ) ),
							( std::istreambuf_iterator< char >() ) );

	}
	catch( const std::ifstream::failure& e )
	{
		std::cout << "ERROR::SHADER::" << shader_type_string << "::FILE_NOT_SUCCESSFULLY_READ:\n\t" << e.what() << std::endl;
		std::terminate();
	}
}

unsigned int Shader::CompileShader( const char* shader_code, const char* shader_type_string, const GLenum shader_type )
{
	GLClearError();
	const auto shader_id = glCreateShader( shader_type );
	ASSERT( GLLogCall( "glCreateShader", __FILE__, __LINE__ ) );

	GLCALL( glShaderSource( shader_id, 1, &shader_code, NULL ) );
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