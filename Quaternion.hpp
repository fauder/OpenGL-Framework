#pragma once

// Project Includes.
#include "Angle.hpp"
#include "ConditionalCompilation.h"
#include "Concepts.h"
#include "Vector.hpp"

#ifdef _DEBUG
#include "Assert.h"
#endif // _DEBUG

namespace Framework
{
	template< std::floating_point ComponentType = float >
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
			w( 1 ),
			xyz()
		{}

		constexpr QuaternionBase( const QuaternionBase& other )					= default;
		constexpr QuaternionBase( QuaternionBase && donor )						= default;
		constexpr QuaternionBase& operator = ( const QuaternionBase & other )	= default;
		constexpr QuaternionBase& operator = ( QuaternionBase && donor )		= default;

		constexpr ~QuaternionBase() = default;

		CONSTEXPR_ON_RELEASE QuaternionBase( const ComponentType w, const ComponentType x, const ComponentType y, const ComponentType z )
			:
			w( w ),
			xyz( x, y, z )
		{
	#ifdef _DEBUG
			const RadiansType theta( std::acos( w ) );
			const VectorType rotation_axis( xyz / std::sin( ComponentType( theta ) ) );
			ASSERT( rotation_axis.IsNormalized() && "QuaternionBase::QuaternionBase( angle, axis ): The axis vector provided is not normalized!" );
	#endif // _DEBUG
		}

		CONSTEXPR_ON_RELEASE QuaternionBase( const ComponentType w, const VectorType& xyz )
			:
			w( w ),
			xyz( xyz )
		{
	#ifdef _DEBUG
			const RadiansType theta( std::acos( w ) );
			const VectorType rotation_axis( xyz / std::sin( ComponentType( theta ) ) );
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

	private:
		ComponentType w;
		VectorType xyz;
	};

	using Quaternion  = QuaternionBase< float  >;
	using QuaternionD = QuaternionBase< double >;
}