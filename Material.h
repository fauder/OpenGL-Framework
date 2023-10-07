#pragma once

// Project Includes.
#include "Color.hpp"
#include "Shader.h"
#include "ShaderTypeInformation.h"

// std Includes.
#include <unordered_map>

namespace Framework
{
	class Material
	{
	public:
		Material( Shader* const shader );

		Material& SetFloat( const std::string& name, const float value );
		Material& SetInt( const std::string& name, const int value );
		Material& SetBool( const std::string& name, const bool value );

		template< Concepts::Arithmetic Coordinate, std::size_t Size >
			requires Concepts::NonZero< Size >
		Material& SetVector( const std::string& name, const Math::Vector< Coordinate, Size >& value )
		{
			const auto& uniform_information = shader->GetUniformInformation( name );
			ASSERT( uniform_information.size == sizeof( Math::Vector< Coordinate, Size > ) );
			ASSERT( uniform_information.size + uniform_information.offset - 1 <= ( int )uniform_blob.size() );

			return CopyUniformToBlob( value.Data(), uniform_information );
		}

		Material& SetColor( const std::string& name, const Color3& color );
		Material& SetColor( const std::string& name, const Color4& color );

		// TODO: Material& SetMatrix( const std::string& name, const Color4& color );

		Material& SetTextureSampler1D( const std::string& name, const int value );
		Material& SetTextureSampler2D( const std::string& name, const int value );
		Material& SetTextureSampler3D( const std::string& name, const int value );

		// TODO: Implement GetFloat() etc.

		template< typename Type >
		const Type& GetUniformValue( const int offset ) const
		{
			ASSERT( sizeof( Type ) + offset - 1 <= ( int )uniform_blob.size() );

			return *reinterpret_cast< const Type* >( uniform_blob.data() + offset );
		}

		template< typename Type >
		const Type& GetUniformValue( const ShaderUniformInformation& uniform_info ) const
		{
			return GetUniformValue< Type >( uniform_info.offset );
		}

	public:
		Shader* const shader;

	private:
		Material& CopyUniformToBlob( const void* value, const ShaderUniformInformation& uniform_info );

	private:
		std::vector< char > uniform_blob;
	};
}
