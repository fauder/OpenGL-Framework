// GLAD Includes.
#include <glad/glad.h>

// Project Includes.
#include "Platform.h"

// GLFW Includes.
#include <GLFW/glfw3.h>

// ImGui Includes.
#include "Vendor/imgui.h"

// std Includes.
#include <stdexcept>

namespace Framework::Platform
{
	GLFWwindow* WINDOW = nullptr; // No need to expose this outside.
	float MOUSE_CURSOR_X_POS = 0.0f, MOUSE_CURSOR_Y_POS = 0.0f;
	float MOUSE_CURSOR_X_DELTA = 0.0f, MOUSE_CURSOR_Y_DELTA = 0.0f;
	float MOUSE_SENSITIVITY = 0.004f;
	bool MOUSE_CAPTURE_IS_RESET = true;
	bool MOUSE_CAPTURE_ENABLED = false;

	void OnResize( GLFWwindow* window, const int width_new_pixels, const int height_new_pixels )
	{
		glViewport( 0, 0, width_new_pixels, height_new_pixels );
	}

	void OnMouseCursorPositionChanged( GLFWwindow* window, const double x_position, const double y_position )
	{
		if( ImGui::GetIO().WantCaptureMouse )
			return;

		if( MOUSE_CAPTURE_IS_RESET )
		{
			MOUSE_CURSOR_X_POS = ( float )x_position;
			MOUSE_CURSOR_Y_POS = ( float )y_position;
			MOUSE_CAPTURE_IS_RESET = false;
		}

		MOUSE_CURSOR_X_DELTA = MOUSE_SENSITIVITY * ( ( float )x_position - MOUSE_CURSOR_X_POS );
		MOUSE_CURSOR_Y_DELTA = MOUSE_SENSITIVITY * ( ( float )y_position - MOUSE_CURSOR_Y_POS );

		MOUSE_CURSOR_X_POS = ( float )x_position;
		MOUSE_CURSOR_Y_POS = ( float )y_position;
	}

	/* GLAD needs the created window's context made current BEFORE it is initialized. */
	void InitializeGLAD()
	{
		if( !gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress ) )
			throw std::logic_error( "ERROR::GRAPHICS::GLAD::FAILED_TO_INITIALIZE!" );
	}

	void RegisterOnResizeCallback()
	{
		glfwSetFramebufferSizeCallback( WINDOW, OnResize );
	}

	void RegisterOnMouseCallback()
	{
		glfwSetCursorPosCallback( WINDOW, OnMouseCursorPositionChanged );
	}

	void InitializeAndCreateWindow( const int width_pixels, const int height_pixels, const int pos_x_pixels, const int pos_y_pixels )
	{
		glfwInit();
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

		//glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE ); // Needed for Mac OS X.

		glfwWindowHint( GLFW_VISIBLE, GLFW_FALSE ); // Start hidden as we will move it shortly.
		WINDOW = glfwCreateWindow( width_pixels, height_pixels, "OpenGL Framework", nullptr, nullptr );
		if( WINDOW == nullptr )
		{
			glfwTerminate();
			throw std::logic_error( "ERROR::PLATFORM::GLFW::FAILED TO CREATE GLFW WINDOW!" );
		}

		glfwSetWindowPos( WINDOW, pos_x_pixels, pos_y_pixels );
		glfwShowWindow( WINDOW );

		glfwMakeContextCurrent( WINDOW );

		// GLAD needs the created window's context made current BEFORE it is initialized.
		InitializeGLAD();

		Resize( width_pixels, height_pixels );

		RegisterOnResizeCallback();
		RegisterOnMouseCallback();
	}

	void Resize( const int width_new_pixels, const int height_new_pixels )
	{
		OnResize( nullptr, width_new_pixels, height_new_pixels );
	}

	void SwapBuffers()
	{
		glfwSwapBuffers( WINDOW );
	}

	void PollEvents()
	{
		MOUSE_CURSOR_X_DELTA = MOUSE_CURSOR_Y_DELTA = 0.0f;
		glfwPollEvents();
	}

	bool IsKeyPressed( const KeyCode key_code )
	{
		if( ImGui::GetIO().WantCaptureKeyboard )
			return false;

		return glfwGetKey( WINDOW, int( key_code ) ) == GLFW_PRESS;
	}

	bool IsKeyReleased( const KeyCode key_code )
	{
		if( ImGui::GetIO().WantCaptureKeyboard )
			return false;

		return glfwGetKey( WINDOW, int( key_code ) ) == GLFW_RELEASE;
	}

	void CaptureMouse( const bool should_capture )
	{
		if( !MOUSE_CAPTURE_ENABLED && should_capture )
			glfwSetInputMode( WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
		else if( MOUSE_CAPTURE_ENABLED && !should_capture )
		{
			MOUSE_CAPTURE_IS_RESET = true;
			glfwSetInputMode( WINDOW, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
		}

		MOUSE_CAPTURE_ENABLED = should_capture;
	}

	float GetMouseSensitivity()
	{
		return MOUSE_SENSITIVITY;
	}

	void SetMouseSensitivity( const float new_sensitivity )
	{
		MOUSE_SENSITIVITY = new_sensitivity;
	}

	std::pair< float, float > GetMouseCursorDeltas()
	{
		return { MOUSE_CURSOR_X_DELTA, MOUSE_CURSOR_Y_DELTA };
	}

	std::pair< float, float > GetMouseCursorPositions()
	{
		return { MOUSE_CURSOR_X_POS, MOUSE_CURSOR_Y_POS };
	}

	float GetCurrentTime()
	{
		return static_cast< float >( glfwGetTime() );
	}

	void SetShouldClose( const bool value )
	{
		glfwSetWindowShouldClose( WINDOW, value );
	}

	bool ShouldClose()
	{
		return glfwWindowShouldClose( WINDOW );
	}

	void ChangeTitle( const char* new_title )
	{
		glfwSetWindowTitle( WINDOW, new_title );
	}

	void CleanUp()
	{
		glfwTerminate();
	}

	void* GetWindowHandle()
	{
		return reinterpret_cast< void* >( WINDOW );
	}
}