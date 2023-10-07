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

	/* Uniform Setters. */
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

			CopyUniformToBlob( value.Data(), uniform_information );

			shader->SetVector( name, value );

			return *this;
		}

		Material& SetColor3( const std::string& name, const Color3& value );
		Material& SetColor4( const std::string& name, const Color4& value );

		template< Concepts::Arithmetic Type, std::size_t RowSize, std::size_t ColumnSize >
			requires Concepts::NonZero< RowSize > && Concepts::NonZero< ColumnSize >
		Material& SetMatrix( const std::string& name, const Math::Matrix< Type, RowSize, ColumnSize >& value )
		{
			const auto& uniform_information = shader->GetUniformInformation( name );
			ASSERT( uniform_information.size == sizeof( Math::Matrix< Type, RowSize, ColumnSize > ) );
			ASSERT( uniform_information.size + uniform_information.offset - 1 <= ( int )uniform_blob.size() );

			CopyUniformToBlob( value.Data(), uniform_information );

			shader->SetMatrix( name, value );

			return *this;
		}

		Material& SetTextureSampler1D( const std::string& name, const int value );
		Material& SetTextureSampler2D( const std::string& name, const int value );
		Material& SetTextureSampler3D( const std::string& name, const int value );

	/* Uniform Getters. */
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

		float GetFloat( const std::string& name );
		int GetInt( const std::string& name );
		bool GetBool( const std::string& name );

		template< Concepts::Arithmetic Coordinate, std::size_t Size >
			requires Concepts::NonZero< Size >
		const Math::Vector< Coordinate, Size >& GetVector( const std::string& name )
		{
			return GetUniformValue< Math::Vector< Coordinate, Size > >( shader->GetUniformInformation( name ) );
		}

		const Color3& GetColor3( const std::string& name );
		const Color4& GetColor4( const std::string& name );

		template< Concepts::Arithmetic Type, std::size_t RowSize, std::size_t ColumnSize >
			requires Concepts::NonZero< RowSize > && Concepts::NonZero< ColumnSize >
		const Math::Matrix< Type, RowSize, ColumnSize >& GetMatrix( const std::string& name )
		{
			return GetUniformValue< Math::Matrix< Type, RowSize, ColumnSize > >( shader->GetUniformInformation( name ) );
		}

		int GetTextureSampler1D( const std::string& name );
		int GetTextureSampler2D( const std::string& name );
		int GetTextureSampler3D( const std::string& name );

	private:
		Material& CopyUniformToBlob( const void* value, const ShaderUniformInformation& uniform_info );

	public:
		Shader* const shader;

	private:
		std::vector< char > uniform_blob;
	};
}
