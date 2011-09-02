#include <fdtd.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace feng
{
    template<typename T1, typename T2>
    T1 lexical_cast( const T2& t2 )
    {
        std::stringstream ss;
        T1 ans;
        ss << t2;
        ss >> ans;
        return ans;
    }
}

using namespace feng;
using namespace std;

int main()
{
    //mesh size
    const std::size_t grid_nx = 100;
    const std::size_t grid_ny = 100;
    //boundary size
    const std::size_t boundary_size = 10;
    //pml size
    const std::size_t pml_size = 5;
    //cylinder radius size
    const std::size_t cylinder_radius_grid = 10;
    //time delay
    const double ct0 = 30;
    //pulse width
    const double pulse_width = 15;
    //fdtd loops
    const std::size_t n_steps = 250;

    fdtd<double> f( grid_nx, grid_ny, boundary_size, pml_size, cylinder_radius_grid, ct0, pulse_width );
    f.initialize();

    ofstream ofs_illuminated( "illuminated.dat" );
    ofstream ofs_shadowed( "shadowed.dat" );

    //start FDTD loop
    for ( std::size_t i = 0; i < n_steps; ++i )
    {
        //execute FDTD
        f();
        //record Ez
        if ( i % 5 == 0 )
        {
            const string file_name = string("Ez_") + lexical_cast<string, std::size_t>(i);
            ofstream ofs( file_name.c_str() );
            ofs << f.ez;
            ofs.close();
        }
        //record illuminated site U
        ofs_illuminated << f.ez[grid_nx/2-1][grid_ny/2-cylinder_radius_grid-2] << "\n";
        //record shadowed site U
        ofs_shadowed << f.ez[grid_nx/2-1][grid_ny/2+cylinder_radius_grid] << "\n";
    }
    ofs_illuminated.close();
    ofs_shadowed.close();

    return 0;
}
