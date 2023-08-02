#pragma once

// Make sure GLAD is included before GLFW3!
#include <glad/glad.h>

// Project Includes.
#include "../Color.hpp"
#include "../Input.h"
#include "../Renderer.h"

namespace Framework::Test
{
	class TestInterface
	{
	public:
		virtual void Run() = 0;
		virtual ~TestInterface() {}
	};

	template< class ActualTest > // CRTP.
	class Test : public TestInterface
	{
	public:
		Test( GLFWwindow** window = nullptr, const Color4 clear_color = Color4::Clear_Default(),
			  const unsigned int width_pixels = 800, const unsigned int height_pixels = 600, const int pos_x = 1000, const int pos_y = 100 )
			:
			window( window ? *window : nullptr ),
			renderer( &this->window, width_pixels, height_pixels, pos_x, pos_y, clear_color )
		{
		}

		~Test()
		{
			renderer.CleanUp();
		}

		void Run() override
		{
			Derived()->OnRun();
		}

		void Update()
		{
			Derived()->OnUpdate();
		}

		void Render()
		{
			Derived()->OnRender();
		}

		/* Default implementations for derived classes. */

		void OnRun()
		{
			while( !glfwWindowShouldClose( window ) )
			{
				Update();
				Render();
				renderer.Update();
			}
		}

		void OnUpdate() { Input::Process( window ); }
		void OnRender() { renderer.Update(); }

	private:
		ActualTest* Derived() { return static_cast< ActualTest* >( this ); }
		ActualTest* Derived() const { return static_cast< ActualTest* >( this ); }

	protected:
		GLFWwindow* window;
		Renderer renderer;
	};
}
