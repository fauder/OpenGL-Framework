// Project Includes.
#include "Input.h"

namespace Framework::Input
{
	void Process( GLFWwindow* window )
	{
		if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
			glfwSetWindowShouldClose( window, true );
	}
}
