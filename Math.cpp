// Project Includes.
#include "Math.h"
#include "Polar.hpp"

namespace Framework::Math
{
	Polar2 ToPolar2( const Vector2& cartesian )
	{
		if( cartesian.IsZero() )
			return Polar2{};

		return Polar2( cartesian.Magnitude(), ToDegrees( std::atan2( cartesian.Y(), cartesian.X() ) ) );
	}

	Vector2 ToVector2( const Polar2& polar )
	{
		const auto r = polar.R();

		if( IsZero( r ) )
			return Vector2{};

		const auto theta_inRadians = ToRadians( polar.Angle() );

		return Vector2( std::cos( theta_inRadians ) * r, std::sin( theta_inRadians ) * r );
	}
}