// Framework Includes.
#include "Math/Math.h"
#include "Math/Polar.hpp"
#include "Math/Matrix.hpp"
#include "Math/TypeTraits.h"

namespace Framework::Math
{
/* Conversions Between Rotation Representations. */

	/* In row-major form. Right-handed. Clockwise rotation.
	 * Describes an extrinsic (fixed-axis) rotation, in this order:    bank (around z) -> pitch (around x) -> heading (around y).
	 * an Euler rotation (is intrinsic, body-axis),  in this order: heading (around y) -> pitch (around x) -> bank (around z), which means */
	Framework::Matrix4x4 EulerToMatrix( Framework::Radians heading_around_y, Framework::Radians pitch_around_x, Framework::Radians bank_around_z )
	{
		const auto sin_pitch   = Math::Sin( pitch_around_x );
		const auto sin_heading = Math::Sin( heading_around_y );
		const auto sin_bank    = Math::Sin( bank_around_z );

		const auto cos_pitch   = Math::Cos( pitch_around_x );
		const auto cos_heading = Math::Cos( heading_around_y );
		const auto cos_bank    = Math::Cos( bank_around_z );

		const auto cos_bank_cos_heading  = cos_bank * cos_heading;
		const auto sin_pitch_sin_heading = sin_pitch * sin_heading;

		return Framework::Matrix4x4
		(
			{
				cos_bank_cos_heading + sin_bank * sin_pitch_sin_heading,		sin_bank * cos_pitch,		-cos_bank * sin_heading + sin_bank * sin_pitch * cos_heading,	0.0f,
				-sin_bank * cos_heading + cos_bank * sin_pitch_sin_heading,		cos_bank * cos_pitch,		sin_bank * sin_heading + sin_pitch * cos_bank_cos_heading,		0.0f,
				cos_pitch * sin_heading,										-sin_pitch,					cos_pitch * cos_heading,										0.0f,
				0.0f,															0.0f,						0.0f,															1.0f
			}
		);
	}

	/* In row-major form. Right-handed. Clockwise rotation.
	 * Describes an extrinsic (fixed-axis) rotation, in this order:    bank (around z) -> pitch (around x) -> heading (around y).
	 * an Euler rotation (is intrinsic, body-axis),  in this order: heading (around y) -> pitch (around x) -> bank (around z), which means */
	Framework::Matrix3x3 EulerToMatrix3x3( Framework::Radians heading_around_y, Framework::Radians pitch_around_x, Framework::Radians bank_around_z )
	{
		const auto sin_pitch   = Math::Sin( pitch_around_x );
		const auto sin_heading = Math::Sin( heading_around_y );
		const auto sin_bank    = Math::Sin( bank_around_z );

		const auto cos_pitch   = Math::Cos( pitch_around_x );
		const auto cos_heading = Math::Cos( heading_around_y );
		const auto cos_bank    = Math::Cos( bank_around_z );

		const auto cos_bank_cos_heading  = cos_bank * cos_heading;
		const auto sin_pitch_sin_heading = sin_pitch * sin_heading;

		return Framework::Matrix3x3
		(
			{
				cos_bank_cos_heading + sin_bank * sin_pitch_sin_heading,		sin_bank * cos_pitch,		-cos_bank * sin_heading + sin_bank * sin_pitch * cos_heading,
				-sin_bank * cos_heading + cos_bank * sin_pitch_sin_heading,		cos_bank * cos_pitch,		sin_bank * sin_heading + sin_pitch * cos_bank_cos_heading,	
				cos_pitch * sin_heading,										-sin_pitch,					cos_pitch * cos_heading
			}
		);
	}

/* Conversions Between Cartesian, Polar, Cylindrical & Spherical Coordinates. */

	Polar2 ToPolar2( const Vector2& cartesian )
	{
		if( cartesian.IsZero() )
			return Polar2( ZERO_INITIALIZATION );

		return Polar2( cartesian.Magnitude(), Radians( std::atan2( cartesian.Y(), cartesian.X() ) ) );
	}

