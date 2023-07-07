#pragma once

// Project Includes.
#include "Concepts.h"
#include "Initialization.h"
#include "Math.h"
#include "Utility.h"

namespace Framework
{
	template< Concepts::Arithmetic Type, 
			  template< class > class Derived > /* Explicitly restrict to template classes */
	class Unit
	{
		using DerivedType = Unit< Type, Derived >;

	public:
	/* Constructors. */
		constexpr Unit() : value( Type( 0 ) ) {}
		constexpr Unit( const DerivedType& other ) = default;

		explicit Unit( Initialization::NoInitialization ) {}

		/* Construct from underlying type. */
		constexpr explicit Unit( Type value ) : value( value ) {}

		/* Construct from OTHER underlying type. */
		template< typename OtherType >
		constexpr Unit( const Unit< OtherType, Derived >&other ) : value( Type( other.value ) ) {}

		constexpr ~Unit() = default;

	/* Comparison Operators. */
		constexpr auto operator<=> ( const DerivedType& ) const = default;

		bool operator== ( const DerivedType & right_hand_side ) const
		{
			if constexpr( std::is_integral_v< Type > )
				return value == right_hand_side.value;
			else
				return Math::IsEqual( value, right_hand_side.value );
		}

		bool operator!= ( const DerivedType & right_hand_side ) const
		{
			return !operator== ( right_hand_side );
		}

	/* Arithmetic Operations: Unary operators. */
		/* Return the value as-is. */
		constexpr DerivedType operator+ () const { return *this; }
		constexpr DerivedType operator- () const requires( std::is_floating_point_v< Type > || std::is_signed_v< Type > )
		{
			return DerivedType( Type( -1 ) * value );
		}

	/* Arithmetic Operations: Binary operators (with an Unit). */
		constexpr DerivedType operator+ ( const DerivedType right_hand_side ) const
		{
			return DerivedType{ value + right_hand_side.value };
		}

		DerivedType& operator+= ( const DerivedType right_hand_side )
		{
			value += right_hand_side.value;

			return *this;
		}

		constexpr DerivedType operator- ( const DerivedType right_hand_side ) const
		{
			return DerivedType{ value - right_hand_side.value };
		}

		DerivedType& operator-= ( const DerivedType right_hand_side )
		{
			value -= right_hand_side;

			return *this;
		}

		/* No operators * and *= are NOT defined on purpose; This is because the multiplication of 2 Units would return UnitSquared (or some other name), which is clearly not of type Unit. */

		constexpr DerivedType operator/ ( const DerivedType right_hand_side ) const
		{
			DerivedType result( *this );
			if constexpr( std::is_integral_v< Type > )
				result.value /= right_hand_side.value; // Divide directly as division of 1/right_hand_side will give zero when right_hand_side > 1.
			else
			{
				const auto inverse_of_right_hand_side = Type( 1 ) / right_hand_side; // We can calculate the inverse safely.
				result.value *= inverse_of_right_hand_side;
			}

			return result;
		}

		DerivedType& operator/= ( const DerivedType right_hand_side )
		{
			if constexpr( std::is_integral_v< Type > )
				value /= right_hand_side.value; // Divide directly as division of 1/right_hand_side will give zero when right_hand_side > 1.
			else
			{
				const auto inverse_of_right_hand_side = Type( 1 ) / right_hand_side; // We can calculate the inverse safely.
				value *= inverse_of_right_hand_side;
			}

			return *this;
		}

	/* Arithmetic Operations: Binary operators (with a scalar), of the the form unit-operator-scalar. */
		constexpr DerivedType operator+ ( const Type right_hand_side ) const
		{
			return DerivedType{ value + right_hand_side };
		}

		constexpr DerivedType operator+= ( const Type right_hand_side ) const
		{
			value += right_hand_side;
			return *this;
		}

		constexpr DerivedType operator- ( const Type right_hand_side ) const
		{
			return DerivedType{ value - right_hand_side };
		}

		constexpr DerivedType operator-= ( const Type right_hand_side ) const
		{
			value -= right_hand_side;
			return *this;
		}

		constexpr DerivedType operator* ( const Type right_hand_side ) const
		{
			return DerivedType{ value * right_hand_side };
		}

		constexpr DerivedType operator*= ( const Type right_hand_side ) const
		{
			value *= right_hand_side;
			return *this;
		}

		constexpr DerivedType operator/ ( const Type right_hand_side ) const
		{
			return DerivedType{ value / right_hand_side };
		}

		constexpr DerivedType operator/= ( const Type right_hand_side ) const
		{
			value /= right_hand_side;
			return *this;
		}

	/* Arithmetic Operations: Binary operators (with a scalar), of the the form scalar-operator-unit. */
		friend DerivedType operator+ ( const Type left_hand_side_value, const DerivedType& right_hand_side_unit )
		{
			return DerivedType( left_hand_side_value + right_hand_side_unit.value );
		}

		friend DerivedType operator- ( const Type left_hand_side_value, const DerivedType& right_hand_side_unit )
		{
			return DerivedType( left_hand_side_value - right_hand_side_unit.value );
		}

		friend DerivedType operator* ( const Type left_hand_side_value, const DerivedType& right_hand_side_unit )
		{
			return DerivedType( left_hand_side_value * right_hand_side_unit.value );
		}

		friend DerivedType operator/ ( const Type left_hand_side_value, const DerivedType& right_hand_side_unit )
		{
			return DerivedType( left_hand_side_value / right_hand_side_unit.value );
		}

	/* Other Operators. */
		constexpr explicit operator Type() const { return value; }

	protected:
		Type value;
	};
}