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
  const G4double field_exp_scale = 1.2; // for real
  //G4RotationMatrix Ri = G4RotationMatrix::IDENTITY;
  //Ri.rotateY(45.0 * deg);
  //Ri.rotateX(19.0 * deg);
  //bcm.SetInitRotationMatrix(Ri);
   
  
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
  WaterPhantom  WP(0.4 * m, 0.4 * m, 0.4 * m, "G4_WATER");
 
  bcm.Add(D.New(0.4 * m));
  //bcm.Add(SMx.New(0.5 * tesla));
  //bcm.Add(D.New(0.3 * m));
  //bcm.Add(BKAPTON.New(0.0002 * m));
  //bcm.Add(BAIR.New(0.1 * m));
  //bcm.Add(RF);
  //bcm.Add(BAIR.New(0.1 * m));
  //bcm.Add(MLC);
  //bcm.Add(BAIR.New(0.1 * m));
  bcm.Add(WP.New());

    
  /*
  QuadrupoleMagnet Q1(15.209 * tesla/m,  // field_gradient
                      50.0 * cm,         // eff_length
                      4.0 * cm,          // inner radius
                      25.0 * cm,         // outer radius
                      "Q1C-G15.209tpm_L49.426.table");

  QuadrupoleMagnet Q2(11.151 * tesla/m,
                      45.0 * cm,
                      4.0 * cm,
                      25.0 * cm,       
                      "Q2C-G11.151tpm_L44.834.table");

  QuadrupoleMagnet Q3(10.149 * tesla/m,
                      30.0 * cm,
                      5.0 * cm,
                      25.0 * cm,
                      "Q3C-G10.149tpm_L29.777.table");

  DipoleMagnet B(7.0 * cm,        // tube_full_height
                 20.0 * cm,       // tube_full_width
                 22.5 * deg,      // bending_angle
                 4.8 * m,         // radii_curv
                 0. * deg,        // direction_angle
                 50.0 * cm,       // body_full_height
                 70.0 * cm,       // body_full_width
                 "B1B-380A_R4.8m_B1.3287T.table");
  B.SetRectShape();

  VirtualMonitor mon(8.0 * cm, 8.0 * cm);

  ThinCollimator col(3.3 * cm, 4.0 * cm, 10.0 * cm, 10.0 * cm);
  */
    
  // for dipole centering ===================================
  //bcm.Add(D.New(1.0 * m));
  //bcm.Add(B.NewScale(1.0293));
  //bcm.Add(D.New(5.0 * m));
  //bcm.Add(mon.New());
  // ========================================================


  // Add component to the beamline
  // section-1 ==============================================
  bcm.Add(D.New(1.0 * m));
  //bcm.Add(Q1.NewGradient(9.93 * tesla/m ));
  bcm.Add(D.New(0.5 * m));
  //bcm.Add(Q1.NewGradient(-14.35 * tesla/m));
  bcm.Add(D.New(0.75 * m));
  //bcm.Add(Q1.NewGradient(14.13 * tesla/m));
  bcm.Add(D.New(1.25 * m));
  //bcm.Add(mon.New());
  // ========================================================
  // for degrader parameters tuning =========================
  Block Degrader(0.0 * cm, 0.2 * m, "G4_GRAPHITE");
  //bcm.Add(Degrader.New());
  //bcm.Add(mon.New());
  // ========================================================

  // section-2 ==============================================
  bcm.Add(Degrader.New());
  bcm.Add(D.New(0.6 * m));
  //bcm.Add(col.NewGap(1.4 * cm));
  bcm.Add(D.New(0.1 * m));
  //bcm.Add(col.NewGap(1.5 * cm, 90.0 * deg));
  bcm.Add(D.New(1.2 * m));
  //bcm.Add(col.NewGap(1.4 * cm));
  bcm.Add(D.New(0.1 * m));
  //bcm.Add(col.NewGap(2.0 * cm, 90.0 * deg));
  bcm.Add(D.New(0.34 * m));
  //bcm.Add(Q2.NewGradient(-5.96 * tesla/m));
  bcm.Add(D.New(0.57 * m));
  //bcm.Add(Q2.NewGradient(12.46 * tesla/m));
  bcm.Add(D.New(0.74 * m));
  //bcm.Add(Q2.NewGradient(-8.98 * tesla/m));
  bcm.Add(D.New(0.1 * m));
  //bcm.Add(col.NewGap(2.5 * cm));
  bcm.Add(D.New(0.1 * m));
  //bcm.Add(col.NewGap(2.5 * cm, 90.0 * deg));
  // ========================================================

  // section-3 ==============================================
  const G4double DIPOLE_SCALE = 1.0293;
  const G4double QP3_1_SCALE = -6.602 * tesla/m;
  const G4double QP3_2_SCALE =  9.650 * tesla/m;

  bcm.Add(D.New(0.2 * m));
  //bcm.Add(B.NewScale(DIPOLE_SCALE));
  bcm.Add(D.New(0.6 * m));
  //bcm.Add(B.NewScale(DIPOLE_SCALE));
  bcm.Add(D.New(0.6 * m));
  //bcm.Add(Q3.NewGradient(QP3_1_SCALE));
  bcm.Add(D.New(0.5 * m));
  //bcm.Add(Q3.NewGradient(QP3_2_SCALE));
  bcm.Add(D.New(0.7 * m));
  //bcm.Add(col.NewGap(3.3 * cm));
  //bcm.Add(mon.New());

  bcm.Add(D.New(0.7 * m));
  //bcm.Add(Q3.NewGradient(QP3_2_SCALE));
  bcm.Add(D.New(0.5 * m));
  //bcm.Add(Q3.NewGradient(QP3_1_SCALE));
  bcm.Add(D.New(0.6 * m));
  //bcm.Add(B.NewScale(DIPOLE_SCALE));
  //bcm.Add(D.New(0.6 * m));
  //bcm.Add(B.NewScale(DIPOLE_SCALE));
  bcm.Add(D.New(0.2 * m));
  // ========================================================
  //bcm.Add(mon.New());


  /*
  // section-4 ==============================================
  bcm.Add(D.New(1.0 * m));
  bcm.Add(Q2.NewGradient(-9.93 * tesla/m ));
  bcm.Add(D.New(0.5 * m));
  bcm.Add(Q2.NewGradient(14.35 * tesla/m));
  bcm.Add(D.New(0.75 * m));
  bcm.Add(Q2.NewGradient(-14.13 * tesla/m));
  bcm.Add(D.New(1.25 * m));
  // ========================================================
  */

  G4VPhysicalVolume* pv = bcm.GenerateVolume();

  return pv;








  /* end of real code */
  /*
   * To use scanning magnet!
   *
  ScanDipoleMagnet SMx(6.0 * cm,        // tube_full_height
                       15.0 * cm,       // tube_full_width
                       50.0 * cm,       // body_full_height
                       70.0 * cm,       // body_full_width
                       30.0 * cm,       // eff_length
                       0.3 * tesla,     // field_strength
                       field_exp_scale,// field_exp_scale
                       "G4_Fe");
  */

  /*
   * To use Ridge Filter
   * you must define crosectional shape and its material first
   *
  RidgeFilter* RF = new RidgeFilter(30.*cm, 30.*cm, "G4_Al", "G4_AIR");
  RF->Add2DCrxPoint(1.*mm,   2.*mm);
  RF->Add2DCrxPoint(2.*mm,   4.*mm);
  RF->Add2DCrxPoint(3.*mm,   8.*mm);
  RF->Add2DCrxPoint(4.*mm,   16.*mm);
  RF->Add2DCrxPoint(5.*mm,   32.*mm);
  RF->Add2DCrxPoint(6.*mm,   42.*mm);
  RF->Add2DCrxPoint(7.*mm,   42.*mm);
  RF->AddSpace(3.0 * mm);
  //RF->SetRectShape(); // FIXME
  */

  /*
   * To use Multi Leaf Collimator
   *
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
  */

