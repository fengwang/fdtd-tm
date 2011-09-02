#include "../../static_matrix.hpp"

#include <cstddef>
#include <algorithm>

namespace sm{


template<>
template <
		 typename T,
         std::size_t Dim
         >
struct  lu_decomposition<static_matrix<T, Dim, Dim> >
{
        typedef static_matrix<T, Dim, Dim> matrix_type;
        typedef typename matrix_type::value_type value_type;

        const matrix_type& a_;
        matrix_type l_;
        matrix_type u_;

        lu_decomposition( const matrix_type& a ) :
            a_( a )
        {
            do_decomposition();
        }

        const matrix_type
        lower() const
        {
            return l_;
        }

        const matrix_type
        upper() const
        {
            return u_;
        }

    private:
        void 
		do_decomposition()
        {
            std::fill( l_.diag_begin(), l_.diag_end(), value_type( 1 ) );
            value_type sum;

            for ( std::size_t j = 0; j < Dim; ++j )
                {
                    for ( std::size_t i = 0; i < j + 1; ++i )
                        {
                            sum = value_type();

                            for ( std::size_t k = 0; k < i; ++k )
                                {
                                    sum += l_[i][k] * u_[k][j];
                                }

                            u_[i][j] = a_[i][j] - sum;
                        }

                    for ( std::size_t i = j + 1; i < Dim; ++i )
                        {
                            sum = value_type();

                            for ( std::size_t k = 0; k < j; ++k )
                                {
                                    sum += l_[i][k] * u_[k][j];
                                }

                            l_[i][j] = a_[i][j] - sum;
                            l_[i][j] /= u_[j][j];
                        }
                }//end of j loop

        }//end of do_decomposition 

};//end of class lu_decomposition 

}//namespace sm


