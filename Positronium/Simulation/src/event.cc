#include <random>
#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>
#include <armadillo>
#include <event.h>


#include "TH1F.h"
#include "TFile.h"


const double C = 3E8;

Event::Event(unsigned long id, bool flag)
{

  identifier = id;
  fake_event = flag;
  std::string event_name = "event_"+std::to_string(id);

  // Generating energy gamma energy
  // spectrum from [1] A. Ore and J. L. Powell paper:
  // Three-Photon Annihilation of an Electron-Positron Pair

  int n_bins = 511;
  energy_spectrum = new TH1F(event_name.c_str(), "Energy Spectrum",n_bins,0,511);
  double energy;
  double term1, term2, term3, term4;
  double m = 511.0;
  double total_energy = m;

  for (size_t i = 1; i < n_bins; i++)
  {
    double k = energy_spectrum->GetBinCenter(i);

    term1 = k*(m-k)/pow((2*m-k),2);
    term2 = 2*m*(pow(m-k,2))/(pow(2*m-k,3))*log((m-k)/m);
    term3 = (2*m-k)/k;
    term4 = ((2*m*(m-k))/(k*k))*log((m-k)/m);

    energy_spectrum->SetBinContent(k,2*( term1 - term2 + term3 + term4));

  }

  // Random generator
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 generator (seed);
  std::uniform_real_distribution<double> uniform01(0.0, 1.0);

  double selector = uniform01(generator);

  if(fake_event)
  {
    // Generating event
    generate_fake(gamma1);
    generate_fake(gamma2);
    generate_fake(gamma3);

  }
  else
  {
    // Generating event
    do{

      if(selector > 0 and selector < 0.33)
      {
        generate_gamma(gamma1);
        generate_gamma(gamma2);
        generate_third_gamma(gamma1, gamma2, gamma3 );
      }
      if(selector > 0.33 and selector < 0.66)
      {
        generate_gamma(gamma2);
        generate_gamma(gamma3);
        generate_third_gamma(gamma2, gamma3, gamma1);
      }
      if(selector > 0.66)
      {
        generate_gamma(gamma3);
        generate_gamma(gamma1);
        generate_third_gamma(gamma3, gamma1, gamma2);
      }

    }while(check_physics());

  }

}

Event::~Event(){}

void Event::generate_gamma(Photon * gamma)
{
  /*
  * This function generate a gamma events and
  */

  // Random generator
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 generator (seed);
  std::uniform_real_distribution<double> uniform01(0.0, 1.0);

  double s_energy;
  double theta, phi; // Spherical coordinate
  arma::vec s_momentum(3);

  // Generating the first gamma
  s_energy = energy_spectrum->GetRandom();

  // Generated according to the spherical differential area dA
  theta = 2 * M_PI * uniform01(generator);
  phi = acos(1 - 2 * uniform01(generator));

  // Mapping theta-phi into spherical coordinates
  s_momentum[0] = (s_energy * (1.0/C) * 1000.0 * sin(phi) * cos(theta));
  s_momentum[1] = (s_energy * (1.0/C) * 1000.0 * sin(phi) * sin(theta));
  s_momentum[2] = (s_energy * (1.0/C) * 1000.0 * cos(phi));

  gamma->set_momentum(s_momentum);
  gamma->set_energy(s_energy);

  return ;
}

void Event::generate_third_gamma(Photon * gamma_s1, Photon * gamma_s2, Photon * gamma_s3 )
{

  // Retrieving momentum direction of gamma 1
  double g1_x = gamma_s1->get_momentum()[0];
  double g1_y = gamma_s1->get_momentum()[1];
  double g1_z = gamma_s1->get_momentum()[2];

  // Retrieving momentum direction of gamma 2
  double g2_x = gamma_s2->get_momentum()[0];
  double g2_y = gamma_s2->get_momentum()[1];
  double g2_z = gamma_s2->get_momentum()[2];

  // Imposing the conservation of the momentum
  double g3_x = -g1_x-g2_x;
  double g3_y = -g1_y-g2_y;
  double g3_z = -g1_z-g2_z;

  arma::vec s_momentum(3);

  s_momentum[0] = g3_x;
  s_momentum[1] = g3_y;
  s_momentum[2] = g3_z;

  gamma_s3->set_momentum(s_momentum);

  double s_energy = gamma_s3->get_totalmomentum() * C / 1000.0;

  gamma_s3->set_energy(s_energy);

  return ;
}

bool Event::check_physics()
{
  // Need to be refactor using a BLAS library such as ARMADILLO C++

  double e1 = gamma1->get_energy();
  double e2 = gamma2->get_energy();
  double e3 = gamma3->get_energy();
  double total_ene = e1+e2+e3;

  // std::cout << "total ene ->" << total_ene-1022.0 << '\n';
  // std::cout << "------------------------------" << '\n';

  if(total_ene-1022.0 <= 5 and total_ene-1022.0>-5) return false;

  return true;
}

std::vector<Photon*> Event::get_gamma_configuration()
{
  std::vector<Photon*> decayed_gamma;

  decayed_gamma.push_back(gamma1);
  decayed_gamma.push_back(gamma2);
  decayed_gamma.push_back(gamma3);

  return decayed_gamma;
}

void Event::generate_fake(Photon * gamma)
{
  // Random generator
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 generator (seed);
  std::uniform_real_distribution<double> uniform01(0.0, 1.0);

  double s_energy;
  double theta, phi; // Spherical coordinate
  arma::vec s_momentum(3);

  s_energy = 1; // fake point that lies on S2

  // Generated according to the spherical differential area dA
  theta = 2 * M_PI * uniform01(generator);
  phi = acos(1 - 2 * uniform01(generator));

  // Mapping theta-phi into spherical coordinates
  s_momentum[0] = ( sin(phi) * cos(theta) );
  s_momentum[1] = ( sin(phi) * sin(theta) );
  s_momentum[2] = ( cos(phi) );

  // s_momentum = normalise(s_momentum);

  gamma->set_momentum(s_momentum);
  gamma->set_energy(s_energy);

}
