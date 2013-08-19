//*****************************************************************************
//  RidgeFilter.cc
//                           - 2012-12-07, by Garam HAHN.
//*****************************************************************************

#include "RidgeFilter.hh"
#include "GaramGlobal.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4TessellatedSolid.hh"
#include "G4TriangularFacet.hh"
#include "G4QuadrangularFacet.hh"
#include "G4VFacet.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"

//=============================================================================
RidgeFilter::RidgeFilter(const G4double ixsize,
                         const G4double iysize,
                         const char*    ibody_nistname,
                         const char*    ibg_nistname)
  :BeamLineComponent(Ridge), 
   xsize(ixsize), ysize(iysize), zsize(0.), 
   gapsize(0.)
{
  try
  {
    body_mat = G4NistManager::Instance()
      ->FindOrBuildMaterial(ibody_nistname, false);
  }
  catch (int i)
  {
    body_mat = G4NistManager::Instance()
      ->FindOrBuildMaterial("G4_Al", false);
  }

  try
  {
    bg_mat = G4NistManager::Instance()
      ->FindOrBuildMaterial(ibg_nistname, false);
  }
  catch (int i)
  {
    bg_mat = G4NistManager::Instance()
      ->FindOrBuildMaterial("G4_AIR", false);
  }
}

//=============================================================================
RidgeFilter::RidgeFilter(const G4double    ixsize,
                         const G4double    iysize,
                         G4Material* ibody_mat,
                         G4Material* ibg_mat)
  :BeamLineComponent(Ridge), 
   xsize(ixsize), ysize(iysize), zsize(0.),
   gapsize(0.), body_mat(ibody_mat), bg_mat(ibg_mat)
{
}

//=============================================================================
RidgeFilter::~RidgeFilter()
{
}

//=============================================================================
RidgeFilter* RidgeFilter::New()
{
  RidgeFilter* r = new RidgeFilter(xsize, ysize, body_mat, bg_mat);
  for (G4int i = 0; i < (G4int)zs.size(); ++i)
    r->Add2DCrxPoint(xs[i], zs[i]);
  return r;
}

//=============================================================================
void RidgeFilter::Add2DCrxPoint(const G4double ix, const G4double iz)
{
  // point validation
  //  1. Xi > Xi-1
  //  2. Zi > 0
  //  3. Xi < xsize
  //  4. Zi == 0, Zi+1 == 0 means ridge def. was finished before.
  if (iz < 0) return; // will be changed to G4Exception handler
  
  if (xs.size() != 0)
  {
    if (xs.back() >= ix) return; // backward def. --> Raise Error
    if ((ix - xs.front()) > xsize) return; // big size --> Raise Error 
  }

  const int arrsize = zs.size();
  if ((arrsize > 1) &&
      (zs.back() == 0.0)) return; // Raise ExceptionErr

  // set zsize(zmax)
  if (iz > zsize) zsize = iz;
  
  zs.push_back(iz);
  xs.push_back(ix);
}

//=============================================================================
void RidgeFilter::AddSpace(const G4double igap)
{
  gapsize = igap;
}

//===================================================================================
G4RotationMatrix RidgeFilter::GetNextRotationMatrix(const G4RotationMatrix& R0)
{
  return R0;
}

//===================================================================================
G4ThreeVector RidgeFilter::GetNextPosition(const G4ThreeVector&    V0,
                                           const G4RotationMatrix& R0)
{
  return V0 + R0 * (zsize * zhat);
}

