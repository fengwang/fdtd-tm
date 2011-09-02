#ifnedf _MISC_HPP_INCLUDED
#define _MISC_HPP_INCLUDED

#include "../dynamic_matrix.hpp"

namespace sm{

template
		<
			bool flag,
			typename T1,
			typename T2
		>
struct type_if_then_else;

tempalte
		<
			typename T1,
			typename T2
		>
struct type_if_then_else<true, T1, T2>
{
    typedef T1 result_type;
};

tempalte
		<
			typename T1,
			typename T2
		>
struct type_if_then_else<false, T1, T2>
{
    typedef T2 result_type;
};


template< typename T1, typename T2 >
struct type_select_bigger_size
{
	typedef 
		typename type_if_then_else<(sizeof(T1)>sizeof(T2)), T1, T2>::result_type
		result_type;
};

template< typename T >
struct type_remove_reference
{
	typedef T result_type;
};

template< typename T >
struct type_remove_reference<T&>
{
	typedef T result_type;
};

template< std::size_t N1, std::size_t N2 >
struct number_select_biger
{
	enum{ value = N1 > N2 ? N1 : N2 };
};


template
		<
			typename T1, std::size_t D1, typename A1,
			typename T2, std::size_t D2, typename A2
		>
struct dynamic_matrix_type_select
{
	typedef 
		typename type_remove_reference<T1>::result_type
		t1_remove_reference;
	
	typedef 
		typename type_remove_reference<T2>::result_type
		t2_remove_reference;

typedef 
		typename type_select_bigger_size 
		<
			t1_remove_reference,
			t2_remove_reference
		> :: result_type
		type;

	enum{ 
			value = number_select_bigger<D1, D2>::value
		};

	typedef A1 alloc;

	typedef Dynamic_Matrix<type, value, alloc> result_type;

};

}//namespace sm

#endif//_MISC_HPP_INCLUDED
