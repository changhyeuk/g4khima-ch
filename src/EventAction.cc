//*****************************************************************************
//  EventAction.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "EventAction.hh"
#include "AnalysisManager.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4HCtable.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"


//=============================================================================
EventAction::EventAction(){}

//=============================================================================
EventAction::~EventAction(){}

//=============================================================================
void EventAction::BeginOfEventAction(const G4Event* e)
{
  G4cout << "event: " << e->GetEventID() << G4endl;
}

//=============================================================================
void EventAction::EndOfEventAction(const G4Event* e)
{
  // data accumulation
  AnalysisManager* ana = AnalysisManager::GetInstance();
  G4HCofThisEvent* hcte = e->GetHCofThisEvent();
  const G4HCtable* hctable = G4SDManager::GetSDMpointer()->GetHCtable();
  G4String hchead;
  for (int i = 0; i < hctable->entries(); ++i)
  {
    G4String hcname = hctable->GetHCname(i);
    hchead.assign(hcname, 4);
    if (hchead == "Trac")
    {
      //ana->FillTrackTuple(hcname, (TrackHitsCollection*) hcte->GetHC(i));
      ana->FillTrackHisto2D(hcname, (TrackHitsCollection*) hcte->GetHC(i));
        
    }
    else if (hchead == "Calo")
    {
      //ana->FillCaloHisto3D(hcname, (CaloHitsCollection*) hcte->GetHC(i));
      ana->FillCaloHisto1D(hcname, (CaloHitsCollection*) hcte->GetHC(i));
    }
  }
}
