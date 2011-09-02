#ifndef _PARALLEL_ORGANIZER_HPP_INCLUDED_SDOU487FSDKLUSFLKJSFDJFDLKJSLKAJSDLKJSKLJSFDLKJSFLDKJDIJSFLKJASFLKDJSFDKLJ
#define _PARALLEL_ORGANIZER_HPP_INCLUDED_SDOU487FSDKLUSFLKJSFDJFDLKJSLKAJSDLKJSKLJSFDLKJSFLDKJDIJSFLKJASFLKDJSFDKLJ

#include <cstddef>
#include <thread>
#include <functional>
#include <algorithm>
#include <iterator>

namespace feng 
{

template<typename T>
struct return_tag {};

struct parallel_organizer
{
    typedef std::size_t size_type;

    size_type min_per_thread;
    size_type hardware_thread;

    parallel_organizer( const size_type min_per_thread_ = 25 ) : min_per_thread( min_per_thread_ ) 
    {
        const size_type thread = std::thread::hardware_concurrency(); 
        hardware_thread = thread > 3 ? thread : 4;
    }

    // example:
    //      vector<double> arr;
    //      vector<double> ans;
    //      parallel_organizer()( arr.begin(), arr.end(), 
    //                            [](vector<double>::iterator first, vector<double>::iterator last)
    //                              { for_each( first, last, [](double& d){ d *= d; } }
    //                          );
    //      double acc = std::accumulate( ans.begin(), ans.end() );
    template< typename Itor, typename Function >
    void
    operator() ( Itor first, Itor last, Function f ) const 
    {
        const size_type length          = std::distance( first, last );
        if ( !length ) return; // return if input range empty

        const size_type max_thread_num  = (length+min_per_thread-1) / min_per_thread;
        const size_type thread_num      = std::min( hardware_thread, max_thread_num );
        const size_type block_size      = length/thread_num; //number of entries

        std::thread                     threads[thread_num-1]; 

        Itor block_first                = first;
        Itor block_last                 = first;
        for ( size_type i = 0; i < thread_num - 1; ++i )
        {
            std::advance( block_last, block_size );
            threads[i]                  = std::thread( f, block_first, block_last );
            block_first                 = block_last;
        }
        func( block_first, last );

        std::for_each( threads, threads+thread_num-1, [](std::thread& t){ t.join(); } );
    }

    // example:
    //      vector<double> arr;
    //      vector<double> ans;
    //      parallel_organizer()( arr.begin(), arr.end(), back_iserter(ans), 
    //                            [](vector<double>::iterator first, vector<double>::iterator last)
    //                              { return accumulate( first, last, double(0)); } 
    //                          );
    //      double acc = std::accumulate( ans.begin(), ans.end() );
    template<typename Itor, typename Otor, typename Function >
    Otor
    operator() ( Itor first, Itor last, Otor out, Function f ) const 
    {
        auto fake_first                 = first;
        auto tmp                        = f( fake_first, fake_first );
        typedef decltype(tmp)           value_type;

        const size_type length          = std::distance( first, last );
        if ( !length ) return out; // return if input range empty

        const size_type max_thread_num  = (length+min_per_thread-1) / min_per_thread;
        const size_type thread_num      = std::min( hardware_thread, max_thread_num );
        const size_type block_size      = length/thread_num; //number of entries

        value_type                      result[thread_num]; //cache result of each thread
        std::thread                     threads[thread_num-1]; 

        // the function to be passed to the thread
        auto func                       = [f](Itor first_, Itor last_, value_type* ans_) { *ans_ = f( first_, last_ ); };
        Itor block_first                = first;
        Itor block_last                 = first;
        for ( size_type i = 0; i < thread_num - 1; ++i )
        {
            std::advance( block_last, block_size );
            threads[i]                  = std::thread( func, block_first, block_last, result+i );
            block_first                 = block_last;
        }
        func( block_first, last, result+thread_num-1 );

        std::for_each( threads, threads+thread_num-1, [](std::thread& t){ t.join(); } );
       
        std::copy( result, result+thread_num, out );

        return out;
    }
    
