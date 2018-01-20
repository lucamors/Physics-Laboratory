#ifndef VECTOR_3D_H
#define VECTOR_3D_H

class VECTOR_3D
{
  private:

    double x;
    double y;
    double z;

  public:

    VECTOR_3D();
    VECTOR_3D(double, double, double);
  //  ~VECTOR_3D();

    // Getter Methods

    // Setter Methods

    void set_x(double);
    void set_y(double);
    void set_z(double);

    // Getter Methods

    double get_x();
    double get_y();
    double get_z();

};

#endif
