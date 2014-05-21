#ifndef CaloHit_hh_
#define CaloHit_hh_

//*****************************************************************************
//  CaloHit
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class CaloHit : public G4VHit
{
public:
  CaloHit();
  CaloHit(const CaloHit&);
  ~CaloHit();

  const CaloHit& operator=(const CaloHit&);
  G4int operator==(const CaloHit&) const;

public:
  void SetPosition(const G4ThreeVector ipos);
  void SetEdeposit(const G4double iedep);
  void SetDedx(const G4double idedx);
  void SetParticleId(const G4int ipid);
  void SetNStrangeQuark(const G4int iL);
  void SetAtomicMass(const G4int iA);
  void SetAtomicNumber(const G4int iZ);
  void SetIsomericExtEnergy(const G4int iI);
  void SetPrimary(const G4int is_primary);
  void SetParticleName(const G4String PN);

  G4ThreeVector GetPosition() const;
  G4double      GetEdeposit() const;
  G4double      GetDedx() const;
  G4int         GetParticleId() const;
  G4int         GetNStrangeQuark() const;
  G4int         GetAtomicMass() const;
  G4int         GetAtomicNumber() const;
  G4int         GetIsomericExtEnergy() const;
  G4bool        IsPrimary() const;
  G4String      GetParticleName() const;

private:
  G4ThreeVector position;
  G4double      e_deposit;
  G4double      dedx;
  G4int         particle_id;
  G4int         L;
  G4int         Z;
  G4int         A;
  G4int         I;
  G4int         iP;
  G4String      PName;
};

typedef G4THitsCollection<CaloHit> CaloHitsCollection;
extern G4Allocator<CaloHit> CaloHitAllocator;
#endif
