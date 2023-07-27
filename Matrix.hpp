#pragma once

// Project Includes.
#include "Assert.h"
#include "Concepts.h"
#include "Initialization.h"
#include "TypeTraits.h"
#include "Utility.hpp"

// std Includes.
#include <array>

namespace Framework::Math
{
	/* Row-major. Post-multiplies a row vector to transform it. */
	template< Concepts::Arithmetic Type, size_t RowSize, size_t ColumnSize >
		requires Concepts::NonZero< RowSize > && Concepts::NonZero< ColumnSize >
	class Matrix
	{
	public:
	/* Constructors. */
		constexpr Matrix( const Matrix& other )             = default;
		constexpr Matrix( Matrix&& donor )                  = default;
		constexpr Matrix& operator= ( const Matrix& other ) = default;
		constexpr Matrix& operator= ( Matrix&& donor )      = default;

		constexpr ~Matrix() = default;

		/* Initializes to identity matrix. */
		constexpr Matrix()
			:
			data{}
		{
			if constexpr( RowSize == ColumnSize ) // Do not initialize non-square matrices to identity.
				for( auto i = 0; i < RowSize; i++ )
					data[ i ][ i ] = Type( 1 );
		}

		constexpr explicit Matrix( Initialization::ZeroInitialization )
			:
			data{}
		{}

#pragma warning(disable:26495) // Suppress "variable is uninitialized" warning, as not initializing it is the whole point of this constructor.
		explicit Matrix( Initialization::NoInitialization )
		{}
#pragma warning(default:26495)

		constexpr explicit Matrix( const Type( &values )[ RowSize ][ ColumnSize ] )
		{
			for( auto i = 0; i < RowSize; i++ )
				for( auto j = 0; j < ColumnSize; j++ )
					data[ i ][ j ] = values[ i ][ j ];
		}
		constexpr explicit Matrix( Type( &&values )[ RowSize ][ ColumnSize ] )
		{
			for( auto i = 0; i < RowSize; i++ )
				for( auto j = 0; j < ColumnSize; j++ )
					data[ i ][ j ] = values[ i ][ j ];
		}

		constexpr explicit Matrix( const Type( &values )[ RowSize * ColumnSize ] ) // 1D array version.
		{
			for( auto i = 0; i < RowSize; i++ )
				for( auto j = 0; j < ColumnSize; j++ )
					data[ i ][ j ] = values[ i * ColumnSize + j ];
		}

		constexpr explicit Matrix( Type( && values )[ RowSize * ColumnSize ] ) // 1D array version.
		{
			for( auto i = 0; i < RowSize; i++ )
				for( auto j = 0; j < ColumnSize; j++ )
					data[ i ][ j ] = values[ i * ColumnSize + j ];
		}

	/* Comparison operators. */
		bool operator== ( const Matrix& right_hand_side ) const
		{
			bool result = true;

			for( auto i = 0; i < RowSize; i++ )
				for( auto j = 0; j < ColumnSize; j++ )
					if constexpr( std::is_integral_v< Type > )
						result &= data[ i ][ j ] == right_hand_side.data[ i ][ j ];
					else
						result &= Math::IsEqual( data[ i ][ j ], right_hand_side.data[ i ][ j ] );
			
			return result;
		}

		bool operator!= ( const Matrix& right_hand_side ) const
		{
			return !operator==( right_hand_side );
		}

	/* Getters & Setters. */
		const Type* Data() const { return &data[ 0 ][ 0 ]; };
		constexpr Type* operator[] ( const unsigned int row_index ) { return data[ row_index ]; }
		constexpr const Type* operator[] ( const unsigned int row_index ) const { return data[ row_index ]; }

		template< typename... Values >
		constexpr Matrix& Set( Values... values )
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

		template< typename VectorType >
		constexpr Matrix& SetDiagonals( const VectorType& vector ) requires( RowSize == ColumnSize && VectorType::Dimension() <= RowSize )
		{
			for( auto i = 0; i < VectorType::Dimension(); i++ )
				data[ i ][ i ] = vector[ i ];

			return *this;
		}

