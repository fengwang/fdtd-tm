#ifndef _MATRIX_VALUE_OPERATORS_HPP_INCLUDED
#define _MATRIX_VALUE_OPERATORS_HPP_INCLUDED

namespace sm{


/*
 * dynamic_matrix
 */


template< typename T1, std::size_t D, typename A, typename T2 >
const dynamic_matrix<T1,D,A>
operator + ( const dynamic_matrix<T1,D,A>& lhs, const T2& rhs )
{
	dynamic_matrix<T1,D,A> ans( lhs );
	ans += rhs;
	return ans;
}

template< typename T1, std::size_t D, typename A, typename T2 >
const dynamic_matrix<T1,D,A>
operator + ( const T2& lhs, const dynamic_matrix<T1,D,A>& rhs )
{
	return rhs + lhs;
}


template< typename T1, std::size_t D, typename A, typename T2 >
const dynamic_matrix<T1,D,A>
operator - ( const dynamic_matrix<T1,D,A>& lhs, const T2& rhs )
{
	dynamic_matrix<T1,D,A> ans( lhs );
	ans -= rhs;
	return ans;
}

template< typename T1, std::size_t D, typename A, typename T2 >
const dynamic_matrix<T1,D,A>
operator - ( const T2& lhs, const dynamic_matrix<T1,D,A>& rhs )
{
	return - rhs + lhs;
}

template< typename T1, std::size_t D, typename A, typename T2 >
const dynamic_matrix<T1,D,A>
operator * ( const dynamic_matrix<T1,D,A>& lhs, const T2& rhs )
{
	dynamic_matrix<T1,D,A> ans( lhs );
	ans *= rhs;
	return ans;
}

template< typename T1, std::size_t D, typename A, typename T2 >
const dynamic_matrix<T1,D,A>
operator * ( const T2& lhs, const dynamic_matrix<T1,D,A>& rhs )
{
	return rhs * lhs;
}

template< typename T1, std::size_t D, typename A, typename T2 >
const dynamic_matrix<T1,D,A>
operator / ( const dynamic_matrix<T1,D,A>& lhs, const T2& rhs )
{
	dynamic_matrix<T1,D,A> ans( lhs );
	ans /= rhs;
	return ans;
}

template< typename T1, std::size_t D, typename A, typename T2 >
const dynamic_matrix<T1,D,A>
operator / ( const T2& lhs, const dynamic_matrix<T1,D,A>& rhs )
{
	return lhs * rhs.inverse();
}


template< typename T, std::size_t D, typename A >
const dynamic_matrix<T,D,A>
operator || ( const dynamic_matrix<T,D,A>& lhs, const T& rhs )
{
	dynamic_matrix<T,D,A> ans( lhs.row(), lhs.col()+1 );

	typedef typename dynamic_matrix<T,D,A>::size_type size_type;

	for ( size_type i = 0; i < lhs.row(); ++i )
	{
		std::copy( lhs.row_begin(i), lhs.row_end(i), ans.row_begin(i) );	
	}
	
	std::fill( ans.col_begin(lhs.col()), ans.col_end(lhs.col()), rhs );

	return ans;
}

template< typename T, std::size_t D, typename A >
const dynamic_matrix<T,D,A>
operator || ( const T& lhs, const dynamic_matrix<T,D,A>& rhs )
{
	dynamic_matrix<T,D,A> ans( rhs.row(), rhs.col()+1 );

	typedef typename dynamic_matrix<T,D,A>::size_type size_type;

	for ( size_type i = 0; i < rhs.row(); ++i )
	{
		std::copy( rhs.row_begin(i), rhs.row_end(i), ans.row_begin(i)+1 );	
	}

	std::fill( ans.col_begin(0), ans.col_end(0), lhs );
	
	return ans;
}

template< typename T, std::size_t D, typename A >
const dynamic_matrix<T,D,A>
operator && ( const dynamic_matrix<T,D,A>& lhs, const T& rhs )
{
	dynamic_matrix<T,D,A> ans( lhs.row()+1, lhs.col() );

	typedef typename dynamic_matrix<T,D,A>::size_type size_type;

	for ( size_type i = 0; i < lhs.row(); ++i )
	{
		std::copy( lhs.row_begin(i), lhs.row_end(i), ans.row_begin(i) );	
	}
	
	std::fill( ans.row_begin(lhs.row()), ans.row_end(lhs.row()), rhs );

	return ans;
}

template< typename T, std::size_t D, typename A >
const dynamic_matrix<T,D,A>
operator && ( const T& lhs, const dynamic_matrix<T,D,A>& rhs )
{
	dynamic_matrix<T,D,A> ans( rhs.row()+1, rhs.col() );

	typedef typename dynamic_matrix<T,D,A>::size_type size_type;

	for ( size_type i = 0; i < rhs.row(); ++i )
	{

		std::copy( rhs.row_begin(i), rhs.row_end(i), ans.row_begin(i+1) );	
	}

	std::fill( ans.row_begin(0), ans.row_end(0), lhs );
	
	return ans;
}


#if 0

/*
 *static_matrix
 */

//
//TO DO
//BE IMPLEMENTED
//

template<typename T, std::size_t M, std::size_t N>
const static_matrix<T,M,N>
operator + ( const static_matrix<T,M,N>& lhs, const T& rhs )
{
	static_matrix<T,M,N> ans( lhs );
	ans += rhs;
	return ans;
}

template<typename T, std::size_t M, std::size_t N>
const static_matrix<T,M,N>
operator + ( const T& lhs, const static_matrix<T,M,N>& rhs )
{
	returh rhs+lhs;
}

template<typename T, std::size_t M, std::size_t N>
const static_matrix<T,M,N>
operator - ( const static_matrix<T,M,N>& lhs, const T& rhs )
{
	static_matrix<T,M,N> ans( lhs );
	ans -= rhs;
	return ans;
}

template<typename T, std::size_t M, std::size_t N>
const static_matrix<T,M,N>
operator - ( const T& lhs, const static_matrix<T,M,N>& rhs )
{
	returh -rhs+lhs;
}

template<typename T, std::size_t M, std::size_t N>
const static_matrix<T,M,N>
operator * ( const static_matrix<T,M,N>& lhs, const T& rhs )
{
	static_matrix<T,M,N> ans( lhs );
	ans *= rhs;
	return ans;
}

template<typename T, std::size_t M, std::size_t N>
const static_matrix<T,M,N>
operator / ( const static_matrix<T,M,N>& lhs, const T& rhs )
{
	static_matrix<T,M,N> ans( lhs );
	ans /= rhs;
	return ans;
}


template<typename T, std::size_t M, std::size_t N>
const static_matrix<T,M,N+1>
operator || ( const static_matrix<T,M,N>& lhs, const T& rhs )
{
	dynamic_matrix<T,M,N+1> ans;

	typedef typename dynamic_matrix<T,M,N+1>::size_type size_type;

	//optimization required
	for ( size_type i = 0; i < lhs.row(); ++i )
	{
		std::copy( lhs.row_begin(i), lhs.row_end(i), ans.row_begin(i) );	
	}
	
	std::fill( ans.col_begin(lhs.col()), ans.col_end(lhs.col()), rhs );

	return ans;
}
#endif

#if 0

template<typename T, std::size_t M, std::size_t N>
const static_matrix<T,M,N>
operator || ( const T& lhs, const dynamic_matrix<T,D,A>& rhs )
{
	dynamic_matrix<T,D,A> ans( rhs.row(), rhs.col()+1 );

	typedef typename dynamic_matrix<T,D,A>::size_type size_type;

	for ( size_type i = 0; i < lhs.row(); ++i )
	{
		std::copy( lhs.row_begin(i), lhs.row_end(i), ans.row_begin(i)+1 );	
	}

	std::fill( ans.col_begin(0), ans.col_end(0), rhs );
	
	return ans;
}

template<typename T, std::size_t M, std::size_t N>
const static_matrix<T,M,N>
operator && ( const dynamic_matrix<T,D,A>& lhs, const T& rhs )
{
	dynamic_matrix<T,D,A> ans( lhs.row()+1, lhs.col() );

	typedef typename dynamic_matrix<T,D,A>::size_type size_type;

	for ( size_type i = 0; i < lhs.row(); ++i )
	{
		std::copy( lhs.row_begin(i), lhs.row_end(i), ans.row_begin(i) );	
	}
	
	std::fill( ans.row_begin(lhs.row()), ans.row_end(lhs.row()), rhs );

	return ans;
}

template<typename T, std::size_t M, std::size_t N>
const static_matrix<T,M,N>
operator && ( const T& lhs, const dynamic_matrix<T,D,A>& rhs )
{
	dynamic_matrix<T,D,A> ans( rhs.row()+1, rhs.col() );

	typedef typename dynamic_matrix<T,D,A>::size_type size_type;

	for ( size_type i = 0; i < lhs.row(); ++i )
	{

		std::copy( lhs.row_begin(i), lhs.row_end(i), ans.row_begin(i+1) );	
	}

	std::fill( ans.row_begin(0), ans.row_end(0), rhs );
	
	return ans;
}





#endif








}//namespace sm


#endif//_MATRIX_VALUE_OPERATORS_HPP_INCLUDED



