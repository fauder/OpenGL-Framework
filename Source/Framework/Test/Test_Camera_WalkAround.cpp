// Framework Includes.
#include "Core/Log_ImGui.h"
#include "Core/Platform.h"

#include "Math/Matrix.h"

#include "Renderer/MeshUtility.hpp"
#include "Renderer/Primitive/Primitive_Cube.h"

#include "Test_Camera_WalkAround.h"

#include "Utility/ImGuiUtility.h"

using namespace Framework::Math::Literals;

namespace Framework::Test
{
	Test_Camera_WalkAround::Test_Camera_WalkAround()
		:
		Test( false /* UI starts disabled, to allow for camera movement via mouse input. */ ),
		camera_controller_flight( &camera, ResetCameraMoveSpeed() ),
		camera_delta_position( ZERO_INITIALIZATION ),
		input_is_enabled( true )
	{
		using namespace Framework;

		Platform::CaptureMouse( input_is_enabled );

		shader   = std::make_unique< Shader >( "Source/Asset/Shader/textured_mix.vertex", "Source/Asset/Shader/textured_mix.fragment", "Textured Mix" );
		material = std::make_unique< Material >( shader.get() );

		constexpr auto vertices = MeshUtility::Interleave( Primitives::Cube::Positions, Primitives::Cube::UVs_CubeMap );

		VertexBuffer vertex_buffer( vertices.data(), static_cast< unsigned int >( vertices.size() ) );

		VertexBufferLayout vertex_buffer_layout;
		vertex_buffer_layout.Push< float >( 3 ); // NDC positions.
		vertex_buffer_layout.Push< float >( 2 ); // Tex. coords.
		//vertex_buffer_layout.Push< float >( 4 ); // Vertex colors.
		cube_vertex_array = std::make_unique< VertexArray >( vertex_buffer, vertex_buffer_layout );

		cube_1 = std::make_unique< Drawable >( material.get(), &cube_transform, cube_vertex_array.get() );

		renderer.AddDrawable( cube_1.get() );
		renderer.SetPolygonMode( PolygonMode::FILL );

		texture_test_cube = std::make_unique< Texture >( "Asset/Texture/test_tex_cube.png", GL_RGBA );
		texture_test_cube->ActivateAndBind( GL_TEXTURE0 ); // Above line may bind the texture to whatever texture slot was active before, so more than 1 slots may be bound to this texture.

		shader->Bind();

		material->SetTextureSampler2D( "texture_sampler_1", 0 );

		// Initial camera position and rotation:
		ResetCameraTranslation();
	}

	Test_Camera_WalkAround::~Test_Camera_WalkAround()
	{
		Test::~Test();

		Platform::SetKeyboardEventCallback();
		Platform::CaptureMouse( false );
	}

	void Test_Camera_WalkAround::OnKeyboardEvent( const Platform::KeyCode key_code, const Platform::KeyAction action, const Platform::KeyMods mods )
	{
		switch( key_code )
		{
			case Platform::KeyCode::KEY_GRAVE_ACCENT:
				if( action == Platform::KeyAction::PRESS )
				{
					input_is_enabled = !input_is_enabled;
					Platform::CaptureMouse( input_is_enabled );
				}
				break;
			case Platform::KeyCode::KEY_SPACE:
				if( action == Platform::KeyAction::PRESS )
				{
					material->SetBool( "use_vertex_color", !material->GetBool( "use_vertex_color" ) );
				}
				break;
			default:
				break;
		}
	}

	void Test_Camera_WalkAround::OnProcessInput()
	{
		camera_delta_position = Vector3::Zero();

		if( input_is_enabled == false )
			return;

		if( Platform::IsKeyPressed( Platform::KeyCode::KEY_R ) )
		{
			ResetCameraRotation();
			ResetCameraTranslation();
			return;
		}

		if( Platform::IsKeyPressed( Platform::KeyCode::KEY_W ) )
			camera_delta_position += camera.Forward();
		if( Platform::IsKeyPressed( Platform::KeyCode::KEY_S ) )
			camera_delta_position -= camera.Forward();
		if( Platform::IsKeyPressed( Platform::KeyCode::KEY_A ) )
			camera_delta_position -= camera.Right();
		if( Platform::IsKeyPressed( Platform::KeyCode::KEY_D ) )
			camera_delta_position += camera.Right();

		if( !camera_delta_position.IsZero() )
			camera_delta_position.Normalize() *= camera_controller_flight.move_speed * time_delta;

		camera_displacement = camera_delta_position.Magnitude();

		const auto [ mouse_delta_x, mouse_delta_y ] = Platform::GetMouseCursorDeltas();

		camera_controller_flight.SetHeading( camera_controller_flight.GetHeading() + mouse_delta_x );
		camera_controller_flight.SetPitch( Math::Clamp( camera_controller_flight.GetPitch() - mouse_delta_y, -Constants< Radians >::Pi_Over_Six(), +Constants< Radians >::Pi_Over_Six() ) );

		const Degrees fov_offset( -Platform::GetMouseScrollOffsets().second );
		camera.SetFieldOfView( Math::Clamp( camera.GetFieldOfView() - fov_offset, 1_deg, 45_deg ) );
	}

