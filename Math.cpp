// Project Includes.
#include "Math.h"
#include "Polar.hpp"
#include "Matrix.hpp"
#include "TypeTraits.h"

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

	/* In-place modification of the upper-left 3x3 portion. */
	/* In row-major form. Right-handed. Clockwise rotation.
	 * Describes an extrinsic (fixed-axis) rotation, in this order:    bank (around z) -> pitch (around x) -> heading (around y).
	 * an Euler rotation (is intrinsic, body-axis),  in this order: heading (around y) -> pitch (around x) -> bank (around z), which means */
	void EulerToMatrix( Framework::Matrix4x4& matrix, Framework::Radians heading_around_y, Framework::Radians pitch_around_x, Framework::Radians bank_around_z )
	{
		const auto sin_pitch   = Math::Sin( pitch_around_x );
		const auto sin_heading = Math::Sin( heading_around_y );
		const auto sin_bank    = Math::Sin( bank_around_z );

		const auto cos_pitch   = Math::Cos( pitch_around_x );
		const auto cos_heading = Math::Cos( heading_around_y );
		const auto cos_bank    = Math::Cos( bank_around_z );

		const auto cos_bank_cos_heading = cos_bank * cos_heading;
		const auto sin_pitch_sin_heading = sin_pitch * sin_heading;

		matrix[ 0 ][ 0 ] = cos_bank_cos_heading + sin_bank * sin_pitch_sin_heading;		matrix[ 0 ][ 1 ] = sin_bank * cos_pitch;	matrix[ 0 ][ 2 ] = -cos_bank * sin_heading + sin_bank * sin_pitch * cos_heading;
		matrix[ 1 ][ 0 ] = -sin_bank * cos_heading + cos_bank * sin_pitch_sin_heading;	matrix[ 1 ][ 1 ] = cos_bank * cos_pitch;	matrix[ 1 ][ 2 ] = sin_bank * sin_heading + sin_pitch * cos_bank_cos_heading;	
		matrix[ 2 ][ 0 ] = cos_pitch * sin_heading;										matrix[ 2 ][ 1 ] = -sin_pitch;				matrix[ 2 ][ 2 ] = cos_pitch * cos_heading;									
	}

	/* Expects matrix in row-major form. Right-handed. Clockwise rotation.
	 * The matrix should describe an extrinsic (fixed-axis) rotation, in this order:	bank (around z) -> pitch (around x) -> heading (around y), which results in
	 * the Euler rotation (that is intrinsic & around body-axes) in this order:		 heading (around y) -> pitch (around x) -> bank (around z). */
	void MatrixToEuler( const Framework::Matrix4x4& matrix, Framework::Radians& heading_around_y, Framework::Radians& pitch_around_x, Framework::Radians& bank_around_z )
	{
		/* The matrix' values for reference:
		{
			matrix[ 0 ][ 0 ] = cos_bank_cos_heading + sin_bank * sin_pitch_sin_heading		matrix[ 0 ][ 1 ] = sin_bank * cos_pitch		matrix[ 0 ][ 2 ] = -cos_bank * sin_heading + sin_bank * sin_pitch * cos_heading
			matrix[ 1 ][ 0 ] = -sin_bank * cos_heading + cos_bank * sin_pitch_sin_heading	matrix[ 1 ][ 1 ] = cos_bank * cos_pitch		matrix[ 1 ][ 2 ] = sin_bank * sin_heading + sin_pitch * cos_bank_cos_heading
			matrix[ 2 ][ 0 ] = cos_pitch * sin_heading										matrix[ 2 ][ 1 ] = -sin_pitch				matrix[ 2 ][ 2 ] = cos_pitch * cos_heading
		}*/

		using namespace Math::Literals;

		const auto sin_pitch = -matrix[ 2 ][ 1 ];

		if( sin_pitch <= -1.0f )
			pitch_around_x = Radians( -Constants< float >::Half_Pi() );
		else if( sin_pitch >= +1.0f )
			pitch_around_x = Radians( Constants< float >::Half_Pi() );
		else
			pitch_around_x = Math::Asin( sin_pitch );

		const auto cos_pitch = Math::Cos( pitch_around_x );

		if( Math::IsZero( cos_pitch ) ) // Which means pitch is +90 or -90 -> gimbal lock!
		{
			// Set bank to zero & assign all rotation around the vertical axis to heading.
			bank_around_z = 0.0_rad;

			// Since bank = 0, it means that cos_bank = 1 & sin_bank = 0. Reminder: cos_pitch is also 0.

			/* Updated matrix' values for reference:
			{
				matrix[ 0 ][ 0 ] = cos_heading					matrix[ 0 ][ 1 ] = 0				matrix[ 0 ][ 2 ] = -sin_heading
				matrix[ 1 ][ 0 ] = sin_pitch_sin_heading		matrix[ 1 ][ 1 ] = 0				matrix[ 1 ][ 2 ] = sin_pitch * cos_heading
				matrix[ 2 ][ 0 ] = 0							matrix[ 2 ][ 1 ] = -sin_pitch		matrix[ 2 ][ 2 ] = 0
			}*/

			heading_around_y = Math::Atan2( -matrix[ 0 ][ 2 ], matrix[ 0 ][ 0 ] );
		}
		else
		{
			/*
			const auto sin_heading = matrix[ 2 ][ 0 ] / cos_pitch;
			const auto cos_heading = matrix[ 2 ][ 2 ] / cos_pitch;
			*/

			/* Since cos_pitch is a common denominator between these two terms, we can directly pass the numerators of these fractions to atan2() instead. */

			heading_around_y = Math::Atan2( matrix[ 2 ][ 0 ], matrix[ 2 ][ 2 ] );

			/* Similar method for bank: */

			bank_around_z = Math::Atan2( matrix[ 0 ][ 1 ], matrix[ 1 ][ 1 ] );
		}
	}

/* Conversions Between Cartesian, Polar, Cylindrical & Spherical Coordinates. */

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