// Project Includes.
#include "Test_Camera_LookAt.h"

#include "../Matrix.h"
#include "../ImGuiUtility.h"

using namespace Framework::Math::Literals;

namespace Framework::Test
{
	Test_Camera_LookAt::Test_Camera_LookAt( Renderer& renderer )
		:
		Test( renderer ),
		camera_transform(),
		method_lookAt( LookAtMethod::LookAtMatrix ),
		rotation_plane( RotationPlane::ZX ),
		time_current( 0.0f ),
		time_multiplier( 1.0f ),
		zoom( 0.25f ),
		test_is_paused( false )
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

	Test_Camera_LookAt::~Test_Camera_LookAt()
	{
		renderer.RemoveDrawable( cube_1.get() );
	}

	void Test_Camera_LookAt::OnUpdate()
	{
		if( !test_is_paused )
		{
			time_current  = static_cast< float >( glfwGetTime() ) * time_multiplier;
			time_sin      = Math::Sin( Radians( time_current ) );
			time_cos      = Math::Cos( Radians( time_current ) );
			time_mod_2_pi = std::fmod( time_current, Constants< float >::Two_Pi() );
		}
	}

	void Test_Camera_LookAt::OnRender()
	{
		const Vector3 target( ZERO_INITIALIZATION ); // Look at the origin.

		const auto inverse_zoom = 1.0f / zoom;

		Vector3 camera_position = camera_transform.GetTranslation();

		switch( rotation_plane )
		{
			case Framework::Test::Test_Camera_LookAt::RotationPlane::ZX:
				camera_transform.SetTranslation( camera_position = camera_position
													.SetX( time_cos * inverse_zoom )
													.SetZ( time_sin * inverse_zoom ) );
				break;
			case Framework::Test::Test_Camera_LookAt::RotationPlane::YZ:
				camera_transform.SetTranslation( camera_position = camera_position
													.SetY( time_sin * inverse_zoom )
													.SetZ( time_cos * inverse_zoom ) );
				break;
			default:
				break;
		}

		const auto lookAt_direction( ( target - camera_position ).Normalized() );

		if( method_lookAt == LookAtMethod::LookAtMatrix )
		{
			shader->SetMatrix( "transformation_view", Matrix::LookAt( camera_position, lookAt_direction,
																	  rotation_plane == RotationPlane::ZX
																		? Vector3::Up()
																		: Math::Cross( Vector3::Right(), lookAt_direction ).Normalize() ) );
		}
		else if( method_lookAt == LookAtMethod::QuaternionLookRotation ) // Alternatively, we can also take the inverse of the camera object's transform matrix, yielding the same result.
		{
			camera_transform.SetRotation( Quaternion::LookRotation( lookAt_direction,
																	rotation_plane == RotationPlane::ZX
																		? Vector3::Up()
																		: Math::Cross( Vector3::Right(), lookAt_direction ).Normalize() ) );

			shader->SetMatrix( "transformation_view", camera_transform.GetInverseOfFinalMatrix() );
		}
		else if( method_lookAt == LookAtMethod::QuaternionLookRotation_Naive )
		{
			camera_transform.SetRotation( Quaternion::LookRotation_Naive( lookAt_direction,
																		  rotation_plane == RotationPlane::ZX
																			? Vector3::Up()
																			: Math::Cross( Vector3::Right(), lookAt_direction ).Normalize() ) );

			shader->SetMatrix( "transformation_view", camera_transform.GetInverseOfFinalMatrix() );
		}
		else if( method_lookAt == LookAtMethod::ManualRotationViaQuaternionSlerp )
		{
			// Counter-clockwise rotation of the scene = clockwise rotation of the camera.
			const auto rotation = Math::QuaternionToMatrix3x3( Math::Slerp( rotation_plane == RotationPlane::ZX
																				? Math::EulerToQuaternion( -90_deg, 0_deg, 0_deg )
																				: Math::EulerToQuaternion( 0_deg, -90_deg, 0_deg ),
																			rotation_plane == RotationPlane::ZX
																				? Math::EulerToQuaternion( +90_deg, 0_deg, 0_deg )
																				: Math::EulerToQuaternion( 0_deg, +90_deg, 0_deg ),
																			time_mod_2_pi / Constants< float >::Pi() ) );
			const auto translation = Vector3::Forward() / zoom; // Moving the scene away (i.e, toward -Z) = inverse of zooming the camera out (i.e, moving toward +Z).

			shader->SetMatrix( "transformation_view", Matrix4x4( rotation, translation ) ); // Create the view matrix from the viewpoint of the scene objects' transformation.
		}
		else if( method_lookAt == LookAtMethod::ManualRotationViaEulerToQuaternion )
		{
			const auto rotation    = rotation_plane == RotationPlane::ZX
										? Math::EulerToMatrix3x3( 360_deg * time_mod_2_pi / Constants< float >::Two_Pi(), 0_deg, 0_deg )
										: Math::EulerToMatrix3x3( 0_deg, 360_deg * time_mod_2_pi / Constants< float >::Two_Pi(), 0_deg ); // Counter-clockwise rotation of the scene = clockwise rotation of the camera.
			const auto translation = Vector3::Forward() / zoom; // Moving the scene away (i.e, toward -Z) = inverse of zooming the camera out (i.e, moving toward +Z).

			shader->SetMatrix( "transformation_view", Matrix4x4( rotation, translation ) ); // Create the view matrix from the viewpoint of the scene objects' transformation.
		}
	}

