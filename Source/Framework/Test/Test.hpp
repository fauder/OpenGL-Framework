#pragma once

// Vendor GLAD Includes (Has to be included before GLFW!).
#include "Vendor/OpenGL/glad/glad.h"

// Framework Includes.
#include "Core/Platform.h"
#include "Core/ImGuiSetup.h"

#include "Math/Color.hpp"

#include "Renderer/Renderer.h"

#include "Utility/ImGuiUtility.h"

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
		Test( bool ui_starts_enabled = true, bool display_flags = true )
			:
			camera( &camera_transform, Platform::GetFrameBufferAspectRatio() ),
			renderer( &camera ),
			name( ExtractTestNameFromTypeName( typeid( *this ).name() ) ),
			display_frame_statistics( true ),
			time_current( 0.0f ),
			time_multiplier( 1.0f ),
			time_previous( 0.0f ),
			time_previous_since_start( 0.0f ),
			time_since_start( 0.0f ),
			executing( true ),
			ui_interaction_enabled( ui_starts_enabled ),
			display_flags( display_flags )
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
			Platform::PollEvents();

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
			if( display_flags )
				RenderImGui_Flags();
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

			Platform::SetKeyboardEventCallback(
				[ = ]( const Platform::KeyCode key_code, const Platform::KeyAction key_action, const Platform::KeyMods key_mods )
				{
					this->OnKeyboardEvent_Internal( key_code, key_action, key_mods );
				} );

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

		void OnKeyboardEvent( const Platform::KeyCode key_code, const Platform::KeyAction key_action, const Platform::KeyMods key_mods ) {}
		void OnProcessInput()	{}
		void OnUpdate()			{}
		void OnRender()			{}
		void OnRenderImGui()	{}

		ImGuiWindowFlags CurrentImGuiWindowFlags() const { return ImGuiWindowFlags_NoFocusOnAppearing | ( ui_interaction_enabled ? 0 : ImGuiWindowFlags_NoMouseInputs ); }
		void SetUIInteraction( const bool enable ) { ui_interaction_enabled = enable; }

	private:
		ActualTest* Derived() { return static_cast< ActualTest* >( this ); }
		ActualTest* Derived() const { return static_cast< ActualTest* >( this ); }

		void OnKeyboardEvent_Internal( const Platform::KeyCode key_code, const Platform::KeyAction key_action, const Platform::KeyMods key_mods )
		{
			if( ImGui::GetIO().WantCaptureKeyboard )
				return;

			switch( key_code )
			{
				case Platform::KeyCode::KEY_ESCAPE:
					if( key_action == Platform::KeyAction::PRESS )
						executing = false;
					break;
				/* Use the key below ESC to toggle between game & menu/UI. */
				case Platform::KeyCode::KEY_GRAVE_ACCENT: 
					if( key_action == Platform::KeyAction::PRESS )
						ui_interaction_enabled = !ui_interaction_enabled;
					break;
				case Platform::KeyCode::KEY_I:
					if( key_action == Platform::KeyAction::PRESS )
						display_flags = !display_flags;
					break;
				default:
					break;
			}

			Derived()->OnKeyboardEvent( key_code, key_action, key_mods );
		}

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
			ImGuiUtility::SetNextWindowPos( ImGuiUtility::HorizontalWindowPositioning::LEFT, ImGuiUtility::VerticalWindowPositioning::TOP );
			ImGui::Begin( "Test Menu", nullptr, CurrentImGuiWindowFlags() | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove );

			ImGui::Text( std::string( R"(Executing Test ")" + name + R"("...)" ).c_str() );
			if( ImGui::Button( "<-" ) )
				executing = false;

			ImGui::End();
		}

		void RenderImGui_Flags()
		{
			ImGuiUtility::SetNextWindowPos( ImGuiUtility::HorizontalWindowPositioning::CENTER, ImGuiUtility::VerticalWindowPositioning::TOP );
			if( ImGui::Begin( "App. Flags", nullptr, CurrentImGuiWindowFlags() | ImGuiWindowFlags_AlwaysAutoResize ) )
			{
				ImGui::Checkbox( "UI Interaction", &ui_interaction_enabled );
			}

			ImGui::End();
		}

		void RenderImGui_FrameStatistics()
		{
			ImGuiUtility::SetNextWindowPos( ImGuiUtility::HorizontalWindowPositioning::RIGHT, ImGuiUtility::VerticalWindowPositioning::TOP );
			if( ImGui::Begin( "Frame Statistics", nullptr, CurrentImGuiWindowFlags() | ImGuiWindowFlags_AlwaysAutoResize ) )
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
		Transform camera_transform;
		Camera camera;
		Renderer renderer;
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
		bool ui_interaction_enabled;
		bool display_flags;
	};
}
