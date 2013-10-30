
#ifndef CalorimeterSD_hh_
#define CalorimeterSD_hh_

//*****************************************************************************
//  CalorimeterSD
//                           - 2012-11-23, by Garam HAHN.
//*****************************************************************************

#include "CaloHit.hh"
#include "SensitiveDetector.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class CalorimeterSD : public SensitiveDetector
{
public:
  CalorimeterSD(const G4String iname,
                const G4RotationMatrix& iR,
                const G4ThreeVector& iV,
                const G4int inx, const G4double ixmin, const G4double ixmax,
                const G4int iny, const G4double iymin, const G4double iymax,
                const G4int inz, const G4double izmin, const G4double izmax);
  ~CalorimeterSD();

  void   Initialize(G4HCofThisEvent* ihce);
  G4bool ProcessHits(G4Step* istp, G4TouchableHistory* ith);
  void   EndOfEvent(G4HCofThisEvent* ihce);

private:
  CaloHitsCollection* hits;

  // primary or secondary particle can be selected from here!!
  // by implementing filtering method or variable here!!
};

#endif
