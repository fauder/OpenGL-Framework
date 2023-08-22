#pragma once

// Project Includes.
#include "Test.hpp"
#include "../Camera.h"
#include "../Drawable.h"
#include "../Polar.hpp"
#include "../Texture.h"

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

	private:
		/* Test "overrides". */
		void OnKeyboardEvent( const Platform::KeyCode key_code, const Platform::KeyAction action, const Platform::KeyMods mods );
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
		Math::Polar3_Spherical_Game_RightHanded camera_direction_spherical;
		Vector3 camera_look_at_direction;
		float displacement;

		bool input_is_enabled;

		static constexpr Vector3 rotation_axis = Vector3( Constants< float >::Sqrt_Half(), Constants< float >::Sqrt_Half(), 0.0f );
	};
}
