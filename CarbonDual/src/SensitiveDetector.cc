//*****************************************************************************
//  SensitiveDetector.cc
//                           - 2012-11-29, by Garam HAHN.
//*****************************************************************************

#include "SensitiveDetector.hh"
#include "GaramGlobal.hh"

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"

#include "G4SDManager.hh"
#include "G4ios.hh"


G4int SensitiveDetector::id = 0;

//=============================================================================
SensitiveDetector::SensitiveDetector(const G4String iname,
                                     const G4RotationMatrix& iR,
                                     const G4ThreeVector& iV)
  :G4VSensitiveDetector(iname + "_" + garam::to_str(++id))
{
  // invert() of hep rotation matrix class just transpose matrix!
  // because of Det[R] == 1, dont worry about cpu resource.
  R = iR;
  R.invert();
  V = iV;
}

//=============================================================================
SensitiveDetector::~SensitiveDetector()
{
}
