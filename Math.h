#pragma once

// Project Includes.
#include "Concepts.h"
#include "Constants.h"

// std Includes.
#include <cmath>

namespace Framework
{
	class Polar2;
	template< Concepts::Arithmetic Coordinate, size_t Size > requires Concepts::Nonzero< Size >
	class VectorBase;
	using Vector2 = VectorBase< float, 2 >;
}

namespace Framework::Math
{
/* Forward Declarations. */

/* Trigonometry. */
	template< std::floating_point Value >
	Value Hypothenuse( const Value x, const Value y ) { return std::sqrt( x * x + y * y ); }

	template< std::floating_point Value >
	Value NonHyptothenuseEdge( const Value edge, const Value hyptothenuse ) { return std::sqrt( hyptothenuse * hyptothenuse - edge * edge ); }

	template< std::floating_point Value >
	Value ToDegrees( const Value angle_inRadians ) { return angle_inRadians * Constants::RAD_TO_DEG; }

	template< std::floating_point Value >
	Value ToRadians( const Value angle_inDegrees ) { return angle_inDegrees * Constants::DEG_TO_RAD; }

/* Floating point comparisons. */
	template< std::floating_point Value, auto epsilon = std::numeric_limits< Value >::epsilon() >
	bool IsEqualTo( Value lhs, Value rhs ) { return std::abs( lhs - rhs ) < epsilon; }

	template< std::floating_point Value, auto epsilon = std::numeric_limits< Value >::epsilon() >
	bool IsGreaterThan( Value lhs, Value rhs ) { return lhs - rhs > epsilon; }

	template< std::floating_point Value, auto epsilon = std::numeric_limits< Value >::epsilon() >
	bool IsGreaterThanOrEqual( Value lhs, Value rhs ) {
		return std::abs( lhs - rhs ) < epsilon ||
			lhs - rhs > epsilon;
	}
	template< std::floating_point Value, auto epsilon = std::numeric_limits< Value >::epsilon() >
	bool IsLessThan( Value lhs, Value rhs ) { return rhs - lhs > epsilon; }

	template< std::floating_point Value, auto epsilon = std::numeric_limits< Value >::epsilon() >
	bool IsLessThanOrEqual( Value lhs, Value rhs ) {
		return std::abs( rhs - lhs ) < epsilon ||
			rhs - lhs > epsilon;
	}
	template< std::floating_point Value, auto epsilon = std::numeric_limits< Value >::epsilon() >
	bool IsZero( Value val ) { return std::abs( val ) < epsilon; }

/* Arithmetic. */
	template< std::floating_point Value >
	Value SquareOf( Value val ) { return std::pow( val, 2 ); }

/* Other. */
	Polar2 ToPolar2( const Vector2& cartesian );
	Vector2 ToVector2( const Polar2& polar );
}