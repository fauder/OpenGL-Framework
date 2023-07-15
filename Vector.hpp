#pragma once

// Project Includes.
#include "Concepts.h"
#include "Matrix.hpp"
#include "Utility.h"
#include "TypeTraits.h"

// std Includes.
#include <cmath>
#include <utility>

namespace Framework
{
	template< Concepts::Arithmetic Coordinate, size_t Size >
		requires Concepts::NonZero< Size >
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
		constexpr VectorBase& Set( Values... values )
		{
			int i = 0;
			( /* Lambda: */ [ & ]{ data[ i++ ] = values; }(), ... ); // Utilize fold expressions with a lambda to "loop over" the parameter pack.

			return *this;
		}

	/* Other Queries. */
		static consteval size_t Dimension()		{ return Size; }

		static constexpr VectorBase Zero()		{ return VectorBase{}; }
		static constexpr VectorBase One()		{ return VectorBase{ Coordinate( 1 ) }; }

		static consteval VectorBase Left()		requires( Size >= 1 ) { return VectorBase{ -1.0f }; }
		static consteval VectorBase Right()		requires( Size >= 1 ) { return VectorBase{ +1.0f }; }
		static consteval VectorBase Bottom()	requires( Size >= 2 ) { return VectorBase{ 0.0f, -1.0f }; }
		static consteval VectorBase Top()		requires( Size >= 2 ) { return VectorBase{ 0.0f, +1.0f }; }
		/* Using right-handed coordinate system. */
		static consteval VectorBase Backward()	requires( Size >= 3 ) { return VectorBase{ 0.0f, 0.0f, +1.0f }; }
		/* Using right-handed coordinate system. */
		static consteval VectorBase Forward()	requires( Size >= 3 ) { return VectorBase{ 0.0f, 0.0f, -1.0f }; }

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

		constexpr VectorBase& operator+= ( const VectorBase& right_hand_side )
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

		constexpr VectorBase& operator-= ( const VectorBase& right_hand_side )
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

		constexpr VectorBase& operator*= ( const VectorBase& right_hand_side )
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

		constexpr VectorBase& operator/= ( const VectorBase& right_hand_side )
		{
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { data[ index ] /= right_hand_side.data[ index ]; } );

			return *this;
		}

	/* Arithmetic Operations: Binary operators (with a scalar), of the the form vector-operator-scalar. */
		constexpr VectorBase operator+ ( const Coordinate scalar ) const
		{
			VectorBase result( *this );
			Utility::constexpr_for< 0, Size, 1 >( [ & ]( const auto index ) { result.data[ index ] += scalar; } );

			return result;
		}

		constexpr VectorBase& operator+= ( const Coordinate scalar )
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

		constexpr VectorBase& operator-= ( const Coordinate scalar )
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

		constexpr VectorBase& operator*= ( const Coordinate scalar )
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

		constexpr VectorBase& operator/= ( const Coordinate scalar )
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
		constexpr friend VectorBase operator + ( const Coordinate scalar, const VectorBase& vector )
		{
			VectorBase result( UNIFORM_INITIALIZATION, scalar );
			return result + vector;
		}

		constexpr friend VectorBase operator - ( const Coordinate scalar, const VectorBase& vector )
		{
			VectorBase result( UNIFORM_INITIALIZATION, scalar );
			return result - vector;
		}

		constexpr friend VectorBase operator * ( const Coordinate scalar, const VectorBase& vector )
		{
			VectorBase result( UNIFORM_INITIALIZATION, scalar );
			return result * vector;
		}

		constexpr friend VectorBase operator / ( const Coordinate scalar, const VectorBase& vector )
		{
			VectorBase result( UNIFORM_INITIALIZATION, scalar );
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
		template< Concepts::Arithmetic Coordinate_, size_t Size_ > // Have to use different template parameters here because C++...
		friend constexpr Coordinate_ Dot( const VectorBase< Coordinate_, Size_ >& u, const VectorBase< Coordinate_, Size_ >& v );

		// Cross product is only defined for vectors of 3 & 7 dimensions apparently, but practically we only need it for 3D.
		template< Concepts::Arithmetic Coordinate_ > // Have to use different template parameters here because C++...
		friend constexpr VectorBase< Coordinate_, 3 > Cross( const VectorBase< Coordinate_, 3 >& u, const VectorBase< Coordinate_, 3 >& v );

		constexpr Coordinate SquareMagnitude() const { return Dot(); }
		Coordinate Magnitude() const requires( std::floating_point< Coordinate > ) { return std::sqrt( SquareMagnitude() ); }

		VectorBase Normalized() const requires( std::floating_point< Coordinate > )
		{
			if( const auto magnitude = Magnitude(); magnitude > Constants< Coordinate >::Epsilon() )
			{
				const Coordinate one_over_magnitude = 1.0f / Magnitude();
				return *this * one_over_magnitude;
			}

			return {};
		}

		VectorBase& Normalize() requires std::floating_point< Coordinate >
		{
			const Coordinate one_over_magnitude = 1.0f / Magnitude();
			*this *= one_over_magnitude;
			return *this;
		}

		/* Vector-matrix multiplication. */
		template< size_t ColumnSize >
		constexpr VectorBase< Coordinate, ColumnSize > operator* ( const MatrixBase< Coordinate, Size, ColumnSize >& transform_matrix ) const
		{
			VectorBase< Coordinate, ColumnSize > vector_transformed;
			for( auto j = 0; j < ColumnSize; j++ )
				for( auto k = 0; k < Size; k++ )
					vector_transformed[ j ] += data[ k ] * transform_matrix[ k ][ j ];

			return vector_transformed;
		}

		/* Vector-matrix multiplication. */
		template< size_t ColumnSize >
		constexpr VectorBase< Coordinate, ColumnSize >& operator*= ( const MatrixBase< Coordinate, Size, ColumnSize >& transform_matrix )
		{
			return *this = *this * transform_matrix;
		}

	protected:
		Coordinate data[ Size ];
	};

	template< Concepts::Arithmetic Coordinate, size_t Size >
	constexpr Coordinate Dot( const VectorBase< Coordinate, Size >& u, const VectorBase< Coordinate, Size >& v ) // Practically we won't use this for any vectors other than 3D & 4D, but no need to restrict.
	{
		Coordinate result( 0 );

		Utility::constexpr_for< 0, Size, 1 >( [&]( const auto index ) { result += u.data[ index ] * v.data[ index ]; } );

		return result;
	}

	// Cross product is only defined for vectors of 3 & 7 dimensions apparently, but practically we only need it for 3D.
	template< Concepts::Arithmetic Coordinate >
	constexpr VectorBase< Coordinate, 3 > Cross( const VectorBase< Coordinate, 3 >& u, const VectorBase< Coordinate, 3 >& v )
	{
		// u X v = ( u2v3 - u3v2, u3v1 - u1v3, u1v2 - u2v1 )
		return VectorBase< Coordinate, 3 >( u.data[ 1 ] * v.data[ 2 ] - u.data[ 2 ] * v.data[ 1 ],
											u.data[ 2 ] * v.data[ 0 ] - u.data[ 0 ] * v.data[ 2 ],
											u.data[ 0 ] * v.data[ 1 ] - u.data[ 1 ] * v.data[ 0 ] );
	}

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
