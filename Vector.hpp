#pragma once

// std Includes.
#include <array>

template< typename Type, unsigned int Size, 
		  typename = typename std::enable_if< std::is_arithmetic_v< Type > > >
class VectorBase
{
public:
/* Constructors. */
	VectorBase()
		:
		data{ 0 }
	{
	}
	template< typename... Values >
	VectorBase( Values... values )
		: 
		data{ values... }
	{
	}

	template< typename = std::enable_if_t< Size >= 1 > >
	VectorBase( const Type x )
		:
		data{ x, Type( 0 ), Type( 0 ) }
	{
	}

	template< typename = std::enable_if_t< Size >= 2 > >
	VectorBase( const Type x, const Type y )
		:
		data{ x, y, Type( 0 ) }
	{
	}

	template< typename = std::enable_if_t< Size >= 3 > >
	VectorBase( const Type x, const Type y, const Type z )
		:
		data{ x, y, z }
	{
	}

	template< typename = std::enable_if_t< Size >= 4 > >
	VectorBase( const Type x, const Type y, const Type z, const Type w )
		:
		data{ x, y, z, w }
	{
	}

	~VectorBase()
	{
	}

/* Getters & Setters. */
	const Type* Data() const { return data.data(); };

	template< typename = std::enable_if_t< Size >= 1 > >
	Type X() const { return data[ 0 ]; };
	template< typename = std::enable_if_t< Size >= 2 > >
	Type Y() const { return data[ 1 ]; };
	template< typename = std::enable_if_t< Size >= 3 > >
	Type Z() const { return data[ 2 ]; };
	template< typename = std::enable_if_t< Size >= 4 > >
	Type W() const { return data[ 3 ]; };

	template< typename... Values >
	VectorBase& Set( Values... values )
	{
		// Utilize fold expressions with a lambda to "loop over" the parameter pack.
		int i = 0;
		( /* Lambda: */ [&] { data[ i++ ] = values; }( ), ... );

		return *this;
	}
protected:
	std::array< Type, Size > data;
};

using Vector2  = VectorBase< float,  2 >;
using Vector3  = VectorBase< float,  3 >;
using Vector4  = VectorBase< float,  4 >;
using Vector2D = VectorBase< double, 2 >;
using Vector3D = VectorBase< double, 3 >;
using Vector4D = VectorBase< double, 4 >;
using Vector2I = VectorBase< int,    2 >;
using Vector3I = VectorBase< int,    3 >;
using Vector4I = VectorBase< int,    4 >;