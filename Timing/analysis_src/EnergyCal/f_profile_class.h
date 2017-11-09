#include "TH1F.h"

#ifndef F_PROFILE_CLASS_H
#define F_PROFILE_CLASS_H

class f_profile_class
{

  private:

    TH1F * h_smooth;
    Double_t max_calib, max_smooth, t_shift;


  public:

    f_profile_class(TH1F*, Double_t, Double_t, Double_t);

    double operator() (double *x, double *par);

};


#endif
