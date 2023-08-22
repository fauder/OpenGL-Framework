// Project Includes.
#include "Test_Camera_WalkAround.h"

#include "../Matrix.h"
#include "../ImGuiUtility.h"
#include "../Platform.h"

using namespace Framework::Math::Literals;

namespace Framework::Test
{
	Test_Camera_WalkAround::Test_Camera_WalkAround( Renderer& renderer )
		:
		Test( renderer ),
		camera_move_speed( ResetCameraMoveSpeed() ),
		camera_direction_spherical( 1.0f, 0.0_rad, 0.0_rad ),
		camera_look_at_direction( Vector3::Forward() ),
		delta_position( ZERO_INITIALIZATION )
	{
		using namespace Framework;

		shader = std::make_unique< Shader >( "Asset/Shader/Vertex.shader", "Asset/Shader/Fragment.shader" );

		const std::vector< Math::Vector< float, 3 + 2 /*+ 4*/ > > vertices
		{
			/* Object space coordinates			UVs.				Vertex Colors	*/
			{ -0.5f, -0.5f, -0.5f,			1.0f,  0.333f,		/* 1.0f, 0.0f, 0.0f, 1.0f */ }, // BACK
			{  0.5f, -0.5f, -0.5f,			0.75f, 0.333f,		/* 1.0f, 0.0f, 0.0f, 1.0f */ }, // BACK
			{  0.5f,  0.5f, -0.5f,			0.75f, 0.666f,		/* 1.0f, 0.0f, 0.0f, 1.0f */ }, // BACK
			{  0.5f,  0.5f, -0.5f,			0.75f, 0.666f,		/* 1.0f, 0.0f, 0.0f, 1.0f */ }, // BACK
			{ -0.5f,  0.5f, -0.5f,			1.0f,  0.666f,		/* 1.0f, 0.0f, 0.0f, 1.0f */ }, // BACK
			{ -0.5f, -0.5f, -0.5f,			1.0f,  0.333f,		/* 1.0f, 0.0f, 0.0f, 1.0f */ }, // BACK

			{ -0.5f, -0.5f,  0.5f,			0.25f, 0.333f,		/* 0.0f, 1.0f, 0.0f, 1.0f */ }, // FRONT
			{  0.5f, -0.5f,  0.5f,			0.5f,  0.333f,		/* 0.0f, 1.0f, 0.0f, 1.0f */ }, // FRONT
			{  0.5f,  0.5f,  0.5f,			0.5f,  0.666f,		/* 0.0f, 1.0f, 0.0f, 1.0f */ }, // FRONT
			{  0.5f,  0.5f,  0.5f,			0.5f,  0.666f,		/* 0.0f, 1.0f, 0.0f, 1.0f */ }, // FRONT
			{ -0.5f,  0.5f,  0.5f,			0.25f, 0.666f,		/* 0.0f, 1.0f, 0.0f, 1.0f */ }, // FRONT
			{ -0.5f, -0.5f,  0.5f,			0.25f, 0.333f,		/* 0.0f, 1.0f, 0.0f, 1.0f */ }, // FRONT

			{ -0.5f,  0.5f,  0.5f,			0.25f, 0.666f,		/* 0.0f, 0.0f, 1.0f, 1.0f */ }, // LEFT
			{ -0.5f,  0.5f, -0.5f,			0.0f,  0.666f,		/* 0.0f, 0.0f, 1.0f, 1.0f */ }, // LEFT
			{ -0.5f, -0.5f, -0.5f,			0.0f,  0.333f,		/* 0.0f, 0.0f, 1.0f, 1.0f */ }, // LEFT
			{ -0.5f, -0.5f, -0.5f,			0.0f,  0.333f,		/* 0.0f, 0.0f, 1.0f, 1.0f */ }, // LEFT
			{ -0.5f, -0.5f,  0.5f,			0.25f, 0.333f,		/* 0.0f, 0.0f, 1.0f, 1.0f */ }, // LEFT
			{ -0.5f,  0.5f,  0.5f,			0.25f, 0.666f,		/* 0.0f, 0.0f, 1.0f, 1.0f */ }, // LEFT

			{  0.5f,  0.5f,  0.5f,			0.5f,  0.666f,		/* 1.0f, 1.0f, 0.0f, 1.0f */ }, // RIGHT
			{  0.5f,  0.5f, -0.5f,			0.75f, 0.666f,		/* 1.0f, 1.0f, 0.0f, 1.0f */ }, // RIGHT
			{  0.5f, -0.5f, -0.5f,			0.75f, 0.333f,		/* 1.0f, 1.0f, 0.0f, 1.0f */ }, // RIGHT
			{  0.5f, -0.5f, -0.5f,			0.75f, 0.333f,		/* 1.0f, 1.0f, 0.0f, 1.0f */ }, // RIGHT
			{  0.5f, -0.5f,  0.5f,			0.5f,  0.333f,		/* 1.0f, 1.0f, 0.0f, 1.0f */ }, // RIGHT
			{  0.5f,  0.5f,  0.5f,			0.5f,  0.666f,		/* 1.0f, 1.0f, 0.0f, 1.0f */ }, // RIGHT

			{ -0.5f, -0.5f, -0.5f,			0.25f, 0.0f,		/* 0.0f, 1.0f, 1.0f, 1.0f */ }, // BOTTOM
			{  0.5f, -0.5f, -0.5f,			0.5f,  0.0f,		/* 0.0f, 1.0f, 1.0f, 1.0f */ }, // BOTTOM
			{  0.5f, -0.5f,  0.5f,			0.5f,  0.333f,		/* 0.0f, 1.0f, 1.0f, 1.0f */ }, // BOTTOM
			{  0.5f, -0.5f,  0.5f,			0.5f,  0.333f,		/* 0.0f, 1.0f, 1.0f, 1.0f */ }, // BOTTOM
			{ -0.5f, -0.5f,  0.5f,			0.25f, 0.333f,		/* 0.0f, 1.0f, 1.0f, 1.0f */ }, // BOTTOM
			{ -0.5f, -0.5f, -0.5f,			0.25f, 0.0f,		/* 0.0f, 1.0f, 1.0f, 1.0f */ }, // BOTTOM

			{ -0.5f,  0.5f, -0.5f,			0.25f, 1.0f,		/* 1.0f, 0.0f, 1.0f, 1.0f */ }, // TOP
			{  0.5f,  0.5f, -0.5f,			0.5f,  1.0f,		/* 1.0f, 0.0f, 1.0f, 1.0f */ }, // TOP
			{  0.5f,  0.5f,  0.5f,			0.5f,  0.666f,		/* 1.0f, 0.0f, 1.0f, 1.0f */ }, // TOP
			{  0.5f,  0.5f,  0.5f,			0.5f,  0.666f,		/* 1.0f, 0.0f, 1.0f, 1.0f */ }, // TOP
			{ -0.5f,  0.5f,  0.5f,			0.25f, 0.666f,		/* 1.0f, 0.0f, 1.0f, 1.0f */ }, // TOP
			{ -0.5f,  0.5f, -0.5f,			0.25f, 1.0f			/* 1.0f, 0.0f, 1.0f, 1.0f */ }  // TOP
		};

		VertexBuffer vertex_buffer( vertices.data(), static_cast< unsigned int >( vertices.size() ) );

		VertexBufferLayout vertex_buffer_layout;
		vertex_buffer_layout.Push< float >( 3 ); // NDC positions.
		vertex_buffer_layout.Push< float >( 2 ); // Tex. coords.
		//vertex_buffer_layout.Push< float >( 4 ); // Vertex colors.
		cube_vertex_array = std::make_unique< VertexArray >( vertex_buffer, vertex_buffer_layout );

		cube_1 = std::make_unique< Drawable >( shader.get(), cube_vertex_array.get() );

		renderer.AddDrawable( cube_1.get() );
		renderer.SetPolygonMode( PolygonMode::FILL );

		texture_test_cube = std::make_unique< Texture >( "Asset/Texture/test_tex_cube.png", GL_RGBA );

		texture_test_cube->ActivateAndBind( GL_TEXTURE0 );

		shader->Bind();
		shader->SetInt( "texture_sampler_1", 0 );

		shader->SetMatrix( "transformation_projection", Matrix::PerspectiveProjection( 0.1f, 100.0f, renderer.AspectRatio(), 45_deg ) );

		// Initial camera position and rotation:
		ResetCameraTranslation();
	}

