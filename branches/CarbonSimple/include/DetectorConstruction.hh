#ifndef DetectorConstruction_hh_
#define DetectorConstruction_hh_

//*****************************************************************************
//  DetectorConstruction.hh
//                           - 2012-11-29, by Garam HAHN.
//*****************************************************************************

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  ~DetectorConstruction();
  
  G4VPhysicalVolume* Construct();

private:
  DetectorMessenger* dm;
};

#endif /* DetectorConstruction_hh_ */
