#pragma once

// Project Includes.
#include "Angle.hpp"
#include "ConditionalCompilation.h"
#include "Concepts.h"
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

		constexpr Quaternion( const Quaternion& other )					= default;
		constexpr Quaternion( Quaternion && donor )						= default;
		constexpr Quaternion& operator = ( const Quaternion & other )	= default;
		constexpr Quaternion& operator = ( Quaternion && donor )		= default;

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

		/* Expects a unit vector for the axis! */
		Quaternion( RadiansType angle, const VectorType& rotation_axis_normalized )
		{
			const RadiansType half_angle = angle / 2.0f;
			w   = std::cos( ComponentType( half_angle ) );
			xyz = std::sin( ComponentType( half_angle ) ) * rotation_axis_normalized;

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

			return RadiansType( std::acos( w ) );
		}

		// Returns half the angular displacement between *this Quaternion and the other.
		constexpr RadiansType HalfAngleBetween( const Quaternion& other ) const
		{
		#ifdef _DEBUG
			ASSERT( this->IsNormalized() && R"(Quaternion::HalfAngleBetween(other) : The quaternion "*this" is not normalized!)" );
			ASSERT( other.IsNormalized() && R"(Quaternion::HalfAngleBetween(other) : The quaternion "other" is not normalized!)" );
		#endif

			return RadiansType( std::acos( Framework::Math::Dot( *this, other ) ) );
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

			return VectorType( xyz / std::sin( ComponentType( HalfAngle() ) ) );
		}

		constexpr ComponentType SquareMagnitude() const { return Dot(); }
		ComponentType Magnitude() const { return std::sqrt( SquareMagnitude() ); }

		constexpr bool IsIdentity() const
		{
			return Math::IsEqual( std::abs( w ), ComponentType( 1 ) ) && xyz.IsZero();
		}

		constexpr bool IsNormalized() const
		{
			return Math::IsEqual( SquareMagnitude(), ComponentType( 1 ) );
		}

	/* Arithmetic Operations: Binary operators (with a scalar), of the the form vector-operator-scalar. */
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

	/* Other Arithmetic Operations. */
		constexpr Quaternion operator*( const Quaternion& other ) const
		{
			return Quaternion( w * other.xyz + other.w * xyz + Framework::Math::Cross( xyz, other.xyz ),
								   w * other.w - Framework::Math::Dot( xyz, other.xyz ) );
		}

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
			Quaternion result( *this );

			// Check for the case of an identity quaternion. This will protect against divide by zero.
			if( std::abs( result.w ) < ComponentType{ 0.9999 } )
			{
				// Extract the half angle alpha (alpha = theta/2).
				ComponentType alpha = std::acos( result.w );

				// Compute new alpha, w & xyz values.
				ComponentType newAlpha = alpha * exponent;
				result.w = std::cos( newAlpha );
				result.xyz *= std::sin( newAlpha ) / std::sin( alpha );
			}

			return result;
		}

		constexpr Quaternion SlerpNaive( const Quaternion& other, const ComponentType t ) const
		{
			return DifferenceBetween( other ).Exp( t ) * *this;
		}

	/* Other Arithmetic Operations. */
		/* With self. */
		constexpr ComponentType Dot() const
		{
			ComponentType result( 0 );

			return w * w + xyz.Dot();
		}

		template< std::floating_point ComponentType_ > // Have to use a different template parameter here because C++...
		constexpr friend ComponentType_ Dot( const Quaternion< ComponentType_ >& q1, const Quaternion< ComponentType_ >& q2 );

	private:
		VectorType xyz;
		ComponentType w;
	};

	template< std::floating_point ComponentType >
	constexpr ComponentType Dot( const Quaternion< ComponentType >& q1, const Quaternion< ComponentType >& q2 )
	{
		return q1.w * q2.w + Dot( q1.xyz, q2.xyz );
	}
}

namespace Framework
{
	using Quaternion  = Math::Quaternion< float  >;
	using QuaternionD = Math::Quaternion< double >;
}
