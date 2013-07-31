//*****************************************************************************
//  AnalyticDipoleField.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************
#include "globals.hh"
#include "GaramGlobal.hh"
#include "G4ElectroMagneticField.hh"
//#include "G4ThreeVector.hh"
//#include "G4RotationMatrix.hh"

#include "AnalyticDipoleField.hh"


//==========================================================================
AnalyticDipoleField::AnalyticDipoleField(const G4double fldstr,
                                         const G4RotationMatrix& iR,
                                         const G4ThreeVector& iV)
  :G4ElectroMagneticField(), fB(fldstr), R0(iR), V0(iV)
{
}

//==========================================================================
AnalyticDipoleField::AnalyticDipoleField(const G4double w_per_u,    // [KE/u]
                                         const G4double q_over_a,   // [Q/A]
                                         const G4double radii_curv,
                                         const G4RotationMatrix& iR,
                                         const G4ThreeVector&    iV)
  :G4ElectroMagneticField(), R0(iR), V0(iV)
{
  fB = garam::brho(w_per_u, q_over_a) / radii_curv;
}

//==========================================================================
AnalyticDipoleField::~AnalyticDipoleField()
{
}

//==========================================================================
AnalyticDipoleField::AnalyticDipoleField(const AnalyticDipoleField &r)
  :G4ElectroMagneticField(r)
{
  fB = r.fB;
  V0 = r.V0;
  R0 = r.R0;
}

//==========================================================================
AnalyticDipoleField& 
AnalyticDipoleField::operator=(const AnalyticDipoleField &r)
{
  if (&r == this) return *this;
  *this = r;
  return *this;
} // POTENTIAL MEMORY LEAK!!


//==========================================================================
void AnalyticDipoleField::GetFieldValue(const G4double*,
                                              G4double f[]) const
{
  // det[R] = 1. this means that invert is exactly the same to transpose
  // and of course this dose not compute heavy calculation
  const G4ThreeVector B = R0 * G4ThreeVector(0., -fB, 0.);
  f[0] = B.x(); f[1] = B.y(); f[2] = B.z();
  f[3] = 0.;    f[4] = 0.;    f[5] = 0.;
}
