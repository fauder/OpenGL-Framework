#pragma once

// Project Includes.
#include "Assert.h"
#include "Concepts.h"
#include "Initialization.h"
#include "TypeTraits.h"
#include "Utility.hpp"
#include "Vector.hpp"

// std Includes.
#include <array>

namespace Framework::Math
{
	/* Row-major. Post-multiplies a row vector to transform it. */
	template< Concepts::Arithmetic Type, std::size_t RowSize, std::size_t ColumnSize >
		requires Concepts::NonZero< RowSize > && Concepts::NonZero< ColumnSize >
	class Matrix
	{
		template< Concepts::Arithmetic, std::size_t RowSize_, std::size_t ColumnSize_ > requires Concepts::NonZero< RowSize_ > && Concepts::NonZero< ColumnSize_ >
		friend class Matrix;

	public:
	/* Constructors. */
		constexpr Matrix( const Matrix& other )					= default;
		constexpr Matrix( Matrix&& donor ) noexcept				= default;
		constexpr Matrix& operator= ( const Matrix& other )		= default;
		constexpr Matrix& operator= ( Matrix&& donor ) noexcept	= default;

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

		/* Initialize diagonals. */
		constexpr Matrix( const Vector< Type, RowSize >& vector ) requires( RowSize == ColumnSize )
			:
			data{}
		{
			SetDiagonals( vector );
		}

		constexpr explicit Matrix( Initialization::ZeroInitialization )
			:
			data{}
		{}

#pragma warning(disable:26495) // Suppress "variable is uninitialized" warning, as not initializing it is the whole point of this constructor.
		constexpr explicit Matrix( Initialization::NoInitialization )
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

#pragma warning(disable:26495) // Suppress "variable is uninitialized" warning, as not initializing it is the whole point of this constructor.
		/* Construct from an upper sub-matrix & a vector for the last row.
		 * Sub-matrix & last row_vector can be ANY size smaller than the size of the matrix to be constructed.
		 */
		template< std::size_t SubMatrixSize, std::size_t VectorSize > requires( RowSize == ColumnSize /* Only allow square matrices. */ && SubMatrixSize < RowSize && VectorSize < RowSize )
		constexpr Matrix( const Matrix< Type, SubMatrixSize, SubMatrixSize >& upper_sub_matrix, const Vector< Type, VectorSize >& last_row_vector )
		{
			/* Initialize the portion covered by the provided upper sub-matrix. */
			Utility::constexpr_for< 0, SubMatrixSize, +1 >( [ & ]( const auto row_index )
			{
				Utility::constexpr_for< 0, SubMatrixSize, +1 >( [ & ]( const auto column_index )
				{
					data[ row_index ][ column_index ] = upper_sub_matrix.data[ row_index ][ column_index ];
				} );

				/* Set elements not covered by the upper sub-matrix to zero. */
				Utility::constexpr_for< SubMatrixSize, RowSize, +1 >( [ & ]( const auto column_index )
				{
					data[ row_index ][ column_index ] = Type( 0 );
				} );
			} );

			/* If there are any rows between the last row-vector & the upper sub-matrix provided, initialize those rows to zero. */
			Utility::constexpr_for< SubMatrixSize, RowSize, +1 >( [ & ]( const auto row_index )
			{
				Utility::constexpr_for< 0, RowSize /* Same as ColumnSize. */, +1 >( [ & ]( const auto column_index )
				{
					data[ row_index ][ column_index ] = Type( 0 );
				} );
			} );

			/* Initialize last row. */
			Utility::constexpr_for< 0, VectorSize, +1 >( [ & ]( const auto index )
			{
				data[ RowSize - 1 ][ index ] = last_row_vector.data[ index ];
			} );

			/* Initialize elements of the last row not covered by the provided vector to zero, except for the last element, which is set to 1. */
			Utility::constexpr_for< VectorSize, RowSize /* Same as ColumnSize. */, +1 >( [ & ]( const auto index )
			{
				data[ RowSize - 1 ][ index ] = Type( index == RowSize - 1 );
			} );
		}
#pragma warning(default:26495)

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
		
