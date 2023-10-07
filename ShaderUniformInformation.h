#pragma once

// GLAD Includes.
#include <glad/glad.h>

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
