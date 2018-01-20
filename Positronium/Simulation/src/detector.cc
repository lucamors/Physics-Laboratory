#include <detector.h>
#include <armadillo>
#include <fstream>

Detector::Detector(){}
Detector::Detector(arma::vec x, double r)
{
  position = x;
  radius = r;

  arma::vec e2 = {0.0,1.0,0.0};

  arma::vec e_r = normalise(cross(position,e2));

  e_r = radius*e_r;

  arma::vec d1_edges = e_r+position;

  d1_edges.print("edges:");
  half_aperture = acos((dot(d1_edges,position))/(norm(d1_edges)*norm(position)));

  std::ofstream outfile("prova.txt");

  outfile << position[0] << " " << position[1] << " " << position[2] << std::endl;
  outfile << d1_edges[0] << " " << d1_edges[1] << " " << d1_edges[2] << std::endl;

}

double Detector::get_halfaperture(){ return half_aperture; }

bool Detector::check_detection(arma::vec gamma_direction)
{
  double angle =  (dot(gamma_direction,position))/(norm(gamma_direction)*norm(position));

  if ( angle > cos(half_aperture)) { return true; }
  
  return false;
}
