#ifndef SensitiveDetector_hh_
#define SensitiveDetector_hh_

//*****************************************************************************
//  SensitiveDetector
//                           - 2012-11-29, by Garam HAHN.
//*****************************************************************************

#include "TrackHit.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4VSensitiveDetector.hh"

class SensitiveDetector : public G4VSensitiveDetector
{
public:
  SensitiveDetector(const G4String iname,
                    const G4RotationMatrix& iR,
                    const G4ThreeVector& iV);
  ~SensitiveDetector();

protected:
  static G4int         id;
  G4RotationMatrix     R;
  G4ThreeVector        V;
};

#endif
