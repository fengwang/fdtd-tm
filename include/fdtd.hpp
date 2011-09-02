#ifndef _FDTD_HPP_INCLUDED_SDOIFJ498YUFSDLKJSALFDKJASLKDFJLKSJFDOIUJFSDLJSALFDKJF
#define _FDTD_HPP_INCLUDED_SDOIFJ498YUFSDLKJSALFDKJASLKDFJLKSJFDOIUJFSDLJSALFDKJF

#include <matrix.hpp>
#include <vector>
#include <cmath>

#include <iostream>

namespace feng
{

    template<typename T = double>
    struct fdtd
    {
        typedef T value_type;
        typedef sm::dynamic_matrix<value_type> matrix_type;
        typedef std::vector<value_type> vector_type;

        //constructor
        fdtd( const std::size_t grid_nx_=100, 
              const std::size_t grid_ny_=100, 
              const std::size_t boundary_size_ = 10, 
              const std::size_t pml_size_ = 5, 
              const std::size_t cylinder_radius_grid_ = 10,
              const value_type ct0_ = 30,
              const value_type pulse_width_ = 8 
            )
        {
            grid_nx = grid_nx_;
            grid_ny = grid_ny_;
            grid_nx_center = grid_nx / 2 - 1;
            grid_ny_center = grid_ny / 2 - 1;
            grid_nx_left_boundary = boundary_size_;
            grid_nx_right_boundary = grid_nx - grid_nx_left_boundary - 1;
            grid_ny_upper_boundary = boundary_size_;
            grid_ny_lower_boundary = grid_ny - grid_ny_upper_boundary - 1;
            pml = pml_size_;
            radius = cylinder_radius_grid_;
            ct0 = ct0_;
            pulse_width = pulse_width_;
            dz.resize( grid_nx, grid_ny );
            ez.resize( grid_nx, grid_ny );
            hx.resize( grid_nx, grid_ny );
            hy.resize( grid_nx, grid_ny );
            pec_ga.resize( grid_nx, grid_ny );
            ihx.resize( grid_nx, grid_ny );
            ihy.resize( grid_nx, grid_ny );
            pml_nx_gi1.resize(grid_nx);
            pml_nx_gi2.resize(grid_nx);
            pml_nx_gi3.resize(grid_nx);
            pml_nx_fi1.resize(grid_nx);
            pml_nx_fi2.resize(grid_nx);
            pml_nx_fi3.resize(grid_nx);
            pml_ny_gj1.resize(grid_ny);
            pml_ny_gj2.resize(grid_ny);
            pml_ny_gj3.resize(grid_ny);
            pml_ny_fj1.resize(grid_ny);
            pml_ny_fj2.resize(grid_ny);
            pml_ny_fj3.resize(grid_ny);
            ez_inc.resize(grid_ny);
            hx_inc.resize(grid_ny);
        }

        void initialize()
        {
            initialize_array();
            apply_pec();
            apply_pml(); 
        }

        void operator()()
        {
            incident_ez();
            incident_abc();
            iterate_dz();
            add_source();
            incident_dz();
            iterate_ez();
            incident_hx();
            iterate_hx();
            iterate_hy();
            incident_hy();
        }

        void incident_hy()
        {
            for ( std::size_t j = grid_ny_upper_boundary; j <= grid_ny_lower_boundary; ++j )
                {
                    hy[grid_nx_left_boundary - 1][j] -= value_type(0.5) * ez_inc[j];
                    hy[grid_nx_right_boundary][j]   += value_type(0.5) * ez_inc[j];
                }
        }

        void iterate_hy()
        {
            for ( std::size_t i = 0; i < grid_nx - 1; ++i )
                for ( std::size_t j = 0; j <= grid_ny - 1; ++j )
                    {
                        const value_type curl_e  = ez[i + 1][j] - ez[i][j];
                        ihy[i][j] += pml_ny_fj1[j] * curl_e ;
                        hy[i][j] = pml_nx_fi3[i] * hy[i][j] + pml_nx_fi2[i] * value_type(0.5) * ( curl_e + ihy[i][j] );
                    }
        }

        void iterate_hx()
        {
            for ( std::size_t i = 0; i < grid_nx; ++i )
                for ( std::size_t j = 0; j < grid_ny - 1; ++j )
                    {
                        const value_type curl_e =   ez[i][j] - ez[i][j + 1]  ;
                        ihx[i][j] += pml_nx_fi1[i] * curl_e ;
                        hx[i][j] = pml_ny_fj3[j] * hx[i][j] + pml_ny_fj2[j] * .5 * ( curl_e + ihx[i][j] );
                    }
            for ( std::size_t i = grid_nx_left_boundary; i <= grid_nx_right_boundary; ++i )
                {
                    hx[i][grid_ny_upper_boundary - 1] += value_type(0.5) * ez_inc[grid_ny_upper_boundary];
                    hx[i][grid_ny_lower_boundary]   -= value_type(0.5) * ez_inc[grid_ny_lower_boundary];
                }
        }

