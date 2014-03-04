#ifndef LinInterpolation_HH
#define LinInterpolation_HH

#include <vector>
#include <string>
using namespace std;

#include "G4ThreeVector.hh"

class LinInterpolation
{
public:
  LinInterpolation(const char*  filename,
                   const int    line_skip_first,
                   const int    line_read   = 0,
                   const double length_unit = cm,
                   const double field_unit  = gauss);
  LinInterpolation(const LinInterpolation& r);
  ~LinInterpolation();

  LinInterpolation& operator=(const LinInterpolation& r);

  bool Get(const double x[], double B[]) const;
  bool IsNamed(const char* fname) const;

private:
  char   filename[100];
  double dx, dy, dz;
  double *x, *y, *z;
  int    NX, NY, NZ;
  double ***Bx, ***By, ***Bz;
  double xmin, xmax, ymin, ymax, zmin, zmax;
};

#endif
