#pragma once

// Project Includes.
#include "Concepts.h"
#include "Matrix.hpp"
#include "Utility.h"
#include "Vector.hpp"

// std Includes.
#include <iomanip>
#include <iostream>

namespace Framework::Log
{
	template< Concepts::Arithmetic Coordinate, unsigned int Size > requires( Concepts::NonZero< Size > )
	void Dump( const Math::Vector< Coordinate, Size >& vector )
	{
		std::cout << '<' << vector[ 0 ];

		for( auto i = 1; i < Size; i++ )
			std::cout << ", " << vector[ i ];

		std::cout << '>';
	}

	template< Concepts::Arithmetic Coordinate, unsigned int Size > requires( Concepts::NonZero< Size > )
	void Dump( const char* debug_name, const Math::Vector< Coordinate, Size >& vector )
	{
		std::cout << debug_name << ": ";
		Dump( vector );
	}

	template< Concepts::Arithmetic Coordinate, unsigned int RowSize, unsigned int ColumnSize >
		requires( Concepts::NonZero< RowSize > && Concepts::NonZero< ColumnSize > )
	void Dump( const MatrixBase< Coordinate, RowSize, ColumnSize >& matrix )
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

	template< Concepts::Arithmetic Coordinate, unsigned int RowSize, unsigned int ColumnSize >
		requires( Concepts::NonZero< RowSize >&& Concepts::NonZero< ColumnSize > )
	void Dump( const char* debug_name, const MatrixBase< Coordinate, RowSize, ColumnSize >& matrix )
	{
		std::cout << debug_name << ":\n";
		Dump( matrix );
	}

	template< typename T >
	void DumpNewLine( const T& stuff )
	{
		Dump( stuff );
		std::cout << "\n";
	}

	template< typename T >
	void DumpNewLine( const char* debug_name, const T& stuff )
	{
		Dump( debug_name, stuff );
		std::cout << "\n";
	}
}
