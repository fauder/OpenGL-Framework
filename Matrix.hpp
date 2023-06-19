#pragma once

// std Includes.
#include <array>

namespace Matrix
{
	struct MatrixInitializeZero {};
	static constexpr MatrixInitializeZero MATRIX_INITIALIZE_ZERO;
};

template< typename Type, unsigned int RowSize, unsigned int ColumnSize,
		  typename = typename std::enable_if< std::is_arithmetic_v< Type > > >
class MatrixBase
{
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

	MatrixBase( Matrix::MatrixInitializeZero )
		:
		data{ 0 }
	{
	}

	MatrixBase( const Type( &values )[ RowSize ][ ColumnSize ] )
	{
		for( auto i = 0; i < RowSize; i++ )
			for( auto j = 0; j < ColumnSize; j++ )
				data[ i ][ j ] = values[ i ][ j ];
	}

	MatrixBase( Type( &&values )[ RowSize ][ ColumnSize ] )
	{
		for( auto i = 0; i < RowSize; i++ )
			for( auto j = 0; j < ColumnSize; j++ )
				data[ i ][ j ] = values[ i ][ j ];
	}

	MatrixBase( const Type( &values )[ RowSize * ColumnSize ] )
	{
		for( auto i = 0; i < RowSize; i++ )
			for( auto j = 0; j < ColumnSize; j++ )
				data[ i ][ j ] = values[ i * ColumnSize + j ];
	}

	MatrixBase( Type( && values )[ RowSize * ColumnSize ] )
	{
		for( auto i = 0; i < RowSize; i++ )
			for( auto j = 0; j < ColumnSize; j++ )
				data[ i ][ j ] = values[ i * ColumnSize + j ];
	}

	MatrixBase( std::initializer_list< Type > initializer_list )
	{
		int i = 0, j = 0;
		for( auto& value : initializer_list )
		{
			data[ i ][ j ] = value;
			j++;
			if( j > ColumnSize - 1 )
			{
				i++;
				j = 0;
			}
		}
	}

	MatrixBase( const MatrixBase& other ) = default;
	MatrixBase( MatrixBase&& donor ) = default;
	MatrixBase& operator= ( const MatrixBase& right_hand_side ) = default;

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

/* Arithmetic Operations. */
	template< unsigned int RowSizeOther, unsigned int ColumnSizeOther >
	MatrixBase< Type, RowSize, ColumnSizeOther > operator * ( const MatrixBase< Type, RowSizeOther, ColumnSizeOther >& other ) const
	{
		MatrixBase< Type, RowSize, ColumnSizeOther > result( Matrix::MATRIX_INITIALIZE_ZERO );
		for( auto i = 0; i < RowSize; i++ )
			for( auto j = 0; j < ColumnSizeOther; j++ )
				for( auto k = 0; k < ColumnSize; k++ )
					result[ i ][ j ] += data[ i ][ k ] * other[ k ][ j ];

		return result;
	}

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