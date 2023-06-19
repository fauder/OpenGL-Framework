#pragma once

// std Includes.
#include <array>

template< typename Type, unsigned int RowSize, unsigned int ColumnSize,
		  typename = typename std::enable_if< std::is_arithmetic_v< Type > > >
class MatrixBase
{
public:
	struct MatrixInitializeZero {};
	static constexpr MatrixInitializeZero MATRIX_INITIALIZE_ZERO;

public:
/* Constructors. */
	MatrixBase()
		:
		data{ 0 }
	{
		if constexpr( RowSize == ColumnSize ) // Do not initialize non-square matrices to identity.
			for( auto i = 0; i < RowSize; i++ )
				data[ i ][ i ] = Type( 1 );
	}

	MatrixBase( const MatrixInitializeZero )
		:
		data{ 0 }
	{
	}

	template< typename... Values >
	MatrixBase( Values... values )
		:
		data{ values... }
	{
	}

	~MatrixBase()
	{
	}

/* Getters & Setters. */
	const Type* Data() const { return data; };
	Type* operator[] ( const unsigned int row_index ) { return data[ row_index ]; }
	const Type* operator[] ( const unsigned int row_index ) const { return data[ row_index ]; }

	template< typename... Values >
	MatrixBase& Set( Values... values )
	{
		// Utilize fold expressions with a lambda to "loop over" the parameter pack.
		int i = 0;
		( /* Lambda: */ [&] { data[ i++ ] = values; }( ), ... );

		return *this;
	}

/* Other Queries. */
	constexpr unsigned int RowCount()     const { return RowSize; }
	constexpr unsigned int ColumnCount()  const { return ColumnSize; }
	constexpr unsigned int ElementCount() const { return RowSize * ColumnSize; }

protected:
	/* Row-major. */
	Type data[ RowSize ][ ColumnSize ];
};

using Matrix2x2  = MatrixBase< float,  2, 2 >;
using Matrix3x3  = MatrixBase< float,  3, 3 >;
using Matrix4x4  = MatrixBase< float,  4, 4 >;
using Matrix2x2D = MatrixBase< double, 2, 2 >;
using Matrix3x3D = MatrixBase< double, 3, 3 >;
using Matrix4x4D = MatrixBase< double, 4, 4 >;
using Matrix2x2I = MatrixBase< int,    2, 2 >;
using Matrix3x3I = MatrixBase< int,    3, 3 >;
using Matrix4x4I = MatrixBase< int,    4, 4 >;