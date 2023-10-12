#pragma once

// Framework Includes.
#include "Math/Polar.hpp"

#include "Renderer/CameraController_Flight.h"
#include "Renderer/Drawable.h"
#include "Renderer/Material.h"
#include "Renderer/Texture.h"

#include "Test/Test.hpp"

// std Includes.
#include <memory>

namespace Framework::Test
{
	class Test_Lighting : public Test< Test_Lighting >
	{
		friend Test< Test_Lighting >;

	public:
		Test_Lighting();
		~Test_Lighting() override;

	private:
		/* Test "overrides". */
		void OnKeyboardEvent( const Platform::KeyCode key_code, const Platform::KeyAction action, const Platform::KeyMods mods );
		void OnProcessInput();
		void OnUpdate();
		void OnRender();
		void OnRenderImGui();

		void ResetCameraTranslation();
		void ResetCameraRotation();
		float ResetCameraMoveSpeed();

	private:
		std::unique_ptr< VertexArray > cube_vertex_array;
		std::unique_ptr< Shader > shader_lit;
		std::unique_ptr< Shader > shader_unlit;
		std::unique_ptr< Material > material_cube;
		std::unique_ptr< Material > material_light_source;
		std::unique_ptr< Drawable > drawable_cube;
		std::unique_ptr< Drawable > drawable_light_source;
		//std::unique_ptr< Texture > texture_test_cube;

		Transform transform_cube;
		Transform transform_light_source;

		CameraController_Flight camera_controller_flight;

		Vector3 camera_delta_position;
		float camera_displacement;

		bool input_is_enabled;

		static constexpr Vector3 rotation_axis = Vector3( Constants< float >::Sqrt_Half(), Constants< float >::Sqrt_Half(), 0.0f );
	};
}
