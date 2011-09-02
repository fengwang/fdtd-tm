#ifndef _SPARSE_MATRIX_HPP_INCLUDED_SDLKJ5OIJSDFEKLJSDOUJ4LKJSDFLKJSDFLKMNCLKJDFLKJSOIJULKJDFOIJ4LKJSFDOIJFJ
#define _SPARSE_MATRIX_HPP_INCLUDED_SDLKJ5OIJSDFEKLJSDOUJ4LKJSDFLKJSDFLKMNCLKJDFLKJSOIJULKJDFOIJ4LKJSFDOIJFJ

//!! c++0x must be enabled 

#include <cstddef>
#include <cassert>
#include <vector>
#include <valarray>
#include <map>
#include <algorithm>
#include <thread>

#include <iostream>

namespace feng
{

    struct make_key
    {
        typedef std::size_t size_type;
        typedef unsigned long long key_type;

        key_type operator() ( const size_type r, const size_type c ) const 
        {
            return ( key_type(r) << 32 ) | key_type(c);
        }
    };

    struct isolate_row
    {
        typedef make_key::size_type size_type;
        typedef make_key::key_type key_type;
        size_type operator()( const key_type k ) const 
        {
            return k >> 32;
        }
    };

    struct isolate_col
    {
        typedef make_key::size_type size_type;
        typedef make_key::key_type key_type;
        size_type operator()( const key_type k ) const 
        {
            return k & 0xFFFFFFFFUL ;
        }
    };

    template< typename T >
    struct sparse_matrix
    {
        typedef sparse_matrix self_type;
        
        typedef T value_type;
        typedef make_key::size_type size_type;
        typedef make_key::key_type key_type;
        typedef std::map<key_type, value_type> coordinate_value_associate_type;
        typedef typename coordinate_value_associate_type::value_type wrapped_value_type; 

    private:
        coordinate_value_associate_type data_;

        size_type row_;
        size_type col_;

    public:
        sparse_matrix( const size_type row = 10, const size_type col = 10 ) : row_( row ), col_( col ) 
        {   
            assert( 0 == ( key_type(row) >> 32 ) ); 
            assert( 0 == ( key_type(col) >> 32 ) ); 
        }

        sparse_matrix( const self_type& other ) : data_( other.data_ ), row_( other.row_ ), col_( other.col_ ) {}
        sparse_matrix( self_type&& other ) : data_( other.data_ ), row_( other.row_ ), col_( other.col_ ) {}

        const value_type& 
        operator() ( const size_type r, const size_type c ) const 
        {
            assert( row_ > r );
            assert( col_ > c );

            if ( auto it = data_.find(make_key()(r, c)) != data_.end() ) return *it;

            return value_type();
        }

        value_type& 
        operator() ( const size_type r, const size_type c )
        {
            assert( row_ > r );
            assert( col_ > c );

            return data_[make_key()(r, c)];
        }
        
    private:
        void remove_invalid()
        {
            data_.erase( std::remove_if( data_.begin(), data_.end(),[](wrapped_value_type p){ value_type() == p.second; } ), data_.end() );
        }

    public:
        self_type& 
        operator += ( const self_type& other )
        {
            std::for_each( other.data_.begin(), other.data_.end(), [this](wrapped_value_type p)
                            { 
                                ((*this).data_)[p.first]+= p.second; 
                                if ( value_type() == ((*this).data_)[p.first] ) ((*this).data_).erase(p.first);
                            } 
                         );
            return *this;
        }

        self_type& 
        operator -= ( const self_type& other )
        {
            std::for_each( other.data_.begin(), other.data_.end(), [this](wrapped_value_type p)
                            { 
                                ((*this).data_)[p.first]-= p.second; 
                                if ( value_type() == ((*this).data_)[p.first] ) ((*this).data_).erase(p.first);
                            } 
                         );
            return *this;
        }

        self_type& 
        operator *= ( const value_type v ) 
        {
            std::for_each( data_.begin(), data_.end(), [v](wrapped_value_type& p) { p.second *= v; } );
            return *this;
        }

        self_type& 
        operator /= ( const value_type v ) 
        {
            std::for_each( data_.begin(), data_.end(), [v](wrapped_value_type& p) { p.second /= v; } );
            return *this;
        }

