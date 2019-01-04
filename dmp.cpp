#include "dmp.h"

dmp::dmp()
{

}


void dmp::load_basis(std::vector<basis*> base_){
 //base = base_;
    base.clear();
    for(int i=0 ; i<base_.size() ; i++)
    {
        basis  *b= base_[i];
        base.push_back(*b);
    }
//    for(int i=0 ; i<base_.size() ; i++){
//        std::cout <<base[i].sig << "    "<< base[i].c << std::endl;
//    }

}


double dmp::nonlinear_term(double x_dot , double x , double g , double y0){
    double sum_num = 0.0 ;
    double sum_den = 0.0 ;
    double output =  0.0 ;
    for(unsigned int i=0; i< base.size();i++){

        double weight =  base[i].get_weight() ;
        double num = base[i].psi(x , base[i].sig , base[i].c) * weight ;
        double den = base[i].psi(x , base[i].sig , base[i].c)  ;
        sum_num += num;
        sum_den += den;

    }
    output =  (sum_num /sum_den )* x *(g-y0);
return output;
}

