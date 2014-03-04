//*****************************************************************************
//  MultiLeafCollimator.cc
//                           - 2012-12-07, by Garam HAHN.
//*****************************************************************************

#include "MultiLeafCollimator.hh"
#include "GaramGlobal.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"



//=============================================================================
MultiLeafCollimator::MultiLeafCollimator(const G4double ixsize,
                                         const G4double iysize,
                                         const G4double izsize,
                                         const char*    ibody_nistname,
                                         const char*    ibg_nistname)
  :BeamLineComponent(MLC), 
   calc_done(false),
   xsize(ixsize), ysize(iysize), zsize(izsize)
{
  try
  {
    body_mat = G4NistManager::Instance()
      ->FindOrBuildMaterial(ibody_nistname, false);
  }
  catch (int i)
  {
    body_mat = G4NistManager::Instance()
      ->FindOrBuildMaterial("G4_Fe", false);
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
MultiLeafCollimator::MultiLeafCollimator(const G4double    ixsize,
                                         const G4double    iysize,
                                         const G4double    izsize,
                                               G4Material* ibody_mat,
                                               G4Material* ibg_mat)
  :BeamLineComponent(MLC), 
   calc_done(false), 
   xsize(ixsize), ysize(iysize), zsize(izsize),
   body_mat(ibody_mat), bg_mat(ibg_mat)
{
}

//=============================================================================
MultiLeafCollimator::~MultiLeafCollimator()
{

}


//=============================================================================
MultiLeafCollimator* MultiLeafCollimator::New()
{
  MultiLeafCollimator* r 
    = new MultiLeafCollimator(xsize, ysize, zsize, body_mat, bg_mat);
  // copy constructor!!!
  return r;
}

//=============================================================================
void MultiLeafCollimator::DefineLeaf(const G4int    inleaf,
                                     const G4double ithick)
{
  G4double yhalf = 0.;
  for (G4int i = 0; i < (G4int)nleaf.size(); ++i)
    yhalf += G4double(nleaf[i]) * tleaf[i];
  yhalf += G4double(inleaf) * ithick;
  if (2.0 * yhalf < ysize)
  {
    nleaf.push_back(inleaf);
    tleaf.push_back(ithick);
  }
  calc_done = false;
}

//===================================================================================
void MultiLeafCollimator::SetLeaf(const G4int    iid,
                                  const G4double ilp,
                                  const G4double irp)
{
  G4int n_leafs = 0;
  for (G4int i = 0; i < (G4int)nleaf.size(); ++i)
    n_leafs += nleaf[i];
  //std::cout << "n(L) = " << n_leafs << std::endl;
  
  if ((garam::abs(iid) > n_leafs) || (iid == 0) ||
      (garam::abs(ilp) >= 0.5 * xsize) || (garam::abs(irp) >= 0.5 * xsize) ||
      (ilp > irp))
  {
    std::cout << "SetLeaf profile was ignored : " << iid << std::endl;
    return;
  }
  
  if (!calc_done) // initialize
  {
    lu.assign(n_leafs, 0.);
    ru.assign(n_leafs, 0.);
    ll.assign(n_leafs, 0.);
    rl.assign(n_leafs, 0.);
    calc_done = true;
  }
  
  if (iid > 0)
  {
    lu[iid-1] = ilp;
    ru[iid-1] = irp;
  }
  else
  {
    ll[-iid-1] = ilp;
    rl[-iid-1] = irp;
  }
}

//===================================================================================
G4RotationMatrix
MultiLeafCollimator::GetNextRotationMatrix(const G4RotationMatrix& R0)
{
  return R0;
}

//===================================================================================
G4ThreeVector MultiLeafCollimator::GetNextPosition(const G4ThreeVector&    V0,
                                                   const G4RotationMatrix& R0)
{
  return V0 + R0 * (zsize * zhat);
}

//===================================================================================
void MultiLeafCollimator::Generate(const G4RotationMatrix& R0,
                                   const G4ThreeVector&    V0,
                                         G4LogicalVolume*  mother_lv)
{
  const G4ThreeVector VC = V0 + R0 * (0.5 * zsize * zhat);
  G4Box* sv_top = new G4Box("sv_top", xsize, 0.5 * ysize, 0.5 * zsize);
  G4LogicalVolume* lv_top = new G4LogicalVolume(sv_top, bg_mat, "lv_top");
  lv_top->SetVisAttributes(G4VisAttributes::GetInvisible());
  G4RotationMatrix* rr = new G4RotationMatrix(R0);
  rr->invert();
  new G4PVPlacement(rr, VC, lv_top, "pv_top", mother_lv, false, 0);
  
  const G4int n_ltype = nleaf.size();
  G4int n_leafs = 0;
  for (G4int i = 0; i < n_ltype; ++i)
    n_leafs += nleaf[i];

  G4double ycnt = 0.0;
  G4LogicalVolume* lv = 0;
  G4String si, sk;
  for (G4int i = 0, k = 0; i < n_ltype; ++i)
  {
    si = garam::to_str(i);
    lv = new G4LogicalVolume(new G4Box(("sv_t" + si).c_str(), 
                                       0.25 * xsize, 
                                       0.5 * tleaf[i],
                                       0.5 * zsize), 
                             body_mat, 
                             ("lv_t" + si).c_str());
    lv->SetVisAttributes(new G4VisAttributes(cPurple));
    for (G4int j = 0; j < nleaf[i]; ++j, ++k)
    {
      if (k == 0)
        ycnt += 0.5 * tleaf[0];
      else if (j == 0)
        ycnt += 0.5 * (tleaf[i-1] + tleaf[i]);
      else 
        ycnt += tleaf[i];
      new G4PVPlacement(0, // upper left leaf
                        (-0.25 * xsize + lu[k]) * xhat + ycnt * yhat,
                        lv,
                        ("pv_l" + sk).c_str(),
                        lv_top,
                        false,
                        k);
      new G4PVPlacement(0, // upper right leaf
                        ( 0.25 * xsize + ru[k]) * xhat + ycnt * yhat,
                        lv,
                        ("pv_l" + sk).c_str(),
                        lv_top,
                        false,
                        n_leafs + k);
      new G4PVPlacement(0, // lower left leaf
                        (-0.25 * xsize + ll[k]) * xhat - ycnt * yhat,
                        lv,
                        ("pv_l" + sk).c_str(),
                        lv_top,
                        false,
                        2 * n_leafs + k);
      new G4PVPlacement(0, // lower right leaf
                        ( 0.25 * xsize + rl[k]) * xhat - ycnt * yhat,
                        lv,
                        ("pv_l" + sk).c_str(),
                        lv_top,
                        false,
                        3 * n_leafs + k);
    }
  }
}
