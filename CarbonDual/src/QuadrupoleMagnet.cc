//*****************************************************************************
//  QuadrupoleMagnet.cc
//                           - 2012-10-02, by Garam HAHN.
//*****************************************************************************

#include "QuadrupoleMagnet.hh"
#include "FieldTable.hh"
#include "LinInterpolation.hh"
#include "GridBField.hh"
#include "GaramGlobal.hh"
#include "TrackerSD.hh"

#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4RotationMatrix.hh"
#include "G4VisAttributes.hh"


//===================================================================================
QuadrupoleMagnet::QuadrupoleMagnet(const G4double fgrad,
                                   const G4double l,
                                   const G4double tr,
                                   const G4double br,
                                   const char*    fn)
  :BeamLineComponent(Quadrupole)
{
  if (tr > br)
    G4Exception("QuadrupoleMagnet::Generate",
                "tube_r > body_r",
                FatalErrorInArgument,
                "body radius must larger than tube.");

  field_grad = fgrad;
  eff_length = l;
  tube_r     = tr;
  body_r     = br;
  filename   = fn;
}

//===================================================================================
QuadrupoleMagnet::~QuadrupoleMagnet()
{
}

//===================================================================================
QuadrupoleMagnet* QuadrupoleMagnet::New()
{
  QuadrupoleMagnet* t
    = new QuadrupoleMagnet(field_grad,
                           eff_length,
                           tube_r,
                           body_r,
                           filename);
  t->field_scale_factor = field_scale_factor;
  return t;

}


//===================================================================================
QuadrupoleMagnet* QuadrupoleMagnet::NewScale(const G4double if_scale)
{
  QuadrupoleMagnet* t
    = new QuadrupoleMagnet(field_grad,
                           eff_length,
                           tube_r,
                           body_r,
                           filename);
  t->field_scale_factor = if_scale;
  return t;

}

//===================================================================================
QuadrupoleMagnet* QuadrupoleMagnet::NewGradient(const G4double if_grad)
{
  QuadrupoleMagnet* t
    = new QuadrupoleMagnet(field_grad,
                           eff_length,
                           tube_r,
                           body_r,
                           filename);
  t->field_scale_factor = if_grad / field_grad;
  filename = t->filename;
  return t;
}

//===================================================================================
G4RotationMatrix
QuadrupoleMagnet::GetNextRotationMatrix(const G4RotationMatrix& R0)
{
  return R0;
}

//===================================================================================
G4ThreeVector
QuadrupoleMagnet::GetNextPosition(const G4ThreeVector&    V0,
                                  const G4RotationMatrix& R0 )
{
  return V0 + R0 * (eff_length * zhat);
}

//===================================================================================
void QuadrupoleMagnet::Generate(const G4RotationMatrix& R0,
                                const G4ThreeVector&    V0,
                                      G4LogicalVolume*  mother_lv )
{
  const G4double real_length = eff_length - tube_r;

  const G4ThreeVector VC = V0 + R0 * (0.5 * eff_length * zhat);
  const G4ThreeVector VF = V0 + R0 * (eff_length * zhat);
  
  // Magnetic field define
  //-------------------------------------------------------------------------------
  FieldTable::GetInstance()->RegisterLinIntpolation(filename.c_str());
  FieldTable::GetInstance()->AddFieldEntry(filename.c_str(), VC, R0, field_scale_factor);
  
  // topmost mother volume
  //-------------------------------------------------------------------------------
  // Geometrical Volume Define
  G4Tubs* sv_top = new G4Tubs("sTop",
                              0., 1.05 * body_r,
                              0.5 * real_length,
                              0. * deg, 360. * deg);
  G4LogicalVolume* lv_top 
    = new G4LogicalVolume(sv_top, mother_lv->GetMaterial(), "lv_top");
  
  G4RotationMatrix* rr = new G4RotationMatrix(R0);
  rr->invert();
  new G4PVPlacement(rr,
                    VC,
                    lv_top,
                    "pv_top",
                    mother_lv,
                    false,
                    0);
  
  // body volume
  //-------------------------------------------------------------------------------
  G4Tubs* sv_body = new G4Tubs("sv_body",
                               tube_r, body_r,
                               0.49 * real_length,
                               0. * deg, 360. * deg);
  G4LogicalVolume* lv_body = new G4LogicalVolume(sv_body,
                                                 GetG4NistMaterial("G4_Fe"),
                                                 "lv_body");
  new G4PVPlacement(0,
                    origin,
                    lv_body,
                    "pv_body",
                    lv_top,
                    false,
                    0);
  
  // beam monitors : front and back
  //-------------------------------------------------------------------------------
  G4Tubs* sv_mon = new G4Tubs("sv_mon", 0., tube_r, 0.5 * mon_t, 0., 360.0 * deg);
  /*
  const G4ThreeVector Vmoni = V0 + R0 * (mon_t * zhat);
  G4LogicalVolume* lv_moni 
    = new G4LogicalVolume(sv_mon, 
                          lv_top->GetMaterial(), 
                          "lv_moni", 
                          0,
                          new TrackerSD("Qi", R0, Vmoni));
  lv_moni->SetVisAttributes(new G4VisAttributes(cRed));
  new G4PVPlacement(0,
                    (- 0.5 * real_length + 0.5 * mon_t) * zhat,
                    lv_moni,
                    "pv_moni",
                    lv_top,
                    false,
                    0);
  */
  const G4ThreeVector Vmonf = V0 + R0 * (eff_length * zhat);
  G4LogicalVolume* lv_monf 
    = new G4LogicalVolume(sv_mon, 
                          lv_top->GetMaterial(),
                          "lv_monf",
                          0,
                          new TrackerSD("Q", R0, Vmonf));
  lv_monf->SetVisAttributes(new G4VisAttributes(cRed));
  new G4PVPlacement(0,
                    (0.5 * real_length - 0.5 * mon_t) * zhat,
                    lv_monf,
                    "pv_monf",
                    lv_top,
                    false,
                    0);
}
