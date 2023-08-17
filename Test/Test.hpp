#pragma once

// Make sure GLAD is included before GLFW3!
#include <glad/glad.h>

// Project Includes.
#include "../Color.hpp"
#include "../Input.h"
#include "../Platform.h"
#include "../Renderer.h"
#include "../ImGuiSetup.h"
#include "../ImGuiUtility.h"

namespace Framework::Test
{
	class TestInterface
	{
	public:
		virtual void Execute() = 0;
		virtual ~TestInterface() {}
		virtual const std::string& GetName() const = 0;
	};

	template< class ActualTest > // CRTP.
	class Test : public TestInterface
	{
	public:
		Test( Renderer& renderer )
			:
			renderer( renderer ),
			name( ExtractTestNameFromTypeName( typeid( *this ).name() ) ),
			display_frame_statistics( true ),
			time_current( 0.0f ),
			time_multiplier( 1.0f ),
			time_previous( 0.0f ),
			time_previous_since_start( 0.0f ),
			time_since_start( 0.0f ),
			executing( true )
		{
			renderer.SetClearColor( Color4::Clear_Default() );
		}

		~Test()
		{
		}

		const std::string& GetName() const override { return name; }

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
			if( display_frame_statistics )
				RenderImGui_FrameStatistics();
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

		void FreezeTime()	{ time_multiplier = 0.0f; }
		void UnfreezeTime() { time_multiplier = 1.0f; }
		bool TimeIsFrozen() { return Math::IsZero( time_multiplier ); }

		/* Default implementations for derived classes. */

		void OnExecute()
		{
			executing = true;
			while( executing && !Platform::ShouldClose() )
			{
				CalculateTimeInformation();

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

		void OnProcessInput()	{ Input::Process(); }
		void OnUpdate()			{}
		void OnRender()			{}
		void OnRenderImGui()	{}

	private:
		ActualTest* Derived() { return static_cast< ActualTest* >( this ); }
		ActualTest* Derived() const { return static_cast< ActualTest* >( this ); }

		void CalculateTimeInformation()
		{
			time_since_start = Platform::GetCurrentTime();

			time_delta_real = time_since_start - time_previous_since_start;

			time_current += time_delta_real * time_multiplier;

			time_delta = time_current - time_previous;

			time_previous             = time_current;
			time_previous_since_start = time_since_start;

			time_sin      = Math::Sin( Radians( time_current ) );
			time_cos      = Math::Cos( Radians( time_current ) );
			time_mod_1	  = std::fmod( time_current, 1.0f );
			time_mod_2_pi = std::fmod( time_current, Constants< float >::Two_Pi() );
		}

		void RenderImGui_Menu_BackButton()
		{
			ImGui::Begin( "Test Menu" );

			ImGui::Text( std::string( R"(Executing Test ")" + name + R"("...)" ).c_str() );
			if( ImGui::Button( "<-" ) )
				executing = false;

			ImGui::End();
		}

		void RenderImGui_FrameStatistics()
		{
			if( ImGui::Begin( "Frame Statistics." ) )
			{
				ImGui::Text( "FPS: %.1f fps", 1.0f / time_delta_real );
				ImGui::Text( "Delta time (multiplied): %.3f ms | Delta time (real): %.3f", time_delta * 1000.0f, time_delta_real * 1000.0f );
				ImGui::Text( "Time since start: %.3f.", time_since_start );
				ImGui::SliderFloat( "Time Multiplier", &time_multiplier, 0.01f, 5.0f, "x %.2f", ImGuiSliderFlags_Logarithmic ); ImGui::SameLine(); if( ImGui::Button( "Reset##time_multiplier" ) ) time_multiplier = 1.0f;
				if( !TimeIsFrozen() && ImGui::Button( "Pause" ) )
					FreezeTime();
				else if( TimeIsFrozen() && ImGui::Button( "Resume" ) )
					UnfreezeTime();

				auto sin_time = time_sin;
				auto cos_time = time_cos;
				auto time_mod_1 = std::fmod( time_current, 1.0f );

				ImGui::ProgressBar( time_mod_1, ImVec2( 0.0f, 0.0f ) ); ImGui::SameLine(); ImGui::TextUnformatted( "Time % 1" );
				ImGui::ProgressBar( time_mod_2_pi / Constants< float >::Two_Pi(), ImVec2( 0.0f, 0.0f ) ); ImGui::SameLine(); ImGui::TextUnformatted( "Time % (2 * Pi)" );
				ImGui::SliderFloat( "Sin(Time) ", &sin_time, -1.0f, 1.0f, "%.1f", ImGuiSliderFlags_NoInput );
				ImGui::SliderFloat( "Cos(Time) ", &cos_time, -1.0f, 1.0f, "%.1f", ImGuiSliderFlags_NoInput );
			}

			ImGui::End();
		}

	protected:
		Renderer& renderer;
		std::string name;

		bool display_frame_statistics;

		float time_delta;
		float time_current;
		float time_multiplier;

		float time_sin;
		float time_cos;
		float time_mod_1;
		float time_mod_2_pi;

	private:
		float time_delta_real;
		float time_previous;
		float time_previous_since_start;
		float time_since_start;

		bool executing;
	};
}
