//
//  DetectorMessenger.hh
//  projg4khima
//
//  Created by Kim Chang Hyeuk on 13. 10. 11..
//
//

#ifndef DETECTORMESSENGER_HH_
#define DETECTORMESSENGER_HH_

#include "DetectorConstruction.hh"
#include "G4UImessenger.hh"
#include "GaramGlobal.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;
class G4UIcmdWith3VectorAndUnit;

class DetectorMessenger : public G4UImessenger
{
public:
    DetectorMessenger(DetectorConstruction* idc);
    ~DetectorMessenger();
    
private:
    DetectorConstruction*      dc;
    G4UIdirectory*             detDir;
    
    G4UIcmdWithAString*     Monitor_ON;
    G4UIcmdWithAString*     RGF_ON;
    G4UIcmdWithAString*     RSF_ON;
    G4UIcmdWithAString*     MLC_ON;
    //G4UIcmdWithAString*        degraderMaterial;
    //G4UIcmdWithADouble*        deg_Length;
    //G4UIcmdWithADouble*        deg_pressure;
    
public:
    void SetNewValue(G4UIcommand* icmd, G4String istr);
    
};

#endif /* DETECTORCONSTRUCTIONMESSENGER_HH_ */