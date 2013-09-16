//*****************************************************************************
//  CaloHit.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "CaloHit.hh"

class G4VHit;
G4Allocator<CaloHit> CaloHitAllocator;

// Constructors
//=============================================================================
CaloHit::CaloHit()
  : position(0., 0., 0.), e_deposit(0.), dedx(0.), particle_id(0),
    L(0), Z(0), A(0), I(0), iP(0.), PName(0.)
{}

//=============================================================================
CaloHit::~CaloHit() {}

//=============================================================================
CaloHit::CaloHit(const CaloHit& r)
  : G4VHit()
{
  position    = r.position;
  e_deposit   = r.e_deposit;
  dedx        = r.dedx;
  particle_id = r.particle_id;
  L           = r.L;
  Z           = r.Z;
  A           = r.A;
  I           = r.I;
  iP          = r.iP;
  PName       = r.PName;
}

// Operators for stl containers
//=============================================================================
const CaloHit& CaloHit::operator=(const CaloHit& r)
{
  position    = r.position;
  e_deposit   = r.e_deposit;
  dedx        = r.dedx;
  particle_id = r.particle_id;
  L           = r.L;
  Z           = r.Z;
  A           = r.A;
  I           = r.I;
  iP          = r.iP;
  PName       = r.PName; 
  return *this;
}

//=============================================================================
G4int CaloHit::operator==(const CaloHit& r) const
{
  return (this==&r) ? 1 : 0;
}

// Setters
//=============================================================================
void CaloHit::SetPosition(const G4ThreeVector ipos){position = ipos;}
void CaloHit::SetEdeposit(const G4double iedep){e_deposit = iedep;}
void CaloHit::SetDedx(const G4double idedx){dedx = idedx;}
void CaloHit::SetParticleId(const G4int ipid){particle_id = ipid;}
void CaloHit::SetNStrangeQuark(const G4int iL){L = iL;}
void CaloHit::SetAtomicNumber(const G4int iZ){Z = iZ;}
void CaloHit::SetAtomicMass(const G4int iA){A = iA;}
void CaloHit::SetIsomericExtEnergy(const G4int iI){I = iI;}
void CaloHit::SetPrimary(const G4int is_primary){ iP = is_primary;}
void CaloHit::SetParticleName(const G4String PN){ PName = PN;}

// Getters
//=============================================================================
G4ThreeVector CaloHit::GetPosition() const {return position;}
G4double      CaloHit::GetEdeposit() const {return e_deposit;}
G4double      CaloHit::GetDedx() const {return dedx;}
G4int         CaloHit::GetParticleId() const {return particle_id;}
G4int         CaloHit::GetNStrangeQuark() const {return L;}
G4int         CaloHit::GetAtomicNumber() const {return Z;}
G4int         CaloHit::GetAtomicMass() const {return A;}
G4int         CaloHit::GetIsomericExtEnergy() const {return I;}
G4bool        CaloHit::IsPrimary() const {return iP;}
G4String      CaloHit::GetParticleName() const{return PName;}


