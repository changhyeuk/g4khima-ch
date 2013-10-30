//*****************************************************************************
//  TrackHit.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "TrackHit.hh"

class G4VHit;
G4Allocator<TrackHit> TrackHitAllocator;

// Constructors
//=============================================================================
TrackHit::TrackHit()
  : position(0., 0., 0.), momentum(0., 0., 0.), kinetic_energy(0.),
    particle_id(0), track_id(0),
    L(0), Z(0), A(0), I(0), is_primary(false)
{}

//=============================================================================
TrackHit::~TrackHit() {}

//=============================================================================
TrackHit::TrackHit(const TrackHit& r)
  : G4VHit()
{
  position       = r.position;
  momentum       = r.momentum;
  kinetic_energy = r.kinetic_energy;
  particle_id    = r.particle_id;
  track_id       = r.track_id;
  L           = r.L;
  Z           = r.Z;
  A           = r.A;
  I           = r.I;
  is_primary  = r.is_primary;
}

// Operators for stl containers
//=============================================================================
const TrackHit& TrackHit::operator=(const TrackHit& r)
{
  position       = r.position;
  momentum       = r.momentum;
  kinetic_energy = r.kinetic_energy;
  particle_id    = r.particle_id;
  track_id       = r.track_id;
  L           = r.L;
  Z           = r.Z;
  A           = r.A;
  I           = r.I;
  is_primary  = r.is_primary;
  return *this;
}

//=============================================================================
G4int TrackHit::operator==(const TrackHit& r) const
{
  return (this==&r) ? 1 : 0;
}

// Setters
//=============================================================================
void TrackHit::SetPosition(const G4ThreeVector& ipos){position = ipos;}
void TrackHit::SetMomentum(const G4ThreeVector& imom){momentum = imom;}
void TrackHit::SetKineticEnergy(const G4double ike){kinetic_energy = ike;}
void TrackHit::SetParticleId(const G4int ipid){particle_id = ipid;}
void TrackHit::SetTrackId(const G4int itid){track_id = itid;}
void TrackHit::SetNStrangeQuark(const G4int iL){L = iL;}
void TrackHit::SetAtomicMass(const G4int iA){A = iA;}
void TrackHit::SetAtomicNumber(const G4int iZ){Z = iZ;}
void TrackHit::SetIsomericExtEnergy(const G4int iI){I = iI;}
void TrackHit::SetPrimary(){is_primary = true;}

// Getters
//=============================================================================
G4ThreeVector TrackHit::GetPosition() const {return position;}
G4ThreeVector TrackHit::GetMomentum() const {return momentum;}
G4double      TrackHit::GetKineticEnergy() const {return kinetic_energy;}
G4int         TrackHit::GetParticleId() const {return particle_id;}
G4int         TrackHit::GetTrackId() const {return track_id;}
G4int         TrackHit::GetNStrangeQuark() const {return L;}
G4int         TrackHit::GetAtomicMass() const {return A;}
G4int         TrackHit::GetAtomicNumber() const {return Z;}
G4int         TrackHit::GetIsomericExtEnergy() const {return I;}
G4bool        TrackHit::IsPrimary() const {return is_primary;}
