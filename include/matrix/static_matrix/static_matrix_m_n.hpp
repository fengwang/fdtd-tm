// -*- C++ -*-
/* 
 * File:   static_matrix_m_n.hpp
 * Author: feng
 *
 * Created on October 9, 2009, 3:31 PM
 */

#ifndef _STATIC_MATRIX_M_N_HPP_INCLUDED
#define	_STATIC_MATRIX_M_N_HPP_INCLUDED

#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <iosfwd>


#include <boost/operators.hpp>

#include "static_check.hpp"
#include "static_matrix_buffer.hpp"
#include "static_matrix_stride_iterator.hpp"

namespace sm{


/* <<!!NOT NECESSARY ANY LONGER!!>>
 * known problems:
 * 
 *  1) operations between static_matrix with different dims:
 *     static_matrix<int, 13, 21> m1;
 *     static_matrix<int, 21, 99> m2;
 *     m1 *= m2; // <<-- this does not work
 *     static_matrix<int, 13, 99> m3 = m1*m2; // <<-- this ok
 *     static_matrix<int, 21, 21> m4;
 *     m1 /= m4;   // <<-- not work
 *     m1 = m1/m4; // <<-- will be defined
 *
 */

/*
	[I] Operators Overloaded:
			static_matrix<T,m,n> : M<m,n> 	----     T : V
		1.  M1(M2); <--> M1 = M2;
		2.  M1 += M2;
		3. 	M = M1 + M2;
		
		4.  M += v;
		5.  M1 = v + M2;
		6.  M1 = M2 + v;

		7.  M1 -= M2;
		8.  M = M1 - M2;

		9.  M -= v;
		10. M1 = M2 - v;

		11. M *= v;
		12. M = M * v;
		13. M = v * M;

		14. M /= v;
		15. M1 = M2 / v;

		16. M1 == M2;
		17. M1 != M2;

		18. M == v;
		19. v == M;
		20. M != v;
		21. v != M;

		22. M1 > M2;
		23. M1 >= M2;
		24. M1 < M2;
		25. M1 <= M2;

		26. M > v;
		27. v > M;
		28. M >= v;
		29. v >= M;
		30. M < v;
		31. v < M;
		32. M <= v;
		33. v <= M;


		34. M<T,m,n> = M1<T,m,p> * M2<T,p,n>;
		35. M[i][j] = v;
		36. v = M[i][j];
		37. M1<T,n,m> = ~M2<T,m,n>;
		38. M1 = +M2;
		39. M1 = -M2;
*/

template<typename Type, std::size_t Row, std::size_t Col>
class static_matrix
:
boost::addable 					<static_matrix<Type, Row, Col>,
boost::addable 					<static_matrix<Type, Row, Col>, Type,
boost::subtractable 			<static_matrix<Type, Row, Col>,
boost::subtractable 			<static_matrix<Type, Row, Col>, Type,
boost::multipliable 			<static_matrix<Type, Row, Col>, Type,
boost::dividable 				<static_matrix<Type, Row, Col>, Type,
boost::equality_comparable 		<static_matrix<Type, Row, Col>,
boost::equality_comparable 		<static_matrix<Type, Row, Col>, Type,
boost::less_than_comparable 	<static_matrix<Type, Row, Col>,
boost::less_than_comparable 	<static_matrix<Type, Row, Col>, Type
> > > > > > > > > >
{
public:

    typedef Type 														value_type;
    typedef static_matrix 												self_type;
    typedef value_type* 												iterator;
    typedef const value_type* 											const_iterator;
    typedef value_type* 												row_type;
    typedef const value_type* 											const_row_type;
    typedef static_matrix_stride_iterator<value_type*, Col> 			col_type;
    typedef static_matrix_stride_iterator<const value_type*, Col> 		const_col_type;
    typedef static_matrix_stride_iterator<value_type*, Col+1> 			diag_type;
    typedef static_matrix_stride_iterator<const value_type*, Col+1> 	const_diag_type;
    typedef static_matrix_buffer<value_type, Row*Col> 					storage_type;
    typedef std::size_t 												size_type;

	typedef std::reverse_iterator<iterator> 							reverse_iterator;
	typedef std::reverse_iterator<const_iterator> 						const_reverse_iterator;

	typedef std::reverse_iterator<row_type> 							reverse_row_type;
	typedef std::reverse_iterator<const_row_type> 						const_reverse_row_type;
	
	typedef std::reverse_iterator<col_type> 							reverse_col_type;
	typedef std::reverse_iterator<const_col_type> 						const_reverse_col_type;
	
	typedef std::reverse_iterator<diag_type> 							reverse_upper_diag_type;
	typedef std::reverse_iterator<const_diag_type> 						const_reverse_upper_diag_type;
	
	typedef std::reverse_iterator<diag_type> 							reverse_lower_diag_type;
	typedef std::reverse_iterator<const_diag_type> 						const_reverse_lower_diag_type;
	
	typedef std::reverse_iterator<diag_type> 							reverse_diag_type;
	typedef std::reverse_iterator<const_diag_type> 						const_reverse_diag_type;

public:

    static_matrix() {}

    ~static_matrix() {}

    template<typename U>
    static_matrix(const static_matrix<U, Row, Col>& rhs)
    {
        operator=<U>(rhs);
    }

    template<typename U>
    self_type &
	operator=(const static_matrix<U, Row, Col>& rhs)
    {
        do_copy<U>(rhs);
        return *this;
    }

    static_matrix(const self_type& rhs)
    {
        operator=(rhs);
    }

    self_type & 
	operator=(const self_type& rhs)
    {
        do_copy(rhs);
        return *this;
    }

private:

    template<typename U>
    void 
	do_copy(const static_matrix<U, Row, Col>& rhs)
    {
        std::copy(rhs.begin(), rhs.end(), begin());
    }

    void 
	do_copy(const self_type& rhs)
    {
        data_ = rhs.data_;
    }

public:

    size_type
	row()const
    {
        return Row;
    }

    size_type 
	col()const
    {
        return Col;
    }

    size_type 
	size()const
    {
        return Row*Col;
    }

public:

    iterator 
	begin()
    {
        return data_.begin();
    }

    iterator 
	end()
    {
        return data_.end();
    }

    const_iterator begin() const
    {
        return data_.begin();
    }

    const_iterator end() const
    {
        return data_.end();
    }

public:

    row_type row_begin(const size_type index = 0)
    {
        return begin() + index * Col;
    }

    row_type row_end(const size_type index = 0)
    {
        return row_begin(index) + Col;
    }

    const_row_type row_begin(const size_type index = 0) const
    {
        return begin() + index * Col;
    }

    const_row_type row_end(const size_type index = 0) const
    {
        return row_begin(index) + Col;
    }

public:

    col_type col_begin(const size_type index = 0)
    {
        return col_type(begin() + index);
    }

    col_type col_end(const size_type index = 0)
    {
        //return col_begin(index) + Col;
        return col_begin(index) + Row;
    }

    const_col_type col_begin(const size_type index = 0 ) const
    {
        return const_col_type(begin() + index);
    }

    const_col_type col_end(const size_type index = 0 ) const
    {
        //return col_begin(index) + Col;
        return col_begin(index) + Row;
    }

public:

    diag_type upper_diag_begin(const size_type index = 0 )
    {
        return diag_type(begin() + index);
    }

    diag_type upper_diag_end(const size_type index = 0 )
    {
        size_type depth = Col - index;
        if (Row < depth)
            depth = Row;
        return upper_diag_begin(index) + depth;
    }

    const_diag_type upper_diag_begin(const size_type index = 0 ) const
    {
        return const_diag_type(begin() + index);
    }

    const_diag_type upper_diag_end(const size_type index = 0 ) const
    {
        size_type depth = Col - index;
        if (Row < depth)
            depth = Row;
        return upper_diag_begin(index) + depth;
    }

public:

    diag_type lower_diag_begin(const size_type index = 0 )
    {
        return diag_type(begin() + index * Col);
    }

    diag_type lower_diag_end(const size_type index = 0 )
    {
        size_type depth = Row - index;
        if (Col < depth)
            depth = Col;
        return lower_diag_begin(index) + depth;
    }

    const_diag_type lower_diag_begin(const size_type index = 0 ) const
    {
        return const_diag_type(begin() + index * Col);
    }

    const_diag_type lower_diag_end(const size_type index = 0 ) const
    {
        size_type depth = Row - index;
        if (Col < depth)
            depth = Col;
        return lower_diag_begin(index) + depth;
    }

public:

    diag_type diag_begin()
    {
        return lower_diag_begin(0);
    }

    diag_type diag_end()
    {
        return lower_diag_end(0);
    }

    const_diag_type diag_begin() const
    {
        return lower_diag_begin(0);
    }

    const_diag_type diag_end() const
    {
        return lower_diag_end(0);
    }

public:

    reverse_iterator rbegin()
    {
        return reverse_iterator( end() );
    }

	reverse_iterator rend()
	{
		return reverse_iterator( begin() );
	}

    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator( end() );
    }