		template< typename VectorType >
		constexpr Matrix& SetRow( const VectorType& vector, const unsigned int row_index = 0, const unsigned int start_index_inRow = 0 ) requires( VectorType::Dimension() <= RowSize )
		{
			ASSERT( row_index < RowSize && "Row index out of bounds." );
			ASSERT( start_index_inRow + VectorType::Dimension() <= ColumnSize && "Given vector does not fit inside the row when starting from start_index_inRow." );

			for( unsigned int i = 0; i < VectorType::Dimension(); i++ )
				data[ row_index ][ i + start_index_inRow ] = vector[ i ];

			return *this;
		}

		template< typename VectorType >
		constexpr Matrix& SetColumn( const VectorType& vector, const unsigned int column_index = 0, const unsigned int start_index_inColumn = 0 ) requires( VectorType::Dimension() <= ColumnSize )
		{
			ASSERT( column_index < ColumnSize && "Column index out of bounds." );
			ASSERT( start_index_inColumn + VectorType::Dimension() <= RowSize && "Given vector does not fit inside the column when starting from start_index_inColumn." );

			for( unsigned int i = 0; i < VectorType::Dimension(); i++ )
				data[ i + start_index_inColumn ][ column_index ] = vector[ i ];

			return *this;
		}

		template< typename VectorType >
		constexpr Matrix& SetScaling( const VectorType& vector ) requires( RowSize == ColumnSize && VectorType::Dimension() <= RowSize - 1 )
		{
			return SetDiagonals( vector );
		}

		template< typename VectorType >
		constexpr Matrix& SetTranslation( const VectorType& vector ) requires( RowSize == ColumnSize && VectorType::Dimension() <= RowSize - 1 )
		{
			return SetRow( vector, RowSize - 1 );
		}

	/* Other Queries. */
		static consteval size_t RowCount()     { return RowSize; }
		static consteval size_t ColumnCount()  { return ColumnSize; }
		static consteval size_t ElementCount() { return RowSize * ColumnSize; }

	/* Arithmetic Operations. */

		/* Matrix-matrix multiplication. */
		template< size_t RowSizeOther, size_t ColumnSizeOther >
		constexpr Matrix< Type, RowSize, ColumnSizeOther > operator* ( const Matrix< Type, RowSizeOther, ColumnSizeOther >& other ) const requires( ColumnSize == RowSizeOther )
		{
			Matrix< Type, RowSize, ColumnSizeOther > result( ZERO_INITIALIZATION );
			for( auto i = 0; i < RowSize; i++ )
				for( auto j = 0; j < ColumnSizeOther; j++ )
					for( auto k = 0; k < ColumnSize; k++ )
						result.data[ i ][ j ] += data[ i ][ k ] * other.data[ k ][ j ];

			return result;
		}

		Matrix& Transpose()
		{
			for( auto i = 0; i < RowSize; i++ )
			{
				for( auto j = i + 1; j < ColumnSize; j++ )
					std::swap( data[ i ][ j ], data[ j ][ i ] );
			}

			return *this;
		}

		Matrix Transposed() const
		{
			return Matrix( *this ).Transpose();
		}

		constexpr Type Trace() const requires( RowSize == ColumnSize )
		{
			Type result( 0 );
			Utility::constexpr_for< 0, RowSize, 1 >( [ & ]( const auto index )
			{
				result += data[ index ][ index ];
			} );

			return result;
		}

	protected:
		/* Row-major. */
		Type data[ RowSize ][ ColumnSize ];
	};
}

namespace Framework
{
	using Matrix2x2  = Math::Matrix< float,		2, 2 >;
	using Matrix3x3  = Math::Matrix< float,		3, 3 >;
	using Matrix4x4  = Math::Matrix< float,		4, 4 >;
	using Matrix2x2D = Math::Matrix< double,	2, 2 >;
	using Matrix3x3D = Math::Matrix< double,	3, 3 >;
	using Matrix4x4D = Math::Matrix< double,	4, 4 >;
	using Matrix2x2I = Math::Matrix< int,		2, 2 >;
	using Matrix3x3I = Math::Matrix< int,		3, 3 >;
	using Matrix4x4I = Math::Matrix< int,		4, 4 >;
}
