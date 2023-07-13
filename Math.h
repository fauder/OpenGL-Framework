#pragma once

// Project Includes.
#include "Concepts.h"
#include "Constants.h"

// std Includes.
#include <cmath>

namespace Framework
{
	class Polar2;
	class Polar3_Cylindrical;
	class Polar3_Spherical;
	class Polar3_Spherical_Game;
	template< Concepts::Arithmetic Coordinate, size_t Size > requires Concepts::NonZero< Size >
	class VectorBase;
	using Vector2 = VectorBase< float, 2 >;
	using Vector3 = VectorBase< float, 3 >;
}

namespace Framework::Math
{
/* Trigonometry. */
	template< std::floating_point Value >
	Value Hypothenuse( const Value x, const Value y ) { return std::sqrt( x * x + y * y ); }

	template< std::floating_point Value >
	Value NonHyptothenuseEdge( const Value edge, const Value hyptothenuse ) { return std::sqrt( hyptothenuse * hyptothenuse - edge * edge ); }

/* Floating point comparisons. */
	/* Based on Magnum's TypeTraits::Equals(). */
	template< std::floating_point Value >
	constexpr bool IsEqual( Value lhs, Value rhs, const float epsilon = Constants< Value >::Epsilon() )
	{
		if( lhs == rhs ) // Check for binary and infinity equalities.
			return true;

		Value abs_lhs    = std::abs( lhs );
		Value abs_rhs    = std::abs( rhs );
		Value difference = std::abs( lhs - rhs );

		/* Do not use relative error if one of the values is zero or the difference is smaller than the epsilon. */
		if( lhs == Value{} || rhs == Value{} || difference < epsilon )
			return true;

		return difference / std::abs( lhs + rhs ) < epsilon;
	}

	template< std::floating_point Value >
	bool IsEqualSquared( Value lhs, Value rhs, const float epsilon = Constants< Value >::TwoEpsilon() )
	{
		return IsEqual( lhs, rhs, epsilon );
	}

	template< std::floating_point Value >
	constexpr bool IsGreaterThan( Value lhs, Value rhs, const float epsilon = Constants< Value >::Epsilon() ) { return lhs - rhs > epsilon; }

	template< std::floating_point Value >
	bool IsGreaterThanOrEqual( Value lhs, Value rhs, const float epsilon = Constants< Value >::Epsilon() ) {
		return std::abs( lhs - rhs ) < epsilon ||
			lhs - rhs > epsilon;
	}

	template< std::floating_point Value >
	constexpr bool IsLessThan( Value lhs, Value rhs, const float epsilon = Constants< Value >::Epsilon() ) { return rhs - lhs > epsilon; }

	template< std::floating_point Value >
	bool IsLessThanOrEqual( Value lhs, Value rhs, const float epsilon = Constants< Value >::Epsilon() ) {
		return std::abs( rhs - lhs ) < epsilon ||
			rhs - lhs > epsilon;
	}

	template< std::floating_point Value >
	bool IsZero( Value value, const float epsilon = Constants< Value >::Epsilon() ) { return IsEqual( value, Value{ 0 } ); }

/* Arithmetic. */
	template< std::floating_point Value >
	Value SquareOf( Value value ) { return std::pow( value, Value{ 2 } ); }

/* Other. */
	Polar2 ToPolar2( const Vector2& cartesian );
	Vector2 ToVector2( const Polar2& polar2 );
	Polar3_Cylindrical ToPolar3_Cylindrical( const Vector3& cartesian );
	Vector3 ToVector3( const Polar3_Cylindrical& polar3 );
	Polar3_Spherical ToPolar3_Spherical( const Vector3& cartesian );
	Vector3 ToVector3( const Polar3_Spherical& polar3 );
	Polar3_Spherical_Game ToPolar3_Spherical_Game( const Vector3& cartesian );
	Vector3 ToVector3( const Polar3_Spherical_Game& polar3 );
}