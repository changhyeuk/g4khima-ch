//*****************************************************************************
//  DetectorConstruction.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "DetectorConstruction.hh"
#include "AnalysisManager.hh"
#include "GaramGlobal.hh"
#include "GridBField.hh"
#include "BeamLineComponentManager.hh"
#include "DriftSpace.hh"
#include "QuadrupoleMagnet.hh"
#include "DipoleMagnet.hh"
#include "RidgeFilter.hh"
#include "MultiLeafCollimator.hh"
#include "Block.hh"
#include "ScanDipoleMagnet.hh"
#include "FieldTable.hh"
#include "VirtualMonitor.hh"
#include "ThinCollimator.hh"
#include "WaterPhantom.hh"
#include "MultiLeafCollimator.hh"
//#include "CalorimeterBlock.cc"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Region.hh"


//==========================================================================
DetectorConstruction::DetectorConstruction()
{
}

//==========================================================================
DetectorConstruction::~DetectorConstruction()
{
  FieldTable::Dispose();
}

//==========================================================================
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  //G4SDManager::GetSDMpointer()->SetVerboseLevel(2);
  
  BeamLineComponentManager bcm(430.0 * MeV, 0.5);
  bcm.SetBGMaterial("G4_Galactic");
  //const G4double field_exp_scale = 1.2; // for real
  //G4RotationMatrix Ri = G4RotationMatrix::IDENTITY;
  //Ri.rotateY(45.0 * deg);
  //Ri.rotateX(19.0 * deg);
  //bcm.SetInitRotationMatrix(Ri);

    // Test Subversion
    // From macbook pro thorugh google
    
  
  DriftSpace    D(0.4 * m);
  /*
  ScanDipoleMagnet SMx(6.0 * cm,        // tube_full_height
                       15.0 * cm,       // tube_full_width
                       50.0 * cm,       // body_full_height
                       70.0 * cm,       // body_full_width
                       30.0 * cm,       // eff_length
                       0.3 * tesla,     // field_strength
                       field_exp_scale, // field_exp_scale
                       "G4_Fe");
   */
  Block         BKAPTON(0.01 * m, 0.1 * m,"G4_KAPTON");
  Block         BAIR(0.01 * m, 0.1 * m,"G4_AIR");
  RidgeFilter* RF = new RidgeFilter(30.*cm, 30.*cm, "G4_Al", "G4_AIR");
    RF->Add2DCrxPoint(1.*mm,   2.*mm);
    RF->Add2DCrxPoint(2.*mm,   4.*mm);
    RF->Add2DCrxPoint(3.*mm,   8.*mm);
    RF->Add2DCrxPoint(4.*mm,   16.*mm);
    RF->Add2DCrxPoint(5.*mm,   32.*mm);
    RF->Add2DCrxPoint(6.*mm,   42.*mm);
    RF->Add2DCrxPoint(7.*mm,   42.*mm);
    RF->AddSpace(3.0 * mm);
    
  MultiLeafCollimator* MLC
    = new MultiLeafCollimator(40.*cm, 40.*cm, 10.*cm, "G4_Al", "G4_AIR");
    MLC->DefineLeaf(5, 1.0*cm);
    MLC->DefineLeaf(5, 2.0*cm); // leafs are totally 40(left) + 40(right)
    
    MLC->SetLeaf(-10,  0.*cm, 0.*cm);
    MLC->SetLeaf(- 9, -1.*cm, 1.*cm);
    MLC->SetLeaf(- 8, -2.*cm, 2.*cm);
    MLC->SetLeaf(- 7, -3.*cm, 3.*cm);
    MLC->SetLeaf(- 6, -4.*cm, 4.*cm);
    
    MLC->SetLeaf(- 5, -10.*cm, -10.*cm);
    MLC->SetLeaf(- 4, -11.*cm, -9.*cm);
    MLC->SetLeaf(- 3, -12.*cm, -8.*cm);
    MLC->SetLeaf(- 2, -13.*cm, -7.*cm);
    MLC->SetLeaf(- 1, -14.*cm, -6.*cm);
    
    MLC->SetLeaf( 1, 5.*cm, 5.*cm);
    MLC->SetLeaf( 2, 4.*cm, 6.*cm);
    MLC->SetLeaf( 3, 3.*cm, 7.*cm);
    MLC->SetLeaf( 4, 2.*cm, 8.*cm);
    MLC->SetLeaf( 5, 1.*cm, 9.*cm);
    
    MLC->SetLeaf( 6, 1.*cm, 9.*cm);
    MLC->SetLeaf( 7, 2.*cm, 8.*cm);
    MLC->SetLeaf( 8, 3.*cm, 7.*cm);
    MLC->SetLeaf( 9, 4.*cm, 6.*cm);
    MLC->SetLeaf(10, 5.*cm, 5.*cm);
  WaterPhantom  WP(0.3 * m, 0.3 * m, 0.4 * m, "G4_WATER");
    
  bcm.Add(D.New(0.4 * m));
  bcm.Add(RF);
  bcm.Add(BAIR.New(0.1 * m));
  bcm.Add(MLC);
  bcm.Add(WP.New());
    //bcm.Add(WB.New());
    
    

    
  G4VPhysicalVolume* pv = bcm.GenerateVolume();

  return pv;

}
