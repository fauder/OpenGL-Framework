#pragma once

// Framework Includes.
#include "Renderer/Drawable.h"
#include "Renderer/Texture.h"
#include "Renderer/Camera.h"

#include "Test/Test.hpp"

// std Includes.
#include <memory>

namespace Framework::Test
{
	class Test_Camera_LookAt : public Test< Test_Camera_LookAt >
	{
		friend Test< Test_Camera_LookAt >;

	public:
		Test_Camera_LookAt();
		//~Test_Camera_LookAt() override;

	protected:
		void OnUpdate();
		void OnRender();
		void OnRenderImGui();

		void ResetCameraTranslation();

	private:
		std::unique_ptr< VertexArray > cube_vertex_array;
		std::unique_ptr< Material > material;
		std::unique_ptr< Shader > shader;
		/* This test does not use a Drawable as the Drawable (currently) sets the view matrix by querying it from the Camera, which may or may not be set directly in this test
		 * (i.e., constructed by some other means and uploaded to shader on OnRender() ). Using a Drawable would cause the view matrix to be set again and lead to incorrect behaviour. */
		//std::unique_ptr< Drawable > cube_1;
		std::unique_ptr< Texture > texture_test_cube;

		Transform cube_transform;

		enum class LookAtMethod : int
		{
			LookAtMatrix,
			QuaternionLookRotation,
			QuaternionLookRotation_Naive,
			ManualRotationViaQuaternionSlerp,
			ManualRotationViaEulerToQuaternion
		} method_lookAt;

		enum class RotationPlane : int
		{
			ZX,
			YZ
		} rotation_plane;

		float zoom;
	};
}