		/* For now, just allow square sub-matrices. */
		template< std::size_t Size >
		constexpr Matrix< Type, Size, Size > SubMatrix() const requires( RowSize == ColumnSize && Size < RowSize /* If Size == RowSize, no need to create a new Matrix.So, do not use <= , use < instead. */ )
		{
			Matrix< Type, Size, Size > sub_matrix( NO_INITIALIZATION );

			Utility::constexpr_for< 0, Size, +1 >( [ & ]( const auto row_index )
			{
				Utility::constexpr_for< 0, Size, +1 >( [ & ]( const auto column_index )
				{
					sub_matrix[ row_index ][ column_index ] = data[ row_index ][ column_index ];
				} );
			} );

			return sub_matrix;
		}

		template< typename... Values >
		constexpr Matrix& Set( Values... values ) requires( sizeof...( Values ) <= RowSize * ColumnSize )
		{
			// Utilize fold expressions with a lambda to "loop over" the parameter pack.
			int column_index = 0;
			int row_index = 0;
			( /* Lambda: */ [&]
				{
					data[ row_index ][ column_index ] = values;
					if( ++column_index == ColumnSize )
					{
						column_index = 0;
						row_index++;
					}
				}
			( ), ... );

			return *this;
		}

		template< std::size_t VectorSize >
		constexpr Matrix& SetDiagonals( const Vector< Type, VectorSize >& vector ) requires( RowSize == ColumnSize && VectorSize <= RowSize )
		{
			for( auto i = 0; i < VectorSize; i++ )
				data[ i ][ i ] = vector.data[ i ];

			return *this;
		}

		template< std::size_t VectorSize >
		constexpr Matrix& SetRow( const Vector< Type, VectorSize >& vector, const unsigned int row_index = 0, const unsigned int start_index_inRow = 0 ) requires( VectorSize <= RowSize )
		{
			ASSERT( row_index < RowSize && "Row index out of bounds." );
			ASSERT( start_index_inRow + VectorSize <= ColumnSize && "Given vector does not fit inside the row when starting from start_index_inRow." );

			for( unsigned int i = 0; i < VectorSize; i++ )
				data[ row_index ][ i + start_index_inRow ] = vector.data[ i ];

			return *this;
		}

		template< std::size_t VectorSize >
		constexpr Matrix& SetColumn( const Vector< Type, VectorSize >& vector, const unsigned int column_index = 0, const unsigned int start_index_inColumn = 0 ) requires( VectorSize <= ColumnSize )
		{
			ASSERT( column_index < ColumnSize && "Column index out of bounds." );
			ASSERT( start_index_inColumn + VectorSize <= RowSize && "Given vector does not fit inside the column when starting from start_index_inColumn." );

			for( unsigned int i = 0; i < VectorSize; i++ )
				data[ i + start_index_inColumn ][ column_index ] = vector.data[ i ];

			return *this;
		}

		template< std::size_t VectorSize >
		constexpr Vector< Type, VectorSize > GetRow( const unsigned int row_index = 0, const unsigned int start_index_inRow = 0 ) const requires( VectorSize <= RowSize )
		{
			ASSERT( row_index < RowSize && "Row index out of bounds." );
			ASSERT( start_index_inRow + VectorSize <= ColumnSize && "Given vector does not fit inside the row when starting from start_index_inRow." );

			return Vector< Type, VectorSize >( *( reinterpret_cast< const Vector< Type, VectorSize >* >( data[ row_index ] + start_index_inRow ) ) );
		}

		template< std::size_t VectorSize >
		constexpr Vector< Type, VectorSize > GetColumn( const unsigned int column_index = 0, const unsigned int start_index_inColumn = 0 ) const requires( VectorSize <= ColumnSize )
		{
			ASSERT( column_index < ColumnSize && "Column index out of bounds." );
			ASSERT( start_index_inColumn + VectorSize <= RowSize && "Given vector does not fit inside the column when starting from start_index_inColumn." );

			Vector< Type, VectorSize > vector( NO_INITIALIZATION );

			for( unsigned int i = 0; i < VectorSize; i++ )
				vector.data[ i ] = data[ i + start_index_inColumn ][ column_index ];

			return vector;
		}

