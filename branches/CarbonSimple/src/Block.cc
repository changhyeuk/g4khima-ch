//*****************************************************************************
//  Block.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Tubs.hh"

#include "GaramGlobal.hh"
#include "Block.hh"

//===================================================================================
Block::Block(const G4double thick, const G4double totlength, const char* bm)
  :BeamLineComponent(DummyBlock)
{
  thickness = thick;
  total_length  = totlength;

  try
  {
    body_mat =
      G4NistManager::Instance()->FindOrBuildMaterial(bm, false);
  }
  catch (int e)
  {
    body_mat =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_GRAPHITE", false);
  }
}



//===================================================================================
Block::Block(const G4double thick, const G4double totlength, G4Material* bm)
  :BeamLineComponent(DummyBlock)
{
  thickness = thick;
  total_length  = totlength;
  body_mat  = bm;
}


//===================================================================================
Block::~Block()
{
}



//===================================================================================
Block* Block::New()
{
  return new Block(thickness, total_length, body_mat);
}


//===================================================================================
Block*
Block::New(const G4double thick)
{
  return new Block(thick, total_length, body_mat);
}

//===================================================================================
G4RotationMatrix
Block::GetNextRotationMatrix(const G4RotationMatrix& R0)
{
  return R0;
}

//===================================================================================
G4ThreeVector
Block::GetNextPosition(const G4ThreeVector&    V0,
                       const G4RotationMatrix& R0)
{
  return V0 + R0 * (total_length * zhat);
}


//===================================================================================
void Block::Generate(const G4RotationMatrix& R0,
                     const G4ThreeVector&    V0,
                           G4LogicalVolume*  mother_lv )
{
  if (thickness == 0.0) return;

  // Geometrical Volume Define
  G4Tubs* s_blk = new G4Tubs("degBlock", 0., 100.0 * mm, 0.5 * thickness, 0., 360.0 * deg);
  const G4ThreeVector VC = V0 + R0 * (0.5 * thickness * zhat);
  G4LogicalVolume* l_blk = new G4LogicalVolume(s_blk, body_mat, "l_blk");
  l_blk->SetVisAttributes(G4VisAttributes::Invisible);
  G4RotationMatrix* rr = new G4RotationMatrix(R0);
  rr->invert();
  new G4PVPlacement(rr,
                    VC,
                    l_blk,
                    "p_blk",
                    mother_lv,
                    false,
                    0);

}


