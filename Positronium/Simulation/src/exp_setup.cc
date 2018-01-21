#include <exp_setup.h>
#include <detector.h>

ExperimentalSetup::ExperimentalSetup()
{
  // Here we need to construct our experimental exp_setup
  // need to be refactored in separated derived class maybe+

  arma::vec p1 = {1,0,0};

  CylindricalDetector * d1 = new CylindricalDetector(p1,0.2);

  arma::vec z_axis = {0,0,1};

  CylindricalDetector * d2 = new CylindricalDetector(*d1, 120, z_axis);
  CylindricalDetector * d3 = new CylindricalDetector(*d1, 240, z_axis);

  detector_list.push_back(d1);
  detector_list.push_back(d2);
  detector_list.push_back(d3);

}

std::vector<CylindricalDetector*> ExperimentalSetup::get_detector_list()
{ return detector_list; }