	const_reverse_iterator rend() const
	{
		return const_reverse_iterator( begin() );
	}

public:
	reverse_row_type row_rbegin( const size_type index = 0 )
	{
		return reverse_row_type( row_end( index ) );
	}

	reverse_row_type row_rend( const size_type index = 0 )
	{
		return reverse_row_type( row_begin( index ) );
	}

	const_reverse_row_type row_rbegin( const size_type index = 0 ) const
	{
		return const_reverse_row_type( row_end( index ) );
	}

	const_reverse_row_type row_rend( const size_type index = 0 ) const
	{
		return const_reverse_row_type( row_begin( index ) );
	}

public:
	reverse_col_type col_rbegin( const size_type index = 0 )
	{
		return reverse_col_type( col_end( index ) );
	}

	reverse_col_type col_rend( const size_type index = 0 )
	{
		return reverse_col_type( col_begin( index ) );
	}

	const_reverse_col_type col_rbegin( const size_type index = 0 ) const
	{
		return const_reverse_col_type( col_end( index ) );
	}

	const_reverse_col_type col_rend( const size_type index = 0 ) const
	{
		return const_reverse_col_type( col_begin( index ) );
	}

public:
	reverse_upper_diag_type upper_diag_rbegin( const size_type index = 0 )
	{
		return reverse_upper_diag_type( upper_diag_end( index ) );
	}

