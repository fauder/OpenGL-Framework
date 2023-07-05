#pragma once

// Project Includes.
#include "Concepts.h"
#include "Math.h"
#include "Matrix.hpp"
#include "Utility.h"

// std Includes.
#include <array>
#include <cmath>
#include <utility>

namespace Framework
{
	template< Concepts::Arithmetic Coordinate, size_t Size >
		requires Concepts::Nonzero< Size >
	class VectorBase
	{
	public:
	/* Constructors. */
		constexpr VectorBase() : data{} {}

		constexpr VectorBase( const VectorBase& other )					= default;
		constexpr VectorBase( VectorBase && donor )						= default;
		constexpr VectorBase& operator = ( const VectorBase & other )	= default;
		constexpr VectorBase& operator = ( VectorBase && donor )		= default;

		constexpr ~VectorBase() = default;

		constexpr explicit VectorBase( Initialization::UniformInitialization, const Coordinate value )
		{
			std::fill_n( data, Size, value );
		}

		constexpr explicit VectorBase( const Coordinate x )
			:
			data{ x }
		{
		}

		constexpr VectorBase( const Coordinate x, const Coordinate y ) requires( Size >= 2 )
			:
			data{ x, y }
		{
		}

		constexpr VectorBase( const Coordinate x, const Coordinate y, const Coordinate z ) requires( Size >= 3 )
			:
			data{ x, y, z }
		{
		}

		constexpr VectorBase( const Coordinate x, const Coordinate y, const Coordinate z, const Coordinate w ) requires( Size >= 4 )
			:
			data{ x, y, z, w }
		{
		}

		template< typename... Values >
		constexpr VectorBase( Values... values )
			:
			data{ values... }
		{
		}

	/* Comparison Operators. */
		constexpr auto operator<=>( const VectorBase& ) const = default;

