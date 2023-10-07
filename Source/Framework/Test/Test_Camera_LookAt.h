#pragma once

// Framework Includes.
#include "Renderer//Drawable.h"
#include "Renderer//Texture.h"
#include "Renderer//Camera.h"

#include "Test/Test.hpp"

// std Includes.
#include <memory>

namespace Framework::Test
{
	class Test_Camera_LookAt : public Test< Test_Camera_LookAt >
	{
		friend Test< Test_Camera_LookAt >;

	public:
		Test_Camera_LookAt( Renderer& renderer );
		~Test_Camera_LookAt() override;

	protected:
		void OnUpdate();
		void OnRender();
		void OnRenderImGui();

		void ResetCameraTranslation();

	private:
		std::unique_ptr< VertexArray > cube_vertex_array;
		std::unique_ptr< Shader > shader;
		std::unique_ptr< Drawable > cube_1;
		std::unique_ptr< Texture > texture_test_cube;

		Transform camera_transform;

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

		static constexpr Vector3 rotation_axis = Vector3( Constants< float >::Sqrt_Half(), Constants< float >::Sqrt_Half(), 0.0f );
	};
}
