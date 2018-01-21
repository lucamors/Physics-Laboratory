#ifndef DETECTOR_H
#define DETECTOR_H
#include <armadillo>

/*
*
* This class implement a simple cylindrical detector shape
*
*/

class CylindricalDetector
{
  private:

    arma::vec position;
    double radius; // restricting to cylindrical detectors
    double half_aperture;  // in radians

  public:

    CylindricalDetector(); // to be deleted
    CylindricalDetector(const &CylindricalDetector, double, arma::vec); // angle in degrees
    CylindricalDetector(arma::vec, double);

    // Getter Methods

    double get_halfaperture();

    // Setter Methods


    // Geometrical Methods

    void XRotate(double); // angle in degrees
    void YRotate(double); // angle in degrees
    void ZRotate(double); // angle in degrees

    bool check_detection(arma::vec );

};

#endif
