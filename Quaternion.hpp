#pragma once

// Project Includes.
#include "Angle.hpp"
#include "ConditionalCompilation.h"
#include "Concepts.h"
#include "Math.h"
#include "TypeTraits.h"
#include "Vector.hpp"

#ifdef _DEBUG
#include "Assert.h"
#endif // _DEBUG

namespace Framework::Math
{
/* Forward Declarations. */
	template< std::floating_point ComponentType >
	class Quaternion;

	template< std::floating_point ComponentType >
	constexpr ComponentType Dot( const Quaternion< ComponentType >& q1, const Quaternion< ComponentType >& q2 );

	template< Concepts::Arithmetic Type, std::size_t RowSize, std::size_t ColumnSize >
		requires Concepts::NonZero< RowSize > && Concepts::NonZero< ColumnSize >
	class Matrix;

	template< std::floating_point ComponentType >
	class Quaternion
	{
	private:
		using VectorType  = Math::Vector< ComponentType, 3 >;
		using RadiansType = Math::Radians< ComponentType >;

	public:
	/* Constructors. */
		/* Identity quaternion w=1 & n=<0,0,0>. */
		constexpr Quaternion()
			:
			xyz(),
			w( 1 )
		{}

		constexpr Quaternion( const Quaternion& other )				= default;
		constexpr Quaternion( Quaternion && donor )					= default;
		constexpr Quaternion& operator= ( const Quaternion& other )	= default;
		constexpr Quaternion& operator= ( Quaternion && donor )		= default;

		constexpr ~Quaternion() = default;

		constexpr Quaternion( const ComponentType x, const ComponentType y, const ComponentType z, const ComponentType w )
			:
			xyz( x, y, z ),
			w( w )
		{}

		constexpr Quaternion( const VectorType& xyz, const ComponentType w )
			:
			xyz( xyz ),
			w( w )
		{}

		/* Promotes a 3D vector to a pure quaternion. */
		constexpr Quaternion( const VectorType& xyz )
			:
			xyz( xyz ),
			w( ComponentType( 0 ) )
		{}

		/* Expects a unit vector for the axis! */
		Quaternion( RadiansType angle, const VectorType& rotation_axis_normalized )
		{
			const RadiansType half_angle = angle / 2.0f;
			w   = Cos( half_angle );
			xyz = Sin( half_angle ) * rotation_axis_normalized;

	#ifdef _DEBUG
			ASSERT( rotation_axis_normalized.IsNormalized() && "QuaternionBase::QuaternionBase( angle, axis ): The axis vector provided is not normalized!" );
	#endif // _DEBUG
		}

	/* Comparison Operators. */
		//constexpr auto operator<=>( const Quaternion& ) const = default;

		bool operator==( const Quaternion & right_hand_side ) const
		{
			return Math::IsEqual( w, right_hand_side.w ) && xyz == right_hand_side.xyz;
		}

		bool operator!=( const Quaternion & right_hand_side ) const
		{
			return !operator==( right_hand_side );
		}

	/* Other Queries. */
		constexpr RadiansType HalfAngle() const
		{
		#ifdef _DEBUG
			ASSERT( IsNormalized() && R"(QuaternionBase::HaflAngle(): The quaternion "*this" is not normalized!)" );
		#endif // _DEBUG

			return RadiansType( Acos( w ) );
		}

		// Returns half the angular displacement between *this Quaternion and the other.
		constexpr RadiansType HalfAngleBetween( const Quaternion& other ) const
		{
		#ifdef _DEBUG
			ASSERT( this->IsNormalized() && R"(Quaternion::HalfAngleBetween(other) : The quaternion "*this" is not normalized!)" );
			ASSERT( other.IsNormalized() && R"(Quaternion::HalfAngleBetween(other) : The quaternion "other" is not normalized!)" );
		#endif

			return RadiansType( Acos( Math::Dot( *this, other ) ) );
		}

		constexpr RadiansType Angle() const
		{
			return ComponentType{ 2 } * HalfAngle();
		}

		// Returns the angular displacement between *this Quaternion and the other.
		constexpr RadiansType AngleBetween( const Quaternion& other ) const
		{
			return ComponentType{ 2 } * HalfAngleBetween( other );
		}

