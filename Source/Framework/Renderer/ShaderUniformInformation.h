#pragma once

// Vendor GLAD Includes (Has to be included before GLFW!).
#include "Vendor/OpenGL/glad/glad.h"

namespace Framework
{
	struct ShaderUniformInformation
	{
		int location;
		int size;
		int offset;
		GLenum type;
	};
}
