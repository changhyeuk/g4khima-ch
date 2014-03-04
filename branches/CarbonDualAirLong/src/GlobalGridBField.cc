//*****************************************************************************
//  GlobalGridBField.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************
//
// this module is changed to global one, not the local one.

#include "LinInterpolation.hh"
#include "GaramGlobal.hh"

#include "GlobalGridBField.hh"
#include "G4ElectroMagneticField.hh"
#include "FieldTable.hh"

#include <list>
#include <algorithm>

using namespace std;

//=================================================================================
GlobalGridBField::GlobalGridBField()
  :G4ElectroMagneticField()
{
  ft = FieldTable::GetInstance();
}

//=================================================================================
GlobalGridBField::GlobalGridBField(const GlobalGridBField& r)
  :G4ElectroMagneticField(r)
{
  ft = r.ft;
}

//=================================================================================
GlobalGridBField::~GlobalGridBField()
{
}

//=================================================================================
GlobalGridBField& GlobalGridBField::operator = (const GlobalGridBField& p)
{
  if (&p == this) return *this;
  *this = p;
  return *this;
}

//=================================================================================
void GlobalGridBField::GetFieldValue(const G4double x[],
                                           G4double f[] ) const
{
  ft->GetGlobalB(x, f);
  //f[0] = 0.; f[1] = 0.; f[2] = 0.;
  //f[3] = 0.; f[4] = 0.; f[5] = 0.;

}
