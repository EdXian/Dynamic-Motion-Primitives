#include "basis.h"

basis::basis(std::vector<double> x  , double sig_ , double c_ ,double g_,double y0_)
{
    sig=sig_;
    c=c_;
    xs = x;
    a=1.0;
    b=1.0;
//    g=1.0;
//    y0=0;
    tau=1.0;
    start = y0_;
    end = g_;
}


double basis::psi(double x, double sigma, double center){

    double k = sigma* sigma * 2;
    return exp( -1*(x-center) * (x-center)/k);
}


std::vector<double> basis::get_psi_vector(){
    std::vector<double> data;
    data.clear();
    for(unsigned int i =0 ; i< xs.size();i++ ){
        data.push_back(psi(xs[i] , sig , c));
    }
    return data;
}


double basis::get_weight(){
    Eigen::VectorXd s;
    Eigen::MatrixXd gamma;
    Eigen::VectorXd f_tar;
    s.setZero(xs.size());
    gamma.setZero(xs.size() , xs.size());
    f_tar.setZero(xs.size());
    for(unsigned int i=0;i<xs.size();i++){
        gamma(i,i) = psi( xs[i], sig ,c);
    }
    for(unsigned int i=0;i<xs.size();i++){
        s(i) = xs[i] * (end - start);
    }
    for(unsigned int i=0;i<xs.size();i++){
        f_tar(i) = tau*tau*y_ddot_demo[i] -  a*(b*(end-y_demo[i])-tau*y_dot_demo[i]);
    }

    double num = s.transpose() * gamma  * f_tar;
    double den = s.transpose() * gamma  * s;

    return num / den;
}

void basis::learn_demo(std::vector< double> y, std::vector<double> y_d ,std::vector<double> y_dd){

    y_demo = y;
    y_dot_demo = y_d;
    y_ddot_demo = y_dd;

}



