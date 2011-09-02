#ifndef _STATIC_MATRIX_1_1_HPP
#define	_STATIC_MATRIX_1_1_HPP

#include "static_matrix_buffer.hpp"
#include "static_matrix_stride_iterator.hpp"

#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <iosfwd>

#include <boost/operators.hpp>

namespace sm{



template<typename Type>
class static_matrix<Type, 1, 1>
:
boost::addable 						<static_matrix<Type, 1, 1>,
boost::addable 						<static_matrix<Type, 1, 1>, Type,
boost::subtractable 				<static_matrix<Type, 1, 1>,
boost::subtractable 				<static_matrix<Type, 1, 1>, Type,
boost::multipliable 				<static_matrix<Type, 1, 1>,
boost::multipliable 				<static_matrix<Type, 1, 1>, Type,
boost::dividable 					<static_matrix<Type, 1, 1>,
boost::dividable 					<static_matrix<Type, 1, 1>, Type,
boost::equality_comparable 			<static_matrix<Type, 1, 1>,
boost::equality_comparable 			<static_matrix<Type, 1, 1>, Type,
boost::less_than_comparable 		<static_matrix<Type, 1, 1>,
boost::less_than_comparable 		<static_matrix<Type, 1, 1>, Type
> > > > > > > > > > > >
{
public:
    
    typedef Type 														value_type;
    typedef static_matrix 												self_type;
    typedef value_type* 												iterator;
    typedef const value_type* 											const_iterator;
    typedef value_type* 												row_type;
    typedef const value_type* 											const_row_type;
    typedef static_matrix_stride_iterator<value_type*, 1> 				col_type;
    typedef static_matrix_stride_iterator<const value_type*, 1> 		const_col_type;
    typedef static_matrix_stride_iterator<value_type*, 2> 				diag_type;
    typedef static_matrix_stride_iterator<const value_type*, 2> 		const_diag_type;
    typedef static_matrix_buffer<value_type, 1> 						storage_type;
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

    static_matrix(){}

    ~static_matrix(){}

private:

    void do_copy( const self_type& rhs)
    {
        data_ = rhs.data_;
    }

    template<typename U>
    void do_copy(const static_matrix<U, 1, 1>& rhs)
    {
		*begin() = *(rhs.begin());
    }

public:

    self_type& operator=( const self_type& rhs )
    {
        do_copy(rhs);
        return *this;
    }

    template<typename U>
    self_type& operator=( const static_matrix<U, 1, 1>& rhs )
    {
        do_copy<U>(rhs);
        return *this;
    }

public:
    static_matrix( const self_type& rhs )
    {
        operator=(rhs);
    }

    template<typename U>
    static_matrix( const static_matrix<U, 1, 1>& rhs )
    {
        operator=<U>(rhs);
    }
    
public:
    iterator begin()
    {
        return data_.begin();
    }

    const_iterator begin() const
    {
        return data_.begin();
    }

    iterator end()
    {
        return data_.end();
    }

    const_iterator end() const
    {
        return data_.end();
    }

public:

    size_type row() const
    {
        return 1;
    }

    size_type col() const
    {
        return 1;
    }

    size_type size()const
    {
        return 1;
    }

public:

    row_type row_begin(const size_type index = 0)
    {
        return begin() + index;
    }

    row_type row_end(const size_type index = 0)
    {
        return row_begin(index) + 1;
    }

    const_row_type row_begin(const size_type index = 0) const
    {
        return begin() + index;
    }

    const_row_type row_end(const size_type index = 0) const
    {
        return row_begin(index) + 1;
    }

public:

    col_type col_begin(const size_type index)
    {
        return col_type(begin() + index);
    }

    col_type col_end(const size_type index)
    {
        return col_begin(index) + 1;
    }

    const_col_type col_begin(const size_type index) const
    {
        return const_col_type(begin() + index);
    }

    const_col_type col_end(const size_type index) const
    {
        return col_begin(index) + 1;
    }

public:

    diag_type upper_diag_begin(const size_type index)
    {
        return diag_type(begin() + index);
    }

    diag_type upper_diag_end(const size_type index)
    {
        const size_type depth = 1 - index;
        return upper_diag_begin(index) + depth;
    }

    const_diag_type upper_diag_begin(const size_type index) const
    {
        return const_diag_type(begin() + index);
    }

    const_diag_type upper_diag_end(const size_type index) const
    {
        const size_type depth = 1 - index;
        return upper_diag_begin(index) + depth;
    }


public:

    diag_type lower_diag_begin(const size_type index)
    {
        return diag_type(begin() + index);
    }

    diag_type lower_diag_end(const size_type index)
    {
        const size_type depth = 1 - index;
        return lower_diag_begin(index) + depth;
    }

    const_diag_type lower_diag_begin(const size_type index) const
    {
        return const_diag_type(begin() + index);
    }

    const_diag_type lower_diag_end(const size_type index) const
    {
        const size_type depth = 1 - index;
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

    row_type operator[](const size_type index)
    {
        return row_begin(index);
    }

    const_row_type operator[](const size_type index) const
    {
        return row_begin(index);
    }

public:

    bool operator == ( const self_type& rhs ) const
    {
		return *(begin()) == *(rhs.begin());
    }

    bool operator == ( const value_type& rhs ) const
    {
		return *(begin()) == rhs;
    }

    bool operator < ( const self_type& rhs ) const
    {
		return *(begin()) < *(rhs.begin());
    }

   bool operator < ( const value_type& rhs ) const
   {
		return *(begin()) < rhs;
   }

   bool operator > ( const value_type& rhs ) const
   {
		return *(begin()) > *(rhs.begin());
   }

public:

    self_type & operator +=(const value_type& rhs)
    {
		*(begin()) += rhs;
        return *this;
    }

    self_type & operator +=(const self_type& rhs)
    {
		*(begin()) += *(rhs.begin());
        return *this;
    }

    self_type & operator -=(const value_type& rhs)
    {
		*begin() -= rhs;
        return *this;
    }

    self_type & operator -=(const self_type& rhs)
    {
		*begin() -= *(rhs.begin());
        return *this;
    }

    self_type & operator *=(const value_type& rhs)
    {
		*begin() *= rhs;
        return *this;
    }

    self_type & operator *=(const self_type& rhs)
    {
		operator *=( *rhs.begin() );
        return *this;
    }

    self_type & operator /=(const value_type& rhs)
    {
		*begin() /= rhs;
        return *this;
    }

    self_type & operator /=(const self_type& rhs)
    {
		operator /= ( rhs.begin() );
        return *this;
    }

public:
    friend const self_type operator / ( const value_type& lhs, const self_type& rhs )
    {
		self_type ans( lhs );
		ans /= rhs;
        return ans;
    }

public:
    const value_type det() const
    {
		return *begin();
    }


public:

    const self_type operator-() const
    {
        self_type ans(*this);
        *ans.begin() = - *(ans.begin());
		return ans;
    }

    const self_type operator+() const
    {
        self_type ans(*this);
        return ans;
    }

    const self_type operator~() const
    {
		self_type ans( *this );
		return ans;
    }

    const self_type operator!() const
    {
		return invert();
	}

public:
    const self_type transpose() const
    {
		self_type ans( *this );
		return ans;
    }

public:

    const self_type invert() const
    {
		self_type ans(*this);
		*ans.begin() = value_type(1)/(*ans.begin());
    	return ans;
    }

public:

    bool is_square() const
    {
        return true;
    }

    bool is_row_vector() const
    {
        return true;
    }

    bool is_col_vector() const
    {
        return true;
    }

    bool is_lower_triangular() const
    {
		return true;
    }

    bool is_upper_triangular() const
    {
		return true;
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
		return true;
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
		return true;
    }

private:
    storage_type data_;
    
};


}//namespace sm


#endif	/* _STATIC_MATRIX_1_1_HPP */