    public:
        self_type&
        operator *= ( const self_type& other )
        {
            assert( col_ == other.row_ );

            self_type ans( row_, other.col_ );
            
            // !!!! TODO: parallel here !!!!
            std::for_each( data_.begin(), data_.end(), [&](wrapped_value_type& p)
                           {
                            std::for_each( other.data_.begin(), other.data_.end(), [&](wrapped_value_type pp)
                                           {
                                            if ( isolate_col()(p.first) == isolate_row()(pp.first) )
                                                ans.data_[make_key()(isolate_row()(p.first), isolate_col()(pp.first))] += p.second * pp.second;
                                           } 
                                        );// foreach other 
                           } 
                        );// foreach *this
            swap( ans );
            //remove_invalid();
            return *this;
        }

    public:
        const self_type 
        operator - () 
        {
            self_type ans( *this );
            std::for_each( ans.begin(), ans.end(), []( wrapped_value_type& p ) { p.second = - p.second; } );
            return std::move(ans);
        }

        const self_type 
        operator + () 
        {
            self_type ans( *this );
            return std::move(ans);
        }
    
    public:
        size_type row() const { return row_; }
        size_type col() const { return col_; }
        size_type& row() { return row_; }
        size_type& col() { return col_; }

        bool empty() const { return data_.empty(); }
        bool size() const { return data_.size(); }
        void clear() { data_.clear(); }

        const self_type
        transpose() const 
        {
            self_type ans(col(), row());
            //std::for_each( data_.begin(), data_.end(), 
            //        [&ans](wrapped_value_type& p) { ans.insert( (p.first)<<32, (p.first)>>32, p.second ); } );
            for( auto i = data_.begin(); i != data_.end(); ++i )
            {
                ans.insert( ((((*i).first)<<32)>>32), ((*i).first)>>32, (*i).second ); 
            }
            return std::move( ans );
        }

        void insert( const size_type r, const size_type c, const value_type v )
        {
            assert( row_ > r );
            assert( col_ > c );

            const key_type key = make_key()(r, c);
            data_[key] = v;
            if ( value_type() == v ) data_.erase( key );
        }

        void resize( const size_type r, const size_type c )
        {
            if ( r < row_ )
                data_.erase( std::remove_if( data_.begin(), data_.end(),[r](wrapped_value_type& p){ ((p.first)>>32) > r; } ), data_.end() );
            if ( c < col_ )
                data_.erase( std::remove_if( data_.begin(), data_.end(),[c](wrapped_value_type& p){ ((p.first)&0xFFFFFFFFUL) > c; } ), data_.end() );

            row_ = r;
            col_ = c;
        }

        void swap( self_type& other )
        {
            data_.swap(other.data_);
            row_ ^= other.row_; other.row_ ^= row_; row_ ^= other.row_;
            col_ ^= other.col_; other.col_ ^= col_; col_ ^= other.col_;
        }

    friend const self_type // A+B
        operator + ( const self_type& lhs, const self_type& rhs )
        {
            self_type ans( lhs );
            ans += rhs;
            return std::move( ans );
        }

    friend const self_type // A-B
        operator - ( const self_type& lhs, const self_type& rhs )
        {
            self_type ans( lhs );
            ans -= rhs;
            return std::move( ans );
        }

    friend const self_type // A*b
        operator * ( const self_type& lhs, const value_type rhs )
        {
            self_type ans( lhs );
            ans *= rhs;
            return std::move( ans );
        }

    friend const self_type // A/b
        operator / ( const self_type& lhs, const value_type rhs )
        {
            self_type ans( lhs );
            ans /= rhs;
            return std::move( ans );
        }

    friend const self_type // AB
        operator * ( const self_type& lhs, const self_type& rhs )
        {
            self_type ans( lhs );
            ans *= rhs;
            return std::move( ans );
        }

    friend const std::vector<value_type> // Ax
        operator * ( const self_type& lhs, const std::vector<value_type>& rhs )
        {
            assert( rhs.size() == lhs.col_ );
            std::vector<value_type> ans( lhs.row_ ); 
            lhs.multiply_array( rhs.begin(), rhs.end(), ans.begin() );
            return std::move( ans );
        }

