#include <armadillo>
#include <detector.h>
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <ctime>
#include <string>

int main()
{

  arma::vec p1 = {1,0,0};
  Detector * d1 = new Detector(p1, 0.2);

  arma::vec p2 = {0,1.0/sqrt(2),1/sqrt(2)};
  Detector * d2 = new Detector(p2, 0.2);

  std::cout << "half aperture" << d1->get_halfaperture() << '\n';

  // Random generator
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 generator (seed);
  std::uniform_real_distribution<double> uniform01(0.0, 1.0);
  arma::vec s_momentum(3);

  double s_energy;
  double theta, phi; // Spherical coordinate
  int counter = 0;
  int counter2 = 0;

  std::ofstream det("det.txt");
  std::ofstream ndet("ndet.txt");

  std::ofstream det2("det2.txt");
  std::ofstream ndet2("ndet2.txt");

  for (size_t i = 0; i < 10000; i++)
  {
    // Generating momentum vector for the first gamma
    // Generated according to the spherical differential area dA
    theta = 2 * M_PI * uniform01(generator);
    phi = acos(1 - 2 * uniform01(generator));

    // Mapping theta-phi into spherical coordinates
    s_momentum[0] = ( sin(phi) * cos(theta) );
    s_momentum[1] = ( sin(phi) * sin(theta) );
    s_momentum[2] = ( cos(phi) );

    std::string label = "Event " + std::to_string(i) + ": ";
    // check check_detection
    //s_momentum.print(label.c_str());

    if(d1->check_detection(s_momentum))
    {
      counter++;
      det << s_momentum[0] << " "
          << s_momentum[1] << " "
          << s_momentum[2] << std::endl;


    }else{

      ndet << s_momentum[0] << " "
          << s_momentum[1] << " "
          << s_momentum[2] << std::endl;

    }

    if(d2->check_detection(s_momentum))
    {
      counter2++;
      det2 << s_momentum[0] << " "
          << s_momentum[1] << " "
          << s_momentum[2] << std::endl;


    }else{

      ndet2 << s_momentum[0] << " "
          << s_momentum[1] << " "
          << s_momentum[2] << std::endl;

    }

  }

  det.close();
  ndet.close();
  det2.close();
  ndet2.close();

  std::cout << "Number of detected events Det#1 : " << counter << '\n';
  std::cout << "Number of detected events Det#2 : " << counter2 << '\n';

  return 0;
}
