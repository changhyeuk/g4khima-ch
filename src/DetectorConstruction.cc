//*****************************************************************************
//  DetectorConstruction.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
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
#include "Slab.hh"
#include "ScanDipoleMagnet.hh"
#include "FieldTable.hh"
#include "VirtualMonitor.hh"
#include "ThinCollimator.hh"
#include "WaterPhantom.hh"
#include "MultiLeafCollimator.hh"
#include "RangeShifter.hh"
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
:detMsn(0)
{
    detMsn = new DetectorMessenger(this);
}

//==========================================================================
DetectorConstruction::~DetectorConstruction()
{
  delete detMsn;
  FieldTable::Dispose();
}

//==========================================================================
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  //G4SDManager::GetSDMpointer()->SetVerboseLevel(2);
  
  BeamLineComponentManager bcm(430.0 * MeV, 0.5);
  bcm.SetBGMaterial("G4_Galactic");
  const G4double field_exp_scale = 1.2; // for real
  //G4RotationMatrix Ri = G4RotationMatrix::IDENTITY;
  //Ri.rotateY(45.0 * deg);
  //Ri.rotateX(19.0 * deg);
  //bcm.SetInitRotationMatrix(Ri);


  DriftSpace    D(0.4 * m);
  
  ScanDipoleMagnet SMx(6.0 * cm,        // tube_full_height
                       15.0 * cm,       // tube_full_width
                       50.0 * cm,       // body_full_height
                       70.0 * cm,       // body_full_width
                       30.0 * cm,       // eff_length
                       0.3 * tesla,     // field_strength
                       field_exp_scale, // field_exp_scale
                       "G4_Fe");


  Slab      BKAPTON(0.3*m, 0.3*m,0.2*mm,"G4_KAPTON");
  Block     BAIR(0.01 * m, 0.1 * m,"G4_AIR");
  
  Slab      BPET(0.3*m,0.3*m,25.0*um,"G4_POLYETHYLENE");
  Slab      BCU(0.3*m,0.3*m,50.0*um,"G4_Cu");
  Slab      BPG(0.3*m,0.3*m,200.0*um,"G4_Pyrex_Glass");
  Slab      BGAIR(0.3*m,0.3*m,5.0*mm,"G4_AIR");
  Block     BACKUPMON(46.15*mm,46.15*mm,"G4_AIR");
  Block     BACKUPRGF(42.0*mm,42.0*mm,"G4_AIR"); // The Value Should be connected with the highest value with RGF
  Block     BACKUPRSF(30.0*cm,30.0*cm,"G4_AIR");
  Block     BACKUPMLC(15.0*cm,15.0*cm,"G4_AIR");
  
  RangeShifter   RSF(0.3*m, 0.3*m, 0.3*m, 7,"G4_AIR","G4_PLEXIGLASS");
  WaterPhantom   WP(0.3 * m, 0.3 * m, 0.4 * m, "G4_WATER");
  VirtualMonitor mon(20.0 *cm,20.0 *cm);
    
  RidgeFilter* RF = new RidgeFilter(30.*cm, 30.*cm, "G4_Al", "G4_AIR");
    RF->Add2DCrxPoint(0.2*mm,   22.5*mm);
    RF->Add2DCrxPoint(0.27*mm,   22.0*mm);
    RF->Add2DCrxPoint(0.36*mm,   21.5*mm);
    RF->Add2DCrxPoint(0.36*mm,   21.0*mm);
    RF->Add2DCrxPoint(0.43*mm,   20.5*mm);
    RF->Add2DCrxPoint(0.44*mm,   20.0*mm);
    RF->Add2DCrxPoint(0.50*mm,   19.5*mm);
    RF->Add2DCrxPoint(0.53*mm,   19.0*mm);
    RF->Add2DCrxPoint(0.59*mm,   18.5*mm);
    RF->Add2DCrxPoint(0.62*mm,   18.0*mm);
    RF->Add2DCrxPoint(0.67*mm,   17.5*mm);
    RF->Add2DCrxPoint(0.72*mm,   17.0*mm);
    RF->Add2DCrxPoint(0.76*mm,   16.5*mm);
    RF->Add2DCrxPoint(0.81*mm,   16.0*mm);
    RF->Add2DCrxPoint(0.86*mm,   15.5*mm);
    RF->Add2DCrxPoint(0.91*mm,   15.0*mm);
    RF->Add2DCrxPoint(0.96*mm,   14.5*mm);
    RF->Add2DCrxPoint(1.0*mm,   14.0*mm);
    RF->Add2DCrxPoint(1.08*mm,   13.5*mm);
    RF->Add2DCrxPoint(1.10*mm,   13.0*mm);
    RF->Add2DCrxPoint(1.19*mm,   12.5*mm);
    RF->Add2DCrxPoint(1.20*mm,   12.0*mm);
    RF->Add2DCrxPoint(1.30*mm,   11.5*mm);
    RF->Add2DCrxPoint(1.32*mm,   11.0*mm);
    RF->Add2DCrxPoint(1.41*mm,   10.5*mm);
    RF->Add2DCrxPoint(1.45*mm,   10.0*mm);
    RF->Add2DCrxPoint(1.55*mm,   9.5*mm);
    RF->Add2DCrxPoint(1.60*mm,   9.0*mm);
    RF->Add2DCrxPoint(1.68*mm,   8.5*mm);
    RF->Add2DCrxPoint(1.77*mm,   8.0*mm);
    RF->Add2DCrxPoint(1.83*mm,   7.5*mm);
    RF->Add2DCrxPoint(1.95*mm,   7.0*mm);
    RF->Add2DCrxPoint(2.01*mm,   6.5*mm);
    RF->Add2DCrxPoint(2.15*mm,   6.0*mm);
    RF->Add2DCrxPoint(2.21*mm,   5.5*mm);
    RF->Add2DCrxPoint(2.38*mm,   5.0*mm);
    RF->Add2DCrxPoint(2.46*mm,   4.5*mm);
    RF->Add2DCrxPoint(2.65*mm,   4.0*mm);
    RF->Add2DCrxPoint(2.76*mm,   3.5*mm);
    RF->Add2DCrxPoint(2.00*mm,   3.0*mm);
    RF->Add2DCrxPoint(2.11*mm,   2.5*mm);
    RF->Add2DCrxPoint(3.49*mm,   2.0*mm);
    RF->Add2DCrxPoint(3.52*mm,   1.5*mm);
    RF->Add2DCrxPoint(4.26*mm,   1.0*mm);
    RF->Add2DCrxPoint(5.00*mm,   0.5*mm);
    RF->AddSpace(0.2 * mm);
    
  MultiLeafCollimator* MLC
    = new MultiLeafCollimator(25.*cm, 25.*cm, 15.*cm, "G4_Fe", "G4_AIR");
    MLC->DefineLeaf(40, 0.25*cm);
    MLC->SetLeaf(40,   0.*cm, 0.*cm);
    MLC->SetLeaf(39,   0.*cm, 0.*cm);
    MLC->SetLeaf(38,   0.*cm, 0.*cm);
    MLC->SetLeaf(37,   0.*cm, 0.*cm);
    MLC->SetLeaf(36,   0.*cm, 0.*cm);
    MLC->SetLeaf(35,   0.*cm, 0.*cm);
    MLC->SetLeaf(34,   0.*cm, 0.*cm);
    MLC->SetLeaf(33,   0.*cm, 0.*cm);
    MLC->SetLeaf(32,   0.*cm, 0.*cm);
    MLC->SetLeaf(31,   0.*cm, 0.*cm);
    MLC->SetLeaf(30,   0.*cm, 0.*cm);
    MLC->SetLeaf(29,   0.*cm, 0.*cm);
    MLC->SetLeaf(28,   0.*cm, 0.*cm);
    MLC->SetLeaf(27,   0.*cm, 0.*cm);
    MLC->SetLeaf(26,   0.*cm, 0.*cm);
    MLC->SetLeaf(25,   0.*cm, 0.*cm);
    MLC->SetLeaf(24,   -1.5*cm, 1.75*cm);
    MLC->SetLeaf(23,   -2.5*cm, 2.75*cm);
    MLC->SetLeaf(22,   -3.0*cm, 3.25*cm);
    MLC->SetLeaf(21,   -3.5*cm, 3.75*cm);
    MLC->SetLeaf(20,   -4.0*cm, 4.25*cm);
    MLC->SetLeaf(19,   -4.25*cm, 4.5*cm);
    MLC->SetLeaf(18,   -4.75*cm, 5.0*cm);
    MLC->SetLeaf(17,   -5.0*cm, 5.25*cm);
    MLC->SetLeaf(16,   -5.25*cm, 5.5*cm);
    MLC->SetLeaf(15,   -5.5*cm, 5.75*cm);//-12.5*cm, 12.5*cm);
    MLC->SetLeaf(14,   -5.75*cm, 6.0*cm);//-13.*cm, 13.*cm);
    MLC->SetLeaf(13,   -5.75*cm, 6.0*cm);//-13.5*cm, 13.5*cm);
    MLC->SetLeaf(12,   -6.0*cm, 6.25*cm);//-14.*cm, 14.*cm);
    MLC->SetLeaf(11,   -6.25*cm, 6.5*cm);//-14.5*cm, 14.5*cm);
    MLC->SetLeaf(10,   -6.25*cm, 6.5*cm);//-15.*cm, 15.*cm);
    MLC->SetLeaf( 9,   -6.5*cm, 6.75*cm);//-15.5*cm, 15.5*cm);
    MLC->SetLeaf( 8,   -6.5*cm, 6.75*cm);//-16.*cm, 16.*cm);
    MLC->SetLeaf( 7,   -6.5*cm, 6.75*cm);//-16.5*cm, 16.5*cm);
    MLC->SetLeaf( 6,   -6.75*cm, 7.0*cm);//-17.*cm, 17.*cm);
    MLC->SetLeaf( 5,   -6.75*cm, 7.0*cm);//-17.5*cm, 17.5*cm);
    MLC->SetLeaf( 4,   -6.75*cm, 7.0*cm);//-18.*cm, 18.*cm);
    MLC->SetLeaf( 3,   -6.75*cm, 7.0*cm);//-18.5*cm, 18.5*cm);
    MLC->SetLeaf( 2,   -6.75*cm, 7.0*cm);//-19.*cm, 19.*cm);
    MLC->SetLeaf( 1,   -7.0*cm, 7.25*cm);//-19.5*cm, 19.5*cm);
    
    MLC->SetLeaf( -1,   -7.0*cm, 7.25*cm);//-19.5*cm, 19.5*cm);
    MLC->SetLeaf( -2,   -6.75*cm, 7.0*cm);//-19.*cm, 19.*cm);
    MLC->SetLeaf( -3,   -6.75*cm, 7.0*cm);//-18.5*cm, 18.5*cm);
    MLC->SetLeaf( -4,   -6.75*cm, 7.0*cm);//-18.*cm, 18.*cm);
    MLC->SetLeaf( -5,   -6.75*cm, 7.0*cm);//-17.5*cm, 17.5*cm);
    MLC->SetLeaf( -6,   -6.75*cm, 7.0*cm);//-17.*cm, 17.*cm);
    MLC->SetLeaf( -7,   -6.5*cm, 6.75*cm);//-16.5*cm, 16.5*cm);
    MLC->SetLeaf( -8,   -6.5*cm, 6.75*cm);//-16.*cm, 16.*cm);
    MLC->SetLeaf( -9,   -6.5*cm, 6.75*cm);//-15.5*cm, 15.5*cm);
    MLC->SetLeaf(-10,   -6.25*cm, 6.5*cm);//-15.*cm, 15.*cm);
    MLC->SetLeaf(-11,   -6.25*cm, 6.5*cm);//-14.5*cm, 14.5*cm);
    MLC->SetLeaf(-12,   -6.0*cm, 6.25*cm);//-14.*cm, 14.*cm);
    MLC->SetLeaf(-13,   -5.75*cm, 6.0*cm);//-13.5*cm, 13.5*cm);
    MLC->SetLeaf(-14,   -5.75*cm, 6.0*cm);//-13.*cm, 13.*cm);
    MLC->SetLeaf(-15,   -5.5*cm, 5.75*cm);//-12.5*cm, 12.5*cm);
    MLC->SetLeaf(-16,   -5.25*cm, 5.5*cm);//-12.*cm, 12.*cm);
    MLC->SetLeaf(-17,   -5.0*cm, 5.25*cm);
    MLC->SetLeaf(-18,   -4.75*cm, 5.0*cm);
    MLC->SetLeaf(-19,   -4.25*cm, 4.5*cm);
    MLC->SetLeaf(-20,   -4.0*cm, 4.25*cm);
    MLC->SetLeaf(-21,   -3.5*cm, 3.75*cm);
    MLC->SetLeaf(-22,   -3.0*cm, 3.25*cm);
    MLC->SetLeaf(-23,   -2.5*cm, 2.75*cm);
    MLC->SetLeaf(-24,   -1.5*cm, 1.75*cm);
    MLC->SetLeaf(-25,   0*cm, 0*cm);
    MLC->SetLeaf(-26,   0*cm, 0*cm);
    MLC->SetLeaf(-27,   0*cm, 0*cm);
    MLC->SetLeaf(-28,   0*cm, 0*cm);
    MLC->SetLeaf(-29,   0*cm, 0*cm);
    MLC->SetLeaf(-30,   0*cm, 0*cm);
    MLC->SetLeaf(-31,   0*cm, 0*cm);
    MLC->SetLeaf(-32,   0*cm, 0*cm);
    MLC->SetLeaf(-33,   0*cm, 0*cm);
    MLC->SetLeaf(-34,   0*cm, 0*cm);
    MLC->SetLeaf(-35,   0*cm, 0*cm);
    MLC->SetLeaf(-36,   0*cm, 0*cm);
    MLC->SetLeaf(-37,   0*cm, 0*cm);
    MLC->SetLeaf(-38,   0*cm, 0*cm);
    MLC->SetLeaf(-39,   0*cm, 0*cm);
    MLC->SetLeaf(-40,   0.*cm, 0.*cm);
    
  // ************************ Beam Line *************************
  
    /*
  bcm.Add(D.New(5.0 * m));
  bcm.Add(BKAPTON.New());
     */
    bcm.Add(BAIR.New(0.1 * m));
    bcm.Add(SMx.New(0.3));
    
    if ( MonitorONOFF == "on" )
    {
        G4cout<<" ==========================================="<<G4endl;
        G4cout<<" Beam Monitor was created in the Beam Line. "<<G4endl;
        G4cout<<" ==========================================="<<G4endl;

        // Monitor ======================================================
        // Monitor Cover Front
        bcm.Add(BPET.New());
        bcm.Add(BGAIR.New());
        // Dose Monitor 1
        bcm.Add(BPET.New());
        bcm.Add(BGAIR.New());
        bcm.Add(BCU.New());
        bcm.Add(BPG.New());
        // Gap
        bcm.Add(BGAIR.New());
        // Dose Monitor 2
        bcm.Add(BPET.New());
        bcm.Add(BGAIR.New());
        bcm.Add(BCU.New());
        bcm.Add(BPG.New());
        // Gap
        bcm.Add(BGAIR.New());
        // Position Monitor 1
        bcm.Add(BPET.New());
        bcm.Add(BGAIR.New());
        bcm.Add(BCU.New());
        bcm.Add(BPG.New());
        // Gap
        bcm.Add(BGAIR.New());
        // Position Monitor 2
        bcm.Add(BPET.New());
        bcm.Add(BGAIR.New());
        bcm.Add(BCU.New());
        bcm.Add(BPG.New());
    
        //Monitor Cover Back
        bcm.Add(BGAIR.New());
        bcm.Add(BPET.New());
    }
    else if ( MonitorONOFF != "on")
    {
        G4cout<<" ==========================================="<<G4endl;
        G4cout<<" Beam Monitor was disappeard in the Beam Line. "<<G4endl;
        G4cout<<" ==========================================="<<G4endl;
        bcm.Add(BACKUPMON.New());
    }
    
    bcm.Add(BAIR.New(0.1 * m));
  
    // RGF ======================================================
    if ( RGFONOFF == "on" )
    {
        G4cout<<" ==========================================="<<G4endl;
        G4cout<<" Ridge Filter was created in the Beam Line. "<<G4endl;
        G4cout<<" ==========================================="<<G4endl;
        bcm.Add(RF);
    }
    else if( RGFONOFF != "on" )
    {
        G4cout<<" ==========================================="<<G4endl;
        G4cout<<" Ridge Filter was disappeard in the Beam Line. "<<G4endl;
        G4cout<<" ==========================================="<<G4endl;
        bcm.Add(BACKUPRGF.New());
    }
    bcm.Add(BAIR.New(0.1 * m));
    
    // RSF ======================================================
    if (RSFONOFF=="on")
    {
        G4cout<<" ==========================================="<<G4endl;
        G4cout<<" Range Shifter was created in the Beam Line. "<<G4endl;
        G4cout<<" ==========================================="<<G4endl;
        bcm.Add(RSF.New());
    }
    else if(RSFONOFF!="on")
    {
        G4cout<<" ==========================================="<<G4endl;
        G4cout<<" Range Shifter was disappeard in the Beam Line. "<<G4endl;
        G4cout<<" ==========================================="<<G4endl;
        bcm.Add(BACKUPRSF.New());
    }
    bcm.Add(BAIR.New(0.1 * m));
    
  // MLC ======================================================
    if ( MLCONOFF == "on")
    {
        G4cout<<" ==========================================="<<G4endl;
        G4cout<<" MLC was created in the Beam Line. "<<G4endl;
        G4cout<<" ==========================================="<<G4endl;
        bcm.Add(MLC);
    }
    else if ( MLCONOFF != "on")
    {
        G4cout<<" ==========================================="<<G4endl;
        G4cout<<" MLC was disappeard in the Beam Line. "<<G4endl;
        G4cout<<" ==========================================="<<G4endl;
        bcm.Add(BACKUPMLC.New());
    }
    /*
    // Virtural Monitor ========================================
    bcm.Add(BAIR.New(0.1 * m));
    bcm.Add(D.New(0.001 * m));
    bcm.Add(mon.New());
    bcm.Add(D.New(0.001 * m));
    
    // Water Phantom ========================================
    bcm.Add(WP.New());
    */
  G4VPhysicalVolume* pv = bcm.GenerateVolume();

  return pv;

}

void DetectorConstruction::SetMonitorONOFF(std::string iMonitorONOFF)
{
    MonitorONOFF = iMonitorONOFF;
}

void DetectorConstruction::SetRGFONOFF(std::string iRGFONOFF)
{
    RGFONOFF = iRGFONOFF;
}

void DetectorConstruction::SetRSFONOFF(std::string iRSFONOFF)
{
    RSFONOFF = iRSFONOFF;
}

void DetectorConstruction::SetMLCONOFF(std::string iMLCONOFF)
{
    MLCONOFF = iMLCONOFF;
}