	Vector2 ToVector2( const Polar2& polar2 )
	{
		const auto r = polar2.R();

		if( IsZero( r ) )
			return Vector2( ZERO_INITIALIZATION );


		return Vector2( std::cos( float( polar2.Theta() ) ) * r, std::sin( float( polar2.Theta() ) ) * r );
	}

	Polar3_Cylindrical ToPolar3_Cylindrical( const Vector3& cartesian )
	{
		const auto x = cartesian.X(), y = cartesian.Y(), z = cartesian.Z();

		if( cartesian.IsZero() )
			return Polar3_Cylindrical( ZERO_INITIALIZATION );

		return Polar3_Cylindrical( Math::Hypothenuse( x, y ), Radians( std::atan2( y, x ) ), z );
	}

	Vector3 ToVector3( const Polar3_Cylindrical& polar3 )
	{
		const auto r = polar3.R(), z = polar3.Z();

		if( IsZero( r ) && IsZero( z ) )
			return Vector3( ZERO_INITIALIZATION );

		return Vector3( r * std::cos( float( polar3.Theta() ) ), r * std::sin( float( polar3.Theta() ) ), z );
	}

	Polar3_Spherical ToPolar3_Spherical( const Vector3& cartesian )
	{
		const auto x = cartesian.X(), y = cartesian.Y(), z = cartesian.Z();

		if( cartesian.IsZero() )
			return Polar3_Spherical( ZERO_INITIALIZATION );

		const auto xy_plane_distance = Math::Hypothenuse( x, y );

		return Polar3_Spherical( cartesian.Magnitude(), Radians( std::atan2( y, x ) ), 90.0f - Radians( std::atan2( z, xy_plane_distance ) ) );
	}

	Vector3 ToVector3( const Polar3_Spherical& polar3 )
	{
		const auto r = polar3.R();

		if( IsZero( r ) )
			return Vector3( ZERO_INITIALIZATION );

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
			return Polar3_Spherical_Game( ZERO_INITIALIZATION );

		const auto xz_projection_of_r = Math::Hypothenuse( x, z );

		return Polar3_Spherical_Game( cartesian.Magnitude(), Radians( std::atan2( x, z ) ), Radians( std::atan2( -y, xz_projection_of_r ) ) );
	}

	Vector3 ToVector3( const Polar3_Spherical_Game& polar3 )
	{
		const auto r = polar3.R();

		if( IsZero( r ) )
			return Vector3( ZERO_INITIALIZATION );

		const auto xz_projection_of_r = r * std::cos( float( polar3.Pitch() ) );

		return Vector3( xz_projection_of_r * std::sin( float( polar3.Heading() ) ), 
						r * -std::sin( float( polar3.Pitch() ) ),
						xz_projection_of_r * std::cos( float( polar3.Heading() ) ) );
	}

	Polar3_Spherical_Game_RightHanded ToPolar3_Spherical_Game_RightHanded( const Vector3& cartesian )
	{
		// NOTE: Forward is <0,0-1>, not <0,0+1>!

		const auto x = cartesian.X(), y = cartesian.Y(), z = cartesian.Z();

		if( cartesian.IsZero() )
			return Polar3_Spherical_Game_RightHanded( ZERO_INITIALIZATION );

		const auto xz_projection_of_r = Math::Hypothenuse( x, z );

		return Polar3_Spherical_Game_RightHanded( cartesian.Magnitude(), Radians( std::atan2( x, -z ) ), Radians( std::atan2( y, xz_projection_of_r ) ) );
	}

	Vector3 ToVector3( const Polar3_Spherical_Game_RightHanded& polar3 )
	{
		const auto r = polar3.R();

		if( IsZero( r ) )
			return Vector3( ZERO_INITIALIZATION );

		const auto xz_projection_of_r = r * std::cos( float( polar3.Pitch() ) );

		return Vector3( xz_projection_of_r * std::sin( float( polar3.Heading() ) ),
						r * std::sin( float( polar3.Pitch() ) ),
						xz_projection_of_r * -std::cos( float( polar3.Heading() ) ) );
	}
}