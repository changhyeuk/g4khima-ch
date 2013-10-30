#ifndef TrackHit_hh_
#define TrackHit_hh_

//*****************************************************************************
//  TrackHit
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"


class TrackHit : public G4VHit
{
public:
  TrackHit();
  TrackHit(const TrackHit& ith);
  ~TrackHit();

  const TrackHit& operator=(const TrackHit& right);
  G4int operator==(const TrackHit& right) const;

public:
  void SetPosition(const G4ThreeVector& ipos);
  void SetMomentum(const G4ThreeVector& imom);
  void SetKineticEnergy(const G4double ike);
  void SetParticleId(const G4int ipid);
  void SetTrackId(const G4int itid);
  void SetNStrangeQuark(const G4int iL);
  void SetAtomicNumber(const G4int iZ);
  void SetAtomicMass(const G4int iA);
  void SetIsomericExtEnergy(const G4int iI);
  void SetPrimary();

  G4ThreeVector GetPosition() const;
  G4ThreeVector GetMomentum() const;
  G4double      GetKineticEnergy() const;
  G4int         GetParticleId() const;
  G4int         GetTrackId() const;
  G4int         GetNStrangeQuark() const;
  G4int         GetAtomicMass() const;
  G4int         GetAtomicNumber() const;
  G4int         GetIsomericExtEnergy() const;
  G4bool        IsPrimary() const;

private:
  G4ThreeVector position;
  G4ThreeVector momentum;
  G4double      kinetic_energy;
  G4int         particle_id;
  G4int         track_id;
  G4int         L;
  G4int         Z;
  G4int         A;
  G4int         I;
  G4bool        is_primary;

};

typedef G4THitsCollection<TrackHit> TrackHitsCollection;
extern G4Allocator<TrackHit> TrackHitAllocator;

#endif
