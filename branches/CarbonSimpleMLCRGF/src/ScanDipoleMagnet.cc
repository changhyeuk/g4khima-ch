//*****************************************************************************
//  ScanDipoleMagnet.cc
//                           - 2012-12-07, by Garam HAHN.
//*****************************************************************************

#include "ScanDipoleMagnet.hh"
#include "AnalysisManager.hh"
#include "FieldTable.hh"
#include "LinInterpolation.hh"
#include "GridBField.hh"
#include "GaramGlobal.hh"
#include "AnalyticDipoleField.hh"
#include "TrackerSD.hh"

#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4NistManager.hh"
#include "G4FieldManager.hh"
#include "G4ElectroMagneticField.hh"
#include "G4ChordFinder.hh"
#include "G4ClassicalRK4.hh"
#include "G4EqMagElectricField.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Box.hh"
#include "G4UserLimits.hh"


//=============================================================================
ScanDipoleMagnet::ScanDipoleMagnet(const G4double tfh,
                                   const G4double tfw,
                                   const G4double bfh,
                                   const G4double bfw,
                                   const G4double el,
                                   const G4double fs,
                                   const G4double fem,
                                   const char*    bm)
  :BeamLineComponent(ScanDipole),
   tube_full_height(tfh), tube_full_width(tfw),
   body_full_height(bfh), body_full_width(bfw), eff_length(el),
   field_strength(fs), field_exp_scale(fem), 
   body_mat(GetG4NistMaterial(bm))
{
}

//=============================================================================
ScanDipoleMagnet::ScanDipoleMagnet(const G4double tfh,
                                   const G4double tfw,
                                   const G4double bfh,
                                   const G4double bfw,
                                   const G4double el,
                                   const G4double fs,
                                   const G4double fem,
                                         G4Material* bm)
  :BeamLineComponent(ScanDipole),
   tube_full_height(tfh), tube_full_width(tfw),
   body_full_height(bfh), body_full_width(bfw), eff_length(el),
   field_strength(fs), field_exp_scale(fem), 
   body_mat(bm)
{

}

//=============================================================================
ScanDipoleMagnet::~ScanDipoleMagnet()
{

}

//=============================================================================
ScanDipoleMagnet*
ScanDipoleMagnet::New()
{
  ScanDipoleMagnet* t
    = new ScanDipoleMagnet(tube_full_height,
                           tube_full_width,
                           body_full_height,
                           body_full_width,
                           eff_length,
                           field_strength,
                           field_exp_scale,
                           body_mat);
  t->field_scale_factor = field_scale_factor;
  filename = t->filename;
  return t;
}

//=============================================================================
ScanDipoleMagnet*
ScanDipoleMagnet::New(const G4double B)
{
  return new ScanDipoleMagnet(tube_full_height,
                              tube_full_width,
                              body_full_height,
                              body_full_width,
                              eff_length,
                              B,
                              field_exp_scale,
                              body_mat);
}

//=================================================================================
ScanDipoleMagnet*
ScanDipoleMagnet::New(const G4double field_scale,
                      const char*    fname)
{
  ScanDipoleMagnet* t
    = new ScanDipoleMagnet(tube_full_height,
                           tube_full_width,
                           body_full_height,
                           body_full_width,
                           eff_length,
                           field_strength,
                           field_exp_scale,
                           body_mat);
  t->field_scale_factor = field_scale;
  filename = t->filename;
  return t;
}


//=================================================================================
G4RotationMatrix
ScanDipoleMagnet::GetNextRotationMatrix(const G4RotationMatrix& R0)
{
  return R0;
}

//=================================================================================
G4ThreeVector
ScanDipoleMagnet::GetNextPosition(const G4ThreeVector&    V0,
                                  const G4RotationMatrix& R0)
{
  return V0 + R0 * (eff_length * zhat);
}

//=================================================================================
void ScanDipoleMagnet::Generate( const G4RotationMatrix& R0,
                                 const G4ThreeVector&    V0,
                                       G4LogicalVolume*  mother_lv)
{
  if (field_exp_scale < 1.) field_exp_scale = 1.;

  const G4ThreeVector VC = V0 + R0 * (0.5 * eff_length * zhat);

  // Magnetic field define
  //-------------------------------------------------------------------------------
  FieldTable::GetInstance()->RegisterLinIntpolation(filename.c_str());
  FieldTable::GetInstance()->AddFieldEntry(filename.c_str(), VC, R0, field_scale_factor);

  //-------------------------------------------------------------------------------
  // top volume
  G4Box* sv_top = new G4Box("sv_sm_top", 
                            0.51 * body_full_width,
                            0.51 * body_full_height,
                            0.5 * eff_length * field_exp_scale);
  G4LogicalVolume* lv_top 
    = new G4LogicalVolume(sv_top, mother_lv->GetMaterial(), "lv_sm_top");
  G4RotationMatrix* rr = new G4RotationMatrix(R0);
  rr->invert();
  new G4PVPlacement(rr,
                    VC,
                    lv_top,
                    "pv_sm_top",
                    mother_lv,
                    false,
                    0);
  
  // body
  G4Box* sv_body_v = new G4Box("sv_sm_body_v", 
                               0.5 * body_full_width,
                               0.25 * (body_full_height - tube_full_height),
                               0.49 * eff_length);
  G4LogicalVolume* lv_body_v 
    = new G4LogicalVolume(sv_body_v, body_mat, "lv_sm_body_v");
  new G4PVPlacement(0,
                    origin + 0.25 * (body_full_height + tube_full_height) * yhat,
                    lv_body_v,
                    "pv_sm_body_upper",
                    lv_top,
                    false,
                    0);

  new G4PVPlacement(0,
                    origin - 0.25 * (body_full_height + tube_full_height) * yhat,
                    lv_body_v,
                    "pv_sm_body_lower",
                    lv_top,
                    false,
                    1);

  G4Box* sv_body_h = new G4Box("sv_sm_body_h", 
                               0.25 * (body_full_width - tube_full_width),
                               0.5 * tube_full_height,
                               0.49 * eff_length);
  G4LogicalVolume* lv_body_h 
    = new G4LogicalVolume(sv_body_h, body_mat, "lv_sm_body_h");
  new G4PVPlacement(0,
                    origin + 0.25 * (body_full_width + tube_full_width) * xhat,
                    lv_body_h,
                    "pv_sm_body_left",
                    lv_top,
                    false,
                    0);

  new G4PVPlacement(0,
                    origin - 0.25 * (body_full_width + tube_full_width) * xhat,
                    lv_body_h,
                    "pv_sm_body_right",
                    lv_top,
                    false,
                    1);

  
  
}
