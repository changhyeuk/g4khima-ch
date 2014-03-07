//*****************************************************************************
//  BeamLineComponentManager.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"
#include "G4ClassicalRK4.hh"
#include "G4CashKarpRKF45.hh"
#include "G4ElectroMagneticField.hh"
#include "G4EqMagElectricField.hh"
#include "G4TransportationManager.hh"

#include "AnalysisManager.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "GaramGlobal.hh"
#include "BeamLineComponent.hh"
#include "DriftSpace.hh"
#include "QuadrupoleMagnet.hh"
#include "DipoleMagnet.hh"
#include "GlobalGridBField.hh"
#include "Block.hh"
#include "ThinCollimator.hh"
#include "FieldTable.hh"
#include "Slab.hh"
#include "WaterPhantom.hh"
#include "RidgeFilter.hh"
#include "MultiLeafCollimator.hh"
#include "RangeShifter.hh"
#include "ScanDipoleMagnet.hh"

#include "BeamLineComponentManager.hh"

#include "TH2D.h"
#include "TCanvas.h"

G4RotationMatrix 
BeamLineComponentManager::R_init = G4RotationMatrix::IDENTITY;
G4ThreeVector 
BeamLineComponentManager::V_init(0., 0., 0.);

//============================================================================
BeamLineComponentManager::BeamLineComponentManager(const G4double iw_over_u,
                                                   const G4double iq_over_a,
                                                   const char*    inistname,
                                                   const G4double iw_margin)
  :q_over_a(iq_over_a), e_over_u(iw_over_u), world_margin(iw_margin)
{
  bg_mat = BeamLineComponent::GetG4NistMaterial(inistname);
}

//============================================================================
BeamLineComponentManager::~BeamLineComponentManager()
{
  for (std::vector<BeamLineComponent*>::iterator ii = devices.begin();
       ii != devices.end(); ++ii)
  {
    delete *ii;
  }
}

//============================================================================
void BeamLineComponentManager::Add(BeamLineComponent* blc)
{
  blc->SetBeamLineComponentManager(this);
  devices.push_back(blc);
}

