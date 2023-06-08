#pragma once

// GLFW Includes.
#include <GLFW/glfw3.h>

namespace Window
{
	void Framebuffer_Size_Callback( GLFWwindow* window, int width_new, int height_new );
	void InitializeGLFWAndCreateWindow( GLFWwindow*& window );
}