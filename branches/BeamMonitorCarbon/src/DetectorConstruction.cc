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

  DriftSpace    D(0.1 * m);
  Block         BPET(25.0 * um, 25.0 * um, "G4_POLYETHYLENE");
  Block         BCU(50.0 * um, 50.0 * um,"G4_Cu");
  Block         BPG(0.2 * mm, 0.2 * mm, "G4_Pyrex_Glass");
  Block         BKAPTON(200.0 * um, 200.0 * um,"G4_KAPTON");
  Block         BGAIR(0.005 * m, 0.005 * m,"G4_AIR");
  Block         BAIR1(0.1 * m, 0.1 * m,"G4_AIR");
  Block         BAIR2(0.5 * m, 0.5 * m,"G4_AIR");
  WaterPhantom  WP(0.2 * m, 0.2 * m, 0.1 * m, "G4_WATER");
  VirtualMonitor mon(8.0 * cm, 8.0 * cm);
    
  // Beam Line
  bcm.Add(D.New(0.1 * m));
  // Kepton Window
  bcm.Add(BKAPTON.New(200.0 * um));
  // Air Drift
  bcm.Add(BAIR1.New(0.1*m));
  
  // Monitor
    // Monitor Cover Front
    bcm.Add(BPET.New(25.0 * um));
    bcm.Add(BGAIR.New(0.005 * m));
    
    // Dose Monitor 1
    bcm.Add(BPET.New(25.0 * um));
    bcm.Add(BGAIR.New(0.005 * m));
    bcm.Add(BCU.New(50.0 * um));
    bcm.Add(BPG.New(0.2 * mm));
        // Gap
        bcm.Add(BGAIR.New(0.005 * m));
    // Dose Monitor 2
    bcm.Add(BPET.New(25.0 * um));
    bcm.Add(BGAIR.New(0.005 * m));
    bcm.Add(BCU.New(50.0 * um));
    bcm.Add(BPG.New(0.2 * mm));
        // Gap
        bcm.Add(BGAIR.New(0.005 * m));
    // Position Monitor 1
    bcm.Add(BPET.New(25.0 * um));
    bcm.Add(BGAIR.New(0.005 * m));
    bcm.Add(BCU.New(50.0 * um));
    bcm.Add(BPG.New(0.2 * mm));
        // Gap
        bcm.Add(BGAIR.New(0.005 * m));
    // Position Monitor 2
    bcm.Add(BPET.New(25.0 * um));
    bcm.Add(BGAIR.New(0.005 * m));
    bcm.Add(BCU.New(50.0 * um));
    bcm.Add(BPG.New(0.2 * mm));
    
    //Monitor Cover Back
    bcm.Add(BGAIR.New(0.005 * m));
    bcm.Add(BPET.New(25.0 * um));
    
  // Air Drift
  bcm.Add(mon.New());
  bcm.Add(BAIR2.New(0.5*m));
  bcm.Add(mon.New());
    
  // Water Phantom
  //bcm.Add(WP.New());


  G4VPhysicalVolume* pv = bcm.GenerateVolume();

  return pv;




}
