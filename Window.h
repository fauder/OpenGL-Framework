#pragma once

// GLFW Includes.
#include <GLFW/glfw3.h>

namespace Window
{
	void Framebuffer_Size_Callback( GLFWwindow* window, const int width_new, const int height_new );
	void InitializeGLFWAndCreateWindow( GLFWwindow*& window, const int width = 800, const int height = 600, const int pos_x = 0, const int pos_y = 0 );
}