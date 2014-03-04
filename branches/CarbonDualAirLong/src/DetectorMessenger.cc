//
//  DetectorMessenger.cc
//  projg4khima
//
//  Created by Kim Chang Hyeuk on 13. 10. 11..
//
//

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"


//==============================================================================
DetectorMessenger::DetectorMessenger(
                                     DetectorConstruction* idc)
:dc(idc)
{
    // directory definition
    detDir = new G4UIdirectory("/det/");
    detDir->SetGuidance("change geometry information");
    
    //---------------------------------------------------------------------------
    Monitor_ON = new G4UIcmdWithAString("/det/BeamMonitor", this);
    Monitor_ON->SetGuidance("Montor ON or OFF");
    Monitor_ON->SetParameterName("Monitor_ON", false);
    Monitor_ON->AvailableForStates(G4State_PreInit);
    
    //---------------------------------------------------------------------------
    RGF_ON = new G4UIcmdWithAString("/det/RGF", this);
    RGF_ON->SetGuidance("Ridge Filter ON or OFF");
    RGF_ON->SetParameterName("RGF_ON", false);
    RGF_ON->AvailableForStates(G4State_PreInit);
    //---------------------------------------------------------------------------
    RSF_ON = new G4UIcmdWithAString("/det/RSF", this);
    RSF_ON->SetGuidance("Range Shifter ON or OFF");
    RSF_ON->SetParameterName("RSF_ON", false);
    RSF_ON->AvailableForStates(G4State_PreInit);
    //---------------------------------------------------------------------------
    MLC_ON = new G4UIcmdWithAString("/det/MLC", this);
    MLC_ON->SetGuidance("MLC ON or OFF");
    MLC_ON->SetParameterName("MLC_ON", false);
    MLC_ON->AvailableForStates(G4State_PreInit);
    //---------------------------------------------------------------------------
    /*
    deg_Length = new G4UIcmdWithADouble("/det/DegraderLength", this);
    deg_Length->SetGuidance("Gas Degrader Length ");
    deg_Length->SetParameterName("DegLenght", false);
    deg_Length->AvailableForStates(G4State_PreInit);
    
    //---------------------------------------------------------------------------
    
    deg_pressure = new G4UIcmdWithADouble("/det/GasPressure", this);
    deg_pressure->SetGuidance("Gas Degrader Pressure ");
    deg_pressure->SetParameterName("DegPress", false);
    deg_pressure->AvailableForStates(G4State_PreInit);
    */
    
}


//==============================================================================
DetectorMessenger::~DetectorMessenger()
{
    delete detDir;
    delete Monitor_ON;
    delete RGF_ON;
    delete RSF_ON;
    delete MLC_ON;
    
    //delete degraderMaterial;
    //delete deg_Length;
    //delete deg_pressure;
}


//==============================================================================
void DetectorMessenger::SetNewValue(G4UIcommand* icmd, G4String istr)
{
    if (icmd == Monitor_ON)
        dc->SetMonitorONOFF(istr);
    if (icmd == RGF_ON)
        dc->SetRGFONOFF(istr);
    if (icmd == RSF_ON)
        dc->SetRSFONOFF(istr);
    if (icmd == MLC_ON)
        dc->SetMLCONOFF(istr);
        
  /*
    if (icmd == deg_Length)
        dc->SetDegraderLength(deg_Length->GetNewDoubleValue(istr));
    
    if (icmd == deg_pressure)
        dc->SetDegraderPressure(deg_pressure->GetNewDoubleValue(istr));
  */
}
