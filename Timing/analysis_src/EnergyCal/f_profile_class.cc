#include "f_profile_class.h"

f_profile_class::f_profile_class(TH1F * pass_histo, Double_t max_calib_pass, Double_t max_smooth_pass, Double_t t_shift_pass)
{
  max_calib = max_calib_pass;
  max_smooth = max_smooth_pass;
  t_shift = t_shift_pass;
  h_smooth = pass_histo;
}


double f_profile_class::operator()(double *x, double* par)
{
  Double_t	x_effective, value, norm;
  Int_t		bin;
  x_effective = x[0]-t_shift;
  bin = (h_smooth->FindBin(x_effective));
  norm = (max_calib-par[0])/max_smooth;
  value = norm*(h_smooth->GetBinContent(bin))+par[0];

  return value;
}
