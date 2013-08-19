//*****************************************************************************
//  PrimaryGeneratorAction.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "BeamLineComponentManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "AnalysisManager.hh"
#include "GaramGlobal.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "globals.hh"

//============================================================================
PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  pgm = new PrimaryGeneratorMessenger(this);
  gun = new G4ParticleGun(1);
}

//============================================================================
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete gun;
  delete pgm;
}

//============================================================================
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  AnalysisManager* ana = AnalysisManager::GetInstance();
    const G4double Z = 6.0;
    const G4double A = 12.0;
  G4ParticleDefinition* pdef =
    G4ParticleTable::GetParticleTable()->FindIon(Z, A, 0, 0); // dummy 0, 0
  gun->SetParticleDefinition(pdef);

  // bullet reload!!
  const BeamInfo& beam = pgm->GetBeamInfo();
  G4double xi, xpi, yi, ypi, wi;
  const G4ThreeVector gunpos = 
    BeamLineComponentManager::GetInitVector();
  const G4RotationMatrix gunrot = 
    BeamLineComponentManager::GetInitRotationMatrix();

  if (pgm->IsPencilBeam())
  {
    if (beam.EsRatio() == 0.0) // reference particle tracking
    {
      wi = beam.MeanEnergy() * A;
      gun->SetParticlePosition(gunpos);
      gun->SetParticleMomentumDirection(gunrot * G4ThreeVector(0., 0., 1.));
      gun->SetParticleEnergy(wi);
      gun->GeneratePrimaryVertex(anEvent);
      ana->FillBullet(0., 0., 0., 0., wi/A);
    }
    else // dispersion particle tracking
    {
      const G4int NDP = 10;
      const G4double fndp = 2./G4double(NDP);
      for (G4int i = 0; i != NDP + 1; ++i)
      {
        wi = beam.MeanEnergy() * A *
             ((1. - beam.EsRatio()) + beam.EsRatio() * fndp * G4double(i));
        gun->SetParticlePosition(gunpos);
        gun->SetParticleMomentumDirection(gunrot * G4ThreeVector(0., 0., 1.));
        gun->SetParticleEnergy(wi);
        gun->GeneratePrimaryVertex(anEvent);
        ana->FillBullet(0., 0., 0., 0., wi/A);
      }
    }
  }
  else
  {
    xi  = G4RandGauss::shoot(0.0, beam.X());
    xpi = G4RandGauss::shoot(beam.Slopx() * xi, beam.XpInt());
    yi  = G4RandGauss::shoot(0.0, beam.Y());
    ypi = G4RandGauss::shoot(beam.Slopy() * yi, beam.YpInt());
    wi  = G4RandGauss::shoot(beam.MeanEnergy() * A,
                             beam.MeanEnergy() * beam.EsRatio() * A);
    gun->SetParticlePosition(gunpos + G4ThreeVector(xi, yi, 0.));
    gun->SetParticleMomentumDirection(gunrot * G4ThreeVector(xpi, ypi, 1.));
    gun->SetParticleEnergy(wi);
    gun->GeneratePrimaryVertex(anEvent);
    ana->FillBullet(xi, xpi, yi, ypi, wi/A);
  }

}





