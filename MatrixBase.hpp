#pragma once

// Project Includes.
#include "Assert.h"

// std Includes.
#include <array>

namespace Matrix
{
	struct MatrixInitializeZero {};
	static constexpr MatrixInitializeZero MATRIX_INITIALIZE_ZERO;
};

/* Row-major. */
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
	const Type* Data() const { return &data[ 0 ][ 0 ]; };
	Type* operator[] ( const unsigned int row_index ) { return data[ row_index ]; }
	const Type* operator[] ( const unsigned int row_index ) const { return data[ row_index ]; }

	template< typename... Values >
	MatrixBase& Set( Values... values )
	{
		static_assert( sizeof...( values ) <= ElementCount(), "More values passed than total element count." );

		// Utilize fold expressions with a lambda to "loop over" the parameter pack.
		int columnIndex = 0;
		int rowIndex = 0;
		( /* Lambda: */ [&]
			{
				data[ rowIndex ][ columnIndex ] = values;
				if( ++columnIndex == ColumnSize )
				{
					columnIndex = 0;
					rowIndex++;
				}
			}
		( ), ... );

		return *this;
	}

	template< typename VectorType, 
			  typename = std::enable_if_t< RowSize == ColumnSize &&
										   VectorType::Dimension() <= RowSize > >
	MatrixBase& SetDiagonals( const VectorType& vector )
	{
		for( unsigned int i = 0; i < vector.Dimension(); i++ )
			data[ i ][ i ] = vector[ i ];

		return *this;
	}

	template< typename VectorType,
			  typename = std::enable_if_t< VectorType::Dimension() <= RowSize > >
	MatrixBase& SetRow( const VectorType& vector, const unsigned int row_index = 0, const unsigned int start_index_inRow = 0 )
	{
		ASSERT( row_index < RowSize && "Row index out of bounds." );
		ASSERT( start_index_inRow + VectorType::Dimension() <= ColumnSize && "Given vector does not fit inside the row when starting from start_index_inRow." );

		for( unsigned int i = 0; i < VectorType::Dimension(); i++ )
			data[ row_index ][ i + start_index_inRow ] = vector[ i ];

		return *this;
	}

	template< typename VectorType,
			  typename = std::enable_if_t< VectorType::Dimension() <= ColumnSize > >
	MatrixBase& SetColumn( const VectorType& vector, const unsigned int column_index = 0, const unsigned int start_index_inColumn = 0 )
	{
		ASSERT( column_index < ColumnSize && "Column index out of bounds." );
		ASSERT( start_index_inColumn + VectorType::Dimension() <= RowSize && "Given vector does not fit inside the column when starting from start_index_inColumn." );

		for( unsigned int i = 0; i < VectorType::Dimension(); i++ )
			data[ i + start_index_inColumn ][ column_index ] = vector[ i ];

		return *this;
	}

	template< typename VectorType, 
			  typename = std::enable_if_t< RowSize == ColumnSize &&
										   VectorType::Dimension() <= RowSize - 1 > >
	MatrixBase& SetScaling( const VectorType& vector )
	{
		return SetDiagonals( vector );
	}

	template< typename VectorType,
			  typename = std::enable_if_t< RowSize == ColumnSize && VectorType::Dimension() <= RowSize - 1 > >
	MatrixBase& SetTranslation( const VectorType& vector )
	{
		return SetRow( vector );
	}

/* Other Queries. */
	static constexpr unsigned int RowCount()     { return RowSize; }
	static constexpr unsigned int ColumnCount()  { return ColumnSize; }
	static constexpr unsigned int ElementCount() { return RowSize * ColumnSize; }

/* Arithmetic Operations. */

	/* Matrix-matrix multiplication. */
	template< unsigned int RowSizeOther, unsigned int ColumnSizeOther >
	MatrixBase< Type, RowSize, ColumnSizeOther > operator* ( const MatrixBase< Type, RowSizeOther, ColumnSizeOther >& other ) const
	{
		MatrixBase< Type, RowSize, ColumnSizeOther > result( Matrix::MATRIX_INITIALIZE_ZERO );
		for( auto i = 0; i < RowSize; i++ )
			for( auto j = 0; j < ColumnSizeOther; j++ )
				for( auto k = 0; k < ColumnSize; k++ )
					result[ i ][ j ] += data[ i ][ k ] * other[ k ][ j ];

		return result;
	}

	// TODO: MatrixBase& Transpose();
	// TODO: MatrixBase Transposed() const;

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
