// Project Includes.
#include "Test_Camera_WalkAround.h"

#include "../Matrix.h"
#include "../ImGuiUtility.h"

using namespace Framework::Math::Literals;

namespace Framework::Test
{
	Test_Camera_WalkAround::Test_Camera_WalkAround( Renderer& renderer )
		:
		Test( renderer ),
		time_current( 0.0f ),
		time_multiplier( ResetTimeMultiplier() ),
		camera_move_speed( ResetCameraMoveSpeed() ),
		test_is_paused( false ),
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
		Test::OnProcessInput();

		delta_position = Vector3::Zero();

		if( glfwGetKey( window, GLFW_KEY_W ) )
			delta_position += Vector3::Forward();
		if( glfwGetKey( window, GLFW_KEY_S ) )
			delta_position += Vector3::Backward();
		if( glfwGetKey( window, GLFW_KEY_A ) )
			delta_position += Vector3::Left();
		if( glfwGetKey( window, GLFW_KEY_D ) )
			delta_position += Vector3::Right();

		if( !delta_position.IsZero() )
			delta_position.Normalize() *= camera_move_speed * time_delta;

		displacement = delta_position.Magnitude();
	}

	void Test_Camera_WalkAround::OnUpdate()
	{
		if( !test_is_paused )
		{
			time_current  = static_cast< float >( glfwGetTime() ) * time_multiplier;
			time_sin      = Math::Sin( Radians( time_current ) );
			time_cos      = Math::Cos( Radians( time_current ) );
			time_mod_2_pi = std::fmod( time_current, Constants< float >::Two_Pi() );
		}

		camera.transform.OffsetTranslation( delta_position );
	}

	void Test_Camera_WalkAround::OnRender()
	{
		const Vector3 target( ZERO_INITIALIZATION ); // Look at the origin.

		Vector3 camera_position = camera.transform.GetTranslation();

		const auto lookAt_direction( ( target - camera_position ).Normalized() );

		//if( !lookAt_direction.IsZero() ) // Or we can clamp the position instead, so the camera does not overlap with the target.
		//	camera.transform.SetRotation( Quaternion::LookRotation( lookAt_direction ) );

		shader->SetMatrix( "transformation_view", camera.transform.GetInverseOfFinalMatrix() );
	}

	void Test_Camera_WalkAround::OnRenderImGui()
	{
		Test::OnRenderImGui();

		if( ImGui::Begin( "Test: Camera " ) )
		{
			ImGui::SeparatorText( "Time Settings" );
			/* ______________________________________ */

			ImGui::Text( "Time: %.2f seconds", time_current ); ImGui::SameLine();

			if( !test_is_paused && ImGui::Button( "Pause" ) )
				test_is_paused = true;
			else if( test_is_paused && ImGui::Button( "Resume" ) )
				test_is_paused = false;

			auto sin_time   = time_sin;
			auto cos_time   = time_cos;
			auto time_mod_1 = std::fmod( time_current, 1.0f );

			ImGui::SliderFloat( "Time Multiplier", &time_multiplier, 0.01f, 5.0f, "x %.2f", ImGuiSliderFlags_Logarithmic ); ImGui::SameLine(); if( ImGui::Button( "Reset##time_multiplier" ) ) ResetTimeMultiplier();

			ImGui::ProgressBar( time_mod_1,	   ImVec2( 0.0f, 0.0f ) ); ImGui::SameLine(); ImGui::TextUnformatted( "Time % 1" );
			ImGui::ProgressBar( time_mod_2_pi / Constants< float >::Two_Pi(), ImVec2( 0.0f, 0.0f ) ); ImGui::SameLine(); ImGui::TextUnformatted( "Time % (2 * Pi)" );
			ImGui::SliderFloat( "Sin(Time) ", &sin_time, -1.0f, 1.0f, "%.1f",		ImGuiSliderFlags_NoInput );
			ImGui::SliderFloat( "Cos(Time) ", &cos_time, -1.0f, 1.0f, "%.1f",		ImGuiSliderFlags_NoInput );

			ImGui::SeparatorText( "Camera Settings" );
			/* ______________________________________ */

			auto camera_position = camera.transform.GetTranslation();
			ImGui::DragFloat3( "Camera Position", reinterpret_cast< float* >( &camera_position ) ); ImGui::SameLine(); if( ImGui::Button( "Reset##camera_position" ) ) ResetCameraTranslation();
			ImGui::SliderFloat( "Move Speed ", &camera_move_speed, 0.5f, 5.0f, "%.2f", ImGuiSliderFlags_Logarithmic ); ImGui::SameLine(); if( ImGui::Button( "Reset##camera_move_speed" ) ) ResetCameraMoveSpeed();
			ImGui::InputFloat( "Delta Position", &displacement, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly );
		}

		ImGui::End();
	}

	void Test_Camera_WalkAround::ResetCameraTranslation()
	{
		camera.transform.SetTranslation( Vector3::Backward() * 4.0f );
	}

	float Test_Camera_WalkAround::ResetTimeMultiplier()
	{
		return time_multiplier = 1.0f;
	}

	float Test_Camera_WalkAround::ResetCameraMoveSpeed()
	{
		return camera_move_speed = 2.0f;
	}
}
