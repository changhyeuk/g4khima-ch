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
    
public:
    void SetMonitorONOFF(std::string iMonitorONOFF);
    void SetRGFONOFF(std::string iRGFONOFF);
    void SetRSFONOFF(std::string iRSFONOFF);
    void SetMLCONOFF(std::string iMLCONOFF);

private:
  DetectorMessenger* detMsn;
    G4String MonitorONOFF;
    G4String RGFONOFF;
    G4String RSFONOFF;
    G4String MLCONOFF;
};

#endif /* DetectorConstruction_hh_ */
