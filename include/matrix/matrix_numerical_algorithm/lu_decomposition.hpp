#ifndef _LU_DECOMPOSITION_HPP_INCLUDED_AIE98U3OIAFD9812894LKJSFDLKJSOI83OAIP8D81
#define _LU_DECOMPOSITION_HPP_INCLUDED_AIE98U3OIAFD9812894LKJSFDLKJSOI83OAIP8D81


namespace sm{

template< typename MATRIX >
struct lu_decomposition
{
	typedef MATRIX matrix_type;

	lu_decomposition( const matrix_type& );

	const matrix_type
		lower() const;
	const matrix_type
		upper() const;
};

}//namespace sm

#include "lu/dynamic_lu_decomposition.tcc"
#include "lu/static_lu_decomposition.tcc"



#endif//_LU_DECOMPOSITION_HPP_INCLUDED_AIE98U3OIAFD9812894LKJSFDLKJSOI83OAIP8D81


