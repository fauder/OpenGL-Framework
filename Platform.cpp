// GLAD Includes.
#include <glad/glad.h>

// Project Includes.
#include "Platform.h"

// GLFW Includes.
#include <GLFW/glfw3.h>

// std Includes.
#include <stdexcept>

GLFWwindow* window; // No need to expose this outside.

namespace Framework::Platform
{
	void OnResize( GLFWwindow* window, const int width_new_pixels, const int height_new_pixels )
	{
		glViewport( 0, 0, width_new_pixels, height_new_pixels );
	}

	/* GLAD needs the created window's context made current BEFORE it is initialized. */
	void InitializeGLAD()
	{
		if( !gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress ) )
			throw std::logic_error( "ERROR::GRAPHICS::GLAD::FAILED_TO_INITIALIZE!" );
	}

	void InitializeAndCreateWindow( const int width_pixels, const int height_pixels, const int pos_x_pixels, const int pos_y_pixels )
	{
		glfwInit();
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

		//glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE ); // Needed for Mac OS X.

		glfwWindowHint( GLFW_VISIBLE, GLFW_FALSE ); // Start hidden as we will move it shortly.
		window = glfwCreateWindow( width_pixels, height_pixels, "OpenGL Framework", nullptr, nullptr );
		if( window == nullptr )
		{
			glfwTerminate();
			throw std::logic_error( "ERROR::PLATFORM::GLFW::FAILED TO CREATE GLFW WINDOW!" );
		}

		glfwSetWindowPos( window, pos_x_pixels, pos_y_pixels );
		glfwShowWindow( window );

		glfwMakeContextCurrent( window );

		// GLAD needs the created window's context made current BEFORE it is initialized.
		InitializeGLAD();

		Resize( width_pixels, height_pixels );
		RegisterOnResizeCallback();
	}

	void Resize( const int width_new_pixels, const int height_new_pixels )
	{
		OnResize( nullptr, width_new_pixels, height_new_pixels );
	}

	void RegisterOnResizeCallback()
	{
		glfwSetFramebufferSizeCallback( window, OnResize );
	}

	void SwapBuffers()
	{
		glfwSwapBuffers( window );
	}

	void PollEvents()
	{
		glfwPollEvents();
	}

	bool IsKeyPressed( const KeyCode key_code )
	{
		return glfwGetKey( window, int( key_code ) ) == GLFW_PRESS;
	}

	bool IsKeyReleased( const KeyCode key_code )
	{
		return glfwGetKey( window, int( key_code ) ) == GLFW_RELEASE;
	}

	float GetCurrentTime()
	{
		return static_cast< float >( glfwGetTime() );
	}

	void SetShouldClose( const bool value )
	{
		glfwSetWindowShouldClose( window, value );
	}

	bool ShouldClose()
	{
		return glfwWindowShouldClose( window );
	}

	void ChangeTitle( const char* new_title )
	{
		glfwSetWindowTitle( window, new_title );
	}

	void CleanUp()
	{
		glfwTerminate();
	}

	void* GetWindowHandle()
	{
		return reinterpret_cast< void* >( window );
	}
}