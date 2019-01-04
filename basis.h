#ifndef BASIS_H
#define BASIS_H
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>



class basis
{
public:
    basis(std::vector<double> x  , double sig_ , double c_ ,double g,double y0);
    //void cannonical();
    double sig;   //weight
    double c;   //center (Gaussian )

    double a;
    double b;
    double tau;

//    double g;
//    double y0;

    double start ;
    double end;

    std::vector<double> xs;
    std::vector<double> s;

    std::vector<double> y_demo;
    std::vector<double> y_dot_demo;
    std::vector<double> y_ddot_demo;

    double nonlinear_term(double x);
    void learn_demo(std::vector< double> y, std::vector<double> y_d ,std::vector<double> y_dd);
    double get_weight();
    double psi(double  x, double sigma , double center);
    std::vector<double> get_psi_vector();

};

#endif // BASIS_H
