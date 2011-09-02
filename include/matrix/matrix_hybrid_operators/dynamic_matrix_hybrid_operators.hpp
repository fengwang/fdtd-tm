#ifndef _DYNAMIC_MATRIX_HYBRID_OPERATORS_HPP_INCLUDED
#define _DYNAMIC_MATRIX_HYBRID_OPERATORS_HPP_INCLUDED

#include "../dynamic_matrix.hpp"
#include "misc.hpp"

#include <cassert>
#include <algorithm>

#include <boost/lambda/lambda.hpp>

using boost::lambda::bind;
using boost::lambda::_1;
using boost::lambda::_2;


namespace sm{

template
		<
			typename T1, std::size_t D1, typename A1,
			typename T2, std::size_t D2, typename A2
		>
const typename 
dynamic_matrix_type_select<T1, D1, A1, T2, D2, A2> :: result_type
operator + ( 
				const Dynamic_Matrix<T1, D1, A1>& lhs, 
				const Dynamic_Matrix<T2, D2, A2>& rhs 
			)
{
	const 	typename dynamic_matrix_type_select<T1, D1, A1, T2, D2, A2> :: 
			result_type ans(lhs);
	
	std::transform( 	ans.begin(), ans.end(),
						rhs.begin(), ans.begin(),
						_1 + _2
				  );

	return ans;
}

template
		<
			typename T1, std::size_t D1, typename A1,
			typename T2, std::size_t D2, typename A2
		>
const typename 
dynamic_matrix_type_select<T1, D1, A1, T2, D2, A2> :: result_type
operator - ( 
				const Dynamic_Matrix<T1, D1, A1>& lhs, 
				const Dynamic_Matrix<T2, D2, A2>& rhs 
			)
{
	const 	typename dynamic_matrix_type_select<T1, D1, A1, T2, D2, A2> :: 
			result_type ans(lhs);
	
	std::transform( 	ans.begin(), ans.end(),
						rhs.begin(), ans.begin(),
						_1 - _2
				  );
	
	return ans;
}

template
		<
			typename T1, std::size_t D1, typename A1,
			typename T2, std::size_t D2, typename A2
		>
const typename 
dynamic_matrix_type_select<T1, D1, A1, T2, D2, A2> :: result_type
operator * ( 
				const Dynamic_Matrix<T1, D1, A1>& lhs, 
				const Dynamic_Matrix<T2, D2, A2>& rhs 
			)
{

}

template
		<
			typename T1, std::size_t D1, typename A1,
			typename T2, std::size_t D2, typename A2
		>
const typename 
dynamic_matrix_type_select<T1, D1, A1, T2, D2, A2> :: result_type
operator / ( 
				const Dynamic_Matrix<T1, D1, A1>& lhs, 
				const Dynamic_Matrix<T2, D2, A2>& rhs 
			)
{

}


template
		<
			typename T1, std::size_t D1, typename A1,
			typename T2, std::size_t D2, typename A2
		>
const bool
operator > ( 
				const Dynamic_Matrix<T1, D1, A1>& lhs, 
				const Dynamic_Matrix<T2, D2, A2>& rhs 
			)
{

template
		<
			typename T1, std::size_t D1, typename A1,
			typename T2, std::size_t D2, typename A2
		>
const bool
operator >= ( 
				const Dynamic_Matrix<T1, D1, A1>& lhs, 
				const Dynamic_Matrix<T2, D2, A2>& rhs 
			)
{
}

template
		<
			typename T1, std::size_t D1, typename A1,
			typename T2, std::size_t D2, typename A2
		>
const bool
operator == ( 
				const Dynamic_Matrix<T1, D1, A1>& lhs, 
				const Dynamic_Matrix<T2, D2, A2>& rhs 
			)
{
}


template
		<
			typename T1, std::size_t D1, typename A1,
			typename T2, std::size_t D2, typename A2
		>
const bool
operator != ( 
				const Dynamic_Matrix<T1, D1, A1>& lhs, 
				const Dynamic_Matrix<T2, D2, A2>& rhs 
			)
{
}


template
		<
			typename T1, std::size_t D1, typename A1,
			typename T2, std::size_t D2, typename A2
		>
const bool
operator <= ( 
				const Dynamic_Matrix<T1, D1, A1>& lhs, 
				const Dynamic_Matrix<T2, D2, A2>& rhs 
			)
{
}

template
		<
			typename T1, std::size_t D1, typename A1,
			typename T2, std::size_t D2, typename A2
		>
const bool
operator < ( 
				const Dynamic_Matrix<T1, D1, A1>& lhs, 
				const Dynamic_Matrix<T2, D2, A2>& rhs 
			)
{
}


}//namespace sm

#endif//_DYNAMIC_MATRIX_HYBRID_OPERATORS_HPP_INCLUDED

