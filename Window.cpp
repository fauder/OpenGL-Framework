// Project Includes.
#include "Window.h"

// std Includes.
#include <iostream>

void Window::Framebuffer_Size_Callback( GLFWwindow* window, int width_new, int height_new )
{
	glViewport( 0, 0, width_new, height_new );
}

void Window::InitializeGLFWAndCreateWindow( GLFWwindow*& window )
{
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	//glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE ); // Needed for Mac OS X.

	window = glfwCreateWindow( 800, 600, "LearnOpenGL", NULL, NULL );
	if( window == nullptr )
	{
		glfwTerminate();
		throw std::logic_error( "ERROR::WINDOW::GLFW::FAILED TO CREATE GLFW WINDOW!" );
	}

	glfwMakeContextCurrent( window );
}