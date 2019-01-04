#ifndef CANONICAL_H
#define CANONICAL_H

#include <stdio.h>
#include <vector>
#include <iostream>

class canonical
{
public:
    canonical(double t_interval , double dt , double ax);
    std::vector<double> xs;
    std::vector<double> ts;
    unsigned int length;
    double x_dot;
    std::vector<double> get_evolution(void);
    double ax;
    double dt;
};

#endif // CANONICAL_H