	void Test_Camera_WalkAround::OnUpdate()
	{
		camera_transform.ResetDirtyFlag();

		camera_transform.OffsetTranslation( camera_delta_position );
	}

	void Test_Camera_WalkAround::OnRender()
	{
		material->SetMatrix( "transform_view",		  camera.GetViewMatrix() );
		material->SetMatrix( "transform_projection", camera.GetProjectionMatrix() );
	}

	void Test_Camera_WalkAround::OnRenderImGui()
	{
		if( ImGui::Begin( "Test: Camera ", nullptr, CurrentImGuiWindowFlags() | ImGuiWindowFlags_AlwaysAutoResize ) )
		{
			Vector3 camera_position          = camera_transform.GetTranslation();
			Vector3 camera_right_direction   = camera.Right();
			Vector3 camera_up_direction      = camera.Up();
			Vector3 camera_forward_direction = camera.Forward();
			float heading                    = ( float )Math::Degrees( camera_controller_flight.GetHeading() );
			float pitch                      = ( float )Math::Degrees( camera_controller_flight.GetPitch() );

			Math::Polar3_Spherical camera_orientation_spherical( 1.0f, Degrees( heading ), Degrees( pitch ) );
			Vector3 camera_look_at_direction( Math::ToVector3( camera_orientation_spherical ) );

			float fov = ( float )camera.GetFieldOfView();

			ImGui::DragFloat3( "Camera Position", reinterpret_cast< float* >( &camera_position ) ); ImGui::SameLine(); if( ImGui::Button( "Reset##camera_position" ) ) ResetCameraTranslation();
			ImGui::InputFloat( "Delta Position", &camera_displacement, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly );
			ImGui::SliderFloat( "Move Speed ", &camera_controller_flight.move_speed, 0.5f, 5.0f, "%.2f", ImGuiSliderFlags_Logarithmic ); ImGui::SameLine(); if( ImGui::Button( "Reset##camera_move_speed" ) ) ResetCameraMoveSpeed();

			ImGui::InputFloat( "Heading", &heading, 0.0f, 0.0f, "%.3f degrees", ImGuiInputTextFlags_ReadOnly );
			ImGui::InputFloat( "Pitch", &pitch, 0.0f, 0.0f, "%.3f degrees", ImGuiInputTextFlags_ReadOnly );
			ImGui::InputFloat3( "Look-At Direction", const_cast< float* >( camera_look_at_direction.Data() ), "%.3f", ImGuiInputTextFlags_ReadOnly );
				ImGui::SameLine(); if( ImGui::Button( "Reset##camera_rotation" ) ) ResetCameraRotation();
			ImGui::InputFloat3( "Camera Right",   reinterpret_cast< float* >( &camera_right_direction	), "%.3f", ImGuiInputTextFlags_ReadOnly );
			ImGui::InputFloat3( "Camera Up",	  reinterpret_cast< float* >( &camera_up_direction		), "%.3f", ImGuiInputTextFlags_ReadOnly );
			ImGui::InputFloat3( "Camera Forward", reinterpret_cast< float* >( &camera_forward_direction ), "%.3f", ImGuiInputTextFlags_ReadOnly );
			ImGui::InputFloat( "Field of View", &fov, 0.0f, 0.0f, "%.3f degrees", ImGuiInputTextFlags_ReadOnly );

			ImGui::SeparatorText( "Mouse Info." );
		/* -----------------------------------*/
			auto [ mouse_delta_x, mouse_delta_y ] = Platform::GetMouseCursorDeltas();
			auto sensitivity = Platform::GetMouseSensitivity();
			if( ImGui::InputFloat( "Sensitivity", &sensitivity, 0.0f, 0.0f, "%.3f" ) )
				Platform::SetMouseSensitivity( sensitivity );
			ImGui::InputFloat( "Platform: Delta X", &mouse_delta_x, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly );
			ImGui::InputFloat( "Platform: Delta Y", &mouse_delta_y, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly );
		}

		ImGui::End();

		ImGuiUtility::SetNextWindowPos( ImGuiUtility::HorizontalWindowPositioning::LEFT, ImGuiUtility::VerticalWindowPositioning::BOTTOM );
		Log::Dump( *shader, CurrentImGuiWindowFlags() );
		ImGuiUtility::SetNextWindowPos( ImGuiUtility::HorizontalWindowPositioning::RIGHT, ImGuiUtility::VerticalWindowPositioning::BOTTOM );
		Log::Dump( *material, CurrentImGuiWindowFlags() );
	}

	void Test_Camera_WalkAround::ResetCameraTranslation()
	{
		camera_transform.SetTranslation( Vector3::Backward() * 4.0f );
	}

	void Test_Camera_WalkAround::ResetCameraRotation()
	{
		camera_controller_flight.SetHeading( 0_rad );
		camera_controller_flight.SetPitch( 0_rad );
	}

	float Test_Camera_WalkAround::ResetCameraMoveSpeed()
	{
		return camera_controller_flight.move_speed = 2.0f;
	}
}