		/* Returns a NaN vector when the half angle represented by *this is zero. */
		constexpr VectorType Axis() const
		{
		#ifdef _DEBUG
			ASSERT( IsNormalized() && R"(QuaternionBase::Axis(): The quaternion "*this" is not normalized!)" );
		#endif // _DEBUG

			// Instead of incurring the cost of sin( HalfAngle() ), sin(theta) can be derived by sqrt(1-w*w), as we know w = cos(theta).
			return VectorType( xyz / Sqrt( ComponentType{ 1 } - w * w ) );
		}

		constexpr ComponentType SquareMagnitude() const { return Dot(); }
		ComponentType Magnitude() const { return Sqrt( SquareMagnitude() ); }

		constexpr bool IsIdentity() const
		{
			return Math::IsEqual( Abs( w ), ComponentType( 1 ) ) && xyz.IsZero();
		}

		constexpr bool IsNormalized() const
		{
			return Math::IsEqual( SquareMagnitude(), ComponentType( 1 ) );
		}

	/* Arithmetic Operations: Unary operators. */
		constexpr Quaternion operator- () const
		{
			return *this * -1;
		}

	/* Arithmetic Operations: Binary operators (with a Quaternion). */
		constexpr Quaternion operator+ ( const Quaternion other ) const
		{
			return { xyz + other.xyz, w + other.w };
		}

		constexpr Quaternion& operator+= ( const Quaternion other )
		{
			xyz += other.xyz;
			w += other.w;

			return *this;
		}

	/* Arithmetic Operations: Binary operators (with a scalar), of the the form quaternion-operator-scalar. */
		constexpr Quaternion operator* ( const ComponentType scalar ) const
		{
			return { xyz * scalar, w * scalar };
		}

		constexpr Quaternion& operator*= ( const ComponentType scalar )
		{
			w   *= scalar;
			xyz *= scalar;

			return *this;
		}

		constexpr Quaternion operator/ ( const ComponentType scalar ) const
		{
			const auto inverse_of_scalar = ComponentType( 1 ) / scalar; // We can calculate the inverse safely.
			return { xyz * inverse_of_scalar, w * inverse_of_scalar };
		}

		constexpr Quaternion& operator/= ( const ComponentType scalar )
		{
			const auto inverse_of_scalar = ComponentType( 1 ) / scalar; // We can calculate the inverse safely.
			w   *= inverse_of_scalar;
			xyz *= inverse_of_scalar;

			return *this;
		}

	/* Arithmetic Operations: Binary operators (with a scalar), of the the form scalar-operator-quaternion. */
		template< std::floating_point ComponentType_ > // Have to use a different template parameter here because C++...
		friend constexpr Quaternion< ComponentType_ > operator* ( const ComponentType_ scalar, Quaternion< ComponentType_ > quaternion );

		template< std::floating_point ComponentType_ > // Have to use a different template parameter here because C++...
		friend constexpr Quaternion< ComponentType_ > operator/ ( const ComponentType_ scalar, Quaternion< ComponentType_ > quaternion );

	/* Other Arithmetic Operations. */
		constexpr Quaternion operator* ( const Quaternion& other ) const
		{
			return Quaternion( w * other.xyz + other.w * xyz + Math::Cross( xyz, other.xyz ),
							   w * other.w - Math::Dot( xyz, other.xyz ) );
		}

		/* Do not use this as it is slow. 
		 * Use Transform(), as it is faster. */
		constexpr Vector3 Transform_Naive( const Vector3& vector_to_rotate ) const
		{
			 return ( *this * Quaternion( vector_to_rotate ) * Inverse() ).xyz;
		}

		/* Use this as it is faster than Transform_Naive(). */
		constexpr Vector3 Transform( const Vector3& vector_to_rotate ) const
		{
		#ifdef _DEBUG
			ASSERT( IsNormalized() && R"(QuaternionBase::Transform(): The quaternion "*this" is not normalized!)" ); // The derivation assumes unit quaternions.
		#endif // _DEBUG

			/* Derivation can be found at https://gamesandsimulations.fandom.com/wiki/Quaternions, which is dead but can be retrieved via wayback machine at
			 * https://web.archive.org/web/20191115092410/https://gamesandsimulations.fandom.com/wiki/Quaternions. */

			const auto t = ComponentType( 2 ) * Math::Cross( xyz, vector_to_rotate );
			return vector_to_rotate + w * t - Math::Cross( t, xyz );
		}

		/* With self. */
		constexpr Quaternion Normalized() const
		{
			return *this / Magnitude();
		}

