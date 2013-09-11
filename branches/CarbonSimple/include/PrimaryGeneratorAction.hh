#ifndef PRIMARYGENERATORACTION_HH
#define PRIMARYGENERATORACTION_HH 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"

#include "GaramGlobal.hh"
class PrimaryGeneratorMessenger;
class DetectorConstruction;
class AnalysisManager;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction();

public:
  void GeneratePrimaries(G4Event*);

private:
  G4ParticleGun* gun;
  PrimaryGeneratorMessenger* pgm;

};

#endif // PRIMARYGENERATORACTION_HH

