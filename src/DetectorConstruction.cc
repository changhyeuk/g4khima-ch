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

  DriftSpace    D(0.4 * m);
  Block         BKAPTON(0.01 * m, 0.1 * m,"G4_KAPTON");
  Block         BAIR(0.01 * m, 0.1 * m,"G4_AIR");
  WaterPhantom  WP(0.2 * m, 0.2 * m, 0.1 * m, "G4_WATER");
 
  bcm.Add(D.New(0.4 * m));
  //bcm.Add(SMx.New(0.5 * tesla));
  //bcm.Add(D.New(0.3 * m));
  //bcm.Add(BKAPTON.New(0.0002 * m));
  //bcm.Add(BAIR.New(0.1 * m));
  //bcm.Add(RF);
  //bcm.Add(BAIR.New(0.1 * m));
  //bcm.Add(MLC);
  //bcm.Add(BAIR.New(0.1 * m));
  //bcm.Add(WP.New());


  VirtualMonitor mon(8.0 * cm, 8.0 * cm);


  G4VPhysicalVolume* pv = bcm.GenerateVolume();

  return pv;




}
