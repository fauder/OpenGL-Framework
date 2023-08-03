// Make sure GLAD is included before GLFW3!
#include <glad/glad.h>

// Project Includes.
#include "Drawable.h"
#include "IndexBuffer.h"
#include "ImGuiSetup.h"
#include "Input.h"
#include "Matrix.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Vector.hpp"
#include "VertexArray.h"

#include "Test/Test_Menu.h"
#include "Test/Test_ClearColor.h"
#include "Test/Test_Transform_2Cubes.h"
#include "Test/Test_ImGui.h"

using namespace Framework;
using namespace Framework::Test;

int main()
{
	GLFWwindow* window = nullptr;
	Renderer renderer( &window, 800, 600, 1000, 100 );

	Framework::ImGuiSetup::Initialize( window );

	/* renderer.CleanUp() will destroy the OpenGL context, which will cause GlGetError() calls (in OpenGL types' destructors) to return an error and cause an endless loop.
	 * To prevent it, all Test code is vacuumed inside a local scope, to ensure all destructors run before renderer.CleanUp(). */
	{
		std::unique_ptr< TestInterface > test_current;
		std::unique_ptr< Test_Menu > test_menu = std::make_unique< Test_Menu >( renderer, test_current );

		test_menu->Register< Test_ClearColor >(); // TODO: Color info should be std::forwarded!
		test_menu->Register< Test_Transfom_2Cubes >();
		test_menu->Register< Test_ImGui >();

		bool continue_executing_tests = false;

		do
		{
			test_menu->Execute();

			continue_executing_tests = ( bool )test_current;

			if( test_current )
			{
				test_current->Execute();
				test_current.reset();
			}

		}
		while( continue_executing_tests );
	}

	Framework::ImGuiSetup::Shutdown();

	renderer.CleanUp();

	return 0;
}