		template< std::size_t VectorSize >
		constexpr Matrix& SetScaling( const Vector< Type, VectorSize >& vector ) requires( RowSize == ColumnSize && VectorSize <= RowSize - 1 )
		{
			return SetDiagonals( vector );
		}

		template< std::size_t VectorSize >
		constexpr Matrix& SetTranslation( const Vector< Type, VectorSize >& vector ) requires( RowSize == ColumnSize && VectorSize <= RowSize - 1 )
		{
			return SetRow( vector, RowSize - 1 );
		}

	/* Other Queries. */
		static consteval std::size_t RowCount()     { return RowSize; }
		static consteval std::size_t ColumnCount()  { return ColumnSize; }
		static consteval std::size_t ElementCount() { return RowSize * ColumnSize; }

		static consteval Matrix Identity()			{ return Matrix(); }

	/* Arithmetic Operations. */

		/* Matrix-matrix multiplication. */
		template< std::size_t RowSizeOther, std::size_t ColumnSizeOther >
		constexpr Matrix< Type, RowSize, ColumnSizeOther > operator* ( const Matrix< Type, RowSizeOther, ColumnSizeOther >& other ) const requires( ColumnSize == RowSizeOther )
		{
			Matrix< Type, RowSize, ColumnSizeOther > result( ZERO_INITIALIZATION );
			for( auto i = 0; i < RowSize; i++ )
				for( auto j = 0; j < ColumnSizeOther; j++ )
					for( auto k = 0; k < ColumnSize; k++ )
						result.data[ i ][ j ] += data[ i ][ k ] * other.data[ k ][ j ];

			return result;
		}

		/* Vector-matrix multiplication. */
		template< Concepts::Arithmetic Type_, std::size_t RowSize_, std::size_t ColumnSize_ > // Have to use different template parameters here because C++...
		friend constexpr Vector< Type_, RowSize_ > operator* ( const Vector< Type_, RowSize_ >& vector, const Matrix< Type_, RowSize_, ColumnSize_ >& matrix );

		/* Vector-matrix multiplication. */
		template< Concepts::Arithmetic Type_, std::size_t RowSize_, std::size_t ColumnSize_ > // Have to use different template parameters here because C++...
		friend constexpr Vector< Type_, RowSize_ >& operator*= ( const Vector< Type_, RowSize_ >& vector, const Matrix< Type_, RowSize_, ColumnSize_ >& matrix );

		/* Arithmetic Operations: Unary operators. */
		constexpr Matrix operator- () const
		{
			return *this * Type( -1 );
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
			Utility::constexpr_for< 0, RowSize, +1 >( [ & ]( const auto index )
			{
				result += data[ index ][ index ];
			} );

			return result;
		}

	protected:
		/* Row-major. */
		Type data[ RowSize ][ ColumnSize ];
	};

	/* Vector-matrix multiplication. */
	template< Concepts::Arithmetic Type_, std::size_t RowSize, std::size_t ColumnSize >
	constexpr Vector< Type_, RowSize > operator* ( const Vector< Type_, RowSize >& vector, const Matrix< Type_, RowSize, ColumnSize >& matrix )
	{
		Vector< Type_, RowSize > vector_transformed;
		for( auto j = 0; j < ColumnSize; j++ )
			for( auto k = 0; k < RowSize; k++ )
				vector_transformed[ j ] += vector[ k ] * matrix.data[ k ][ j ];

		return vector_transformed;
	}

	/* Vector-matrix multiplication. */
	template< Concepts::Arithmetic Type_, std::size_t RowSize, std::size_t ColumnSize >
	constexpr Vector< Type_, RowSize >& operator*= ( const Vector< Type_, RowSize >& vector, const Matrix< Type_, RowSize, ColumnSize >& matrix )
	{
		return *this = *this * matrix;
	}
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
