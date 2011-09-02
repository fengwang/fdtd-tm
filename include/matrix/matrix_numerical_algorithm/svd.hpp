#ifndef _SVD_HPP_INCLUDED_OFSIDHJLKJ34OAHFD98HU398UHYAFOSOIUH3IUHD98U4EIJDFIJSDI
#define _SVD_HPP_INCLUDED_OFSIDHJLKJ34OAHFD98HU398UHYAFOSOIUH3IUHD98U4EIJDFIJSDI


namespace sm{

template< typename MATRIX >
struct svd
{
	typedef MATRIX matrix_type;

	svd_decomposition( const matrix_type& );

	const matrix_type 
	u () const;

	const matrix_type 
	v () const;

	const matrix_type 
	w () const;

	matrix_type a_;
	matrix_type u_;
	matrix_type v_;
	matrix_type w_;
};

#include "svd/dynamic_matrix_svd.hpp"


}//namespace sm

#endif//_SVD_HPP_INCLUDED_OFSIDHJLKJ34OAHFD98HU398UHYAFOSOIUH3IUHD98U4EIJDFIJSDI



