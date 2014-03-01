//*****************************************************************************
//  CalorimeterBlock.cc
//                           - 2012-12-07, by Garam HAHN.
//*****************************************************************************

#include "CalorimeterBlock.hh"
#include "CalorimeterSD.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"


//=============================================================================
CalorimeterBlock::CalorimeterBlock(const G4double ixsize,
                                   const G4double iysize,
                                   const G4double izsize,
                                   const char*    ibody_mat)
  :BeamLineComponent(Calo), xsize(ixsize), ysize(iysize), zsize(izsize)
{
  body_mat = GetG4NistMaterial(ibody_mat);
}

//=============================================================================
CalorimeterBlock::CalorimeterBlock(const G4double ixsize,
                                   const G4double iysize,
                                   const G4double izsize,
                                   G4Material*    ibody_mat)
  :BeamLineComponent(Calo), xsize(ixsize), ysize(iysize), zsize(izsize),
   body_mat(ibody_mat)
{

}

//=============================================================================
CalorimeterBlock::~CalorimeterBlock()
{

}

//==========================================================================
G4RotationMatrix
CalorimeterBlock::GetNextRotationMatrix(const G4RotationMatrix& R0)
{
  return R0;
}

//==========================================================================
G4ThreeVector
CalorimeterBlock::GetNextPosition(const G4ThreeVector&    V0,
                                  const G4RotationMatrix& R0)
{
  return V0 + R0 * (0.5 * zsize * zhat);
}

//==========================================================================
void CalorimeterBlock::Generate(const G4RotationMatrix& R0,
                                const G4ThreeVector&    V0,
                                G4LogicalVolume*  mother_lv)
{
  G4ThreeVector V_half = V0 + R0 * (0.5 * zsize * zhat);
  G4Box* sv_calo = new G4Box("sv_calo", 0.5 * xsize, 0.5 * ysize, 0.5 * zsize);
  CalorimeterSD* csd = new CalorimeterSD("CBlock", R0, V0,
                                         100, -0.5 * xsize, 0.5 * xsize,
                                         100, -0.5 * ysize, 0.5 * ysize,
                                         100, 0., zsize);
  G4LogicalVolume* lv_calo = new G4LogicalVolume(sv_calo,
                                                 body_mat,
                                                 "lv_mon",
                                                 0,
                                                 csd);
  lv_calo->SetVisAttributes(new G4VisAttributes(cRed));
  G4RotationMatrix* rr = new G4RotationMatrix(R0);
  rr->invert();
  new G4PVPlacement(rr, V_half, lv_calo, "pv_calo", mother_lv, false, 0);
}
