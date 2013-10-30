//*****************************************************************************
//  AnalyticQuadrupoleField.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "G4ElectroMagneticField.hh"
#include "AnalyticQuadrupoleField.hh"
#include "GaramGlobal.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

//===================================================================================
AnalyticQuadrupoleField::AnalyticQuadrupoleField(const G4double fgrad,
                                                 const G4RotationMatrix& iR,
                                                 const G4ThreeVector&    iV)
  :G4ElectroMagneticField(), fG(fgrad), R0(iR), V0(iV)
{
}

// copy constructors
//===================================================================================
AnalyticQuadrupoleField::AnalyticQuadrupoleField(const AnalyticQuadrupoleField& r)
  :G4ElectroMagneticField(r)
{
  fG = r.fG;
  R0 = r.R0;
  V0 = r.V0;
}


//===================================================================================
AnalyticQuadrupoleField::~AnalyticQuadrupoleField()
{
}


//===================================================================================
AnalyticQuadrupoleField&
AnalyticQuadrupoleField::operator = (const AnalyticQuadrupoleField &r)
{
  if (&r == this) return *this;
  *this = r;
  return *this;
}


//===================================================================================
void AnalyticQuadrupoleField::GetFieldValue(const G4double x[],
                                                  G4double f[]) const
{
  // det[R] = 1. this means that invert is exactly the same to transpose
  // and of course this dose not compute heavy calculation
  G4RotationMatrix R0i(R0);  R0i.invert();
  const G4ThreeVector X0 = R0i * (G4ThreeVector(x[0], x[1], x[2]) - V0);
  const G4ThreeVector B = R0 * G4ThreeVector(fG * X0.y(), fG * X0.x(), 0.);
  f[0] = B.x();  f[1] = B.y();  f[2] = B.z();
  f[3] = 0.;     f[4] = 0.;     f[5] = 0.;
}
