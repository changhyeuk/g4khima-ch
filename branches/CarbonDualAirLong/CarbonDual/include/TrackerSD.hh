
#ifndef TrackerSD_hh_
#define TrackerSD_hh_

//*****************************************************************************
//  TrackerSD
//                           - 2012-11-23, by Garam HAHN.
//*****************************************************************************

#include "TrackHit.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "SensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;


class TrackerSD : public SensitiveDetector
{
public:
  TrackerSD(const G4String iname,
            const G4RotationMatrix& iR,
            const G4ThreeVector& iV);
  ~TrackerSD();

  void   Initialize(G4HCofThisEvent* ihce);
  G4bool ProcessHits(G4Step* istp, G4TouchableHistory* ith);
  void   EndOfEvent(G4HCofThisEvent* ihce);

private:
  TrackHitsCollection* hits;
};

#endif
