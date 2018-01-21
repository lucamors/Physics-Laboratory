#include <detector.h>
#include <armadillo>
#include <fstream>
#include <chrono>
#include <ctime>
#include <random>

CylindricalDetector::CylindricalDetector(){}



CylindricalDetector::CylindricalDetector(arma::vec x, double r)
{
  // Setting position & radius
  position = x;
  radius = r;

  // Finding a generic orthogonal vector
  arma::vec e_orthogonal;

  // Random generator
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 generator (seed);
  std::uniform_real_distribution<double> uniform01(0.0, 1.0);

  // useless control : the propability of generating a random unity vector
  // equal to our detector position is negligible

  do
  {
    e_orthogonal = {uniform01(generator), uniform01(generator), uniform01(generator)};
    e_orthogonal = normalise(e_orthogonal);

  } while( normalise(cross(position, e_orthogonal)) == 1);

  arma::vec e_r = normalise(cross(position,e_orthogonal));

  e_r = radius*e_r;

  arma::vec d1_edges = e_r+position;

  //d1_edges.print("edges:"); debuggin purpouse

  half_aperture = acos((dot(d1_edges,position))/(norm(d1_edges)*norm(position)));

}

double CylindricalDetector::get_halfaperture(){ return half_aperture; }

bool CylindricalDetector::check_detection(arma::vec gamma_direction)
{
  double cos_angle =  (dot(gamma_direction,position))/(norm(gamma_direction)*norm(position));

  if ( cos_angle > cos(half_aperture)) { return true; }

  return false;
}

// Geometrical Methods

CylindricalDetector::XRotate(double theta)
{
  theta = (theta*M_PI)/180.0;

  arma::mat XR = {{1,0,0},{0,cos(theta), -sin(theta)},{0,sin(theta),cos(theta)}};
  position = affmul(XR,position);
}


CylindricalDetector::YRotate(double theta)
{

  theta = (theta*M_PI)/180.0;

  arma::mat YR = {{cos(theta),0,sin(theta)},{0,1,0},{-sin(theta),0,cos(theta)}};
  position = affmul(YR,position);

}

CylindricalDetector::ZRotate(double theta)
{

  theta = (theta*M_PI)/180.0;

  arma::mat ZR = {{cos(theta),-sin(theta),0},{sin(theta),cos(theta),0},{0,0,1}};
  position = affmul(ZR,position);

}
