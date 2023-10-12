// Framework Includes.
#include "Math/Matrix.h"

#include "Renderer/MeshUtility.hpp"
#include "Renderer/Primitive/Primitive_Cube.h"

#include "Test/Test_Transform_2Cubes.h"

namespace Framework::Test
{
	using namespace Framework::Math::Literals;

	Test_Transfom_2Cubes::Test_Transfom_2Cubes()
	{
		using namespace Framework;

		shader = std::make_unique< Shader >( "Source/Asset/Shader/textured_mix.vertex", "Source/Asset/Shader/textured_mix.fragment", "Textured Mix" );

		constexpr auto vertices = MeshUtility::Interleave( Primitives::Cube::Positions, Primitives::Cube::UVs, Primitives::Cube::VertexColors );

		VertexBuffer vertex_buffer( vertices.data(), static_cast< unsigned int >( vertices.size() ) );

		VertexBufferLayout vertex_buffer_layout;
		vertex_buffer_layout.Push< float >( 3 ); // NDC positions.
		vertex_buffer_layout.Push< float >( 2 ); // Tex. coords.
		vertex_buffer_layout.Push< float >( 4 ); // Vertex colors.
		cube_vertex_array = std::make_unique< VertexArray >( vertex_buffer, vertex_buffer_layout );

		cube_material = std::make_unique< Material >( shader.get(), "Cube" );

		cube_1 = std::make_unique< Drawable >( cube_material.get(), &cube_1_transform, cube_vertex_array.get() );
		cube_2 = std::make_unique< Drawable >( cube_material.get(), &cube_2_transform, cube_vertex_array.get() );

		renderer.AddDrawable( cube_1.get() );
		renderer.AddDrawable( cube_2.get() );
		renderer.SetPolygonMode( PolygonMode::FILL );

		texture_container    = std::make_unique< Texture >( "Asset/Texture/container.jpg", GL_RGB );
		texture_awesome_face = std::make_unique< Texture >( "Asset/Texture/awesomeface.png", GL_RGBA );

		texture_container->ActivateAndBind( GL_TEXTURE0 );
		texture_awesome_face->ActivateAndBind( GL_TEXTURE1 );

		shader->Bind();
		cube_material->SetTextureSampler2D( "texture_sampler_1", 0 );
		cube_material->SetTextureSampler2D( "texture_sampler_2", 1 );

		//cube_material->SetBool( "use_vertex_color", 1 );

		camera_transform.SetTranslation( Vector3::Backward() * 3.0f );
	}

	/*Test_Transfom_2Cubes::~Test_Transfom_2Cubes()
	{
		Test::~Test();
	}*/

	void Test_Transfom_2Cubes::OnUpdate()
	{
		const float sin_time = Math::Sin( Radians( time_current ) );

		cube_1->transform->SetRotation( Quaternion( sin_time * 65.0_deg, rotation_axis ) );
		cube_1->transform->SetTranslation( sin_time * Vector3::Right() );
		cube_1->transform->SetScaling( Vector3( UNIFORM_INITIALIZATION, Math::Clamp( Math::Abs( sin_time ), 0.1f, 1.0f ) ) );

		cube_2->transform->SetRotation( Quaternion( -sin_time * 65.0_deg, rotation_axis ) );
		cube_2->transform->SetTranslation( -sin_time * Vector3::Up() );
		cube_2->transform->SetScaling( Vector3( UNIFORM_INITIALIZATION, Math::Clamp( Math::Abs( sin_time * 0.5f ), 0.1f, 1.0f ) ) );
	}
}