		bool operator==( const VectorBase& right_hand_side ) const
		{
			bool result = true;

			for( auto i = 0; i < Size; i++ )
				if constexpr( std::is_integral_v< Coordinate > )
					Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result &= data[ index ] == right_hand_side.data[ index ]; } );
				else
					Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result &= Math::IsEqual( data[ index ], right_hand_side.data[ index ] ); } );

			return result;
		}

		bool operator!=( const VectorBase& right_hand_side ) const
		{
			return !operator==( right_hand_side );
		}

	/* Getters & Setters. */
		const Coordinate* Data() const { return data; };
		Coordinate& operator[] ( const unsigned int index ) { return data[ index ]; }
		constexpr const Coordinate& operator[] ( const unsigned int index ) const { return data[ index ]; }
		
		constexpr Coordinate X() const requires( Size >= 1 ) { return data[ 0 ]; };
		constexpr Coordinate Y() const requires( Size >= 2 ) { return data[ 1 ]; };
		constexpr Coordinate Z() const requires( Size >= 3 ) { return data[ 2 ]; };
		constexpr Coordinate W() const requires( Size >= 4 ) { return data[ 3 ]; };

		Coordinate& X() requires( Size >= 1 ) { return data[ 0 ]; };
		Coordinate& Y() requires( Size >= 2 ) { return data[ 1 ]; };
		Coordinate& Z() requires( Size >= 3 ) { return data[ 2 ]; };
		Coordinate& W() requires( Size >= 4 ) { return data[ 3 ]; };

		template< typename... Values >
		VectorBase& Set( Values... values )
		{
			int i = 0;
			( /* Lambda: */ [ & ]{ data[ i++ ] = values; }(), ... ); // Utilize fold expressions with a lambda to "loop over" the parameter pack.

			return *this;
		}

	/* Other Queries. */
		static constexpr size_t Dimension() { return Size; }

		constexpr bool IsZero() const
		{
			bool nonZero_component_encountered = false;
			if constexpr( std::is_integral_v< Coordinate > )
				Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { nonZero_component_encountered |= data[ index ] != 0; } );
			else
				Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { nonZero_component_encountered |= !Math::IsZero( data[ index ] ); } );

			return !nonZero_component_encountered;
		}

		constexpr bool IsNormalized() const requires( std::is_integral_v< Coordinate > == false )
		{
			Coordinate sum_of_squares( 0 );
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { sum_of_squares += data[ index ] * data[ index]; } );

			return Math::IsEqual( sum_of_squares, Coordinate( 1 ) );
		}

	/* Arithmetic Operations: Unary operators. */
		constexpr VectorBase operator- () const
		{
			return *this * -1; // Utilize operator * (scalar).
		}

	/* Arithmetic Operations: Binary operators (with a vector). */
		constexpr VectorBase operator+ ( const VectorBase& right_hand_side ) const
		{
			VectorBase result( *this );
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] += right_hand_side.data[ index ]; } );
			return result;
		}

		VectorBase& operator+= ( const VectorBase& right_hand_side )
		{
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { data[ index ] += right_hand_side.data[ index ]; } );

			return *this;
		}

		constexpr VectorBase operator- ( const VectorBase& right_hand_side ) const
		{
			VectorBase result( *this );
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] -= right_hand_side.data[ index ]; } );
			return result;
		}

		VectorBase& operator-= ( const VectorBase& right_hand_side )
		{
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { data[ index ] -= right_hand_side.data[ index ]; } );

			return *this;
		}

		constexpr VectorBase operator* ( const VectorBase& right_hand_side ) const
		{
			VectorBase result( *this );
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] *= right_hand_side.data[ index ]; } );
			return result;
		}

		VectorBase& operator*= ( const VectorBase& right_hand_side )
		{
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { data[ index ] *= right_hand_side.data[ index ]; } );

			return *this;
		}

		constexpr VectorBase operator/ ( const VectorBase& right_hand_side ) const
		{
			VectorBase result( *this );
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] /= right_hand_side.data[ index ]; } );
			return result;
		}

		VectorBase& operator/= ( const VectorBase& right_hand_side )
		{
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { data[ index ] /= right_hand_side.data[ index ]; } );

			return *this;
		}

	/* Arithmetic Operations: Binary operators (with a scalar), of the the form scalar-operator-vector. */
		friend VectorBase operator + ( const Coordinate scalar, const VectorBase& vector )
		{
			VectorBase result( UNIFORM_INITIALIZATION, scalar );
			return result + vector;
		}

		friend VectorBase operator - ( const Coordinate scalar, const VectorBase& vector )
		{
			VectorBase result( UNIFORM_INITIALIZATION, scalar );
			return result - vector;
		}

		friend VectorBase operator * ( const Coordinate scalar, const VectorBase& vector )
		{
			VectorBase result( UNIFORM_INITIALIZATION, scalar );
			return result * vector;
		}

		friend VectorBase operator / ( const Coordinate scalar, const VectorBase& vector )
		{
			VectorBase result( UNIFORM_INITIALIZATION, scalar );
			return result / vector;
		}

	/* Arithmetic Operations: Binary operators (with a scalar), of the the form vector-operator-scalar. */
		constexpr VectorBase operator+ ( const Coordinate scalar ) const
		{
			VectorBase result( *this );
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] += scalar; } );

			return result;
		}

		VectorBase& operator+= ( const Coordinate scalar )
		{
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { data[ index ] += scalar; } );

			return *this;
		}

		constexpr VectorBase operator- ( const Coordinate scalar ) const
		{
			VectorBase result( *this );
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] -= scalar; } );

			return result;
		}

		VectorBase& operator-= ( const Coordinate scalar )
		{
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { data[ index ] -= scalar; } );

			return *this;
		}

		constexpr VectorBase operator* ( const Coordinate scalar ) const
		{
			VectorBase result( *this );
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] *= scalar; } );

			return result;
		}

		VectorBase& operator*= ( const Coordinate scalar )
		{
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { data[ index ] *= scalar; } );

			return *this;
		}

		constexpr VectorBase operator/ ( const Coordinate scalar ) const
		{
			VectorBase result( *this );
			if constexpr( std::is_integral_v< Coordinate > )
				Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] /= scalar; } ); // Divide directly as division of 1/scalar will give zero when scalar > 1.
			else
			{
				const auto inverse_of_scalar = Coordinate( 1 ) / scalar; // We can calculate the inverse safely.
				Utility::constexpr_for< 0, Size, 1 >( [&]( const auto index ) { result.data[ index ] *= inverse_of_scalar; } );
			}

			return result;
		}

		VectorBase& operator/= ( const Coordinate scalar )
		{
			if constexpr( std::is_integral_v< Coordinate > )
				Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { data[ index ] /= scalar; } ); // Divide directly as division of 1/scalar will give zero when scalar > 1.
			else
			{
				const auto inverse_of_scalar = Coordinate( 1 ) / scalar; // We can calculate the inverse safely.
				Utility::constexpr_for< 0, Size, 1 >( [&]( const auto index ) { data[ index ] *= inverse_of_scalar; } );
			}

			return *this;
		}

	/* Other Arithmetic Operations. */
		Coordinate Dot( const VectorBase& other ) const // Practically we won't use this for any vectors other than 3D & 4D, but no need to restrict.
		{
			Coordinate result( 0 );

			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result += data[ index ] * other.data[ index ]; } );

			return result;
		}

		/* With self. */
		Coordinate Dot() const
		{
			Coordinate result( 0 );

			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result += data[ index ] * data[ index ]; } );

			return result;
		}

		// Cross product is only defined for vectors of 3 & 7 dimensions apparently, but practically we only need it for 3D.
		VectorBase Cross( const VectorBase& other ) const requires( Size == 3 )
		{
			// u X v = ( u2v3 - u3v2, u3v1 - u1v3, u1v2 - u2v1 )
			return VectorBase( data[ 1 ] * other.data[ 2 ] - data[ 2 ] * other.data[ 1 ],
							   data[ 2 ] * other.data[ 0 ] - data[ 0 ] * other.data[ 2 ],
							   data[ 0 ] * other.data[ 1 ] - data[ 1 ] * other.data[ 0 ] );
		}

		Coordinate SquareMagnitude() const { return Dot(); }
		Coordinate Magnitude() const requires( std::floating_point< Coordinate > ) { return std::sqrt( SquareMagnitude() ); }

		VectorBase Normalized() const requires( std::floating_point< Coordinate > )
		{
			const Coordinate one_over_magnitude = 1.0f / Magnitude();
			return this * one_over_magnitude;
		}

		VectorBase& Normalize() requires std::floating_point< Coordinate >
		{
			const Coordinate one_over_magnitude = 1.0f / Magnitude();
			*this *= one_over_magnitude;
			return *this;
		}

		/* Vector-matrix multiplication. */
		template< size_t ColumnSize >
		VectorBase< Coordinate, ColumnSize > operator* ( const MatrixBase< Coordinate, Size, ColumnSize >& transform_matrix ) const
		{
			VectorBase< Coordinate, ColumnSize > vector_transformed;
			for( auto j = 0; j < ColumnSize; j++ )
				for( auto k = 0; k < Size; k++ )
					vector_transformed[ j ] += data[ k ] * transform_matrix[ k ][ j ];

			return vector_transformed;
		}

		/* Vector-matrix multiplication. */
		template< size_t ColumnSize >
		VectorBase< Coordinate, ColumnSize >& operator*= ( const MatrixBase< Coordinate, Size, ColumnSize >& transform_matrix )
		{
			return *this = *this * transform_matrix;
		}

	protected:
		Coordinate data[ Size ];
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
}
