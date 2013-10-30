//*****************************************************************************
//  AnalysisMessenger.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "AnalysisMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//==========================================================================
AnalysisMessenger::AnalysisMessenger(AnalysisManager* iam):
  am(iam)
{
  ana = new G4UIdirectory("/ana/");
  ana->SetGuidance("set parameters of analysis");

  result = new G4UIcmdWithAString("/ana/result", this);
  result->SetGuidance("dir filename");
  result->AvailableForStates(G4State_PreInit);
}

//==========================================================================
AnalysisMessenger::~AnalysisMessenger()
{
  delete result;
  delete ana;
}

//==========================================================================
void AnalysisMessenger::SetNewValue(G4UIcommand* icmd, G4String istr)
{
  if (icmd == result)
  {
    am->SetFilename(istr);
  }
}




