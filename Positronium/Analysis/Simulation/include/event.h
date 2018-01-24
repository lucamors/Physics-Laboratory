/*
* This class provide a three-body gamma decay generator
* used in order to compute the measurable decay.
*
*/
#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <photon.h>
#include <armadillo>

#include "TH1F.h"

class Event
{
  private:

    unsigned long identifier;

    Photon* gamma1 = new Photon();
    Photon* gamma2 = new Photon();
    Photon* gamma3 = new Photon();

    TH1F * energy_spectrum;

    bool fake_event;

  public:

    Event(unsigned long, bool);
    ~Event();

    void generate_fake(Photon * gamma);
    void generate_gamma(Photon * gamma);
    void generate_third_gamma(Photon * , Photon *, Photon * );
    bool check_physics();

    std::vector<Photon*> get_gamma_configuration();

};

#endif