    // example:
    //      vector<double> arr1;
    //      vector<double> arr2;
    //      vector<double> ans;
    //      parallel_organizer()( arr1.begin(), arr1.end(), arr2.begin(), back_iserter(ans), 
    //                            [](vector<double>::iterator first1, vector<double>::iterator last1, vector<double>::iterator first2)
    //                              { return inner_product( first1, last1, first2, double(0)); },
    //                            return_tag<void>()
    //                          );
    //      double acc = std::accumulate( ans.begin(), ans.end() );
    template< typename Itor1, typename Itor2, typename Function >
    void 
    operator() ( Itor1 first1, Itor1 last1, Itor2 first2, Function f, return_tag<void> ) const 
    {
        const size_type length          = std::distance( first1, last1 );
        if ( !length ) return; // return if input range empty

        const size_type max_thread_num  = (length+min_per_thread-1) / min_per_thread;
        const size_type thread_num      = std::min( hardware_thread, max_thread_num );
        const size_type block_size      = length/thread_num; //number of entries
        Itor1 block_first1              = first1;
        Itor2 block_first2              = first2;
        Itor1 block_last1               = first1;

        std::thread                     threads[thread_num-1]; 
        for ( size_type i = 0; i < thread_num - 1; ++i )
        {
            std::advance( block_last1, block_size );
            threads[i]                  = std::thread( f, block_first1, block_last1, block_first2 );
            block_first1                = block_last1;
            std::advance( block_first2, block_size );
        }

        f( block_first1, last1, block_first2 );
        std::for_each( threads, threads+thread_num-1, [](std::thread& t){ t.join(); } );
    }

    // example:
    //      vector<double> arr1;
    //      vector<double> arr2;
    //      vector<double> ans;
    //      parallel_organizer()( arr1.begin(), arr1.end(), arr2.begin(), back_iserter(ans), 
    //                            [](vector<double>::iterator first1, vector<double>::iterator last1, vector<double>::iterator first2)
    //                              { return inner_product( first1, last1, first2, double(0)); } 
    //                          );
    //      double acc = std::accumulate( ans.begin(), ans.end() );
    template< typename Itor1, typename Itor2, typename Otor, typename Function >
    Otor 
    operator() ( Itor1 first1, Itor1 last1, Itor2 first2, Otor out, Function f ) const 
    {
        auto fake_first1                = first1;
        auto fake_first2                = first2;
        auto tmp                        = f( first1, first1, first2 );
        typedef decltype( tmp )         value_type;

        const size_type length          = std::distance( first1, last1 );
        if ( !length ) return out; // return if input range empty

        const size_type max_thread_num  = (length+min_per_thread-1) / min_per_thread;
        const size_type thread_num      = std::min( hardware_thread, max_thread_num );
        const size_type block_size      = length/thread_num; //number of entries

        value_type                      result[thread_num]; //cache result of each thread
        std::thread                     threads[thread_num-1]; 

        // the function to be passed to the thread
        auto func                       = [f](Itor1 first1_, Itor1 last1_, Itor2 first2_, value_type* ans_){ *ans_ = f( first1_, last1_, first2_ ); };
        Itor1 block_first1              = first1;
        Itor2 block_first2              = first2;
        Itor1 block_last1               = first1;
        for ( size_type i = 0; i < thread_num - 1; ++i )
        {
            std::advance( block_last1, block_size );
            threads[i]                  = std::thread( func, block_first1, block_last1, block_first2, result+i );
            block_first1                = block_last1;
            std::advance( block_first2, block_size );
        }
        func( block_first1, last1, block_first2, result+thread_num-1 );

        std::for_each( threads, threads+thread_num-1, [](std::thread& t){ t.join(); } );
       
        std::copy( result, result+thread_num, out );

        return out;
    }

};//parallel_organizer

}//namespace feng



















#endif//_PARALLEL_ORGANIZER_HPP_INCLUDED_SDOU487FSDKLUSFLKJSFDJFDLKJSLKAJSDLKJSKLJSFDLKJSFLDKJDIJSFLKJASFLKDJSFDKLJ

