#pragma once

// Project Includes.
#include "Concepts.h"
#include "Constants.h"
#include "Unit.h"

namespace Framework
{
	namespace Math
	{
		template< std::floating_point > class Degrees;
		template< std::floating_point > class Radians;

		template< std::floating_point FloatType >
		class Degrees : public Unit< FloatType, Degrees >
		{
			using Base = Unit< FloatType, Degrees >;

		public:
			constexpr Degrees() : Base() {}
			constexpr Degrees( const Degrees& other ) : Base( other ) {}
			
			explicit Degrees( Initialization::NoInitialization ) : Base( NO_INITIALIZATION ) {}

			constexpr explicit Degrees( FloatType value ) : Base( value ) {}

			/* Construct (convert) from other underlying type. */
			template< typename OtherType >
			constexpr Degrees( const Unit< OtherType, Degrees >& other ) : Base( other ) {}

			/* Construct from Radians. */
			constexpr Degrees( const Unit< FloatType, Radians >& radians ) : Base( FloatType( radians * Constants::RAD_TO_DEG ) ) {}
		};

		template< std::floating_point FloatType >
		class Radians : public Unit< FloatType, Radians >
		{
			using Base = Unit< FloatType, Radians >;

		public:
			constexpr Radians() : Base() {}
			constexpr Radians( const Radians& other ) : Base( other ) {}

			explicit Radians( Initialization::NoInitialization ) : Base( NO_INITIALIZATION ) {}

			constexpr explicit Radians( FloatType value ) : Base( value ) {}

			/* Construct (convert) from other underlying type. */
			template< typename OtherType >
			constexpr Radians( const Unit< OtherType, Radians >& other ) : Base( other ) {}

			/* Construct from Degrees. */
			constexpr Radians( const Unit< FloatType, Degrees >& degrees ) : Base( FloatType( degrees * Constants::DEG_TO_RAD ) ) {}
		};

		namespace Literals
		{
			constexpr Math::Degrees< float > operator"" _deg( long double value )
			{
				return Math::Degrees< float >( static_cast< float >( value ) );
			}

			constexpr Math::Degrees< float > operator"" _deg( unsigned long long value )
			{
				return Math::Degrees< float >( static_cast< float >( value ) );
			}

			constexpr Math::Degrees< double > operator"" _degd( long double value )
			{
				return Math::Degrees< double >( static_cast< double >( value ) );
			}

			constexpr Math::Degrees< double > operator"" _degd( unsigned long long value )
			{
				return Math::Degrees< double >( static_cast< double >( value ) );
			}

			constexpr Math::Radians< float > operator"" _rad( long double value )
			{
				return Math::Radians< float >( static_cast< float >( value ) );
			}

			constexpr Math::Radians< float > operator"" _rad( unsigned long long value )
			{
				return Math::Radians< float >( static_cast< float >( value ) );
			}

			constexpr Math::Radians< double > operator"" _radd( long double value )
			{
				return Math::Radians< double >( static_cast< double >( value ) );
			}

			constexpr Math::Radians< double > operator"" _radd( unsigned long long value )
			{
				return Math::Radians< double >( static_cast< double >( value ) );
			}
		};
	}

	using Degrees = Math::Degrees< float >;
	using Radians = Math::Radians< float >;

	using DegreesD = Math::Degrees< double >;
	using RadiansD = Math::Radians< double >;
}
