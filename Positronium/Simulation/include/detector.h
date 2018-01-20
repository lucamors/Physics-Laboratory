#ifndef DETECTOR_H
#define DETECTOR_H

#include <armadillo>

class Detector
{
  private:

    arma::vec position;
    double radius; // restricting to cylindrical detectors
    double half_aperture;  // in radians

  public:

    Detector();
    Detector(arma::vec, double);

    double get_halfaperture();
    bool check_detection(arma::vec );

};

#endif