//===================================================================================
void RidgeFilter::Generate(const G4RotationMatrix& R0,
                           const G4ThreeVector&    V0,
                                 G4LogicalVolume*  mother_lv )
{
  if (zs.size() < 4) return; // Raise ExceptionErr
  
  std::cout << "zsize = " << zsize << std::endl;
  
  const G4double ridge_x_size         = xs.back() - xs.front();
  const G4double hy                   = 0.5 * ysize;
  const G4int    n_bars               = xsize / (ridge_x_size + gapsize);
  const G4double length_to_next_ridge = ridge_x_size + gapsize;
  const G4double margin_to_side_ridge 
    = 0.5 * (xsize - G4double(n_bars) * length_to_next_ridge);
  for (std::vector<G4double>::iterator xit = xs.begin(); 
       xit != xs.end(); 
       ++xit) 
    (*xit) -= 0.5 * ridge_x_size;
  
  // Build Single Ridge Bar Solid Volume
  //--------------------------------------------------------------------------
  G4TessellatedSolid* sv_ridge = new G4TessellatedSolid("sv_ridge");
  for (G4int i = 0; i < (G4int)zs.size() -1; ++i)
  {
    // build front(-yhat) and back(+yhat) plates
    if (zs[i] == 0.0) // triangular plate (start)
    {
      G4TriangularFacet *frontf = new G4TriangularFacet(
        G4ThreeVector(xs[i],   -hy,       0.0), 
        G4ThreeVector(xs[i+1], -hy,       0.0),
        G4ThreeVector(xs[i+1], -hy, zs[i+1]), ABSOLUTE);
      G4TriangularFacet *backf = new G4TriangularFacet(
        G4ThreeVector(xs[i],    hy,       0.0),
        G4ThreeVector(xs[i+1],  hy, zs[i+1]),
        G4ThreeVector(xs[i+1],  hy,       0.0), ABSOLUTE);
      sv_ridge->AddFacet((G4VFacet*) frontf);
      sv_ridge->AddFacet((G4VFacet*) backf);
    }
    else if (zs[i+1] == 0.0) // triangular plate (end)
    {
      G4TriangularFacet *frontf = new G4TriangularFacet(
        G4ThreeVector(xs[i],   -hy,     0.0), 
        G4ThreeVector(xs[i+1], -hy,     0.0),
        G4ThreeVector(xs[i],   -hy, zs[i]), ABSOLUTE);
      G4TriangularFacet *backf = new G4TriangularFacet(
        G4ThreeVector(xs[i],    hy,     0.0),
        G4ThreeVector(xs[i],    hy, zs[i]),
        G4ThreeVector(xs[i+1],  hy,     0.0), ABSOLUTE);
      sv_ridge->AddFacet((G4VFacet*) frontf);
      sv_ridge->AddFacet((G4VFacet*) backf);
    }
    else // rectangular plate (middle)
    {
      G4QuadrangularFacet *frontf = new G4QuadrangularFacet(
        G4ThreeVector(xs[i],   -hy,       0.0), 
        G4ThreeVector(xs[i+1], -hy,       0.0),
        G4ThreeVector(xs[i+1], -hy, zs[i+1]),
        G4ThreeVector(xs[i],   -hy, zs[i]  ), ABSOLUTE);
      G4QuadrangularFacet *backf = new G4QuadrangularFacet(
        G4ThreeVector(xs[i],    hy,       0.0),
        G4ThreeVector(xs[i],    hy, zs[i]  ),
        G4ThreeVector(xs[i+1],  hy, zs[i+1]),
        G4ThreeVector(xs[i+1],  hy,       0.0), ABSOLUTE);
      sv_ridge->AddFacet((G4VFacet*) frontf);
      sv_ridge->AddFacet((G4VFacet*) backf);
    }
    
    // top(+zhat) and bottom(-zhat) plates 
    G4QuadrangularFacet *topf = new G4QuadrangularFacet(
      G4ThreeVector(xs[i],   -hy, zs[i]  ), 
      G4ThreeVector(xs[i+1], -hy, zs[i+1]),
      G4ThreeVector(xs[i+1],  hy, zs[i+1]),
      G4ThreeVector(xs[i],    hy, zs[i]  ), ABSOLUTE);
    G4QuadrangularFacet *bottomf = new G4QuadrangularFacet(
      G4ThreeVector(xs[i],   -hy, 0.0),
      G4ThreeVector(xs[i],    hy, 0.0),
      G4ThreeVector(xs[i+1],  hy, 0.0),
      G4ThreeVector(xs[i+1], -hy, 0.0), ABSOLUTE);
    sv_ridge->AddFacet((G4VFacet*) topf);
    sv_ridge->AddFacet((G4VFacet*) bottomf);
  }

  if (zs.front() != 0.0) // left(-xhat) rectangular plate, if exist
  {
    G4QuadrangularFacet *leftf = new G4QuadrangularFacet(
      G4ThreeVector(xs.front(), -hy,          0.0), 
      G4ThreeVector(xs.front(), -hy, zs.front()),
      G4ThreeVector(xs.front(),  hy, zs.front()),
      G4ThreeVector(xs.front(),  hy,          0.0), ABSOLUTE);
    sv_ridge->AddFacet((G4VFacet*) leftf);
  }
  
  if (zs.back() != 0.0) // right(+xhat) rectangular plate, if exist
  {
    G4QuadrangularFacet *rightf = new G4QuadrangularFacet(
      G4ThreeVector(xs.back(), -hy,         0.0),
      G4ThreeVector(xs.back(),  hy,         0.0),
      G4ThreeVector(xs.back(),  hy, zs.back()),
      G4ThreeVector(xs.back(), -hy, zs.back()), ABSOLUTE);
    sv_ridge->AddFacet((G4VFacet*) rightf);
  }        
  sv_ridge->SetSolidClosed(true);
  G4LogicalVolume* lv_ridge = new G4LogicalVolume(sv_ridge, body_mat, "lv_ridge");
  lv_ridge->SetVisAttributes(new G4VisAttributes(cOrange));
  
  // Build Top Virtual Volume
  //--------------------------------------------------------------------------
  G4Box* sv_top = new G4Box("sv_ridge_top", 0.5 * xsize, 0.5 * ysize, 0.5 * zsize);
  G4LogicalVolume* lv_top = new G4LogicalVolume(sv_top, bg_mat, "lv_top");
  lv_top->SetVisAttributes(G4VisAttributes::GetInvisible());
  G4RotationMatrix* R_back = new G4RotationMatrix(R0);
  R_back->rotate(M_PI, R0 * yhat);
  new G4PVPlacement(R_back,
                    V0 + R0 * (0.5 * zsize * zhat),
                    lv_top,
                    "pv_top",
                    mother_lv,
                    false,     // boolean op.
                    0);        // copy num
  
  // G4PVReplica is supported only for CSG, not this BREPS
  const G4ThreeVector Vfirst = -0.5 * (zsize * zhat + xsize * xhat) 
    + (margin_to_side_ridge + 0.5 * ridge_x_size) * xhat;
  G4String pvname = "pv_ridge";
  for (G4int i = 0; i < n_bars; ++i)
    new G4PVPlacement(0, 
                      Vfirst + (G4double(i) * length_to_next_ridge * xhat),
                      lv_ridge,
                      (pvname + garam::to_str(i)).c_str(),
                      lv_top,
                      false,     // boolean op.
                      i);        // copy num
}

  
