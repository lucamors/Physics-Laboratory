#include "opt_par.h"


Opt_params::Opt_params(ULong64_t n )
{
  TimeA = new Double_t[N];
  TimeB = new Double_t[N];
  Energy = new Double_t[N];
}

// Opt_params::~Opt_params()
// {
//   // delete TimeA;
//   // delete TimeB;
//   // delete Energy;
// }


ULong64_t * Opt_params::get_id(){ return &id_ev; }
Double_t * Opt_params::get_timeA(){ return TimeA; }
Double_t * Opt_params::get_timeB(){ return TimeB; }
Double_t * Opt_params::get_energy(){ return Energy; }

void Opt_params::set_id(ULong64_t id){ id_ev = id; }