//============================================================================
G4VPhysicalVolume* BeamLineComponentManager::GenerateVolume()
{
  // initial values
  G4ThreeVector vmax(0., 0., 0.);
  G4ThreeVector vmin(0., 0., 0.);
  G4ThreeVector    Vtest(0., 0., 0.);
  G4RotationMatrix Rtest = R_init;
  G4ThreeVector Vtmp(0., 0., 0.);
  G4RotationMatrix Rtmp;
  // geometry check
  for (std::vector<BeamLineComponent*>::iterator it = devices.begin();
       it != devices.end(); ++it)
  {
    Rtmp = (*it)->GetNextRotationMatrix(Rtest);
    Vtmp = (*it)->GetNextPosition(Vtest, Rtest);
    Rtest = Rtmp;
    Vtest = Vtmp;
    vmax = G4ThreeVector(garam::max(Vtest.x(), vmax.x()),
                         garam::max(Vtest.y(), vmax.y()),
                         garam::max(Vtest.z(), vmax.z()));
    vmin = G4ThreeVector(garam::min(Vtest.x(), vmin.x()),
                         garam::min(Vtest.y(), vmin.y()),
                         garam::min(Vtest.z(), vmin.z()));
  }
  G4ThreeVector vmargin(world_margin, world_margin, world_margin);
  G4ThreeVector vabs = vmax - vmin;
  G4ThreeVector world_volume_size = 0.5 * vabs + vmargin;
  
  SetInitVector(0.5 * vabs - vmax);
  G4ThreeVector vcnt = 0.5 * (vmax + vmin);

  G4ThreeVector V0 = -vcnt;
  G4RotationMatrix R0 = R_init;

  // Magnetic field definition
  //-------------------------------------------------------------------------------
  GlobalGridBField* ggf = new GlobalGridBField();
  G4FieldManager* gfm
    = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  //gfm->SetMinimumEpsilonStep( 1.0e-5 );
  //gfm->SetMaximumEpsilonStep( 1.0e-4 );
  //gfm->SetDeltaOneStep( 1.0e-2 * mm );
  gfm->SetDetectorField(ggf);
  //G4ClassicalRK4* stepper = new G4ClassicalRK4(new G4EqMagElectricField(ggf), 8);
  G4CashKarpRKF45* stepper = new G4CashKarpRKF45(new G4EqMagElectricField(ggf), 8);
  G4MagInt_Driver* intdrv
    = new G4MagInt_Driver(0.001 * mm, stepper, stepper->GetNumberOfVariables());
  gfm->SetChordFinder(new G4ChordFinder(intdrv));

  // world volume creation
  G4Box* s_world = new G4Box("s_world",
                             world_volume_size.x(),
                             world_volume_size.y(),
                             world_volume_size.z());
    G4cout<<" World :"<<world_volume_size.z()<<G4endl;
    
  G4LogicalVolume* l_world = new G4LogicalVolume(s_world, bg_mat, "l_world");
  // invisible now
  l_world->SetVisAttributes(G4VisAttributes::Invisible);
  G4PVPlacement* p_world = new G4PVPlacement(0,
                                             G4ThreeVector(0., 0., 0.),
                                             l_world,
                                             "p_world",
                                             0, 0, 0);

  for (std::vector<BeamLineComponent*>::iterator it = devices.begin();
       it != devices.end(); ++it)
  {
    (*it)->Generate(R0, V0, l_world);
    Rtmp = (*it)->GetNextRotationMatrix(R0);
    Vtmp = (*it)->GetNextPosition(V0, R0);
    V0 = Vtmp;
    R0 = Rtmp;
  }

  // for the test pictures
  /*
  double x[3];
  double B[6];
  int NB = 1000;
  double dNB = NB;
  double xmin = -world_volume_size.z();
  double xmax = world_volume_size.z();
  double zmin = -world_volume_size.z();
  double zmax = world_volume_size.z();
  FieldTable* ft = FieldTable::GetInstance();
  TH2D* fmapy = new TH2D("fmapy", "fmapy", NB, zmin, zmax, NB, xmin, xmax);
  TH2D* fmapz = new TH2D("fmapz", "fmapz", NB, zmin, zmax, NB, xmin, xmax);
  TH2D* fmapx = new TH2D("fmapx", "fmapx", NB, zmin, zmax, NB, xmin, xmax);
  double dx = (xmax - xmin)/dNB;
  double dz = (zmax - zmin)/dNB;
  double xi, zi;
  for (int i = 0; i < NB-1; ++i)
  {
    for (int j = 0; j < NB-1; ++j)
    {
      x[0] = xmin + dx * (double(i) + 0.5);
      x[1] = 0.;
      x[2] = zmin + dz * (double(j) + 0.5);
      //std::cout << "call v = (" << x[0] <<", "<< x[1]<<", "<< x[2]<<")\n";
      ft->GetGlobalB(x, B);
      fmapy->Fill(x[2], x[0], B[1]);
      fmapz->Fill(x[2], x[0], B[2]);
      fmapx->Fill(x[2], x[0], B[0]);
    }
  }
  TCanvas c1("c1", "c1", 1, 1, 800, 800);
  fmapy->Draw("colz");
  c1.SaveAs("y.png");
  fmapx->Draw("colz");
  c1.SaveAs("x.png");
  fmapz->Draw("colz");
  c1.SaveAs("z.png");
  */
  return p_world;
}

//============================================================================
void BeamLineComponentManager::SetBGMaterial(G4Material* ibg_mat)
{
  bg_mat = ibg_mat;
}

//============================================================================
void BeamLineComponentManager::SetBGMaterial(const char* inistname)
{
  bg_mat = BeamLineComponent::GetG4NistMaterial(inistname);
}

//============================================================================
void BeamLineComponentManager::SetWorldMargin(const G4double iw_margin)
{
  world_margin = iw_margin;
}

//============================================================================
void
BeamLineComponentManager::SetInitRotationMatrix(const G4RotationMatrix& iR)
{
  R_init = iR;
}

//============================================================================
G4RotationMatrix BeamLineComponentManager::GetInitRotationMatrix()
{
  return R_init;
}

//============================================================================
void BeamLineComponentManager::SetInitVector(const G4ThreeVector& iV)
{
  V_init = iV;
}

//============================================================================
G4ThreeVector BeamLineComponentManager::GetInitVector()
{
  return V_init;
}







