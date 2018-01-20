#include "vector_3.h"

VECTOR_3D::VECTOR_3D(){}

VECTOR_3D::VECTOR_3D(double s_x, double s_y, double s_z)
{
  x = s_x;
  y = s_y;
  z = s_z;
}


void VECTOR_3D::set_x(double s_x){ x = s_x; }
void VECTOR_3D::set_y(double s_y){ y = s_y; }
void VECTOR_3D::set_z(double s_z){ z = s_z; }

double VECTOR_3D::get_x(){ return x; }
double VECTOR_3D::get_y(){ return y; }
double VECTOR_3D::get_z(){ return z; }