	Test_Camera_WalkAround::~Test_Camera_WalkAround()
	{
		renderer.RemoveDrawable( cube_1.get() );
	}

	void Test_Camera_WalkAround::OnProcessInput()
	{
		delta_position = Vector3::Zero();

		if( Platform::IsKeyPressed( Platform::KeyCode::KEY_W ) )
			delta_position += camera.transform.Forward();
		if( Platform::IsKeyPressed( Platform::KeyCode::KEY_S ) )
			delta_position -= camera.transform.Forward();
		if( Platform::IsKeyPressed( Platform::KeyCode::KEY_A ) )
			delta_position -= camera.transform.Right();
		if( Platform::IsKeyPressed( Platform::KeyCode::KEY_D ) )
			delta_position += camera.transform.Right();

		if( !delta_position.IsZero() )
			delta_position.Normalize() *= camera_move_speed * time_delta;

		displacement = delta_position.Magnitude();

		const auto [ mouse_delta_x, mouse_delta_y ] = Platform::GetMouseCursorDeltas();

		camera_direction_spherical.Heading() += mouse_delta_x;
		camera_direction_spherical.Pitch()   -= mouse_delta_y;

		camera_look_at_direction = Math::ToVector3( camera_direction_spherical );
	}

	void Test_Camera_WalkAround::OnUpdate()
	{
		camera.transform.OffsetTranslation( delta_position );
	}