		constexpr Quaternion& Normalize()
		{
			return *this /= Magnitude();
		}

		constexpr ComponentType Dot() const
		{
			ComponentType result( 0 );

			return w * w + xyz.Dot();
		}

		template< std::floating_point ComponentType_ > // Have to use a different template parameter here because C++...
		friend constexpr ComponentType_ Dot( const Quaternion< ComponentType_ >& q1, const Quaternion< ComponentType_ >& q2 );

		constexpr Quaternion Conjugate() const
		{
			return Quaternion( -xyz, w );
		}

		/* Sets this quaternion equal to its conjugate (i.e, [-x -y -z w]). */
		constexpr Quaternion& SetToConjugate()
		{
			xyz = -xyz;

			return *this;
		}

		constexpr Quaternion Inverse() const
		{
			return Conjugate() / Dot();
		}

		/* Assumes *this is a unit quaternion. Faster than Inverse().
		 * Simply returns the conjugate since inverse = conjugate / magnitude, where magnitude = 1 for a unit quaternion. */
		constexpr Quaternion Inverse_Normalized() const
		{
		#ifdef _DEBUG
			ASSERT( IsNormalized() && R"(Quaternion::Inverse_Normalized() : The quaternion "*this" is not normalized!)" );
		#endif

			return Conjugate();
		}

		constexpr Quaternion& Invert()
		{
			return *this = Inverse();
		}

		/* Sets this quaternion equal to its inverse, which is its conjugate since a unit quaternion is assumed. (i.e, [w -x -y -z]). */
		constexpr Quaternion& Invert_Normalized()
		{
			return SetToConjugate();
		}

		/* Assumes unit quaternions.
		 * Returns a quaternion d such that d*a=b (where a = *this Quaternion).
		 * Here, d is the angular displacement between *this and the other. */
		constexpr Quaternion DifferenceBetween( const Quaternion& b ) const
		{
			return b * Inverse();
		}

		/* Taken from https://gamemath.com/book/orient.html#quaternions. */
		constexpr Quaternion Exp( const ComponentType exponent ) const
		{
		#ifdef _DEBUG
			ASSERT( IsNormalized() && R"(Quaternion::Exp() : The quaternion "*this" is not normalized!)" );
		#endif

			Quaternion result( *this );

			// Check for the case of an identity quaternion. This will protect against divide by zero.
			if( Abs( result.w ) < ComponentType{ 0.9999 } )
			{
				// Extract the half angle alpha (alpha = theta/2).
				const RadiansType alpha = Acos( result.w );

				// Compute new alpha, w & xyz values.
				const RadiansType newAlpha = alpha * exponent;
				result.w = Cos( newAlpha );
				result.xyz *= Sin( newAlpha ) / Sin( alpha );
			}

			return result;
		}

		template< std::floating_point ComponentType_ > // Have to use a different template parameter here because C++...
		friend constexpr Quaternion< ComponentType_ > Lerp( const Quaternion< ComponentType_ > q1, const Quaternion< ComponentType_ >& q2, const ComponentType_ t );

		template< std::floating_point ComponentType_ > // Have to use a different template parameter here because C++...
		friend constexpr Quaternion< ComponentType_ > Nlerp( const Quaternion< ComponentType_ > q1, const Quaternion< ComponentType_ >& q2, const ComponentType_ t );

		/* The algebraic derivation. Included for completion. Not to be used in production code, as it is quite inefficient. */
		template< std::floating_point ComponentType_ > // Have to use a different template parameter here because C++...
		friend constexpr Quaternion< ComponentType_ > Slerp_Naive( const Quaternion< ComponentType_ >& q1, const Quaternion< ComponentType_ >& q2, const ComponentType_ t );

		/* Geometric derivation. Computationally more efficient than the naive (algebraic) derivation. */
		template< std::floating_point ComponentType_ > // Have to use a different template parameter here because C++...
		friend constexpr Quaternion< ComponentType_ > Slerp( Quaternion< ComponentType_ >& q1, Quaternion< ComponentType_ > q2, const ComponentType_ t );

		template< std::floating_point ComponentType_ > // Have to use a different template parameter here because C++...
		friend constexpr Matrix< ComponentType_, 4, 4 > QuaternionToMatrix( const Quaternion< ComponentType_ >& quaternion );

