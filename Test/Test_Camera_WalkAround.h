#pragma once

// Project Includes.
#include "Test.hpp"
#include "../Drawable.h"
#include "../Texture.h"
#include "../Camera.h"

// std Includes.
#include <memory>

namespace Framework::Test
{
	class Test_Camera_WalkAround : public Test< Test_Camera_WalkAround >
	{
		friend Test< Test_Camera_WalkAround >;

	public:
		Test_Camera_WalkAround( Renderer& renderer );
		~Test_Camera_WalkAround() override;

	protected:
		void OnProcessInput();
		void OnUpdate();
		void OnRender();
		void OnRenderImGui();

		void ResetCameraTranslation();
		float ResetCameraMoveSpeed();

	private:
		std::unique_ptr< VertexArray > cube_vertex_array;
		std::unique_ptr< Shader > shader;
		std::unique_ptr< Drawable > cube_1;
		std::unique_ptr< Texture > texture_test_cube;

		Camera camera;

		float camera_move_speed;

		Vector3 delta_position;
		float displacement;

		static constexpr Vector3 rotation_axis = Vector3( Constants< float >::Sqrt_Half(), Constants< float >::Sqrt_Half(), 0.0f );
	};
}
