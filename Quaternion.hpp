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
	template< std::floating_point ComponentType > // Have to use a different template parameter here because C++...
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
}

namespace Framework
{
	using Quaternion  = Math::Quaternion< float  >;
	using QuaternionD = Math::Quaternion< double >;
}
