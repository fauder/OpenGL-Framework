// Project Includes.
#include "Window.h"

// std Includes.
#include <stdexcept>

namespace Framework::Window
{
	void Framebuffer_Size_Callback( GLFWwindow* window, const int width_new, const int height_new )
	{
		glViewport( 0, 0, width_new, height_new );
	}

	GLFWwindow* InitializeGLFWAndCreateWindow( const int width, const int height, const int pos_x, const int pos_y )
	{
		glfwInit();
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

		//glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE ); // Needed for Mac OS X.

		glfwWindowHint( GLFW_VISIBLE, GLFW_FALSE ); // Start hidden as we will move it shortly.
		auto* window = glfwCreateWindow( width, height, "LearnOpenGL", nullptr, nullptr );
		if( window == nullptr )
		{
			glfwTerminate();
			throw std::logic_error( "ERROR::WINDOW::GLFW::FAILED TO CREATE GLFW WINDOW!" );
		}

		glfwSetWindowPos( window, pos_x, pos_y );
		glfwShowWindow( window );

		glfwMakeContextCurrent( window );

		return window;
	}
}