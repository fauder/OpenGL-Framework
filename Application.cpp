// Make sure GLAD is included before GLFW3!
#include <glad/glad.h>

// Project Includes.
#include "Drawable.h"
#include "IndexBuffer.h"
#include "Input.h"
#include "Matrix.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Vector.hpp"
#include "VertexArray.h"

#include "Test/Test_ClearColor.h"
#include "Test/Test_Transform_2Cubes.h"

// std Includes.
#include <istream>
#include <vector>
#include <map>

int main()
{
	using namespace Framework;

	std::map< std::string, std::unique_ptr< TestInterface > > tests_by_name;

	tests_by_name[ "clear_color" ] = std::make_unique< Test_ClearColor >();

	const auto& current_test = tests_by_name[ "clear_color" ];

	current_test->Run();

	return 0;
}
