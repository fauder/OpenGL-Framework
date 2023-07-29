#pragma once

// Project Includes.
#include "Concepts.h"
#include "Matrix.hpp"
#include "Utility.hpp"
#include "TypeTraits.h"

// std Includes.
#include <cmath>
#include <utility>

namespace Framework::Math
{
	template< Concepts::Arithmetic Coordinate, std::size_t Size >
		requires Concepts::NonZero< Size >
	class Vector
	{
	public:
	/* Constructors. */
		constexpr Vector() : data{} {}

		constexpr Vector( const Vector& other )					= default;
		constexpr Vector( Vector && donor )						= default;
		constexpr Vector& operator = ( const Vector & other )	= default;
		constexpr Vector& operator = ( Vector && donor )		= default;

		constexpr ~Vector() = default;

		constexpr explicit Vector( Initialization::UniformInitialization, const Coordinate value )
		{
			std::fill_n( data, Size, value );
		}

		constexpr explicit Vector( const Coordinate x )
			:
			data{ x }
		{
		}

		constexpr Vector( const Coordinate x, const Coordinate y ) requires( Size >= 2 )
			:
			data{ x, y }
		{
		}

		constexpr Vector( const Coordinate x, const Coordinate y, const Coordinate z ) requires( Size >= 3 )
			:
			data{ x, y, z }
		{
		}

		constexpr Vector( const Coordinate x, const Coordinate y, const Coordinate z, const Coordinate w ) requires( Size >= 4 )
			:
			data{ x, y, z, w }
		{
		}

		template< typename... Values >
		constexpr Vector( Values... values ) requires( sizeof...( values ) > 1 )
			:
			data{ values... }
		{
		}

	/* Comparison Operators. */
		constexpr auto operator<=>( const Vector& ) const = default;

