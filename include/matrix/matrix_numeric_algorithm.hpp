//this file is to be removed

#ifndef _MATRIX_NUMERIC_ALGORITHM_HPP_INCLUDED
#define _MATRIX_NUMERIC_ALGORITHM_HPP_INCLUDED

#include "dynamic_matrix.hpp"
//#include "static_matrix.hpp"

#include <cassert>
#include <algorithm>

#include <cstring>

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// solving A[][] x[] = B[] :
// algorithms should be written seperatedly as classes,
// an algorithm factory should be built and maintained,
// when trying to solve the equation,
// every algorithm registed in the factory should be tried in order,
// an error signal should be returned if all of them failed.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/*
 *Algorithms:
 *  1. -- Assign
 *  2. -- Elimission
 *  3. -- Decompositation
 *
 *
 */




//
//Convertion between static_matrix and dynamic_matrix
//
template<typename T, typename U, size_t Row, size_t Col>
void assign( const dynamic_matrix<T>& src, static_matrix<U,Row,Col>&dst )
{
    assert(src.row()==Row);
    assert(src.col()==Col);
    std::copy(src.begin(), src.end(), dst.begin());
}

template<typename T, size_t Row, size_t Col>
void assign( const dynamic_matrix<T>& src, static_matrix<T,Row,Col>&dst )
{
    assert(src.row()==Row);
    assert(src.col()==Col);
    memcpy(src.begin(), dst.begin(), Row*Col*sizeof(T));
}

template<typename T, typename U, size_t Row, size_t Col>
void assign( static_matrix<U,Row,Col>&src, dynamic_matrix<T>& dst )
{
    assert(dst.row()==Row);
    assert(dst.col()==Col);
    std::copy(src.begin(), src.end(), dst.begin());
}

template<typename T, size_t Row, size_t Col>
void assign( static_matrix<T,Row,Col>&src, dynamic_matrix<T>& dst )
{
    assert(dst.row()==Row);
    assert(dst.col()==Col);
    memcpy(src.begin(), dst.begin(), Row*Col*sizeof(T));
}

//
// >> Solve Ax=b <<
// where A[n][n] x[n][1] and b [n][1]
//
// 1) For i = 1, ..., n-1 do 2)--4)
//    2) Let p be the smallest integer within [i,n] while a[p][i]!=0
//       if no integer p can be found
//          then throw excetpion -- no unique solution esists.
//               stop
//    3) if p!= i
//          then perform E[p]<-->E[i]
//    4) For j = i+1, ..., n  do 5) and 6)
//       5) set m[j][i]=a[j][i]/a[i][i]
//       6) perform E[j]-m[j][i]E[i] --> E[j]
// 7) if a[n][n]==0
//       then throw exception and stop
// 8) set x[n]=a[n][n+1]
// 9) For i = n-1, ..., 1
//        set x[i] = {a[i][n+1]-\sum_{j=i+1}^{n} a[i][j]x[j]} / a[i][i]
// 10) output x[]
//

template <typename Type>
const dynamic_matrix<Type>
gaussian_elimination(
                     const dynamic_matrix<Type>& A,
                     const dynamic_matrix<Type>& b)
{
    assert(A.is_square());
    assert(b.is_col_vector());
    assert(A.col() == b.col());

    typedef typename dynamic_matrix<Type>::size_type size_type;
    typedef typename dynamic_matrix<Type>::const_col_type const_col_type;
    typedef typename dynamic_matrix<Type>::value_type value_type;

    //a -- [A:b]
    const size_type n = A.row();
    dynamic_matrix<Type> a(n, n + 1);
    for (size_type i = 0; i < n; ++i)
    {
        std::copy(A.col_begin(i), A.col_end(i), a.col_begin(i));
    }
    std::copy(b.col_begin(0), b.col_end(0), a.col_begin(n));
    // 1)
    for (size_type i = 0; i < n - 1; ++i)
    {
        // 2)
        const_col_type it = std::find_if(a.col_begin(i), a.col_end(i),
                                         std::bind1st(std::not_equal_to<value_type > (0)));
        if (it == a.col_end())
        {
            throw "gaussian_elimination : no unique solution esists";
        }
        // 3)
        const size_type p = std::distance(it, a.col_begin(i));
        if (p != i)
        {
            std::swap_ranges(a.row_begin(p), a.row_end(p), a.row_begin(i));
        }
        // 4)
        for (size_type j = i + 1; j < n; ++j)
        { // 5)
            const value_type m = (*(a.col_begin(i) + j)) / (*(a.diag_begin() + i));
            // 6)
            std::transform(a.row_begin(j), a.row_end(j),
                           a.row_begin(j),
                           std::bind1st(std::multiplies<value_type > (m))
                           );
            std::transform(a.row_begin(j), a.row_end(j),
                           a.row_begin(i), a.row_begin(j),
                           std::minus<value_type > ()
                           );
        }
    }
    // 7)
    if (value_type(0) == *(a.diag_end() - 1))
        throw "gaussian_elimination : no unique solution esists";

    // diag array to 1
    for (size_type i = 0; i < n; ++i)
    {
        std::transform(a.row_begin(i) + i, a.row_end(i), a.row_begin(i) + i,
                       std::bind2nd(std::divides<value_type > (*(a.diag_begin() + i)))
                       );
    }

    for (size_type i = n - 1; i > -1; --i)
    {
        std::transform(a.col_begin(i), a.col_end(i) + i - n, a.col_begin(i),
                       std::bind1st(std::multiplies<value_type > (*(a.col_begin(n) + i)))
                       );
        std::transform(a.col_begin(n), a.col_end(n) - n + i, a.col_begin(i),
                       a.col_begin(n), std::minus<value_type > ());
    }

    dynamic_matrix<value_type> ans(n, 1);
    std::copy(a.col_begin(n), a.col_end(n), ans.begin());

    return ans;
}


#endif

