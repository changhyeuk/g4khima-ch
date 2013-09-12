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
  WaterPhantom  WP(0.3 * m, 0.3 * m, 0.4 * m, "G4_WATER");
 
  bcm.Add(D.New(0.4 * m));
  bcm.Add(WP.New());

    
  G4VPhysicalVolume* pv = bcm.GenerateVolume();

  return pv;

}
