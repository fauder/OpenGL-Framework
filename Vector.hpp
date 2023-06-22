#pragma once

// Project Includes.
#include "MatrixBase.hpp"

// std Includes.
#include <array>
#include <cmath>

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
	const Type* Data() const { return data; };
	Type& operator[] ( const unsigned int index ) { return data[ index ]; }
	const Type& operator[] ( const unsigned int index ) const { return data[ index ]; }

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

/* Other Queries. */
	constexpr unsigned int Dimension() const { return Size; }

/* Arithmetic Operations. */
	VectorBase operator* ( const Type scalar ) const
	{
		VectorBase result( *this );
		for( int i = 0; i < Size; i++ )
			result.data[ i ] *= scalar;

		return result;
	}

	VectorBase& operator*= ( const Type scalar )
	{
		for( int i = 0; i < Size; i++ )
			data[ i ] *= scalar;

		return *this;
	}

	Type Dot( const VectorBase& other ) const // Practically we won't use this for any vectors other than 3D & 4D, but no need to restrict.
	{
		const Type result( 0 );

		for( int i = 0; i < Size; i++ )
			result += data[ i ] * other.data[ i ];
	}

	/* With self. */
	Type Dot() const // Practically we won't use this for any vectors other than 3D & 4D, but no need to restrict.
	{
		Type result( 0 );

		for( int i = 0; i < Size; i++ )
			result += data[ i ] * data[ i ];

		return result;
	}

	template< typename = std::enable_if_t< Size == 3 > > // Cross product is only defined for vectors of 3 & 7 dimensions apparently, but practically we only need it for 3D.
	VectorBase Cross( const VectorBase& other ) const
	{
		// u X v = ( u2v3 - u3v2, u3v1 - u1v3, u1v2 - u2v1 )
		return VectorBase( data[ 1 ] * other.data[ 2 ] - data[ 2 ] * other.data[ 1 ],
						   data[ 2 ] * other.data[ 0 ] - data[ 0 ] * other.data[ 2 ],
						   data[ 0 ] * other.data[ 1 ] - data[ 1 ] * other.data[ 0 ] );
	}

	Type SquareMagnitude() const { return Dot(); }

	template< typename = std::enable_if_t< std::is_floating_point_v< Type > > >
	Type Magnitude() const { return std::sqrt( SquareMagnitude() ); }

	template< typename = std::enable_if_t< std::is_floating_point_v< Type > > >
	VectorBase Normalized() const
	{
		const Type one_over_magnitude = 1.0f / Magnitude();
		return this * one_over_magnitude;
	}

	template< typename = std::enable_if_t< std::is_floating_point_v< Type > > >
	VectorBase& Normalize()
	{
		const Type one_over_magnitude = 1.0f / Magnitude();
		*this *= one_over_magnitude;
		return *this;
	}

	template< unsigned int ColumnSize >
	VectorBase< Type, ColumnSize > operator* ( const MatrixBase< Type, Size, ColumnSize >& transform_matrix ) const
	{
		VectorBase< Type, ColumnSize > vector_transformed;
		for( auto j = 0; j < ColumnSize; j++ )
			for( auto k = 0; k < Size; k++ )
				vector_transformed[ j ] += data[ k ] * transform_matrix[ k ][ j ];

		return vector_transformed;
	}

protected:
	Type data[ Size ];
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