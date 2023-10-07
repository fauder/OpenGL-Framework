// Vendor GLAD Includes (Has to be included before GLFW!).
#include "Vendor/OpenGL/glad/glad.h"

// Framework Includes.
#include "Core/Platform.h"

// Vendor Includes.
#include "Vendor/imgui/imgui_impl_glfw.h"

#include <Vendor/OpenGL/GLFW/glfw3.h> 

// std Includes.
#include <stdexcept>

namespace Framework::Platform
{
	GLFWwindow* WINDOW = nullptr; // No need to expose this outside.
	float MOUSE_CURSOR_X_POS = 0.0f, MOUSE_CURSOR_Y_POS = 0.0f;
	float MOUSE_CURSOR_X_DELTA = 0.0f, MOUSE_CURSOR_Y_DELTA = 0.0f;
	float MOUSE_SCROLL_X_OFFSET = 0.0f, MOUSE_SCROLL_Y_OFFSET = 0.0f;
	float MOUSE_SENSITIVITY = 0.004f;
	bool MOUSE_CAPTURE_IS_RESET = true;
	bool MOUSE_CAPTURE_ENABLED = false;
	std::function< void( const KeyCode key_code, const KeyAction action, const KeyMods mods ) > KEYBOARD_CALLBACK;

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

	void OnMouseScrolled( GLFWwindow* window, const double x_offset, const double y_offset )
	{
		if( ImGui::GetIO().WantCaptureMouse )
			return;

		MOUSE_SCROLL_X_OFFSET = ( float )x_offset;
		MOUSE_SCROLL_Y_OFFSET = ( float )y_offset;
	}

	void OnKeyboardEvent( GLFWwindow* window, const int key_code, const int scan_code, const int action, const int mods )
	{
		if( ImGui::GetIO().WantCaptureKeyboard )
			return;

		if( KEYBOARD_CALLBACK )
			KEYBOARD_CALLBACK( KeyCode( key_code ), KeyAction( action ), KeyMods( mods ) );
	}

	/* GLAD needs the created window's context made current BEFORE it is initialized. */
	void InitializeGLAD()
	{
		if( !gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress ) )
			throw std::runtime_error( "ERROR::GRAPHICS::GLAD::FAILED_TO_INITIALIZE!" );
	}

	void RegisterFrameBufferResizeCallback()
	{
		glfwSetFramebufferSizeCallback( WINDOW, OnResize );
	}

	void RegisterMousePositionChangeCallback()
	{
		glfwSetCursorPosCallback( WINDOW, OnMouseCursorPositionChanged );
	}

	void RegisterMouseScrollCallback()
	{
		glfwSetScrollCallback( WINDOW, OnMouseScrolled );
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
			throw std::runtime_error( "ERROR::PLATFORM::GLFW::FAILED TO CREATE GLFW WINDOW!" );
		}

		glfwSetWindowPos( WINDOW, pos_x_pixels, pos_y_pixels );
		glfwShowWindow( WINDOW );

		glfwMakeContextCurrent( WINDOW );

		// GLAD needs the created window's context made current BEFORE it is initialized.
		InitializeGLAD();

		Resize( width_pixels, height_pixels );

		RegisterFrameBufferResizeCallback();
		RegisterMousePositionChangeCallback();
		RegisterMouseScrollCallback();
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
		MOUSE_SCROLL_X_OFFSET = MOUSE_SCROLL_Y_OFFSET = 0.0f;
		glfwPollEvents();
	}

	void SetKeyboardEventCallback( std::function< void( const KeyCode key_code, const KeyAction action, const KeyMods mods ) > callback )
	{
		KEYBOARD_CALLBACK = callback;

		ImGui_ImplGlfw_RestoreCallbacks( WINDOW );
		glfwSetKeyCallback( WINDOW, OnKeyboardEvent );
		ImGui_ImplGlfw_InstallCallbacks( WINDOW );
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

	std::pair< float, float > GetMouseScrollOffsets()
	{
		return { MOUSE_SCROLL_X_OFFSET, MOUSE_SCROLL_Y_OFFSET };
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
		return static_cast< void* >( WINDOW );
	}
}