#include "canonical.h"

canonical::canonical(double t_interval , double d_t , double a_x)
{


    ts.clear();
    xs.clear();
    length = t_interval / d_t;
    ax = a_x ;
    dt = d_t ;
    ts.resize(length);
    xs.resize(length);
    ts[0] = 0.0;
    xs[0] = 1.0;
    x_dot = -1.0 * ax;
}


std::vector<double> canonical::get_evolution(){
   // std::vector<double> data;
    for(unsigned int i=1 ; i<length+1 ; i++){
        ts[i] = i * dt;
        xs[i] = xs[i-1] + x_dot * dt;
        x_dot = -ax * xs[i-1];
    }
    return this->xs;
}