	reverse_upper_diag_type upper_diag_rend( const size_type index = 0 )
	{
		return reverse_upper_diag_type( upper_diag_begin( index ) );
	}

	const_reverse_upper_diag_type upper_diag_rbegin( const size_type index = 0 ) const
	{
		return const_reverse_upper_diag_type( upper_diag_end( index ) );
	}

	const_reverse_upper_diag_type upper_diag_rend( const size_type index = 0 ) const
	{
		return const_reverse_upper_diag_type( upper_diag_begin( index ) );
	}

public:
	reverse_lower_diag_type lower_diag_rbegin( const size_type index = 0 )
	{
		return reverse_lower_diag_type( lower_diag_end( index ) );
	}

	reverse_lower_diag_type lower_diag_rend( const size_type index = 0 )
	{
		return reverse_lower_diag_type( lower_diag_begin( index ) );
	}

	const_reverse_lower_diag_type lower_diag_rbegin( const size_type index = 0 ) const
	{
		return const_reverse_lower_diag_type( lower_diag_end( index ) );
	}

	const_reverse_lower_diag_type lower_diag_rend( const size_type index = 0 ) const
	{
		return const_reverse_lower_diag_type( lower_diag_begin( index ) );
	}

public:
	reverse_diag_type diag_rbegin()
	{
		return reverse_diag_type( diag_end() );
	}

	reverse_diag_type diag_rend()
	{
		return reverse_diag_type( diag_begin() );
	}

	const_reverse_diag_type diag_rbegin() const
	{
		return const_reverse_diag_type( diag_end() );
	}

	const_reverse_diag_type diag_rend() const
	{
		return const_reverse_diag_type( diag_begin() );
	}

public:

