// -*- C++ -*-
/* 
 * File:   static_matrix_buffer.hpp
 * Author: feng
 *
 * Created on October 9, 2009, 3:59 PM
 */

#ifndef _STATIC_MATRIX_buffer_HPP_INCLUDED
#define	_STATIC_MATRIX_buffer_HPP_INCLUDED
#include <cstddef>
#include <algorithm>
#include <cstring>

namespace sm{


// 	for a 64-bit cpu with cache size 2048 KB, 
// 	Size could be set more than 1,048,576 (unsigned long)

template<
			typename Type,
			std::size_t Size
		>
class static_matrix_buffer
{
public:
    typedef Type 									value_type;
    typedef static_matrix_buffer 					self_type;
    typedef value_type* 							pointer;
    typedef const value_type* 						const_pointer;
    typedef value_type& 							reference;
    typedef const value_type& 						const_reference;
    typedef value_type* 							iterator;
    typedef const value_type* 						const_iterator;
	typedef std::reverse_iterator<iterator> 		reverse_iterator;
	typedef std::reverse_iterator<const_iterator> 	const_reverse_iterator;
	typedef std::size_t 							size_type;
    typedef std::ptrdiff_t 							difference_type;
	typedef value_type 								storage_type[Size];

public:

    static_matrix_buffer()
    {
        memset(buffer_, 0, Size * sizeof (Type));
    }

    ~static_matrix_buffer() { }


public:

    static_matrix_buffer(const self_type& rhs)
    {
        operator=(rhs);
    }

    self_type & 
	operator=(const self_type& rhs)
    {
        do_copy(rhs);
        return *this;
    }

    template< typename U>
    static_matrix_buffer(const static_matrix_buffer<U, Size>& rhs)
    {
        operator=(rhs);
    }

    template< typename U>
	self_type & operator=(const static_matrix_buffer<U, Size>& rhs)
    {
        do_copy<U> (rhs);
        return *this;
    }

private:

    template< typename U>
    void do_copy(const static_matrix_buffer<U, Size>& rhs)
    {
        std::copy(rhs.begin(), rhs.end(), begin());
    }

    void do_copy(const self_type& rhs)
    {
        memcpy(buffer_, rhs.buffer_, Size * sizeof (Type));
    }

public:
    size_type 
	size() const
    {
        return Size;
    }

public:

    iterator 
	begin()
    {
        return buffer_;
    }

    const_iterator 
	begin()const
    {
        return buffer_;
    }

    iterator 
	end()
    {
        return begin() + Size;
    }

    const_iterator 
	end()const
    {
        return begin() + Size;
    }

	reverse_iterator 
	rbegin()
	{
		return reverse_iterator( end() );
	}

	reverse_iterator 
	rend()
	{
		return reverse_iterator( begin() );
	}

	const_reverse_iterator 
	rbegin() const
	{
		return const_reverse_iterator( end() );
	}

	const_reverse_iterator 
	rend() const
	{
		return const_reverse_iterator( begin() );
	}

public:

	reference 
	operator[](const size_type index)
	{
		return buffer_[index];
	}

	const_reference 
	operator[](const size_type index) const
	{
		return buffer_[index];
	}

private:
	storage_type buffer_;
};



}//namespace sm

#endif	/* _STATIC_MATRIX_BUFFER_HPP_INCLUDED */

