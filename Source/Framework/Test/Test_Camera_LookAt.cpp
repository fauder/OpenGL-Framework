// Framework Includes.
#include "Core/Log_ImGui.h"

#include "Math/Matrix.h"

#include "Renderer/MeshUtility.hpp"
#include "Renderer/Primitive/Primitive_Cube.h"

#include "Test/Test_Camera_LookAt.h"

#include "Utility/ImGuiUtility.h"

using namespace Framework::Math::Literals;

namespace Framework::Test
{
	Test_Camera_LookAt::Test_Camera_LookAt()
		:
		method_lookAt( LookAtMethod::LookAtMatrix ),
		rotation_plane( RotationPlane::ZX ),
		zoom( 0.25f )
	{
		using namespace Framework;

		shader = std::make_unique< Shader >( "Source/Asset/Shader/textured_mix.vertex", "Source/Asset/Shader/textured_mix.fragment", "Textured Mix" );

		constexpr auto vertices = MeshUtility::Interleave( Primitives::Cube::Positions, Primitives::Cube::UVs_CubeMap );

		VertexBuffer vertex_buffer( vertices.data(), static_cast< unsigned int >( vertices.size() ) );

		VertexBufferLayout vertex_buffer_layout;
		vertex_buffer_layout.Push< float >( 3 ); // NDC positions.
		vertex_buffer_layout.Push< float >( 2 ); // Tex. coords.
		//vertex_buffer_layout.Push< float >( 4 ); // Vertex colors.
		cube_vertex_array = std::make_unique< VertexArray >( vertex_buffer, vertex_buffer_layout );

		material = std::make_unique< Material >( shader.get(), "Cube" );

		/* This test does not use a Drawable as the Drawable (currently) sets the view matrix by querying it from the Camera, which may or may not be set directly in this test
		 * (i.e., constructed by other means & uploaded to shader in OnRender() ). Using a Drawable would cause the view matrix to be set again on Submit() & lead to incorrect behaviour. */
		/*cube_1 = std::make_unique< Drawable >( material.get(), &cube_transform, cube_vertex_array.get() );

		renderer.AddDrawable( cube_1.get() );*/

		renderer.SetPolygonMode( PolygonMode::FILL );

		texture_test_cube = std::make_unique< Texture >( "Asset/Texture/test_tex_cube.png", GL_RGBA );

		texture_test_cube->ActivateAndBind( GL_TEXTURE0 );

		shader->Bind();
		material->SetTextureSampler2D( "texture_sampler_1", 0 );

		// Initial camera position and rotation:
		ResetCameraTranslation();
	}

	/*Test_Camera_LookAt::~Test_Camera_LookAt()
	{
		Test::~Test();
	}*/

	void Test_Camera_LookAt::OnUpdate()
	{
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
	}

	void Test_Camera_LookAt::OnRender()
	{
		const Vector3 target( ZERO_INITIALIZATION ); // Look at the origin.
		Vector3 camera_position = camera_transform.GetTranslation();

		const auto lookAt_direction( ( target - camera_position ).Normalized() );

		if( method_lookAt == LookAtMethod::LookAtMatrix )
		{
			material->SetMatrix( "transform_view", Matrix::LookAt( camera_position, lookAt_direction,
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

			material->SetMatrix( "transform_view", camera_transform.GetInverseOfFinalMatrix() );
		}
		else if( method_lookAt == LookAtMethod::QuaternionLookRotation_Naive )
		{
			camera_transform.SetRotation( Quaternion::LookRotation_Naive( lookAt_direction,
																		  rotation_plane == RotationPlane::ZX
																			? Vector3::Up()
																			: Math::Cross( Vector3::Right(), lookAt_direction ).Normalize() ) );

			material->SetMatrix( "transform_view", camera_transform.GetInverseOfFinalMatrix() );
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

			material->SetMatrix( "transform_view", Matrix4x4( rotation, translation ) ); // Create the view matrix from the viewpoint of the scene objects' transformation.
		}
		else if( method_lookAt == LookAtMethod::ManualRotationViaEulerToQuaternion )
		{
			const auto rotation    = rotation_plane == RotationPlane::ZX
										? Math::EulerToMatrix3x3( 360_deg * time_mod_2_pi / Constants< float >::Two_Pi(), 0_deg, 0_deg )
										: Math::EulerToMatrix3x3( 0_deg, 360_deg * time_mod_2_pi / Constants< float >::Two_Pi(), 0_deg ); // Counter-clockwise rotation of the scene = clockwise rotation of the camera.
			const auto translation = Vector3::Forward() / zoom; // Moving the scene away (i.e, toward -Z) = inverse of zooming the camera out (i.e, moving toward +Z).

			material->SetMatrix( "transform_view", Matrix4x4( rotation, translation ) ); // Create the view matrix from the viewpoint of the scene objects' transformation.
		}

		/* For this test, "manually" issue a draw call for the cube, as we can not use a Drawable for the reasons stated above. */

		material->SetMatrix( "transform_projection", camera.GetProjectionMatrix() );

		cube_vertex_array->Bind();
		shader->Bind();

		material->SetMatrix( "transform_world",	cube_transform.GetFinalMatrix() );

		material->SetMatrix( "transform_projection", camera.GetProjectionMatrix() );

		GLCALL( glDrawArrays( GL_TRIANGLES, 0, cube_vertex_array->VertexCount() ) );
	}

	void Test_Camera_LookAt::OnRenderImGui()
	{
		ImGuiUtility::SetNextWindowPos( ImGuiUtility::HorizontalWindowPositioning::LEFT, ImGuiUtility::VerticalWindowPositioning::CENTER );
		if( ImGui::Begin( "Test: Camera ", nullptr, CurrentImGuiWindowFlags() | ImGuiWindowFlags_AlwaysAutoResize ) )
		{
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

		ImGuiUtility::SetNextWindowPos( ImGuiUtility::HorizontalWindowPositioning::RIGHT, ImGuiUtility::VerticalWindowPositioning::BOTTOM );
		Log::Dump( *material, CurrentImGuiWindowFlags() );

		ImGui::End();
	}

	void Test_Camera_LookAt::ResetCameraTranslation()
	{
		camera_transform.SetTranslation( Vector3::Backward() / zoom );
	}
}
