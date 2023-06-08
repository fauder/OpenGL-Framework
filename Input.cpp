// Project Includes.
#include "Input.h"

void Input::Process( GLFWwindow* window )
{
	if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
		glfwSetWindowShouldClose( window, true );
}