    friend const std::valarray<value_type> 
        operator * ( const self_type& lhs, const std::valarray<value_type>& rhs )
        {
            assert( rhs.size() == lhs.col_ );
            std::valarray<value_type> ans( lhs.row_ ); 

            lhs.multiply_array( &rhs[0], &rhs[lhs.col_], &ans[0] );
            /*
            const size_type hardware_threads = std::thread::hardware_concurrency();
            const size_type threads = hardware_threads ? hardware_threads : 2;

            std::thread thread_pool[threads-1];
            const size_type N = rhs.size();
            const size_type blocks = threads/N;

            for ( size_type index = 0; index < threads-1; ++index )
            {
                thread_pool[i] = std::thread( &self_type::multiply_array, &lhs, &rhs[0], &rhs[N], &ans[index*blocks], &ans[(1+index)*blocks], index*blocks );  
            }
            lhs.multiply_array( &rhs[0], &rhs[N], &ans[index*blocks], &ans[N], N-index*blocks); 

            for ( size_type index = 0; index < threads-1; ++index )
            {
                thread_pool[i].join();
            }

            // ans[i] = lhs[i][] * rhs[][1] 
            //
            // 1) get cpu numbers n
            // 2) arrange every cpu task 0~n/N, n/N~2n/N, ..., ?~N
            // multiply( itor_in_first, itor_in_last, itor_out_first, itor_out_last, index );
            // multiply( &rhs[0], &rhs[N], &ans[i*n/N], &ans[(i+1)*n/N], i*n/N);
            
            */


            return std::move( ans );
        }
    friend std::ostream&
        operator << ( std::ostream& lhs, const self_type& rhs )
        {
            std::for_each( rhs.data_.begin(), rhs.data_.end(), [&](wrapped_value_type p)
                           { 
                            lhs << "(" << isolate_row()(p.first) << ", " << isolate_col()(p.first) << ") = " << p.second << "\n"; 
                           } 
                         );
            return lhs;
        }

    private:
        // multiply array 
        template< typename Itor, typename Otor >
        Otor multiply_array( Itor in_first, Itor in_last, Otor out ) const 
        {
            assert( col_ == size_type( std::distance( in_first, in_last ) ) );
            
            typename coordinate_value_associate_type::const_iterator head = data_.begin();
            typename coordinate_value_associate_type::const_iterator tail = data_.begin();
            for ( size_type i = 0; i < row_; ++i )
            {
                head = tail;
                tail = std::find_if( head, data_.end(), [i](wrapped_value_type p) { return i+1 == isolate_row()(p.first); } );
                if ( std::distance( head, tail ) == 0 ) { *out++ = 0; continue; }
                auto tmp = value_type();
                std::for_each( head, tail, [&tmp, in_first](wrapped_value_type p ) { tmp += (*( in_first + isolate_col()(p.first) )) * p.second; } );
                *out++ = tmp;                            
            }
            return out;
        }
        // multiply array 

        template< typename Itor, typename Otor >
        Otor multiply_array( Itor in_first, Itor in_last, Otor out_first, Otor out_last, size_type index ) const 
        {
            while ( out_first != out_last )
            {
                auto head = std::find_if( data_.begin(), data_.end(), [index](wrapped_value_type p) { return index == isolate_row()(p.first); } );
                auto tail = std::find_if( data_.begin(), data_.end(), [&index](wrapped_value_type p) { return index++ != isolate_row()(p.first); } );
                if ( data_.end() == head ) { *out_first++ = value_type(); continue; }
                auto sum = value_type();
                std::for_each( head, tail, [&sum, in_first](wrapped_value_type p ) { sum += (*( in_first + isolate_col()(p.first) )) * p.second; } );
                *out_first++ = sum;
            }
            return out_first;

            /*
            for ( size_type i = 0; i < row_; ++i )
            {
                head = tail;
                tail = std::find_if( head, data_.end(), [i](wrapped_value_type p) { return i+1 == isolate_row()(p.first); } );
                if ( std::distance( head, tail ) == 0 ) { *out++ = 0; continue; }
                auto tmp = value_type();
                std::for_each( head, tail, [&tmp, in_first](wrapped_value_type p ) { tmp += (*( in_first + isolate_col()(p.first) )) * p.second; } );
                *out++ = tmp;
            }
            return out;
            */
        }

        value_type* 
        multiply_array( value_type* in_first, value_type* in_last, value_type* out_first, value_type* out_last, size_type index ) const 
        {
            return multiply_array<value_type*, value_type*>( in_first, in_last, out_first, out_last, index );
        }





    };//sparse_matrix 

}//namespace feng

#endif//_SPARSE_MATRIX_HPP_INCLUDED_SDLKJ4OIJSDFEKLJSDOUJ4LKJSDFLKJSDFLKMNCLKJDFLKJSOIJULKJDFOIJ4LKJSFDOIJFJ



