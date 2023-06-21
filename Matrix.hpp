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
	const Type* Data() const { return &data[ 0 ][ 0 ]; };
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
	MatrixBase< Type, RowSize, ColumnSizeOther > operator* ( const MatrixBase< Type, RowSizeOther, ColumnSizeOther >& other ) const
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

namespace Matrix
{
	/* In row-major form. */ Matrix4x4 OrthographicProjection( const float left_plane_offset, const float right_plane_offset, 
															   const float bottom_plane_offset, const float top_plane_offset,
															   const float near_plane_offset, const float far_plane_offset );
	/* In row-major form. */ Matrix4x4 PerspectiveProjection( const float near_plane_offset, const float far_plane_offset, 
															  const float aspect_ratio, const float field_of_view_inDegrees );

	/* In row-major form. */ Matrix4x4 Scaling( const float newScale );
	/* In row-major form. */ Matrix4x4 Scaling( const float newScale_x, const float newScale_y, const float newScale_z );
	/* In row-major form. */ Matrix4x4 ScalingOnX( const float newScale );
	/* In row-major form. */ Matrix4x4 ScalingOnY( const float newScale );
	/* In row-major form. */ Matrix4x4 ScalingOnZ( const float newScale );

	/* In row-major form. */ Matrix4x4 RotationAroundX( const float angle_inDegrees );
	/* In row-major form. */ Matrix4x4 RotationAroundY( const float angle_inDegrees );
	/* In row-major form. */ Matrix4x4 RotationAroundZ( const float angle_inDegrees );
	// TODO:/* In row-major form. */ Matrix4x4 RotationAroundAxis( const float angle_inDegrees, const Vector3& vector );

	/* In row-major form. */ Matrix4x4 TranslationOnX( const float delta );
	/* In row-major form. */ Matrix4x4 TranslationOnY( const float delta );
	/* In row-major form. */ Matrix4x4 TranslationOnZ( const float delta );
	/* In row-major form. */ Matrix4x4 Translation( const float delta_x, const float delta_y, const float delta_z );
};