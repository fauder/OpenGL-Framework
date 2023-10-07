// Project Includes.
#include "Material.h"

namespace Framework
{
	Material::Material( Shader* const shader )
		:
		shader( shader )
	{
		const auto& uniform_info_map = shader->GetUniformInformations();

		int total_size = 0;
		for( auto& [ name, uniform_info ] : uniform_info_map )
			total_size += uniform_info.size;

		uniform_blob.resize( total_size );
	}

	Material& Material::SetFloat( const std::string& name, const float value )
	{
		const auto& uniform_information = shader->GetUniformInformation( name );
		ASSERT( uniform_information.size == sizeof( float ) );
		ASSERT( uniform_information.size + uniform_information.offset - 1 <= ( int )uniform_blob.size() );

		CopyUniformToBlob( &value, uniform_information );

		shader->SetFloat( name, value );

		return *this;
	}

	Material& Material::SetInt( const std::string& name, const int value )
	{
		const auto& uniform_information = shader->GetUniformInformation( name );
		ASSERT( uniform_information.size == sizeof( int ) );
		ASSERT( uniform_information.size + uniform_information.offset - 1 <= ( int )uniform_blob.size() );

		CopyUniformToBlob( &value, uniform_information );

		shader->SetInt( name, value );

		return *this;
	}

	Material& Material::SetBool( const std::string& name, const bool value )
	{
		const auto& uniform_information = shader->GetUniformInformation( name );
		ASSERT( uniform_information.size == sizeof( bool ) );
		ASSERT( uniform_information.size + uniform_information.offset - 1 <= ( int )uniform_blob.size() );

		CopyUniformToBlob( &value, uniform_information );

		shader->SetBool( name, value );

		return *this;
	}

	Material& Material::SetColor( const std::string& name, const Color3& value )
	{
		const auto& uniform_information = shader->GetUniformInformation( name );
		ASSERT( uniform_information.size == sizeof( Color3 ) );
		ASSERT( uniform_information.size + uniform_information.offset - 1 <= ( int )uniform_blob.size() );

		CopyUniformToBlob( value.Data(), uniform_information );

		shader->SetColor( name, value );

		return *this;
	}

	Material& Material::SetColor( const std::string& name, const Color4& value )
	{
		const auto& uniform_information = shader->GetUniformInformation( name );
		ASSERT( uniform_information.size == sizeof( Color4 ) );
		ASSERT( uniform_information.size + uniform_information.offset - 1 <= ( int )uniform_blob.size() );

		CopyUniformToBlob( value.Data(), uniform_information );

		shader->SetColor( name, value );

		return *this;
	}

	Material& Material::SetTextureSampler1D( const std::string& name, const int value )
	{
		const auto& uniform_information = shader->GetUniformInformation( name );
		ASSERT( uniform_information.size == sizeof( int ) );
		ASSERT( uniform_information.size + uniform_information.offset - 1 <= ( int )uniform_blob.size() );

		CopyUniformToBlob( &value, uniform_information );

		shader->SetTextureSampler1D( name, value );

		return *this;
	}

	Material& Material::SetTextureSampler2D( const std::string& name, const int value )
	{
		const auto& uniform_information = shader->GetUniformInformation( name );
		ASSERT( uniform_information.size == sizeof( int ) );
		ASSERT( uniform_information.size + uniform_information.offset - 1 <= ( int )uniform_blob.size() );

		CopyUniformToBlob( &value, uniform_information );

		shader->SetTextureSampler2D( name, value );

		return *this;
	}

	Material& Material::SetTextureSampler3D( const std::string& name, const int value )
	{
		const auto& uniform_information = shader->GetUniformInformation( name );
		ASSERT( uniform_information.size == sizeof( int ) );
		ASSERT( uniform_information.size + uniform_information.offset - 1 <= ( int )uniform_blob.size() );

		CopyUniformToBlob( &value, uniform_information );

		shader->SetTextureSampler3D( name, value );

		return *this;
	}

	Material& Material::CopyUniformToBlob( const void* value, const ShaderUniformInformation& uniform_info )
	{
		std::memcpy( uniform_blob.data() + uniform_info.offset, static_cast< const char* >( value ), uniform_info.size );

		return *this;
	}
}