//<<<<<<< .mine
//=======
  // Add component to the beamline
  /*
  bcm.Add(D.New(0.5 * m));
  bcm.Add(Q1.NewGradient(13.53 * tesla/m ));
  bcm.Add(D.New(0.4 * m));
  bcm.Add(Q1.NewGradient(-17.142 * tesla/m));
  bcm.Add(D.New(1.049 * m));
  bcm.Add(Q1.NewGradient(19.485 * tesla/m));
  bcm.Add(D.New(0.801 * m));
  bcm.Add(mon.New());
>>>>>>> .r50

//<<<<<<< .mine
//=======
  bcm.Add(Degrader.New());

  bcm.Add(D.New(0.4 * m));
  bcm.Add(Q2.NewGradient(-12.4 * tesla/m));
  bcm.Add(D.New(0.6 * m));
  bcm.Add(Q2.NewGradient(17.15 * tesla/m));
  bcm.Add(D.New(1.035 * m));
  bcm.Add(Q2.NewGradient(-9.1 * tesla/m));
  bcm.Add(D.New(0.2 * m));
  bcm.Add(mon.New());

  bcm.Add(D.New(0.2 * m));
  bcm.Add(B.NewScale(0.9901));
  bcm.Add(D.New(0.5 * m));
  bcm.Add(B.NewScale(0.9901));
  bcm.Add(D.New(0.4 * m));
  bcm.Add(Q3.NewGradient(15.52 * tesla/m));
  bcm.Add(D.New(0.5 * m));
  bcm.Add(Q3.NewGradient(-18.464 * tesla/m));
  bcm.Add(D.New(0.4 * m));
  bcm.Add(col.NewGap(1.0 * cm));
  bcm.Add(D.New(0.1 * m));
  bcm.Add(mon.New());
  */

  /*
  bcm.Add(D.New(0.2 * m));
  bcm.Add(B.New());
  bcm.Add(D.New(0.5 * m));
  bcm.Add(B.New());
  bcm.Add(D.New(0.4 * m));
  bcm.Add(Q3.New(-0.776));
  bcm.Add(D.New(0.5 * m));
  bcm.Add(Q3.New(0.9232));
  bcm.Add(D.New(0.4 * m));
  */
  /* test SMx begin */
  //bcm.Add(SMx.New(0.5 * tesla));
  /* test SMx end */

  /*
  bcm.Add(D.New(0.4 * m));
  bcm.Add(Q3.New(0.9232));
  bcm.Add(D.New(0.5 * m));
  bcm.Add(Q3.New(-0.776));
  bcm.Add(D.New(0.4 * m));  
  bcm.Add(B.New());
  bcm.Add(D.New(0.5 * m));
  bcm.Add(B.New());
  bcm.Add(D.New(0.2 * m));
  */

  G4VPhysicalVolume* pv = bcm.GenerateVolume();

  return pv;
  /* end of real code */

//>>>>>>> .r50
}