	void Test_Camera_LookAt::OnRenderImGui()
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

			ImGui::SliderFloat( "Time Multiplier", &time_multiplier, 0.01f, 5.0f, "x %.2f", ImGuiSliderFlags_Logarithmic ); ImGui::SameLine(); if( ImGui::Button( "Reset##time_multiplier" ) ) time_multiplier = 1.0f;

			ImGui::ProgressBar( time_mod_1,	   ImVec2( 0.0f, 0.0f ) ); ImGui::SameLine(); ImGui::TextUnformatted( "Time % 1" );
			ImGui::ProgressBar( time_mod_2_pi / Constants< float >::Two_Pi(), ImVec2( 0.0f, 0.0f ) ); ImGui::SameLine(); ImGui::TextUnformatted( "Time % (2 * Pi)" );
			ImGui::SliderFloat( "Sin(Time) ", &sin_time, -1.0f, 1.0f, "%.1f",		ImGuiSliderFlags_NoInput );
			ImGui::SliderFloat( "Cos(Time) ", &cos_time, -1.0f, 1.0f, "%.1f",		ImGuiSliderFlags_NoInput );

			ImGui::SeparatorText( "Camera Settings" );
			/* ______________________________________ */

			ImGui::SliderFloat( "Zoom ", &zoom, 0.05f, 0.5f, "%.2f", ImGuiSliderFlags_Logarithmic ); ImGui::SameLine(); if( ImGui::Button( "Reset##zoom" ) ) zoom = 0.25f;

			ImGui::TextUnformatted( "Rotation plane:" ); ImGui::SameLine();

			if( ImGui::RadioButton( "ZX", ( int* )&rotation_plane, ( int )RotationPlane::ZX ) )
				ResetCameraTranslation();
			ImGui::SameLine();
			if( ImGui::RadioButton( "YZ", ( int* )&rotation_plane, ( int )RotationPlane::YZ ) )
				ResetCameraTranslation();

			ImGui::NewLine();

			ImGui::TextUnformatted( "Look at Origin Method: (They should produce similar (*) results!)" ); ImGui::SameLine();
			ImGuiUtility::HelpMarker( "Due to utilization of sin(time) & cos(time) in the first three methods & time % (2*Pi) in the latter two, "
									  "starting angle offsets may be different, but the overall effect of each method should be the same.", "(*)" );

			ImGui::RadioButton( "Matrix::LookAt()",						( int* )&method_lookAt, ( int )LookAtMethod::LookAtMatrix					);
			ImGui::RadioButton( "Quaternion::LookRotation()",			( int* )&method_lookAt, ( int )LookAtMethod::QuaternionLookRotation			);
			ImGui::RadioButton( "Quaternion::LookRotation() (Naive)",	( int* )&method_lookAt, ( int )LookAtMethod::QuaternionLookRotation_Naive	); ImGui::SameLine();
			ImGuiUtility::HelpMarker( "Naive version constructs a basis with the provided arguments, as the fast version does. "
									  "While the fast version creates a rotation matrix from this basis and converts the matrix to a quaternion (which is computationally more efficient), "
									  "the naive version calculates the angle between the forward and look_at vectors and crosses them to get a rotation axis. "
									  "It then uses this angle & axis to construct the quaternion directly, which is easier to derive, but less efficient to compute." );
			ImGui::RadioButton( "Manual Rotation via Math::Slerp( angle: -90 -> +90 )", ( int* )&method_lookAt, ( int )LookAtMethod::ManualRotationViaQuaternionSlerp );
			ImGui::RadioButton( "Manual Rotation via Math::EulerToQuaternion( angle: 360 * time % (2 * Pi) )", ( int* )&method_lookAt, ( int )LookAtMethod::ManualRotationViaEulerToQuaternion );
		}

		ImGui::End();
	}

	void Test_Camera_LookAt::ResetCameraTranslation()
	{
		camera_transform.SetTranslation( Vector3::Backward() / zoom );
	}
}
