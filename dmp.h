#ifndef DMP_H
#define DMP_H
#include "basis.h"

class dmp
{
public:
    dmp();
    double sig[10] = {1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9};
    double center[10] = {1,2,3,4,5,6,7,8,9,0};
    std::vector<basis> base;
    void load_basis(std::vector<basis*> base_);
    double nonlinear_term(double x_dot , double x , double g , double y0);

private:
};

#endif // DMP_Hb
