/*
* Photon Class
*
*/
#ifndef PHOTON_H
#define PHOTON_H

#include<armadillo>

class Photon
{
  private:

    arma::vec momentum;
    double total_momentum;
    double energy;

  public:

    // Constructor & Destructor
    Photon();
    Photon(arma::vec, double);
    ~Photon();

    // Getter Methods

    arma::vec get_momentum();
    double get_totalmomentum();
    double get_energy();

    // Setter Methods

    void set_momentum(arma::vec);
    void set_energy(double);

};

#endif
