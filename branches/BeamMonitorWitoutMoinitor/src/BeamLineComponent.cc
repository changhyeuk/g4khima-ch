//*****************************************************************************
//  BeamLineComponent.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "globals.hh"
#include "CalorimeterSD.hh"
#include "TrackerSD.hh"

#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"
#include "G4Colour.hh"

#include "BeamLineComponent.hh"
// Initializers
//===================================================================================
G4ThreeVector BeamLineComponent::xhat = G4ThreeVector(1., 0., 0.);
G4ThreeVector BeamLineComponent::yhat = G4ThreeVector(0., 1., 0.);
G4ThreeVector BeamLineComponent::zhat = G4ThreeVector(0., 0., 1.);
G4ThreeVector BeamLineComponent::origin = G4ThreeVector(0., 0., 0.);

G4Material*   BeamLineComponent::mVacuum =
  G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic", false);

G4Colour BeamLineComponent::cRed(1., 0., 0.);
G4Colour BeamLineComponent::cOrange(1., .5, 0.);
G4Colour BeamLineComponent::cYellow(1., 1., 0.);
G4Colour BeamLineComponent::cGreen(0., 1., 0.);
G4Colour BeamLineComponent::cBlue(0., 0., 1.);
G4Colour BeamLineComponent::cSkyBlue(.5, .5, 1.);
G4Colour BeamLineComponent::cPurple(1., 0., 1.);

G4double BeamLineComponent::mon_t = 1.0 * mm; // 0.01 * mm;
G4double BeamLineComponent::mon_step = 0.1 * mm;
G4int    BeamLineComponent::mon_id = 0;

//===================================================================================
BeamLineComponent::BeamLineComponent(const BLCType itype)
  :type(itype), field_scale_factor(1.0)
{
}

//===================================================================================
BeamLineComponent::~BeamLineComponent()
{
}

//===================================================================================
void BeamLineComponent::SetFieldScale(const G4double fsf)
{
  field_scale_factor = fsf;
}

//===================================================================================
G4Material* BeamLineComponent::GetG4NistMaterial(const char* nist)
{
  G4Material* r;
  try
  {
    r = G4NistManager::Instance()->FindOrBuildMaterial(nist, false);
  }
  catch (int e)
  {
    r = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic", false);
  }
  return r;
}
