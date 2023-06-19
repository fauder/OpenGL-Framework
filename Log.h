#pragma once

// Project Includes.
#include "Matrix.hpp"
#include "Utility.h"
#include "Vector.hpp"

// std Includes.
#include <iomanip>
#include <iostream>

namespace Log
{
	template< typename Type, unsigned int Size,
			  typename = typename std::enable_if< std::is_arithmetic_v< Type > > >
	void Vector( const VectorBase< Type, Size >& vector )
	{
		std::cout << '<' << vector.Data(0);

		for( auto i = 1; i < Size; i++ )
			std::cout << ", " << vector.Data( i );

		std::cout << '>';
	}

	template< typename Type, unsigned int RowSize, unsigned int ColumnSize,
			  typename = typename std::enable_if< std::is_arithmetic_v< Type > > >
	void Matrix( const MatrixBase< Type, RowSize, ColumnSize >& matrix )
	{
		int width = 0;
		for( auto i = 0; i < RowSize; i++ )
			for( auto j = 1; j < ColumnSize; j++ )
				if( auto digit_count = Utility::DigitCount( ( unsigned int )matrix[ i ][ j ] ); digit_count > width )
					width = digit_count;

		for( auto i = 0; i < RowSize; i++ )
		{
			std::cout << '|' << std::setw( width ) << std::setprecision( 3 ) << std::fixed << matrix[ i ][ 0 ];
			for( auto j = 1; j < ColumnSize; j++ )
				std::cout << ", " << std::setw( width ) << std::setprecision( 3 ) << std::fixed << matrix[ i ][ j ];
			std::cout << " |" << "\n";
		}
	}
}