//*****************************************************************************
//  CalorimeterSD.cc
//                           - 2012-11-23, by Garam HAHN.
//*****************************************************************************

#include "CalorimeterSD.hh"
#include "CaloHit.hh"
#include "SensitiveDetector.hh"
#include "AnalysisManager.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"



//=============================================================================
CalorimeterSD::CalorimeterSD(const G4String iname,
                             const G4RotationMatrix& iR,
                             const G4ThreeVector& iV,
                             const G4int inx,
                             const G4double ixmin,
                             const G4double ixmax,
                             const G4int iny,
                             const G4double iymin,
                             const G4double iymax,
                             const G4int inz,
                             const G4double izmin,
                             const G4double izmax)
  :SensitiveDetector(G4String("CalorimeterSD_") + iname, iR, iV)
{
  AnalysisManager* ana = AnalysisManager::GetInstance();
  ana->BookCaloHisto3D(SensitiveDetectorName + "_dose",
                       inx, ixmin, ixmax,
                       iny, iymin, iymax,
                       inz, izmin, izmax);
  ana->BookCaloHisto3D(SensitiveDetectorName + "_letP",
                       inx, ixmin, ixmax,
                       iny, iymin, iymax,
                       inz, izmin, izmax);
  ana->BookCaloHisto3D(SensitiveDetectorName + "_letH",
                       inx, ixmin, ixmax,
                       iny, iymin, iymax,
                       inz, izmin, izmax);
  ana->BookCaloHisto3D(SensitiveDetectorName + "_letA",
                       inx, ixmin, ixmax,
                       iny, iymin, iymax,
                       inz, izmin, izmax);
    
  ana->BookCaloHisto1D(SensitiveDetectorName + "_dose_1D",
                       inz, izmin, izmax);
  ana->BookCaloHisto1D(SensitiveDetectorName + "_letP_1D",
                       inz, izmin, izmax);
  ana->BookCaloHisto1D(SensitiveDetectorName + "_letH_1D",
                       inz, izmin, izmax);
  ana->BookCaloHisto1D(SensitiveDetectorName + "_letA_1D",
                       inz, izmin, izmax);

  ana->BookCaloTuple(SensitiveDetectorName);
    
  collectionName.insert(SensitiveDetectorName);

  // below must be bottom line of this constructor
  G4SDManager::GetSDMpointer()->AddNewDetector(this);
}

//=============================================================================
CalorimeterSD::~CalorimeterSD()
{

}

//=============================================================================
void CalorimeterSD::Initialize(G4HCofThisEvent* ihce)
{
  hits = new CaloHitsCollection(SensitiveDetectorName, collectionName[0]);
  ihce->AddHitsCollection(
    (G4int)G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]),
    hits);
}

//=============================================================================
G4bool CalorimeterSD::ProcessHits(G4Step* istp, G4TouchableHistory*)
{
  G4Track* trk = istp->GetTrack();
  CaloHit* hit = new CaloHit();
  hit->SetPosition(R * (trk->GetPosition() - V));
  hit->SetEdeposit(istp->GetTotalEnergyDeposit());
  hit->SetDedx(istp->GetTotalEnergyDeposit()/istp->GetStepLength() /(keV/um));
  // unit of dedx is keV/um
    
  // Chang Added
  G4ParticleDefinition *particleDef = trk->GetDefinition();
  G4String particleName = particleDef->GetParticleName();
  hit->SetParticleName(particleName);
  G4int isPrimary = istp->GetTrack()->GetParentID();
  hit->SetPrimary(isPrimary);
  G4int PDGencoding = trk->GetDefinition()->GetPDGEncoding();
  hit->SetParticleId(PDGencoding);
  G4int L = 0, A = 0, Z = 0, I = 0, TMP = 0;
  if ((PDGencoding > 100000000) || (PDGencoding < -100000000))
  { // if nuclei
    TMP = PDGencoding - 1000000000;
    L = TMP / 10000000;          // N_STRANGE_QUARK
    TMP = TMP - L * 10000000;
    Z = TMP / 10000;             // Atomic Mass (=N_BARYON)
    TMP = TMP - Z * 10000;
    A = TMP / 10;                // Atomic Number (=N_PROTON)
    I = TMP - A * 10;            // Isomeric Excitation Energy (1-4 Lv)
    hit->SetNStrangeQuark(L);
    hit->SetAtomicNumber(Z);
    hit->SetAtomicMass(A);
    hit->SetIsomericExtEnergy(I);
  }
  hits->insert(hit);
  return true;
}

//=============================================================================
void CalorimeterSD::EndOfEvent(G4HCofThisEvent*)
{
}
