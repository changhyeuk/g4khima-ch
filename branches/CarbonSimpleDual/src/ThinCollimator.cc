//*****************************************************************************
//  ThinCollimator.cc
//                           - 2012-12-07, by Garam HAHN.
//*****************************************************************************

#include "ThinCollimator.hh"
#include "TrackerSD.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"



//=============================================================================
ThinCollimator::ThinCollimator(const G4double ihalf_gap,
                               const G4double ithickness,
                               const G4double ix_size,
                               const G4double iy_size,
                               const G4double irot_angle,
                               const G4String inist_name)
  :BeamLineComponent(ThinCol),
   half_gap(ihalf_gap), 
   z_length(ithickness), x_length(ix_size), y_length(iy_size),
   rot_angle(irot_angle)
{
  try
  {
    col_mat =
      G4NistManager::Instance()->FindOrBuildMaterial(inist_name, false);
  }
  catch (int e)
  {
    col_mat =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_Ta", false);
  }
}

//=============================================================================
ThinCollimator::~ThinCollimator()
{

}

//===================================================================================
ThinCollimator*
ThinCollimator::NewGap(const G4double ihalf_gap, const G4double irot_angle)
{
  ThinCollimator* t = new ThinCollimator(ihalf_gap, 
                                         z_length, 
                                         x_length, 
                                         y_length, 
                                         irot_angle);
  t->SetMaterial(col_mat);
  return t;
}

//===================================================================================
G4RotationMatrix
ThinCollimator::GetNextRotationMatrix(const G4RotationMatrix& R0)
{
  return R0;
}

//===================================================================================
G4ThreeVector
ThinCollimator::GetNextPosition(const G4ThreeVector& V0,
                                const G4RotationMatrix&)
{
  return V0;
}

//===================================================================================
void ThinCollimator::Generate(const G4RotationMatrix& R0,
                              const G4ThreeVector&    V0,
                                    G4LogicalVolume*  mother_lv)
{
  // Top volume
  G4RotationMatrix* rr = new G4RotationMatrix(R0);
  rr->rotate(rot_angle, R0 * zhat);
  rr->invert();
  G4Box* tsv = new G4Box("tsv", 
                         half_gap + x_length,
                         0.5 * y_length,
                         0.5 * z_length);
  G4LogicalVolume* tlv = new G4LogicalVolume(tsv, mother_lv->GetMaterial(), "tlv");
  tlv->SetVisAttributes(G4VisAttributes::Invisible);
  new G4PVPlacement(rr,
                    V0,
                    tlv,
                    "tpv",
                    mother_lv,
                    false,
                    0);

  // Blocks
  G4Box* csv = new G4Box("csv", 0.5 * x_length, 0.5 * y_length, 0.5 * z_length);
  G4LogicalVolume* clv = new G4LogicalVolume(csv, col_mat, "clv_l");
  clv->SetVisAttributes(new G4VisAttributes(cYellow));
  G4ThreeVector dx = (0.5 * x_length + half_gap) * xhat;
  new G4PVPlacement(0, -dx, clv, "cpv_l", tlv, false, 0);
  new G4PVPlacement(0,  dx, clv, "cpv_r", tlv, false, 1);
  // Monitor
  G4Box* sv_mon = new G4Box("sv_mon", half_gap, 0.5 * y_length, 0.5 * mon_t);
  G4LogicalVolume* lv_mon
    = new G4LogicalVolume(sv_mon,
                          mother_lv->GetMaterial(),
                          "lv_mon",
                          0,
                          new TrackerSD("C", R0, V0));
  lv_mon->SetVisAttributes(new G4VisAttributes(cRed));
  new G4PVPlacement(0,
                    0.5 * (z_length - mon_t) * zhat,
                    lv_mon,
                    "pv_mon",
                    tlv,
                    false,
                    0);

}

