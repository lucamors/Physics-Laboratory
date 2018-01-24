#ifndef EXPERIMENTAL_SETUP
#define EXPERIMENTAL_SETUP

#include <vector>
#include <detector.h>
/*
*
* Up to now only support CylindricalDetector implementation
* because is that we have in our real experimental setup
*
*/

class ExperimentalSetup
{

  private:

    std::vector<CylindricalDetector*> detector_list;

  public:

    ExperimentalSetup();

    std::vector<CylindricalDetector*> get_detector_list();

    // Maybe we can add a graphics lib in order to dispay
    // configuration
};

#endif
