#pragma once

// GLAD Includes.
#include <glad/glad.h> // TODO: Move this from under C:\Sys or whatever it is in, into the project and get rid of the reference to this external directory.

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
