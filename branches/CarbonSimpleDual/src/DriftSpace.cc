//*****************************************************************************
//  DriftSpace.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "DriftSpace.hh"
#include "TrackerSD.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

//==========================================================================
DriftSpace::DriftSpace(const G4double l)
  :BeamLineComponent(Drift), length(l)
{
}

//==========================================================================
DriftSpace::~DriftSpace()
{
}

//==========================================================================
DriftSpace* DriftSpace::New()
{
  return new DriftSpace(length);
}

//==========================================================================
DriftSpace* DriftSpace::New(const G4double l)
{
  return new DriftSpace(l);
}

//==========================================================================
G4RotationMatrix DriftSpace::GetNextRotationMatrix(const G4RotationMatrix& R0)
{
  return R0;
}

//==========================================================================
G4ThreeVector DriftSpace::GetNextPosition(const G4ThreeVector&    V0,
                                          const G4RotationMatrix& R0)
{
  return V0 + R0 * G4ThreeVector(0., 0., length);
}

//==========================================================================
void DriftSpace::Generate(const G4RotationMatrix&,
                          const G4ThreeVector&,
                                G4LogicalVolume*)
{
  // DO NOTHING HERE!!
}