		/* Source: https://gamemath.com/book/orient.html#euler_to_matrix. */
		template< std::floating_point ComponentType_ > // Have to use a different template parameter here because C++...
		friend constexpr Quaternion< ComponentType_ > MatrixToQuaternion( const Matrix< ComponentType_, 4, 4 >& matrix );

		template< std::floating_point ComponentType_ > // Have to use a different template parameter here because C++...
		friend constexpr Quaternion< ComponentType_ > EulerToQuaternion( const Radians< ComponentType_ > heading_around_y, const Radians< ComponentType_ > pitch_around_x, const Radians< ComponentType_ > bank_around_z );

		/* Since the type system can not deduce the template type when passed Degrees in the specific case of the above function, this overload is provided for ease-of-use. */
		template< std::floating_point ComponentType_ > // Have to use a different template parameter here because C++...
		friend constexpr Quaternion< ComponentType_ > EulerToQuaternion( const Degrees< ComponentType_ > heading_around_y, const Degrees< ComponentType_ > pitch_around_x, const Degrees< ComponentType_ > bank_around_z );

	private:
		VectorType xyz;
		ComponentType w;
	};

	template< std::floating_point ComponentType >
	constexpr Quaternion< ComponentType > operator* ( const ComponentType scalar, Quaternion< ComponentType > quaternion )
	{
		return quaternion * scalar;
	}

	template< std::floating_point ComponentType >
	constexpr Quaternion< ComponentType > operator/ ( const ComponentType scalar, Quaternion< ComponentType > quaternion )
	{
		return quaternion / scalar;
	}

	template< std::floating_point ComponentType >
	constexpr ComponentType Dot( const Quaternion< ComponentType >& q1, const Quaternion< ComponentType >& q2 )
	{
		return q1.w * q2.w + Dot( q1.xyz, q2.xyz );
	}

	template< std::floating_point ComponentType >
	constexpr Quaternion< ComponentType > Lerp( const Quaternion< ComponentType > q1, const Quaternion< ComponentType >& q2, const ComponentType t )
	{
		return ( ComponentType{ 1 } - t ) * q1 + t * q2;
	}

	template< std::floating_point ComponentType >
	constexpr Quaternion< ComponentType > Nlerp( const Quaternion< ComponentType > q1, const Quaternion< ComponentType >& q2, const ComponentType t )
	{
	#ifdef _DEBUG
		ASSERT( q1.IsNormalized() && R"(Quaternion::Nlerp() : The quaternion q1 is not normalized!)" );
		ASSERT( q2.IsNormalized() && R"(Quaternion::Nlerp() : The quaternion q2 is not normalized!)" );
	#endif

		return Lerp( q1, q2, t ).Normalized();
	}

	/* The algebraic derivation. Included for completion. Not to be used in production code, as it is quite inefficient. */
	template< std::floating_point ComponentType >
	constexpr Quaternion< ComponentType > Slerp_Naive( const Quaternion< ComponentType >& q1, const Quaternion< ComponentType >& q2, const ComponentType t )
	{
	#ifdef _DEBUG
		ASSERT( q1.IsNormalized() && R"(Quaternion::Slerp_Naive() : The quaternion q1 is not normalized!)" );
		ASSERT( q2.IsNormalized() && R"(Quaternion::Slerp_Naive() : The quaternion q2 is not normalized!)" );
	#endif

		return q1.DifferenceBetween( q2 ).Exp( t ) * q1;
	}

	/* Geometric derivation. Computationally more efficient than the naive (algebraic) derivation. */
	template< std::floating_point ComponentType >
	constexpr Quaternion< ComponentType > Slerp( Quaternion< ComponentType >& q1, Quaternion< ComponentType > q2, const ComponentType t )
	{
	#ifdef _DEBUG
		ASSERT( q1.IsNormalized() && R"(Quaternion::Slerp() : The quaternion q1 is not normalized!)" );
		ASSERT( q2.IsNormalized() && R"(Quaternion::Slerp() : The quaternion q2 is not normalized!)" );
	#endif

		using RadiansType = Radians< ComponentType >;

		const auto dot = Math::Dot( q1, q2 );

		if( dot < 0 )
			/* Negate one of the input quaternions, to take the shorter 4D "arc". */
			q2 = -q2;

		const auto cos_theta = dot < 0 ? -dot : +dot;

		if( cos_theta > Framework::TypeTraits< ComponentType >::OneMinusEpsilon() )
			/* Quaternions are too close; Revert back to a simple Nlerp(). */
			return Nlerp( q1, q2, t );

		const auto sin_theta = Sqrt( ComponentType{ 1 } - cos_theta * cos_theta ); // sqrt() is faster than sin().

		/* Let's leverage the fact that now we have sin_theta & cos_theta in order to more efficiently find theta, compared to using acos/asin( theta ). */
		const RadiansType theta( Atan2( sin_theta, cos_theta ) );

		/* Compute 1.0 / sin(theta) once here, to not have to incur division of the 4 components by sin(theta) later on. */
		const auto one_over_sin_theta = ComponentType{ 1 } / Sin( theta );

		return ( q1 * Sin( ( ComponentType{ 1 } - t ) * theta ) + q2 * Sin( t * theta ) )
				* one_over_sin_theta;
	}

