#include <cmath>
#include <armadillo>

#include <photon.h>

Photon::Photon(){}
Photon::Photon(arma::vec p, double e)
{
  energy = e;
  momentum = p;
  total_momentum = norm(p);
}

arma::vec Photon::get_momentum(){ return momentum; }
double Photon::get_totalmomentum(){ return total_momentum; }
double Photon::get_energy(){ return energy; }

void Photon::set_momentum(arma::vec p)
{
  momentum = p;
  total_momentum = norm(p);
  return ;
}

void Photon::set_energy(double e){ energy = e; }