    row_type operator[](const size_type index )
    {
        return row_begin(index);
    }

    const_row_type operator[](const size_type index ) const
    {
        return row_begin(index);
    }

public:

    const self_type operator-() const
    {
        self_type ans(*this);
        std::transform(ans.begin(), ans.end(), ans.begin(),
                       std::negate<value_type> ());
        return ans;
    }

    const self_type operator+() const
    {
        self_type ans(*this);
        return ans;
    }

    const static_matrix<value_type, Col, Row> 
	operator~() const
    {
        return transpose();
    }

public:

    const static_matrix<value_type, Col, Row>
    transpose() const
    {
        static_matrix<value_type, Col, Row> ans;

        for (size_type i = 0; i < Col; ++i)
        {
            std::copy(col_begin(i), col_end(i), ans.row_begin(i));
        }

        return ans;
    }


public:

    bool operator == ( const self_type& rhs ) const
    {
    	return std::equal( begin(), end(), rhs.begin() );
    }

    bool operator == ( const value_type& rhs ) const
    {
	return 	std::count( begin(), end(), rhs ) == 
	    	std::distance( begin(), end() );
    }

    bool operator < ( const self_type& rhs ) const
    {
    	return std::lexicographical_compare( begin(), end(), rhs.begin(), rhs.end() );
    }

   bool operator < ( const value_type& rhs ) const
   {
  	return 	std::count_if( begin(), end(), rhs, std::bind2nd(std::less<value_type>(), rhs) ) == size();
   }

   bool operator > ( const value_type& rhs ) const
   {
  	return 	std::count_if( begin(), end(), rhs, std::bind2nd(std::greater<value_type>(), rhs) ) == size();
   }

public:


    self_type & operator +=(const value_type& rhs)
    {
        std::transform(begin(), end(), begin(),
                       std::bind1st(std::plus<value_type > (), rhs));
        return *this;
    }

    self_type & operator +=(const self_type& rhs)
    {
        std::transform(begin(), end(), rhs.begin(), begin(),
                       std::plus<value_type > ());
        return *this;
    }

    self_type & operator -=(const value_type& rhs)
    {
        std::transform(begin(), end(), begin(),
                       std::bind1st(std::minus<value_type > (), rhs));
        return *this;
    }

    self_type & operator -=(const self_type& rhs)
    {
        std::transform(begin(), end(), rhs.begin(), begin(),
                       std::minus<value_type > ());
        return *this;
    }

    self_type & operator *=(const value_type& rhs)
    {
        std::transform(begin(), end(), begin(),
                       std::bind1st(std::multiplies<value_type > (), rhs));
        return *this;
    }

    self_type & operator /=(const value_type& rhs)
    {
        std::transform(begin(), end(), begin(),
                       std::bind2nd(std::divides<value_type > (), rhs));
        return *this;
    }

#if 0
public:

    friend std::ostream & operator <<(std::ostream& lhs, const self_type& rhs)
    {
        for (size_type i = 0; i < rhs.row(); ++i)
        {
            std::copy(rhs.row_begin(i), rhs.row_end(i),
                      std::ostream_iterator<value_type > (lhs, "\t"));
            lhs << "\n";
        }
        return lhs;
    }
#endif 

public:

    bool is_square() const
    {
        return false;
    }

    bool is_row_vector() const
    {
		return false;
    }

    bool is_col_vector() const
    {
		return false;
    }

    bool is_lower_triangular() const
    {
        return false;
    }

    bool is_upper_triangular() const
    {
        return false;
    }

    bool is_lower_hessenberg() const
    {
        return false;
    }

    bool is_upper_hessenberg() const
    {
        return false;
    }

    bool is_diagonal() const
    {
        return false;
    }

    bool is_tridiagonal() const
    {
        return false;
    }

    bool is_pentadiagonal() const
    {
        return false;
    }

    bool is_symmetric() const
    {
        return false;
    }

private:

    storage_type data_;
};


}//namespace sm

#endif	/* _STATIC_MATRIX_M_N_HPP_INCLUDED */

