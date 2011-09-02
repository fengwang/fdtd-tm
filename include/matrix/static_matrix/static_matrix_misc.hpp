#ifndef _STATIC_MATRIX_MISC_HPP_INCLUDED
#define	_STATIC_MATRIX_MISC_HPP_INCLUDED

#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <iosfwd>

namespace sm{


template<typename Type, size_t Row, size_t Dim, size_t Col>
const static_matrix<Type, Row, Col>
operator * ( 	const static_matrix<Type, Row, Dim>& lhs,
        		const static_matrix<Type, Dim, Col>& rhs)
{
    static_matrix<Type, Row, Col> ans;
    typedef typename static_matrix<Type, Row, Col>::size_type size_type;
    typedef typename static_matrix<Type, Row, Col>::value_type value_type;

    for (size_type i = 0; i < Row; ++i)
        for (size_type j = 0; j < Col; ++j)
        {
            ans[i][j] = std::inner_product(lhs.row_begin(i), lhs.row_end(i),
                                           rhs.col_begin(j), value_type(0));
        }
    return ans;
}

template<typename T, size_t Row, size_t Col>
std::ostream & operator << (std::ostream& lhs, const static_matrix<T, Row, Col>& rhs)
{
    typedef typename static_matrix<T, Row, Col>::size_type size_type;
    typedef typename static_matrix<T, Row, Col>::value_type value_type;
    for (size_type i = 0; i < rhs.row(); ++i)
    {
        std::copy(rhs.row_begin(i), rhs.row_end(i),
                  std::ostream_iterator<value_type>(lhs, "\t"));
        lhs << "\n";
    }
    return lhs;
}

}//namespace sm


#endif	/* _STATIC_MATRIX_MISC_HPP_INCLUDED */

