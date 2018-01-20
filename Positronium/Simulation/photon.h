/*
* Photon Class
*
*/
#ifndef PHOTON_H
#define PHOTON_H

#include "vector_3.h"

class Photon
{
  private:

    VECTOR_3D momentum;
    double total_momentum;
    double energy;

  public:

    // Constructor & Destructor
    Photon();
    Photon(VECTOR_3D, double);

    // Getter Methods

    VECTOR_3D get_momentum();
    double get_totalmomentum();
    double get_energy();

    // Setter Methods

    void set_momentum(VECTOR_3D);
    void set_energy(double);

};

#endif
