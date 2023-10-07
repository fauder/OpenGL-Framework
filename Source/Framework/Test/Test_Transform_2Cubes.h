#pragma once

// Framework Includes.
#include "Renderer/Drawable.h"
#include "Renderer/Texture.h"

#include "Test/Test.hpp"

// std Includes.
#include <memory>

namespace Framework::Test
{
	class Test_Transfom_2Cubes : public Test< Test_Transfom_2Cubes >
	{
		friend Test< Test_Transfom_2Cubes >;

	public:
		Test_Transfom_2Cubes( Renderer& renderer );
		~Test_Transfom_2Cubes() override;

	protected:
		void OnRender();

	private:
		std::unique_ptr< VertexArray > cube_vertex_array;
		std::unique_ptr< Shader > shader;
		std::unique_ptr< Drawable > cube_1;
		std::unique_ptr< Drawable > cube_2;
		std::unique_ptr< Texture > texture_container;
		std::unique_ptr< Texture > texture_awesome_face;

		static constexpr Vector3 rotation_axis = Vector3( Constants< float >::Sqrt_Half(), Constants< float >::Sqrt_Half(), 0.0f );
	};
}
