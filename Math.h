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
	template< Concepts::Arithmetic Coordinate, size_t Size > requires Concepts::Nonzero< Size >
	class VectorBase;
	using Vector2 = VectorBase< float, 2 >;
	using Vector3 = VectorBase< float, 3 >;
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
	constexpr Value ToDegrees( const Value angle_inRadians ) { return angle_inRadians * Constants::RAD_TO_DEG; }

	template< std::floating_point Value >
	constexpr Value ToRadians( const Value angle_inDegrees ) { return angle_inDegrees * Constants::DEG_TO_RAD; }

	/* Wraps a given angle (in degrees) between (-180, +180] range (degrees). */
	template< std::floating_point Value >
	Value Wrap180( const Value angle_inDegrees )
	{ 
		return angle_inDegrees - Value( 360 ) * std::floor( ( angle_inDegrees + Value( 180 ) ) / Value( 360 ) );
	}

/* Floating point comparisons. */
	template< std::floating_point Value, auto epsilon = std::numeric_limits< Value >::epsilon() >
	bool IsEqualTo( Value lhs, Value rhs ) { return std::abs( lhs - rhs ) < epsilon; }

	template< std::floating_point Value, auto epsilon = std::numeric_limits< Value >::epsilon() >
	constexpr bool IsGreaterThan( Value lhs, Value rhs ) { return lhs - rhs > epsilon; }

	template< std::floating_point Value, auto epsilon = std::numeric_limits< Value >::epsilon() >
	bool IsGreaterThanOrEqual( Value lhs, Value rhs ) {
		return std::abs( lhs - rhs ) < epsilon ||
			lhs - rhs > epsilon;
	}
	template< std::floating_point Value, auto epsilon = std::numeric_limits< Value >::epsilon() >
	constexpr bool IsLessThan( Value lhs, Value rhs ) { return rhs - lhs > epsilon; }

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
	Vector2 ToVector2( const Polar2& polar2 );
	Polar3_Cylindrical ToPolar3_Cylindrical( const Vector3& cartesian );
	Vector3 ToVector3( const Polar3_Cylindrical& polar3 );
	Polar3_Spherical ToPolar3_Spherical( const Vector3& cartesian );
	Vector3 ToVector3( const Polar3_Spherical& polar3 );
	Polar3_Spherical_Game ToPolar3_Spherical_Game( const Vector3& cartesian );
	Vector3 ToVector3( const Polar3_Spherical_Game& polar3 );
}