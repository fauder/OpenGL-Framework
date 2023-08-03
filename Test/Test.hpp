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
		virtual void Execute() = 0;
		virtual ~TestInterface() {}
	};

	template< class ActualTest > // CRTP.
	class Test : public TestInterface
	{
	public:
		Test( Renderer& renderer )
			:
			renderer( renderer ),
			window( renderer.GetWindow() ),
			executing( true ),
			name( ExtractTestNameFromTypeName( typeid( *this ).name() ) )
		{
		}

		~Test()
		{
		}

		const std::string& GetName() const { return name; }

		inline void StopExecution() { executing = false; }

		void Execute() override
		{
			Derived()->OnExecute();
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
			RenderImGui_Menu_BackButton();
			Derived()->OnRenderImGui();
		}

	protected:
		std::string ExtractTestNameFromTypeName( const std::string& type_name ) const
		{
			const std::string test_type_name( type_name );
			const auto start_pos = test_type_name.find( '_', 0 ) + 1;
			const auto end_pos = test_type_name.find( '>', 0 );
			return test_type_name.substr( start_pos, end_pos != std::string::npos ? end_pos - start_pos : end_pos );
		}

		/* Default implementations for derived classes. */

		void OnExecute()
		{
			executing = true;
			while( executing && !glfwWindowShouldClose( window ) )
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
		void RenderImGui_Menu_BackButton()
		{
			ImGui::Begin( "Test Menu" );

			ImGui::Text( std::string( R"(Executing Test ")" + name + R"("...)" ).c_str() );
			if( ImGui::Button( "<-" ) )
				executing = false;

			ImGui::End();
		}

	protected:
		Renderer& renderer;
		GLFWwindow* window;
		std::string name;

	private:
		bool executing;
	};
}
