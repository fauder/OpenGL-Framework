#pragma once

// Project Includes.
#include "Angle.hpp"
#include "ConditionalCompilation.h"
#include "Concepts.h"
#include "Vector.hpp"

#ifdef _DEBUG
#include "Assert.h"
#endif // _DEBUG

/* Assumption: A quaternion is always used for rotation and should therefore be normalized.
 * Results of this assumption: 
 * 1) On debug configuration: All non-default and user defined constructors (even the x,y,z,w & xyz,w constructors) warn the user if the values passed do not construct a unit quaternion.
 * 2) Inverse() simply returns the Conjugate().
 */
namespace Framework
{
	template< std::floating_point ComponentType >
	class QuaternionBase
	{
	private:
		using VectorType  = VectorBase< ComponentType, 3 >;
		using RadiansType = Math::Radians< ComponentType >;

	public:
	/* Constructors. */
		/* Identity quaternion w=1 & n=<0,0,0>. */
		constexpr QuaternionBase()
			:
			xyz(),
			w( 1 )
		{}

		constexpr QuaternionBase( const QuaternionBase& other )					= default;
		constexpr QuaternionBase( QuaternionBase && donor )						= default;
		constexpr QuaternionBase& operator = ( const QuaternionBase & other )	= default;
		constexpr QuaternionBase& operator = ( QuaternionBase && donor )		= default;

		constexpr ~QuaternionBase() = default;

		CONSTEXPR_ON_RELEASE QuaternionBase( const ComponentType x, const ComponentType y, const ComponentType z, const ComponentType w )
			:
			xyz( x, y, z ),
			w( w )
		{
	#ifdef _DEBUG
			if( IsIdentity() )
				return;

			const RadiansType half_angle( std::acos( w ) );
			const VectorType rotation_axis( xyz / std::sin( ComponentType( half_angle ) ) );
			ASSERT( rotation_axis.IsNormalized() && "QuaternionBase::QuaternionBase( angle, axis ): The axis vector provided is not normalized!" );
	#endif // _DEBUG
		}

		CONSTEXPR_ON_RELEASE QuaternionBase( const VectorType& xyz, const ComponentType w )
			:
			xyz( xyz ),
			w( w )
		{
	#ifdef _DEBUG
			if( IsIdentity() )
				return;

			const RadiansType half_angle( std::acos( w ) );
			const VectorType rotation_axis( xyz / std::sin( ComponentType( half_angle ) ) );
			ASSERT( rotation_axis.IsNormalized() && "QuaternionBase::QuaternionBase( angle, axis ): The axis vector provided is not normalized!" );
	#endif // _DEBUG
		}

		/* Expects a unit vector for the axis! */
		QuaternionBase( RadiansType angle, const VectorType& rotation_axis_normalized )
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
			return RadiansType( std::acos( w ) );
		}

		constexpr RadiansType Angle() const
		{
			return ComponentType{ 2 } * HalfAngle();
		}

		constexpr VectorType Axis() const
		{
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

	/* Arithmetic Operations. */
		constexpr QuaternionBase operator*( const QuaternionBase& other ) const
		{
			return QuaternionBase( w * other.xyz + other.w * xyz + Cross( xyz, other.xyz ),
								   w * other.w - Framework::Dot( xyz, other.xyz ) );
		}

		/* Returns a quaternion d such that d*a=b (where a = this Quaternion (i.e., *this)).
		 * Here, d is the angular displacement between this Quaternion and the other. */
		constexpr QuaternionBase Difference( const QuaternionBase& b ) const
		{
			return b * Inverse();
		}

		constexpr QuaternionBase Conjugate() const
		{
			return QuaternionBase( -xyz, w );
		}

		/* Sets this quaternion equal to its conjugate (i.e, [-x -y -z w]). */
		constexpr QuaternionBase& SetToConjugate()
		{
			xyz = -xyz;

			return *this;
		}

		/* Assumes a unit quaternion, hence, returns the conjugate since inverse = conjugate / magnitude, where magnitude = 1 for a unit quaternion. */
		constexpr QuaternionBase Inverse() const
		{
			return Conjugate();
		}

		/* Sets this quaternion equal to its inverse, which is its conjugate since a unit quaternion is assumed. (i.e, [w -x -y -z]). */
		constexpr QuaternionBase& Invert()
		{
			return SetToConjugate();
		}

	/* Other Arithmetic Operations. */
		/* With self. */
		constexpr ComponentType Dot() const
		{
			ComponentType result( 0 );

			return w * w + xyz.Dot();
		}

		template< std::floating_point ComponentType_ > // Have to use a different template parameter here because C++...
		constexpr friend ComponentType_ Dot( const QuaternionBase< ComponentType_ >& q1, const QuaternionBase< ComponentType_ >& q2 );

	private:
		VectorType xyz;
		ComponentType w;
	};

	template< std::floating_point ComponentType >
	constexpr ComponentType Dot( const QuaternionBase< ComponentType >& q1, const QuaternionBase< ComponentType >& q2 )
	{
		return q1.w * q2.w + Dot( q1.xyz, q2.xyz );
	}

	using Quaternion  = QuaternionBase< float  >;
	using QuaternionD = QuaternionBase< double >;
}