        void incident_hx()
        {
            for ( std::size_t j = 0; j < grid_ny; ++j )
                hx_inc[j] += value_type(0.5) * ( ez_inc[j] - ez_inc[j + 1] );
        }

        void iterate_ez()
        {
            for ( std::size_t j = 1; j < grid_ny - 1; ++j )
                for ( std::size_t i = 1; i < grid_nx - 1; ++i )
                    ez[i][j] = pec_ga[i][j] * dz[i][j];
        }

        void incident_dz()
        {
             for ( std::size_t i = grid_nx_left_boundary; i <= grid_nx_right_boundary; ++i )
                 {
                    dz[i][grid_ny_upper_boundary] += value_type(0.5) * hx_inc[grid_ny_upper_boundary - 1];
                    dz[i][grid_ny_lower_boundary] -= value_type(0.5) * hx_inc[grid_ny_lower_boundary];
                 }
        }
        
        void add_source()
        {
            static value_type current_t = 0;
            const value_type tmp1 = ( ct0 - current_t++ ) / pulse_width;
            //const value_type tmp1 = value_type(4) * ( ct0 - current_t++ ) / pulse_width;
            const value_type tmp2 = - tmp1 * tmp1;
            const value_type pi = 3.1415926535897932384626433;
            const value_type factor = value_type(4) / ( pulse_width * std::sqrt(pi) );
            const value_type pulse = factor * std::exp( tmp2 );
            ez_inc[3] = pulse;
        }

        void incident_ez()
        {
            for ( std::size_t j = 1; j < grid_ny; ++j )
                ez_inc[j] += value_type(0.5) * ( hx_inc[j - 1] - hx_inc[j] );    
        }

        void incident_abc()
        {
            static value_type ez_inc_low_m1 = 0;
            static value_type ez_inc_low_m2 = 0;
            static value_type ez_inc_high_m1 = 0;
            static value_type ez_inc_high_m2 = 0;
            ez_inc[0]      = ez_inc_low_m2;
            ez_inc_low_m2  = ez_inc_low_m1;
            ez_inc_low_m1  = ez_inc[1];
            ez_inc[grid_ny - 1]    = ez_inc_high_m2;
            ez_inc_high_m2  = ez_inc_high_m1;
            ez_inc_high_m1  = ez_inc[grid_ny - 2];
        }

        void iterate_dz()
        {
            for ( std::size_t j = 1; j < grid_nx; ++j )
                for ( std::size_t i = 1; i < grid_nx; ++i )
                    dz[i][j] =  pml_nx_gi3[i] * pml_ny_gj3[j] * dz[i][j] + 
                                pml_nx_gi2[i] * pml_ny_gj2[j] * value_type(0.5) * 
                                ( hy[i][j] - hy[i - 1][j] - hx[i][j] + hx[i][j - 1] ) ;
        }

        
        void initialize_array()
        {
            std::fill( ez_inc.begin(), ez_inc.end(), value_type(0) );
            std::fill( hx_inc.begin(), hx_inc.end(), value_type(0) );
            std::fill( dz.begin(), dz.end(), value_type(0) );
            std::fill( ez.begin(), ez.end(), value_type(0) );
            std::fill( hx.begin(), hx.end(), value_type(0) );
            std::fill( hy.begin(), hy.end(), value_type(0) );
            std::fill( ihx.begin(), ihx.end(), value_type(0) );
            std::fill( ihy.begin(), ihy.end(), value_type(0) );
            std::fill( pec_ga.begin(), pec_ga.end(), value_type(1) );
            std::fill( pml_nx_gi1.begin(), pml_nx_gi1.end(), value_type(0));
            std::fill( pml_nx_gi2.begin(), pml_nx_gi2.end(), value_type(1));
            std::fill( pml_nx_gi3.begin(), pml_nx_gi3.end(), value_type(1));
            std::fill( pml_nx_fi1.begin(), pml_nx_fi1.end(), value_type(0));
            std::fill( pml_nx_fi2.begin(), pml_nx_fi2.end(), value_type(1));
            std::fill( pml_nx_fi3.begin(), pml_nx_fi3.end(), value_type(1));
            std::fill( pml_ny_gj1.begin(), pml_ny_gj1.end(), value_type(0));
            std::fill( pml_ny_gj2.begin(), pml_ny_gj2.end(), value_type(1));
            std::fill( pml_ny_gj3.begin(), pml_ny_gj3.end(), value_type(1));
            std::fill( pml_ny_fj1.begin(), pml_ny_fj1.end(), value_type(0));
            std::fill( pml_ny_fj2.begin(), pml_ny_fj2.end(), value_type(1));
            std::fill( pml_ny_fj3.begin(), pml_ny_fj3.end(), value_type(1));
        }

