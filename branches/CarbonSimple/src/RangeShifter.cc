//
//  RangeShifter.cc
//  projg4khima
//
//  Created by Kim Chang Hyeuk on 13. 10. 1..
//
//

#include <stdio.h>
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
#include "RangeShifter.hh"

//===================================================================================
RangeShifter::RangeShifter(const G4double iHalfX,
                           const G4double iHalfY,
                           const G4double iHalfZ,
                           const G4int    iRSFN,
                           const char*    ibg_name,
                           const char*    ibody_name)
:BeamLineComponent(BLRangeShifter),
HalfX(iHalfX),HalfY(iHalfY),HalfZ(iHalfZ),RSFN(iRSFN)
{
    try
    {
        body_mat = G4NistManager::Instance()
        ->FindOrBuildMaterial(ibody_name, false);
    }
    catch (int i)
    {
        body_mat = G4NistManager::Instance()
        ->FindOrBuildMaterial("G4_Fe", false);
    }
    
    try
    {
        bg_mat = G4NistManager::Instance()
        ->FindOrBuildMaterial(ibg_name, false);
    }
    catch (int i)
    {
        bg_mat = G4NistManager::Instance()
        ->FindOrBuildMaterial("G4_AIR", false);
    }
}

//===================================================================================
RangeShifter::RangeShifter(const G4double iHalfX,
                           const G4double iHalfY,
                           const G4double iHalfZ,
                           const G4int    iRSFN,
                           G4Material* ibg_mat,
                           G4Material* ibody_mat)
:BeamLineComponent(BLRangeShifter), HalfX(iHalfX),HalfY(iHalfY),HalfZ(iHalfZ),RSFN(iRSFN),bg_mat(ibg_mat),body_mat(ibody_mat)
{
}

RangeShifter::~RangeShifter()
{
}

//===================================================================================

RangeShifter* RangeShifter::New()
{
    return new RangeShifter(HalfX, HalfY, HalfZ, RSFN,bg_mat, body_mat);
}

//===================================================================================
G4RotationMatrix
RangeShifter::GetNextRotationMatrix(const G4RotationMatrix& R0)
{
    return R0;
}

//===================================================================================
G4ThreeVector
RangeShifter::GetNextPosition(const G4ThreeVector&    V0,
                      const G4RotationMatrix& R0)
{
    return V0 + R0 * ( HalfZ * zhat);
}
//===================================================================================
void RangeShifter::Generate(const G4RotationMatrix& R0,
                            const G4ThreeVector&    V0,
                            G4LogicalVolume*  mother_lv )
{
    if (RSFN == 1.0)
    {
        RSF_p = 5.05;
        RSF_t = 0.1 * mm;
    }
    else if (RSFN == 2.0)
    {
        RSF_p = 10.6;
        RSF_t = 1.0 * mm;
    }
    else if (RSFN == 3.0)
    {
        RSF_p = 17.1;
        RSF_t = 2.0 * mm;
    }
    else if (RSFN == 4.0)
    {
        RSF_p = 25.1;
        RSF_t = 4.0 * mm;
    }
    else if (RSFN == 5.0)
    {
        RSF_p = 36.1;
        RSF_t = 8.0 * mm;
    }
    else if (RSFN == 6.0)
    {
        RSF_p = 53.1;
        RSF_t = 16.0 * mm;
    }
    else if (RSFN == 7.0)
    {
        RSF_p = 82.1;
        RSF_t = 32.0* mm;
    }
    else if (RSFN == 8.0)
    {
        RSF_p = 135.1;
        RSF_t = 64.0* mm;
    }
    else if (RSFN == 9.0)
    {
        RSF_p = 236.1* mm;
        RSF_t = 128.0* mm;
    }
    
    if ( HalfZ == 0.0 ) return;
    G4Box*s_blk = new G4Box("degRangeShifter",HalfX/2, HalfY/2,HalfZ/2);
    const G4ThreeVector VC = V0 + R0 * ( 0.5 * HalfZ * zhat);
    
    G4LogicalVolume* lv_top = new G4LogicalVolume(s_blk,bg_mat,"lv_top");
    lv_top->SetVisAttributes(G4VisAttributes::Invisible);
    
    G4RotationMatrix* rr = new G4RotationMatrix(R0);
    rr->invert();
    new G4PVPlacement(rr,
                      VC,
                      lv_top,
                      "pv_top",
                      mother_lv,
                      false,
                      0);
    
    G4LogicalVolume* lv = 0;
    lv = new G4LogicalVolume(new G4Box("RSF",
                                       HalfX/2,
                                       HalfY/2,
                                       RSF_t/2),
                             body_mat,
                             "lv_t");
    //const G4ThreeVector VL = V0 + R0 * ( 0.5 * RSF_p * zhat);
    const G4ThreeVector VL = R0 *( ( RSF_p - ( HalfZ / 2 )) * zhat );
    lv->SetVisAttributes(new G4VisAttributes(cSkyBlue));
    new G4PVPlacement(rr,
                      VL,
                      lv,
                      "pv_l",
                      lv_top,
                      false,
                      0);
                             
    
}