	template< std::floating_point ComponentType >
	constexpr Matrix< ComponentType, 4, 4 > QuaternionToMatrix( const Quaternion< ComponentType >& quaternion )
	{
		const auto two_x2  = ComponentType( 2 ) * quaternion.xyz[ 0 ] * quaternion.xyz[ 0 ];
		const auto two_y2  = ComponentType( 2 ) * quaternion.xyz[ 1 ] * quaternion.xyz[ 1 ];
		const auto two_z2  = ComponentType( 2 ) * quaternion.xyz[ 2 ] * quaternion.xyz[ 2 ];
		const auto two_x_y = ComponentType( 2 ) * quaternion.xyz[ 0 ] * quaternion.xyz[ 1 ];
		const auto two_x_z = ComponentType( 2 ) * quaternion.xyz[ 0 ] * quaternion.xyz[ 2 ];
		const auto two_y_z = ComponentType( 2 ) * quaternion.xyz[ 1 ] * quaternion.xyz[ 2 ];
		const auto two_w_x = ComponentType( 2 ) * quaternion.w		  * quaternion.xyz[ 0 ];
		const auto two_w_y = ComponentType( 2 ) * quaternion.w		  * quaternion.xyz[ 1 ];
		const auto two_w_z = ComponentType( 2 ) * quaternion.w		  * quaternion.xyz[ 2 ];

		return Framework::Matrix4x4
		(
			{
				ComponentType( 1 ) - two_y2 - two_z2,		two_x_y + two_w_z,							two_x_z - two_w_y,							0.0f,
				two_x_y - two_w_z,							ComponentType( 1 ) - two_x2 - two_z2,		two_y_z + two_w_x,							0.0f,
				two_x_z + two_w_y,							two_y_z - two_w_x,							ComponentType( 1 ) - two_x2 - two_y2,		0.0f,
				0.0f,										0.0f,										0.0f,										1.0f
			}
		);
	}

