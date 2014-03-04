#ifndef GaramGlobal_h
#define GaramGlobal_h 1

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>

//#include "globals.hh"
//#include "CLHEP/Units/GlobalPhysicalConstants.h"

using namespace std;

namespace garam {
  const double mc2 = 931.494028;
  const double c = 299.792458;
  const double m = 0.0103642688;

  // string splitters
  vector<string>& ssplit(const string& s,
                         char delim,
                         vector<string>& elems);
  vector<string> ssplit(const string &s, char delim = ' ');
  vector<double> ssplit_to_doubles(const string &s, char delim = ' ');
  vector<int> ssplit_to_ints(const string &s, char delim = ' ');

  int    to_int(const string s);
  double to_double(const string s);

  template <typename t> string to_str(const t number)
  {
    stringstream ss;
    ss << number;
    return ss.str();
  }

  // only for comparable variable
  template <typename t> inline t max(const t L, const t R)
  {  return (L < R) ? R : L;  }

  // only for comparable variable
  template <typename t> inline t min(const t L, const t R)
  {  return (L > R) ? R : L;  }

  // only for comparable variable
  template <typename t> inline t abs_max(const t a, const t b)
  { return (((a > 0.)? a : -a) > ((b > 0.)? b : -b))? a : b; }

  template <typename t> inline t abs(const t a)
  // only for double and float
  { return (a > 0.)? a : -a; }

  // only for double and float
  template <typename t> inline t sign(const t a)
  { return (a < 0.)? -1. : 1.; }

  const string timeformat();
  
  const char* mkdir_p(const char* dirname);

  inline double gamma(const double w0)
  {
    // w0: kinetic energy [MeV/u]
    // return : ratio [no unit]
    return (1.0 + abs(w0)/mc2);
  }

  inline double beta(const double w0)
  {
    // w0: kinetic energy [MeV/u]
    // return : ratio [no unit]
    const double g = gamma(w0);
    return sqrt(1.0 - 1.0/(g*g));
  }

  inline double beta_gamma(const double w0)
  {
    // w0: kinetic energy [MeV/u]
    // return : ratio [no unit]
    const double g = gamma(w0);
    return sqrt(g*g - 1.0);
  }

  inline double brho(const double w0, const double q_per_a)
  {
    // w0: kinetic energy [MeV/u]
    // q_per_a : q/a [no unit], for example 1/3, 1/2, 1 etc...
    // return : magnetic_field * length = [T][E][Q^-1][L^-1], clhep unit system
    return beta_gamma(w0) * m * c / q_per_a;
  }

  inline double dpop(const double w0, const double dwow0)
  {
    // w0: kinetic energy [MeV/u]
    // dwow0: kinetic energy ratio |w - w0| / w0 [arb. unit]
    // return : ratio [no unit]
    const double g = gamma(w0);
    return dwow0 * g / (g + 1.0);
  }

  inline double dwow(const double w0, const double dpop0)
  {
    // w0: kinetic energy [MeV/u]
    // dpop0: kinetic energy ratio |p - p0| / p0 [arb. unit]
    // return : ratio [no unit]
    const double g = gamma(w0);
    return dpop0 * (g + 1.0) / g;
  }
}

#endif
