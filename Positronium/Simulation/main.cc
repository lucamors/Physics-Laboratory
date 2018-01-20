#include <iostream>
#include <fstream>
#include <vector>

#include <event.h>
#include <photon.h>

#include "TGraph2D.h"
#include "TROOT.h"

int main(int argc, char const *argv[])
{

  long int n_ev = strtol(argv[1], NULL, 10);

  std::vector<Event*> events_list;

  std::cout << "Testing functionality" << '\n';

  for (size_t i = 0; i < n_ev; i++)
  {
    Event *sample_ev = new Event(i);

    std::cout << "Generating " << i+1 << "event . . ." << '\n';

    events_list.push_back(sample_ev);
  }


  std::ofstream test;
  test.open("decay.txt");

  double xx,yy,zz;

  for (size_t i = 0; i < n_ev; i++)
  {
    std::vector<Photon*> gammas = events_list[i]->get_gamma_configuration();

    // Saving file
    test << gammas[0]->get_momentum().get_x()/gammas[0]->get_totalmomentum() << " "
         << gammas[0]->get_momentum().get_y()/gammas[0]->get_totalmomentum() << " "
         << gammas[0]->get_momentum().get_z()/gammas[0]->get_totalmomentum() << " "
         << std::endl;
    test << gammas[1]->get_momentum().get_x()/gammas[1]->get_totalmomentum() << " "
         << gammas[1]->get_momentum().get_y()/gammas[1]->get_totalmomentum() << " "
         << gammas[1]->get_momentum().get_z()/gammas[1]->get_totalmomentum() << " "
         << std::endl;
    test << gammas[2]->get_momentum().get_x()/gammas[1]->get_totalmomentum() << " "
         << gammas[2]->get_momentum().get_y()/gammas[1]->get_totalmomentum() << " "
         << gammas[2]->get_momentum().get_z()/gammas[1]->get_totalmomentum() << " "
         << std::endl;

  }


  std::cout << "ok!" << '\n';

  return 0;
}
