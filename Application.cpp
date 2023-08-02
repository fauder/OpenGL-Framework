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
#include "Test/Test_ImGui.h"

// std Includes.
#include <vector>
#include <map>

int main()
{
	using namespace Framework::Test;

	// TODO: Switch to keeping lambdas inside the map, so the Tests are not "built" (e.q., constructed) initially.

	std::map< std::string, std::unique_ptr< TestInterface > > tests_by_name;

	//tests_by_name[ "clear_color" ] = std::make_unique< Test_ClearColor >( nullptr, Framework::Color4::Yellow() );
	//tests_by_name[ "transform_2_cubes" ] = std::make_unique< Test_Transfom_2Cubes >();
	tests_by_name[ "imgui" ] = std::make_unique< Test_ImGui >();

	const auto& current_test = tests_by_name[ "imgui" ];

	current_test->Run();

	return 0;
}
