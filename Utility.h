#pragma once

// Project Includes.
#include "Vector.hpp"

// std Includes.
#include <iostream>

namespace Utility
{
	template< typename Type, unsigned int Size,
			  typename = typename std::enable_if< std::is_arithmetic_v< Type > > >
	void Dump( const VectorBase< Type, Size >& vector )
	{
		std::cout << '<' << vector.Data(0);

		for( auto i = 1; i < Size; i++ )
			std::cout << ", " << vector.Data( i );

		std::cout << '>';
	}
}