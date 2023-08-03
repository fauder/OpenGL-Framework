#pragma once

// Make sure GLAD is included before GLFW3!
#include <glad/glad.h>

// Project Includes.
#include "../Color.hpp"
#include "../Input.h"
#include "../Renderer.h"
#include "../ImGuiSetup.h"

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
			Framework::ImGuiSetup::Initialize( this->window );
		}

		~Test()
		{
			Framework::ImGuiSetup::Shutdown();
			renderer.CleanUp();
		}

		void Run() override
		{
			Derived()->OnRun();
		}

		void ProcessInput()
		{
			Derived()->OnProcessInput();
		}

		void Update()
		{
			Derived()->OnUpdate();
		}

		void Render()
		{
			Derived()->OnRender();
		}

		void RenderImGui()
		{
			Derived()->OnRenderImGui();
		}

	protected:
		/* Default implementations for derived classes. */

		void OnRun()
		{
			while( !glfwWindowShouldClose( window ) )
			{
				ProcessInput();

				Update();

				renderer.BeginFrame();
				Render();
				renderer.DrawFrame();

				ImGuiSetup::BeginFrame();
				RenderImGui();
				ImGuiSetup::EndFrame();

				renderer.EndFrame();
			}
		}

		void OnProcessInput()	{ Input::Process( window ); }
		void OnUpdate()			{}
		void OnRender()			{}
		void OnRenderImGui()	{}

	private:
		ActualTest* Derived() { return static_cast< ActualTest* >( this ); }
		ActualTest* Derived() const { return static_cast< ActualTest* >( this ); }

	protected:
		GLFWwindow* window;
		Renderer renderer;
	};
}