		bool operator==( const Vector& right_hand_side ) const
		{
			bool result = true;

			for( auto i = 0; i < Size; i++ )
				if constexpr( std::is_integral_v< Coordinate > )
					Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result &= data[ index ] == right_hand_side.data[ index ]; } );
				else
					Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result &= Math::IsEqual( data[ index ], right_hand_side.data[ index ] ); } );

			return result;
		}

		bool operator!=( const Vector& right_hand_side ) const
		{
			return !operator==( right_hand_side );
		}

	/* Getters & Setters. */
		const Coordinate* Data() const { return data; };
		constexpr Coordinate& operator[] ( const unsigned int index ) { return data[ index ]; }
		constexpr const Coordinate& operator[] ( const unsigned int index ) const { return data[ index ]; }
		
		constexpr Coordinate X() const requires( Size >= 1 ) { return data[ 0 ]; };
		constexpr Coordinate Y() const requires( Size >= 2 ) { return data[ 1 ]; };
		constexpr Coordinate Z() const requires( Size >= 3 ) { return data[ 2 ]; };
		constexpr Coordinate W() const requires( Size >= 4 ) { return data[ 3 ]; };

		constexpr Coordinate& X() requires( Size >= 1 ) { return data[ 0 ]; };
		constexpr Coordinate& Y() requires( Size >= 2 ) { return data[ 1 ]; };
		constexpr Coordinate& Z() requires( Size >= 3 ) { return data[ 2 ]; };
		constexpr Coordinate& W() requires( Size >= 4 ) { return data[ 3 ]; };

		template< typename... Values >
		constexpr Vector& Set( Values... values )
		{
			int i = 0;
			( /* Lambda: */ [ & ]{ data[ i++ ] = values; }(), ... ); // Utilize fold expressions with a lambda to "loop over" the parameter pack.

			return *this;
		}

	/* Other Queries. */
		static consteval std::size_t Dimension()		{ return Size; }

		static constexpr Vector Zero()		{ return Vector{}; }
		static constexpr Vector One()		{ return Vector{ Coordinate( 1 ) }; }

		static consteval Vector Left()		requires( Size >= 1 ) { return Vector{ -Coordinate( 1 ) }; }
		static consteval Vector Right()		requires( Size >= 1 ) { return Vector{ +Coordinate( 1 ) }; }
		static consteval Vector Down()		requires( Size >= 2 ) { return Vector{ Coordinate( 0 ), -Coordinate( 1 ) }; }
		static consteval Vector Up()		requires( Size >= 2 ) { return Vector{ Coordinate( 0 ), +Coordinate( 1 ) }; }
		/* Using right-handed coordinate system. */
		static consteval Vector Backward()	requires( Size >= 3 ) { return Vector{ Coordinate( 0 ), Coordinate( 0 ), +Coordinate( 1 ) }; }
		/* Using right-handed coordinate system. */
		static consteval Vector Forward()	requires( Size >= 3 ) { return Vector{ Coordinate( 0 ), Coordinate( 0 ), -Coordinate( 1 ) }; }

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
			return Math::IsEqualSquared( SquareMagnitude(), Coordinate( 1 ) );
		}

	/* Arithmetic Operations: Unary operators. */
		constexpr Vector operator- () const
		{
			return *this * -1; // Utilize operator * (scalar).
		}

	/* Arithmetic Operations: Binary operators (with a vector). */
		constexpr Vector operator+ ( const Vector& right_hand_side ) const
		{
			Vector result( *this );
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] += right_hand_side.data[ index ]; } );
			return result;
		}

		constexpr Vector& operator+= ( const Vector& right_hand_side )
		{
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { data[ index ] += right_hand_side.data[ index ]; } );

			return *this;
		}

		constexpr Vector operator- ( const Vector& right_hand_side ) const
		{
			Vector result( *this );
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] -= right_hand_side.data[ index ]; } );
			return result;
		}

		constexpr Vector& operator-= ( const Vector& right_hand_side )
		{
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { data[ index ] -= right_hand_side.data[ index ]; } );

			return *this;
		}

		constexpr Vector operator* ( const Vector& right_hand_side ) const
		{
			Vector result( *this );
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] *= right_hand_side.data[ index ]; } );
			return result;
		}

		constexpr Vector& operator*= ( const Vector& right_hand_side )
		{
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { data[ index ] *= right_hand_side.data[ index ]; } );

			return *this;
		}

		constexpr Vector operator/ ( const Vector& right_hand_side ) const
		{
			Vector result( *this );
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] /= right_hand_side.data[ index ]; } );
			return result;
		}

		constexpr Vector& operator/= ( const Vector& right_hand_side )
		{
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { data[ index ] /= right_hand_side.data[ index ]; } );

			return *this;
		}

	/* Arithmetic Operations: Binary operators (with a scalar), of the the form vector-operator-scalar. */
		constexpr Vector operator+ ( const Coordinate scalar ) const
		{
			Vector result( *this );
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] += scalar; } );

			return result;
		}

		constexpr Vector& operator+= ( const Coordinate scalar )
		{
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { data[ index ] += scalar; } );

			return *this;
		}

		constexpr Vector operator- ( const Coordinate scalar ) const
		{
			Vector result( *this );
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] -= scalar; } );

			return result;
		}

		constexpr Vector& operator-= ( const Coordinate scalar )
		{
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { data[ index ] -= scalar; } );

			return *this;
		}

		constexpr Vector operator* ( const Coordinate scalar ) const
		{
			Vector result( *this );
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] *= scalar; } );

			return result;
		}

		constexpr Vector& operator*= ( const Coordinate scalar )
		{
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { data[ index ] *= scalar; } );

			return *this;
		}

		constexpr Vector operator/ ( const Coordinate scalar ) const
		{
			Vector result( *this );
			if constexpr( std::is_integral_v< Coordinate > )
				Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] /= scalar; } ); // Divide directly as division of 1/scalar will give zero when scalar > 1.
			else
			{
				const auto inverse_of_scalar = Coordinate( 1 ) / scalar; // We can calculate the inverse safely.
				Utility::constexpr_for< 0, Size, 1 >( [&]( const auto index ) { result.data[ index ] *= inverse_of_scalar; } );
			}

			return result;
		}

		constexpr Vector& operator/= ( const Coordinate scalar )
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

	/* Arithmetic Operations: Binary operators (with a scalar), of the the form scalar-operator-vector. */
		constexpr friend Vector operator + ( const Coordinate scalar, const Vector& vector )
		{
			Vector result( UNIFORM_INITIALIZATION, scalar );
			return result + vector;
		}

		constexpr friend Vector operator - ( const Coordinate scalar, const Vector& vector )
		{
			Vector result( UNIFORM_INITIALIZATION, scalar );
			return result - vector;
		}

		constexpr friend Vector operator * ( const Coordinate scalar, const Vector& vector )
		{
			Vector result( UNIFORM_INITIALIZATION, scalar );
			return result * vector;
		}

		constexpr friend Vector operator / ( const Coordinate scalar, const Vector& vector )
		{
			Vector result( UNIFORM_INITIALIZATION, scalar );
			return result / vector;
		}

	/* Other Arithmetic Operations. */
		/* With self. */
		constexpr Coordinate Dot() const
		{
			Coordinate result( 0 );

			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result += data[ index ] * data[ index ]; } );

			return result;
		}

		// Practically we won't use this for any vectors other than 3D & 4D, but no need to restrict.
		template< Concepts::Arithmetic Coordinate_, std::size_t Size_ > // Have to use different template parameters here because C++...
		friend constexpr Coordinate_ Dot( const Vector< Coordinate_, Size_ >& u, const Vector< Coordinate_, Size_ >& v );

		// Cross product is only defined for vectors of 3 & 7 dimensions apparently, but practically we only need it for 3D.
		template< Concepts::Arithmetic Coordinate_ > // Have to use different template parameters here because C++...
		friend constexpr Vector< Coordinate_, 3 > Cross( const Vector< Coordinate_, 3 >& u, const Vector< Coordinate_, 3 >& v );

		constexpr Coordinate SquareMagnitude() const { return Dot(); }
		Coordinate Magnitude() const requires( std::floating_point< Coordinate > ) { return std::sqrt( SquareMagnitude() ); }

		Vector Normalized() const requires( std::floating_point< Coordinate > )
		{
			if( const auto magnitude = Magnitude(); magnitude > Constants< Coordinate >::Epsilon() )
			{
				const Coordinate one_over_magnitude = Coordinate( 1 ) / Magnitude();
				return *this * one_over_magnitude;
			}

			return {};
		}

		Vector& Normalize() requires std::floating_point< Coordinate >
		{
			const Coordinate one_over_magnitude = Coordinate( 1 ) / Magnitude();
			*this *= one_over_magnitude;
			return *this;
		}

		/* Vector-matrix multiplication. */
		template< std::size_t ColumnSize >
		constexpr Vector< Coordinate, ColumnSize > operator* ( const Matrix< Coordinate, Size, ColumnSize >& transform_matrix ) const
		{
			Vector< Coordinate, ColumnSize > vector_transformed;
			for( auto j = 0; j < ColumnSize; j++ )
				for( auto k = 0; k < Size; k++ )
					vector_transformed[ j ] += data[ k ] * transform_matrix[ k ][ j ];

			return vector_transformed;
		}

		/* Vector-matrix multiplication. */
		template< std::size_t ColumnSize >
		constexpr Vector< Coordinate, ColumnSize >& operator*= ( const Matrix< Coordinate, Size, ColumnSize >& transform_matrix )
		{
			return *this = *this * transform_matrix;
		}

	protected:
		Coordinate data[ Size ];
	};

	template< Concepts::Arithmetic Coordinate, std::size_t Size >
	constexpr Coordinate Dot( const Vector< Coordinate, Size >& u, const Vector< Coordinate, Size >& v ) // Practically we won't use this for any vectors other than 3D & 4D, but no need to restrict.
	{
		Coordinate result( 0 );

		Utility::constexpr_for< 0, Size, 1 >( [&]( const auto index ) { result += u.data[ index ] * v.data[ index ]; } );

		return result;
	}

	// Cross product is only defined for vectors of 3 & 7 dimensions apparently, but practically we only need it for 3D.
	template< Concepts::Arithmetic Coordinate >
	constexpr Vector< Coordinate, 3 > Cross( const Vector< Coordinate, 3 >& u, const Vector< Coordinate, 3 >& v )
	{
		// u X v = ( u2v3 - u3v2, u3v1 - u1v3, u1v2 - u2v1 )
		return Vector< Coordinate, 3 >( u.data[ 1 ] * v.data[ 2 ] - u.data[ 2 ] * v.data[ 1 ],
											u.data[ 2 ] * v.data[ 0 ] - u.data[ 0 ] * v.data[ 2 ],
											u.data[ 0 ] * v.data[ 1 ] - u.data[ 1 ] * v.data[ 0 ] );
	}

}

namespace Framework
{
	using Vector2  = Math::Vector< float,	2 >;
	using Vector3  = Math::Vector< float,	3 >;
	using Vector4  = Math::Vector< float,	4 >;
	using Vector2D = Math::Vector< double,	2 >;
	using Vector3D = Math::Vector< double,	3 >;
	using Vector4D = Math::Vector< double,	4 >;
	using Vector2I = Math::Vector< int,		2 >;
	using Vector3I = Math::Vector< int,		3 >;
	using Vector4I = Math::Vector< int,		4 >;
}