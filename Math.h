#pragma once

// Project Includes.
#include "Angle.hpp"
#include "Concepts.h"
#include "Constants.h"

// std Includes.
#include <cmath>

/* Forward Declarations. */
namespace Framework::Math
{
	class Polar2;
	class Polar3_Cylindrical;
	class Polar3_Spherical;
	class Polar3_Spherical_Game;
	template< Concepts::Arithmetic Coordinate, size_t Size > requires Concepts::NonZero< Size >
	class Vector;
}

namespace Framework
{
	using Vector2 = Math::Vector< float, 2 >;
	using Vector3 = Math::Vector< float, 3 >;
}

namespace Framework::Math
{
/* Trigonometry. */
	template< std::floating_point Value >
	Value Hypothenuse( const Value x, const Value y ) { return std::sqrt( x * x + y * y ); }

	template< std::floating_point Value >
	Value NonHyptothenuseEdge( const Value edge, const Value hyptothenuse ) { return std::sqrt( hyptothenuse * hyptothenuse - edge * edge ); }

	template< std::floating_point Value >
	Value Sin( const Radians< Value > angle ) { return std::sin( Value( angle ) ); }

	template< std::floating_point Value >
	Value Cos( const Radians< Value > angle ) { return std::cos( Value( angle ) ); }

	template< std::floating_point Value >
	Value Tan( const Radians< Value > angle ) { return std::tan( Value( angle ) ); }

	template< std::floating_point Value >
	Radians< Value > Acos( const Value cosine ) { return Radians< Value >( std::acos( cosine ) ); }

	template< std::floating_point Value >
	Radians< Value > Asin( const Value sine ) { return Radians< Value >( std::asin( sine ) ); }

	template< std::floating_point Value >
	Radians< Value > Atan( const Value slope ) { return Radians< Value >( std::atan( slope ) ); }

	template< std::floating_point Value >
	Radians< Value > Atan2( const Value y, const Value x ) { return Radians< Value >( std::atan2( y, x ) ); }

/* Arithmetic. */
	template< std::floating_point Value >
	Value SquareOf( Value value ) { return std::pow( value, Value{ 2 } ); }

	template< std::floating_point Value >
	Value Abs( const Value value ) { return std::abs( value ); }

	template< std::floating_point Value >
	Value Sqrt( const Value value ) { return std::sqrt( value ); }

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