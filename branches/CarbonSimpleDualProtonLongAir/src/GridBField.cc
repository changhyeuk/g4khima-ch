//*****************************************************************************
//  GridBField.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************
//
// this module is changed to global one, not the local one.

#include "LinInterpolation.hh"
#include "GaramGlobal.hh"

#include "GridBField.hh"
#include "G4ElectroMagneticField.hh"

#include <list>
#include <algorithm>

using namespace std;

//=================================================================================
GridBField::GridBField(      LinInterpolation* ltp,
                       const double            fscale,
                       const G4RotationMatrix& iR,
                       const G4ThreeVector&    iV)
  :G4ElectroMagneticField(), lintp(ltp), scale_factor(fscale), R0(iR), V0(iV)
{
}

//=================================================================================
GridBField::GridBField(const GridBField& r)
  :G4ElectroMagneticField(r)
{
  lintp = r.lintp;
  scale_factor = r.scale_factor;
  R0 = r.R0;
  V0 = r.V0;
}

//=================================================================================
GridBField::~GridBField()
{
}

//=================================================================================
GridBField& GridBField::operator = (const GridBField& p)
{
  if (&p == this) return *this;
  *this = p;
  return *this;
}

//=================================================================================
void GridBField::GetFieldValue(const G4double x[],
                                     G4double f[] ) const
{
  // det[R] = 1. this means that invert is exactly the same to transpose
  // and of course this dose not compute heavy calculation
  G4RotationMatrix R0i(R0);  R0i.invert();
  const G4ThreeVector X0 = R0i * (G4ThreeVector(x[0], x[1], x[2]) - V0);
  G4double x0arr[3] = {X0.x(), X0.y(), X0.z()};
  G4double B0[3];
  lintp->Get(x0arr, B0);
  const G4ThreeVector B1 = R0 * G4ThreeVector(B0[0], B0[1], B0[2]);
  f[0] = scale_factor * B1.x();
  f[1] = scale_factor * B1.y();
  f[2] = scale_factor * B1.z();
  f[3] = 0.;
  f[4] = 0.;
  f[5] = 0.;
}
