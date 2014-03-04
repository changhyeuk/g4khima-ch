//*****************************************************************************
//  g4khima.cc
//                           - 2012-12-03, by Garam HAHN.
//*****************************************************************************
#include <iostream>

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "AnalysisManager.hh"
#include "EventAction.hh"
#include "StepAction.hh"
#include "FieldTable.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4UIterminal.hh"


int main(int argc,char** argv)
{
  // dummy command for the clean code.
  AnalysisManager::GetInstance(); // singleton initializer

  // Geometry and physics definition
  G4RunManager * rman = new G4RunManager();
  rman->SetUserInitialization(new DetectorConstruction);
  rman->SetUserInitialization(new PhysicsList);

  // Actions definition
  rman->SetUserAction(new PrimaryGeneratorAction);
  rman->SetUserAction(new EventAction);
  rman->SetUserAction(new StepAction);

  // Visualization
  G4VisManager* vman = new G4VisExecutive();
  vman->Initialize();

  // Macro batch mode
  G4UImanager * uim = G4UImanager::GetUIpointer();
  try
  {
    G4String cmd = "/control/execute ";
    if (argc == 1) cmd.append("macro/vis.mac");
    else cmd.append(argv[1]);
    uim->ApplyCommand(cmd);
  }
  catch (int i)
  {
    std::cout << "macro file name error" << std::endl;
  }
  
  AnalysisManager::Dispose();
  delete vman;
  delete rman;

  return 0;
 }