        void apply_pec()
        {
            for ( std::size_t i = 0; i < grid_nx; ++i )
                for ( std::size_t j = 0; j < grid_ny; ++j )
                    if ( (i-grid_nx_center)*(i-grid_nx_center) + (j-grid_ny_center)*(j-grid_ny_center) <= radius*radius )
                        pec_ga[i][j] = 0;
        }

        void apply_pml()
        {
            for ( std::size_t i = 0; i <= pml; ++i )
            {
                value_type tmp1  = pml - i;
                value_type tmp2  = pml;
                value_type tmp3 = tmp1 / tmp2; 
                value_type tmp4  =value_type( 0.25 )* pow( tmp3,value_type(3));
                pml_nx_gi1[i] = tmp4; 
                pml_nx_gi1[grid_nx - 1 - i] = tmp4; 
                pml_nx_gi2[i] =value_type(1)/ (value_type(1)+ tmp4 );
                pml_nx_gi2[grid_nx - 1 - i] =value_type(1)/ (value_type(1)+ tmp4 );
                pml_nx_gi3[i] = (value_type(1)- tmp4 ) / (value_type(1)+ tmp4 );
                pml_nx_gi3[grid_nx - i - 1] = (value_type(1)- tmp4 ) / (value_type(1)+ tmp4 );
                tmp3 = ( tmp1 - value_type(0.5) ) / tmp2; 
                tmp4  =value_type( 0.25 )* pow( tmp3,value_type(3));
                pml_nx_fi1[i] = tmp4; 
                pml_nx_fi1[grid_nx - 2 - i] = tmp4; 
                pml_nx_fi2[i] =value_type(1)/ (value_type(1)+ tmp4 );
                pml_nx_fi2[grid_nx - 2 - i] =value_type(1)/ (value_type(1)+ tmp4 );
                pml_nx_fi3[i] = (value_type(1)- tmp4 ) / (value_type(1)+ tmp4 );
                pml_nx_fi3[grid_nx - 2 - i] = (value_type(1)- tmp4 ) / (value_type(1)+ tmp4 );
            }
            for ( std::size_t j = 0; j <= pml; ++j )
            {
                value_type tmp1  = pml - j;
                value_type tmp2  = pml;
                value_type tmp3 = tmp1 / tmp2; 
                value_type tmp4  =value_type( 0.25 )* pow( tmp3,value_type(3));
                pml_ny_gj1[j] = tmp4; 
                pml_ny_gj1[grid_ny - 1 - j] = tmp4; 
                pml_ny_gj2[j] =value_type(1)/ (value_type(1)+ tmp4 );
                pml_ny_gj2[grid_ny - 1 - j] =value_type(1)/ (value_type(1)+ tmp4 );
                pml_ny_gj3[j] = (value_type(1)- tmp4 ) / (value_type(1)+ tmp4 );
                pml_ny_gj3[grid_ny - j - 1] = (value_type(1)- tmp4 ) / (value_type(1)+ tmp4 );
                tmp3 = ( tmp1 - .5 ) / tmp2;
                tmp4  =value_type( 0.25 )* pow( tmp3,value_type(3));
                pml_ny_fj1[j] = tmp4;
                pml_ny_fj1[grid_ny - 2 - j] = tmp4;
                pml_ny_fj2[j] =value_type(1)/ (value_type(1)+ tmp4 );
                pml_ny_fj2[grid_ny - 2 - j] =value_type(1)/ (value_type(1)+ tmp4 );
                pml_ny_fj3[j] = (value_type(1)- tmp4 ) / (value_type(1)+ tmp4 );
                pml_ny_fj3[grid_ny - 2 - j] = (value_type(1)- tmp4 ) / (value_type(1)+ tmp4 );
            }
        }
        
        matrix_type dz, ez, hx, hy, pec_ga, ihx, ihy;
        vector_type pml_nx_gi1, pml_nx_gi2, pml_nx_gi3, pml_ny_gj1, pml_ny_gj2, pml_ny_gj3;
        vector_type pml_nx_fi1, pml_nx_fi2, pml_nx_fi3, pml_ny_fj1, pml_ny_fj2, pml_ny_fj3;
        vector_type ez_inc, hx_inc;

        std::size_t grid_nx, grid_ny;
        std::size_t grid_nx_left_boundary, grid_nx_right_boundary, grid_nx_center;
        std::size_t grid_ny_upper_boundary, grid_ny_lower_boundary, grid_ny_center;
        std::size_t pml, radius;
        
        value_type ct0, pulse_width;
    
    };

}

#endif//_FDTD_HPP_INCLUDED_SDOIFJ498YUFSDLKJSALFDKJASLKDFJLKSJFDOIUJFSDLJSALFDKJF