	void Test_Camera_WalkAround::OnRender()
	{
		camera.transform.SetRotation( Quaternion::LookRotation( camera_look_at_direction ) );

		shader->SetMatrix( "transformation_view", camera.transform.GetInverseOfFinalMatrix() );
	}

	void Test_Camera_WalkAround::OnRenderImGui()
	{
		if( ImGui::Begin( "Test: Camera ", nullptr, CurrentImGuiWindowFlags() ) )
		{
			auto camera_position          = camera.transform.GetTranslation();
			auto camera_right_direction   = camera.transform.Right();
			auto camera_up_direction      = camera.transform.Up();
			auto camera_forward_direction = camera.transform.Forward();

			ImGui::DragFloat3( "Camera Position", reinterpret_cast< float* >( &camera_position ) ); ImGui::SameLine(); if( ImGui::Button( "Reset##camera_position" ) ) ResetCameraTranslation();
			ImGui::InputFloat3( "Camera Right",   reinterpret_cast< float* >( &camera_right_direction	), "%.3f", ImGuiInputTextFlags_ReadOnly );
			ImGui::InputFloat3( "Camera Up",	  reinterpret_cast< float* >( &camera_up_direction		), "%.3f", ImGuiInputTextFlags_ReadOnly );
			ImGui::InputFloat3( "Camera Forward", reinterpret_cast< float* >( &camera_forward_direction ), "%.3f", ImGuiInputTextFlags_ReadOnly );
			ImGui::SliderFloat( "Move Speed ", &camera_move_speed, 0.5f, 5.0f, "%.2f", ImGuiSliderFlags_Logarithmic ); ImGui::SameLine(); if( ImGui::Button( "Reset##camera_move_speed" ) ) ResetCameraMoveSpeed();
			ImGui::InputFloat( "Delta Position", &displacement, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly );

			ImGui::SeparatorText( "Mouse Info." );
			float heading = ( float )camera_direction_spherical.Heading();
			float pitch   = ( float )camera_direction_spherical.Pitch();
			auto [ mouse_delta_x, mouse_delta_y ] = Platform::GetMouseCursorDeltas();
			auto sensitivity = Platform::GetMouseSensitivity();
			if( ImGui::InputFloat( "Sensitivity", &sensitivity, 0.0f, 0.0f, "%.3f" ) )
				Platform::SetMouseSensitivity( sensitivity );
			ImGui::InputFloat( "Platform: Delta X", &mouse_delta_x, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly );
			ImGui::InputFloat( "Platform: Delta Y",	&mouse_delta_y, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly );
			ImGui::InputFloat( "Heading", &heading, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly );
			ImGui::InputFloat( "Pitch",		&pitch, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly );
			ImGui::InputFloat3( "Look-At Direction", const_cast< float* >( camera_look_at_direction.Data() ), "%.3f", ImGuiInputTextFlags_ReadOnly );
		}

		ImGui::End();
	}

	void Test_Camera_WalkAround::ResetCameraTranslation()
	{
		camera.transform.SetTranslation( Vector3::Backward() * 4.0f );
	}

	float Test_Camera_WalkAround::ResetCameraMoveSpeed()
	{
		return camera_move_speed = 2.0f;
	}
}
