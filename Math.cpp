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

	Vector2 ToVector2( const Polar2& polar2 )
	{
		const auto r = polar2.R();

		if( IsZero( r ) )
			return Vector2{};

		const auto theta_inRadians = ToRadians( polar2.Theta() );

		return Vector2( std::cos( theta_inRadians ) * r, std::sin( theta_inRadians ) * r );
	}

	Polar3_Cylindrical ToPolar3_Cylindrical( const Vector3& cartesian )
	{
		const auto x = cartesian.X(), y = cartesian.Y(), z = cartesian.Z();

		if( cartesian.IsZero() )
			return Polar3_Cylindrical{};

		return Polar3_Cylindrical( Math::Hypothenuse( x, y ), ToDegrees( std::atan2( y, x ) ), z );
	}

	Vector3 ToVector3( const Polar3_Cylindrical& polar3 )
	{
		const auto r = polar3.R(), z = polar3.Z();

		if( IsZero( r ) && IsZero( z ) )
			return Vector3{};

		const auto theta_inRadians = ToRadians( polar3.Theta() );

		return Vector3( r * std::cos( theta_inRadians ), r * std::sin( theta_inRadians ), z );
	}

	Polar3_Spherical ToPolar3_Spherical( const Vector3& cartesian )
	{
		const auto x = cartesian.X(), y = cartesian.Y(), z = cartesian.Z();

		if( cartesian.IsZero() )
			return Polar3_Spherical{};

		const auto xy_plane_distance = Math::Hypothenuse( x, y );

		return Polar3_Spherical( cartesian.Magnitude(), ToDegrees( std::atan2( y, x ) ), 90.0f - ToDegrees( std::atan2( z, xy_plane_distance ) ) );
	}

	Vector3 ToVector3( const Polar3_Spherical& polar3 )
	{
		const auto r = polar3.R();

		if( IsZero( r ) )
			return Vector3{};

		const auto theta_inRadians    = ToRadians( polar3.Theta() );
		const auto latitude_inRadians = ToRadians( 90.0f - polar3.Phi() ); // Latitude is also called the Angle of Inclination.
		const auto xy_plane_distance  = r * std::cos( latitude_inRadians );

		return Vector3( xy_plane_distance * std::cos( theta_inRadians ), xy_plane_distance * std::sin( theta_inRadians ), r * std::sin( latitude_inRadians ) );
	}
}