#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"

class G4EmStandardPhysics_option3;
class G4DecayPhysics;
class G4RadioactiveDecayPhysics;
class G4HadronElasticPhysics;
class G4HadronInelasticQBBC;
class G4IonBinaryCascadePhysics;
class G4IonQMDPhysics;

class AnalysisManager;

class PhysicsList: public G4VUserPhysicsList
{
public:
  PhysicsList();
  ~PhysicsList();
  void SetFragmentModelBC();
  void SetFragmentModelJQMD();
  void SetGlobalRangeCut(G4double c);

protected:
  G4EmStandardPhysics_option3* emPhys;            //E.M.
  G4DecayPhysics*              decayPhys;         //Decay
  G4RadioactiveDecayPhysics*   radioDecayPhys;    //Radioactive Decay
  G4HadronElasticPhysics*      hadronElastPhys;   //Hadronic
  G4HadronInelasticQBBC*       hadronInelastPhys;
  G4IonBinaryCascadePhysics*   hadronFragPhysBC;
  G4IonQMDPhysics*             hadronFragPhysJQMD;

  // Construct particle and physics
  void ConstructParticle();
  void ConstructProcess();
  void SetCuts();
  
protected:
  void SetStepLimiter();

};


#endif

