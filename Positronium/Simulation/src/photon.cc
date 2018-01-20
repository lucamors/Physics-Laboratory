#include <cmath>
#include <photon.h>

Photon::Photon(){}
Photon::Photon(VECTOR_3D p, double e)
{
  energy = e;
  momentum = p;
  total_momentum = sqrt(p.get_x()*p.get_x()+p.get_y()*p.get_y()+p.get_z()*p.get_z());
}

VECTOR_3D Photon::get_momentum(){ return momentum; }
double Photon::get_totalmomentum(){ return total_momentum; }
double Photon::get_energy(){ return energy; }

void Photon::set_momentum(VECTOR_3D p)
{
  momentum = p;
  total_momentum = sqrt(p.get_x()*p.get_x()+p.get_y()*p.get_y()+p.get_z()*p.get_z());

  return ;
}

void Photon::set_energy(double e){ energy = e; }
