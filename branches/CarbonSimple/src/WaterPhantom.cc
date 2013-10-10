//*****************************************************************************
//  WaterPhantom.cc
//                           - 2013-04-24, by ChangKim.
//*****************************************************************************

#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Box.hh"
#include "CalorimeterSD.hh"

#include "GaramGlobal.hh"
#include "WaterPhantom.hh"

//===================================================================================
WaterPhantom::WaterPhantom(const G4double pX, const G4double pY, const G4double pZ, const char* bm)
:BeamLineComponent(BLWaterPhantom)
{
    HalfX = pX;
    HalfY = pY;
    HalfZ = pZ;
    
    try
    {
        body_mat =
        G4NistManager::Instance()->FindOrBuildMaterial(bm, false);
    }
    catch (int e)
    {
        body_mat =
        G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER", false);
    }
}

//===================================================================================
WaterPhantom::WaterPhantom(const G4double pX, const G4double pY, const G4double pZ, G4Material* mat)
:BeamLineComponent(BLWaterPhantom), HalfX(pX), HalfY(pY), HalfZ(pZ), body_mat(mat)
{
}

WaterPhantom::~WaterPhantom()
{
}

//===================================================================================

WaterPhantom* WaterPhantom::New()
{
    return new WaterPhantom(HalfX, HalfY, HalfZ, body_mat);
}

WaterPhantom* WaterPhantom::New(const G4double pZ)
{
    return new WaterPhantom(HalfX, HalfY, pZ,body_mat);
}


//===================================================================================
G4RotationMatrix
WaterPhantom::GetNextRotationMatrix(const G4RotationMatrix& R0)
{
    return R0;
}

//===================================================================================
G4ThreeVector
WaterPhantom::GetNextPosition(const G4ThreeVector&    V0,
                              const G4RotationMatrix& R0)
{
    return V0 + R0 * ( HalfZ * zhat);
}
//===================================================================================
void WaterPhantom::Generate(const G4RotationMatrix& R0,
                            const G4ThreeVector&    V0,
                            G4LogicalVolume*  mother_lv )
{
    if ( HalfZ == 0.0 ) return;
    G4Box*s_blk = new G4Box("degWaterPhantom",HalfX/2, HalfY/2,HalfZ/2);
    const G4ThreeVector VC = V0 + R0 * ( 0.5 * HalfZ * zhat);
    //G4LogicalVolume* l_blk = new G4LogicalVolume(s_blk, body_mat, "l_blk");
    G4LogicalVolume* l_blk = new G4LogicalVolume(s_blk, body_mat, "l_blk",0,
                                                 new CalorimeterSD("WaterPhantom",
                                                                   R0,
                                                                   VC,
                                                                   150,-HalfX/2,HalfX/2,
                                                                   150,-HalfY/2,HalfY/2,
                                                                   400,-HalfZ/2,HalfZ/2));
    G4cout<<" Water Phantom Position : "<<VC<<G4endl;
    
    l_blk->SetVisAttributes(new G4VisAttributes(cGreen));
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
