//
//  Slab.cc
//  projg4khima
//
//  Created by Kim Chang Hyeuk on 13. 9. 30..
//
//

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
#include "Slab.hh"

//===================================================================================
Slab::Slab(const G4double pX, const G4double pY, const G4double pZ, const char* bm)
:BeamLineComponent(BLSlab)
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
        G4NistManager::Instance()->FindOrBuildMaterial("G4_GRAPHITE", false);
    }
}

//===================================================================================
Slab::Slab(const G4double pX, const G4double pY, const G4double pZ, G4Material* mat)
:BeamLineComponent(BLSlab), HalfX(pX), HalfY(pY), HalfZ(pZ), body_mat(mat)
{
}

Slab::~Slab()
{
}

//===================================================================================

Slab* Slab::New()
{
    return new Slab(HalfX, HalfY, HalfZ, body_mat);
}

Slab* Slab::New(const G4double pZ)
{
    return new Slab(HalfX, HalfY, pZ,body_mat);
}


//===================================================================================
G4RotationMatrix
Slab::GetNextRotationMatrix(const G4RotationMatrix& R0)
{
    return R0;
}

//===================================================================================
G4ThreeVector
Slab::GetNextPosition(const G4ThreeVector&    V0,
                              const G4RotationMatrix& R0)
{
    return V0 + R0 * ( HalfZ * zhat);
}
//===================================================================================
void Slab::Generate(const G4RotationMatrix& R0,
                            const G4ThreeVector&    V0,
                            G4LogicalVolume*  mother_lv )
{
    if ( HalfZ == 0.0 ) return;
    G4Box*s_blk = new G4Box("degSlab",HalfX/2, HalfY/2,HalfZ/2);
    const G4ThreeVector VC = V0 + R0 * ( 0.5 * HalfZ * zhat);
    G4LogicalVolume* l_blk = new G4LogicalVolume(s_blk, body_mat, "l_blk");
    l_blk->SetVisAttributes(new G4VisAttributes(cYellow));
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