//*****************************************************************************
//  PhysicsList.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "PhysicsList.hh"
#include "AnalysisManager.hh"
#include "GaramGlobal.hh"

#include "globals.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4IonQMDPhysics.hh"
#include "G4StepLimiter.hh"
#include "G4RegionStore.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"

//===================================================================================
PhysicsList::PhysicsList()
  :G4VUserPhysicsList()
{
  SetVerboseLevel(0);
  defaultCutValue = 0.01 * mm;
  emPhys             = new G4EmStandardPhysics_option3();
  decayPhys          = new G4DecayPhysics();
  //radioDecayPhys     = new G4RadioactiveDecayPhysics();

  hadronElastPhys    = new G4HadronElasticPhysics();
  hadronInelastPhys  = new G4HadronInelasticQBBC();
  hadronFragPhysJQMD = new G4IonQMDPhysics();

}

//===================================================================================
PhysicsList::~PhysicsList()
{
  delete hadronFragPhysJQMD;
  delete hadronInelastPhys;
  delete hadronElastPhys;

  //delete radioDecayPhys;
  delete decayPhys;
  delete emPhys;
}

//===================================================================================
void PhysicsList::ConstructParticle()
{
  decayPhys->ConstructParticle();
  //radioDecayPhys->ConstructParticle();
}

//===================================================================================
void PhysicsList::ConstructProcess()
{
  AddTransportation();

  decayPhys->ConstructProcess();
  emPhys->ConstructProcess();
  //radioDecayPhys->ConstructProcess();

  hadronElastPhys->ConstructProcess();
  hadronInelastPhys->ConstructProcess();
  hadronFragPhysJQMD->ConstructProcess();

  //SetStepLimiter();
}

//===================================================================================
void PhysicsList::SetStepLimiter()
{
  // Step limitation seen as a process
  G4StepLimiter* stepLimiter = new G4StepLimiter();
  theParticleIterator->reset();
  while ((*theParticleIterator)())
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (stepLimiter->IsApplicable(*particle) && pmanager)
    {
      pmanager->AddDiscreteProcess(stepLimiter);
    }
  }

}

//===================================================================================
void PhysicsList::SetCuts()
{
  SetCutsWithDefault();
  //DumpCutValuesTable();
}

//===================================================================================
void PhysicsList::SetGlobalRangeCut(G4double c)
{
  defaultCutValue = c;
}

