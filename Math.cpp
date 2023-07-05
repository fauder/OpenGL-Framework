// Project Includes.
#include "Math.h"
#include "Polar.hpp"

namespace Framework::Math
{
	Polar2 ToPolar2( const Vector2& cartesian )
	{
		if( cartesian.IsZero() )
			return Polar2{};

		return Polar2( cartesian.Magnitude(), Radians( std::atan2( cartesian.Y(), cartesian.X() ) ) );
	}

	Vector2 ToVector2( const Polar2& polar2 )
	{
		const auto r = polar2.R();

		if( IsZero( r ) )
			return Vector2{};


		return Vector2( std::cos( float( polar2.Theta() ) ) * r, std::sin( float( polar2.Theta() ) ) * r );
	}

	Polar3_Cylindrical ToPolar3_Cylindrical( const Vector3& cartesian )
	{
		const auto x = cartesian.X(), y = cartesian.Y(), z = cartesian.Z();

		if( cartesian.IsZero() )
			return Polar3_Cylindrical{};

		return Polar3_Cylindrical( Math::Hypothenuse( x, y ), Radians( std::atan2( y, x ) ), z );
	}

	Vector3 ToVector3( const Polar3_Cylindrical& polar3 )
	{
		const auto r = polar3.R(), z = polar3.Z();

		if( IsZero( r ) && IsZero( z ) )
			return Vector3{};

		return Vector3( r * std::cos( float( polar3.Theta() ) ), r * std::sin( float( polar3.Theta() ) ), z );
	}

	Polar3_Spherical ToPolar3_Spherical( const Vector3& cartesian )
	{
		const auto x = cartesian.X(), y = cartesian.Y(), z = cartesian.Z();

		if( cartesian.IsZero() )
			return Polar3_Spherical{};

		const auto xy_plane_distance = Math::Hypothenuse( x, y );

		return Polar3_Spherical( cartesian.Magnitude(), Radians( std::atan2( y, x ) ), 90.0f - Radians( std::atan2( z, xy_plane_distance ) ) );
	}

	Vector3 ToVector3( const Polar3_Spherical& polar3 )
	{
		const auto r = polar3.R();

		if( IsZero( r ) )
			return Vector3{};

		const auto latitude_inRadians = 90.0f - polar3.Phi(); // Latitude is also called the Angle of Inclination.
		const auto xy_plane_distance  = r * std::cos( float( latitude_inRadians ) );

		return Vector3( xy_plane_distance * std::cos( float( polar3.Theta() ) ),
						xy_plane_distance * std::sin( float( polar3.Theta() ) ), 
						r * std::sin( float( latitude_inRadians ) ) );
	}

	Polar3_Spherical_Game ToPolar3_Spherical_Game( const Vector3& cartesian )
	{
		const auto x = cartesian.X(), y = cartesian.Y(), z = cartesian.Z();

		if( cartesian.IsZero() )
			return Polar3_Spherical_Game{};

		const auto xz_projection_of_r = Math::Hypothenuse( x, z );

		return Polar3_Spherical_Game( cartesian.Magnitude(), Radians( std::atan2( x, z ) ), Radians( std::atan2( -y, xz_projection_of_r ) ) );
	}

	Vector3 ToVector3( const Polar3_Spherical_Game& polar3 )
	{
		const auto r = polar3.R();

		if( IsZero( r ) )
			return Vector3{};

		const auto xz_projection_of_r = r * std::cos( float( polar3.Pitch() ) );

		return Vector3( xz_projection_of_r * std::sin( float( polar3.Heading() ) ), 
						r * -std::sin( float( polar3.Pitch() ) ),
						xz_projection_of_r * std::cos( float( polar3.Heading() ) ) );
	}
}