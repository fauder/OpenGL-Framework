#pragma once

// Project Includes.
#include "Test.hpp"
#include "../Drawable.h"
#include "../Texture.h"
#include "../Transform.h"

// std Includes.
#include <memory>

namespace Framework::Test
{
	class Test_Camera : public Test< Test_Camera >
	{
		friend Test< Test_Camera >;

	public:
		Test_Camera( Renderer& renderer );
		~Test_Camera() override;

	protected:
		void OnRender();

	private:
		std::unique_ptr< VertexArray > cube_vertex_array;
		std::unique_ptr< Shader > shader;
		std::unique_ptr< Drawable > cube_1;
		std::unique_ptr< Drawable > ground_plane;
		std::unique_ptr< Texture > texture_container;

		Transform camera_transform;

		static constexpr Vector3 rotation_axis = Vector3( Constants< float >::Sqrt_Half(), Constants< float >::Sqrt_Half(), 0.0f );
	};
}