	/* Source: https://gamemath.com/book/orient.html#euler_to_matrix. */
	template< std::floating_point ComponentType >
	constexpr Quaternion< ComponentType > MatrixToQuaternion( const Matrix< ComponentType, 4, 4 >& matrix )
	{
		ComponentType w, x, y, z;

		// Determine which of w, x, y, or z has the largest absolute value.
		float four_w_squared_minus_1 = matrix[ 0 ][ 0 ] + matrix[ 1 ][ 1 ] + matrix[ 2 ][ 2 ];
		float four_x_squared_minus_1 = matrix[ 0 ][ 0 ] - matrix[ 1 ][ 1 ] - matrix[ 2 ][ 2 ];
		float four_y_squared_minus_1 = matrix[ 1 ][ 1 ] - matrix[ 0 ][ 0 ] - matrix[ 2 ][ 2 ];
		float four_z_squared_minus_1 = matrix[ 2 ][ 2 ] - matrix[ 0 ][ 0 ] - matrix[ 1 ][ 1 ];

		std::size_t biggest_index = 0;
		float four_biggest_squared_minus1 = four_w_squared_minus_1;

		if( four_x_squared_minus_1 > four_biggest_squared_minus1 )
		{
			four_biggest_squared_minus1 = four_x_squared_minus_1;
			biggest_index = 1;
		}
		if( four_y_squared_minus_1 > four_biggest_squared_minus1 )
		{
			four_biggest_squared_minus1 = four_y_squared_minus_1;
			biggest_index = 2;
		}
		if( four_z_squared_minus_1 > four_biggest_squared_minus1 )
		{
			four_biggest_squared_minus1 = four_z_squared_minus_1;
			biggest_index = 3;
		}

		// Perform square root and division.
		float biggestVal = sqrt( four_biggest_squared_minus1 + ComponentType( 1 ) ) * ComponentType( 0.5 );
		float mult = ComponentType( 0.25 ) / biggestVal;

		// Apply table to compute quaternion values
		switch( biggest_index )
		{
			case 0:
				w = biggestVal;
				x = ( matrix[ 1 ][ 2 ] - matrix[ 2 ][ 1 ] ) * mult;
				y = ( matrix[ 2 ][ 0 ] - matrix[ 0 ][ 2 ] ) * mult;
				z = ( matrix[ 0 ][ 1 ] - matrix[ 1 ][ 0 ] ) * mult;
				break;

			case 1:
				x = biggestVal;
				w = ( matrix[ 1 ][ 2 ] - matrix[ 2 ][ 1 ] ) * mult;
				y = ( matrix[ 0 ][ 1 ] + matrix[ 1 ][ 0 ] ) * mult;
				z = ( matrix[ 2 ][ 0 ] + matrix[ 0 ][ 2 ] ) * mult;
				break;

			case 2:
				y = biggestVal;
				w = ( matrix[ 2 ][ 0 ] - matrix[ 0 ][ 2 ] ) * mult;
				x = ( matrix[ 0 ][ 1 ] + matrix[ 1 ][ 0 ] ) * mult;
				z = ( matrix[ 1 ][ 2 ] + matrix[ 2 ][ 1 ] ) * mult;
				break;

			case 3:
				z = biggestVal;
				w = ( matrix[ 0 ][ 1 ] - matrix[ 1 ][ 0 ] ) * mult;
				x = ( matrix[ 2 ][ 0 ] + matrix[ 0 ][ 2 ] ) * mult;
				y = ( matrix[ 1 ][ 2 ] + matrix[ 2 ][ 1 ] ) * mult;
				break;
		}

		return { x, y, z, w };
	}

	template< std::floating_point ComponentType >
	constexpr Quaternion< ComponentType > EulerToQuaternion( const Radians< ComponentType > heading_around_y, const Radians< ComponentType > pitch_around_x, const Radians< ComponentType > bank_around_z )
	{
		const auto half_heading( heading_around_y / ComponentType( 2 ) );
		const auto half_pitch( pitch_around_x / ComponentType( 2 ) );
		const auto half_bank( bank_around_z / ComponentType( 2 ) );

		const auto cos_half_heading = Cos( half_heading );
		const auto cos_half_pitch   = Cos( half_pitch	);
		const auto cos_half_bank    = Cos( half_bank	);

		const auto sin_half_heading = Sin( half_heading );
		const auto sin_half_pitch   = Sin( half_pitch	);
		const auto sin_half_bank    = Sin( half_bank	);

		const auto sin_half_heading_sin_half_pitch = sin_half_heading * sin_half_pitch;
		const auto cos_half_pitch_sin_half_bank    = cos_half_pitch   * sin_half_bank;
		const auto cos_half_heading_cos_half_bank  = cos_half_heading * cos_half_bank;
		const auto cos_half_heading_sin_half_bank  = cos_half_heading * sin_half_bank;

		return
		{
			/* x = */  cos_half_heading_cos_half_bank  * sin_half_pitch + sin_half_heading * cos_half_pitch_sin_half_bank,
			/* y = */ -cos_half_heading_sin_half_bank  * sin_half_pitch + sin_half_heading * cos_half_pitch * cos_half_bank,
			/* z = */ -sin_half_heading_sin_half_pitch * cos_half_bank  + cos_half_heading_sin_half_bank * cos_half_pitch,

			/* w = */  cos_half_heading_cos_half_bank  * cos_half_pitch + sin_half_heading_sin_half_pitch * sin_half_bank
		};
	}

	/* Since the type system can not deduce the template type when passed Degrees in the specific case of the Radians accepting version, this overload is provided for ease-of-use. */
	template< std::floating_point ComponentType >
	constexpr Quaternion< ComponentType > EulerToQuaternion( const Degrees< ComponentType > heading_around_y, const Degrees< ComponentType > pitch_around_x, const Degrees< ComponentType > bank_around_z )
	{
		return EulerToQuaternion< float >( Radians( heading_around_y ), Radians( pitch_around_x ), Radians( bank_around_z ) );
	}
}

namespace Framework
{
	using Quaternion  = Math::Quaternion< float  >;
	using QuaternionD = Math::Quaternion< double >;
}
