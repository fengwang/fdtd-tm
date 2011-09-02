// -*- C++ -*-
/* 
 * File:   static_matrix_stride_iterator.hpp
 * Author: feng
 *
 * Created on October 9, 2009, 4:43 PM
 */

#ifndef _STATIC_MATRIX_STRIDE_ITERATOR_HPP_INCLUDED
#define	_STATIC_MATRIX_STRIDE_ITERATOR_HPP_INCLUDED

#include <cstddef>
#include <iterator>
#include <boost/operators.hpp>

namespace sm{


template< typename Iterator_Type, std::size_t Step >
class static_matrix_stride_iterator
:
boost::incrementable<static_matrix_stride_iterator<Iterator_Type, Step>,
boost::decrementable<static_matrix_stride_iterator<Iterator_Type, Step>,
boost::addable<static_matrix_stride_iterator<Iterator_Type, Step>, typename std::iterator_traits<Iterator_Type>::difference_type,
boost::partially_ordered<static_matrix_stride_iterator<Iterator_Type,Step>
> > > >
{
public:
    typedef typename std::iterator_traits<Iterator_Type>::value_type value_type;
    typedef typename std::iterator_traits<Iterator_Type>::reference reference;
    typedef typename std::iterator_traits<Iterator_Type>::difference_type difference_type;
    typedef typename std::iterator_traits<Iterator_Type>::pointer pointer;
    typedef std::random_access_iterator_tag iterator_category;
    typedef static_matrix_stride_iterator self_type;

    static_matrix_stride_iterator() : iterator_(0){ }

    static_matrix_stride_iterator(const self_type& s) : iterator_(s.iterator_){ }

    explicit static_matrix_stride_iterator(const Iterator_Type& it) : iterator_(it){ }

    self_type & operator++()
    {
        iterator_ += Step;
        return *this;
    }

    self_type & operator+=(const difference_type dt)
    {
        iterator_ += dt*Step;
        return *this;
    }

    self_type & operator--()
    {
        iterator_ -= Step;
        return *this;
    }

    self_type & operator-=(const difference_type dt)
    {
        iterator_ -= dt*Step;
        return *this;
    }

    reference operator[](const difference_type dt)
    {
        return iterator_[dt * Step];
    }

    const reference operator[](const difference_type dt) const
    {
        return iterator_[dt * Step];
    }

    reference operator*()
    {
        return *iterator_;
    }

    const reference operator*() const
    {
        return *iterator_;
    }

    friend bool operator ==(const self_type& lhs, const self_type& rhs)
    {
        return lhs.iterator_ == rhs.iterator_;
    }

    friend bool operator !=(const self_type& lhs, const self_type& rhs)
    {
        return lhs.iterator_ != rhs.iterator_;
    }

    friend bool operator<(const self_type& lhs, const self_type& rhs)
    {
        return lhs.iterator_ < rhs.iterator_;
    }

    friend difference_type operator -(const self_type& lhs, const self_type& rhs)
    {
        return (lhs.iterator_ - rhs.iterator_) / Step;
    }

private:
    Iterator_Type iterator_;
}; //static_matrix_Stride_Iterator

}//namespace sm


#endif	/* _STATIC_MATRIX_STRIDE_ITERATOR_HPP_INCLUDED */

