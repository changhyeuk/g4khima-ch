//*****************************************************************************
//  DipoleMagnet.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "DipoleMagnet.hh"
#include "AnalysisManager.hh"
#include "FieldTable.hh"
#include "LinInterpolation.hh"
#include "GridBField.hh"
#include "GaramGlobal.hh"
#include "AnalyticDipoleField.hh"
#include "TrackerSD.hh"

#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4TessellatedSolid.hh"
#include "G4QuadrangularFacet.hh"
#include "G4VFacet.hh"
#include "G4Box.hh"
#include "G4UserLimits.hh"

//=================================================================================
DipoleMagnet::DipoleMagnet(const G4double tfh,
                           const G4double tfw,
                           const G4double ba,
                           const G4double rc,
                           const G4double da,
                           const G4double bfh,
                           const G4double bfw,
                           const char*    fn)
  :BeamLineComponent(Dipole), 
   tube_full_height(tfh), tube_full_width(tfw), 
   bnd_angle(ba), radii_curv(rc), 
   body_full_height(bfh), body_full_width(bfw), 
   dir_angle(da),
   looks_rect(false)
{
  filename = fn;
}

//=================================================================================
DipoleMagnet::~DipoleMagnet()
{
}

//=================================================================================
DipoleMagnet* DipoleMagnet::New()
{
  DipoleMagnet* t
    = new DipoleMagnet(tube_full_height,
                       tube_full_width,
                       bnd_angle,
                       radii_curv,
                       dir_angle,
                       body_full_height,
                       body_full_width,
                       filename);
  t->looks_rect = looks_rect;
  t->field_scale_factor = field_scale_factor;
  return t;
}

//=================================================================================
DipoleMagnet* DipoleMagnet::NewScale(const G4double if_scale)
{
  DipoleMagnet* t
    = new DipoleMagnet(tube_full_height,
                       tube_full_width,
                       bnd_angle,
                       radii_curv,
                       dir_angle,
                       body_full_height,
                       body_full_width,
                       filename);
  t->looks_rect = looks_rect;
  t->field_scale_factor = if_scale;
  return t;
}

//=================================================================================
G4RotationMatrix
DipoleMagnet::GetNextRotationMatrix(const G4RotationMatrix& R0)
{
  G4RotationMatrix R(R0);
  R.rotate(bnd_angle, R0 * yhat);
  R.rotate(dir_angle, R0 * zhat);
  
  return R;
}

//=================================================================================
G4ThreeVector
DipoleMagnet::GetNextPosition(const G4ThreeVector&    V0,
                              const G4RotationMatrix& R0)
{
  G4RotationMatrix R(R0);
  R.rotate(0.5 * bnd_angle, R0 * yhat);
  R.rotate(dir_angle, R0 * zhat);

  const G4double l = radii_curv * std::sqrt(2.0 * (1. - std::cos(bnd_angle)));
  return V0 + R * (l * zhat);
}

//=================================================================================
void DipoleMagnet::Generate(const G4RotationMatrix& R0,
                            const G4ThreeVector&    V0,
                                  G4LogicalVolume*  mother_lv)
{
  const G4double hba   = 0.5 * bnd_angle;
  const G4double cleng = radii_curv * std::sqrt(2.0 * (1. - std::cos(hba)));
  const G4double fleng = radii_curv * std::sqrt(2.0 * (1. - std::cos(bnd_angle)));
  
  G4RotationMatrix R_quarter(R0);
  R_quarter.rotate(0.25 * bnd_angle, R0 * yhat);
  R_quarter.rotate(dir_angle, R0 * zhat);
  G4RotationMatrix R_half(R0);
  R_half.rotate(0.5 * bnd_angle, R0 * yhat);
  R_half.rotate(dir_angle, R0 * zhat);
  G4RotationMatrix R_all(R0);
  R_all.rotate(bnd_angle, R0 * yhat);
  R_all.rotate(dir_angle, R0 * zhat);
  const G4ThreeVector V_half = V0 + R_quarter * (cleng * zhat);
  const G4ThreeVector V_all = V0 + R_half * (fleng * zhat);

  // Magnetic field definition
  //-------------------------------------------------------------------------------
  FieldTable::GetInstance()->RegisterLinIntpolation(filename.c_str());
  FieldTable::GetInstance()->AddFieldEntry(filename.c_str(), V_half, R_half, field_scale_factor);

  // key variables definition
  //-------------------------------------------------------------------------------
  const G4int     N = 2 * 3;
  const G4double dN = 1./G4double(N);

  const G4ThreeVector ty = 0.51 * body_full_height * yhat;
  const G4ThreeVector by = 0.5 * body_full_height * yhat;
  const G4ThreeVector vy = 0.5 * tube_full_height * yhat;

  // beam comes from front, and goes to back directions
  const G4ThreeVector front = G4ThreeVector(0., 0., -1.).rotateY(-hba);
  const G4ThreeVector back  = G4ThreeVector(0., 0., 1.).rotateY(hba);

  v3s p;  // base volume point vectorss, along beam orbit
  G4double a;  G4ThreeVector v; // temporary variables
  for (G4int i = 0; i <= N; ++i)
  {
    a = - hba + dN * G4double(i) * bnd_angle;
    v = radii_curv * G4ThreeVector(1.0 - std::cos(a), 0., std::sin(a));
    p.push_back(v);
  }
  
  //-------------------------------------------------------------------------------
  v3s t1, t2, t3, t4; // t# : top-virtual volume point array
  v3s b1, b2, b3, b4; // b# : yoke-body volume point array
  v3s v1, v2, v3, v4; // v# : beam-tube volume point array
  
  for (G4int i = 0, j = 0; i != (G4int)p.size(); ++i)
  {
    G4ThreeVector rhat, tx, bx, vx;
    if (looks_rect) // Rectangular edge
    {
      rhat = xhat;
    }
    else // Sector edge
    {
      j = i;
      a = - hba + dN * G4double(j) * bnd_angle;
      rhat = - G4ThreeVector(-std::cos(a), 0., std::sin(a));
    }
    tx = 0.51 * body_full_width * rhat;
    bx = 0.5 * body_full_width * rhat;
    vx = 0.5 * tube_full_width * rhat;

    t1.push_back(p[i] + tx + ty);
    t2.push_back(p[i] - tx + ty);
    t3.push_back(p[i] - tx - ty);
    t4.push_back(p[i] + tx - ty);
    
    b1.push_back(p[i] + bx + by);
    b2.push_back(p[i] - bx + by);
    b3.push_back(p[i] - bx - by);
    b4.push_back(p[i] + bx - by);

    v1.push_back(p[i] + vx + vy);
    v2.push_back(p[i] - vx + vy);
    v3.push_back(p[i] - vx - vy);
    v4.push_back(p[i] + vx - vy);
  }
  
  // beam tube and body solid volume define
  //-----------------------------------------------------------------------------
  G4int NT = t1.size();
  G4int NB = b1.size();

  G4TessellatedSolid* sv_top = new G4TessellatedSolid("sv_top");
  G4TessellatedSolid* sv_body = new G4TessellatedSolid("sv_body");

  // front side : beam comes from.
  sv_top->AddFacet((G4VFacet*) 
                   new G4QuadrangularFacet(t4[0], t3[0], t2[0], t1[0],
                                           ABSOLUTE));
  sv_body->AddFacet((G4VFacet*) 
                   new G4QuadrangularFacet(b1[0], v1[0], v2[0], b2[0],
                                           ABSOLUTE));
  sv_body->AddFacet((G4VFacet*) 
                   new G4QuadrangularFacet(b2[0], v2[0], v3[0], b3[0],
                                           ABSOLUTE));
  sv_body->AddFacet((G4VFacet*) 
                   new G4QuadrangularFacet(b3[0], v3[0], v4[0], b4[0],
                                           ABSOLUTE));
  sv_body->AddFacet((G4VFacet*) 
                   new G4QuadrangularFacet(b4[0], v4[0], v1[0], b1[0],
                                           ABSOLUTE));
  // back side : beam goes out.
  sv_top->AddFacet((G4VFacet*)
                   new G4QuadrangularFacet(t1[NT-1], t2[NT-1], t3[NT-1], t4[NT-1],
                                           ABSOLUTE));
  sv_body->AddFacet((G4VFacet*) 
                   new G4QuadrangularFacet(v1[NB-1], b1[NB-1], b2[NB-1], v2[NB-1],
                                           ABSOLUTE));
  sv_body->AddFacet((G4VFacet*) 
                   new G4QuadrangularFacet(v2[NB-1], b2[NB-1], b3[NB-1], v3[NB-1],
                                           ABSOLUTE));
  sv_body->AddFacet((G4VFacet*) 
                   new G4QuadrangularFacet(v3[NB-1], b3[NB-1], b4[NB-1], v4[NB-1],
                                           ABSOLUTE));
  sv_body->AddFacet((G4VFacet*) 
                   new G4QuadrangularFacet(v4[NB-1], b4[NB-1], b1[NB-1], v1[NB-1],
                                           ABSOLUTE));

  for (G4int i = 0; i != NT - 1; ++i)
  {
    sv_top->AddFacet((G4VFacet*)
                     new G4QuadrangularFacet(t4[i], t1[i], t1[i+1], t4[i+1],
                                             ABSOLUTE));
    sv_top->AddFacet((G4VFacet*)
                     new G4QuadrangularFacet(t1[i], t2[i], t2[i+1], t1[i+1],
                                             ABSOLUTE));
    sv_top->AddFacet((G4VFacet*)
                     new G4QuadrangularFacet(t2[i], t3[i], t3[i+1], t2[i+1],
                                             ABSOLUTE));
    sv_top->AddFacet((G4VFacet*)
                     new G4QuadrangularFacet(t3[i], t4[i], t4[i+1], t3[i+1],
                                             ABSOLUTE));
  }
  sv_top->SetSolidClosed(true);


  for (G4int i = 0; i != NB - 1; ++i)
  {
    sv_body->AddFacet((G4VFacet*) 
                     new G4QuadrangularFacet(b4[i], b1[i], b1[i+1], b4[i+1],
                                             ABSOLUTE));
    sv_body->AddFacet((G4VFacet*) 
                     new G4QuadrangularFacet(b1[i], b2[i], b2[i+1], b1[i+1],
                                             ABSOLUTE));
    sv_body->AddFacet((G4VFacet*) 
                     new G4QuadrangularFacet(b2[i], b3[i], b3[i+1], b2[i+1],
                                             ABSOLUTE));
    sv_body->AddFacet((G4VFacet*) 
                     new G4QuadrangularFacet(b3[i], b4[i], b4[i+1], b3[i+1],
                                             ABSOLUTE));
    sv_body->AddFacet((G4VFacet*) 
                     new G4QuadrangularFacet(v4[i+1], v1[i+1], v1[i], v4[i],
                                             ABSOLUTE));
    sv_body->AddFacet((G4VFacet*) 
                     new G4QuadrangularFacet(v1[i+1], v2[i+1], v2[i], v1[i],
                                             ABSOLUTE));
    sv_body->AddFacet((G4VFacet*) 
                     new G4QuadrangularFacet(v2[i+1], v3[i+1], v3[i], v2[i],
                                             ABSOLUTE));
    sv_body->AddFacet((G4VFacet*) 
                     new G4QuadrangularFacet(v3[i+1], v4[i+1], v4[i], v3[i],
                                             ABSOLUTE));
  }
  sv_body->SetSolidClosed(true);
  
  
  // log. phys. volume of top, body(yoke) and beam tube definitions
  //-----------------------------------------------------------------------------
  G4LogicalVolume* lv_top 
    = new G4LogicalVolume(sv_top, mother_lv->GetMaterial(), "lv_top");
  lv_top->SetVisAttributes(new G4VisAttributes(cOrange));
  G4RotationMatrix* rr = new G4RotationMatrix(R_half);
  rr->invert();
  new G4PVPlacement(rr,
                    V_half,
                    lv_top,
                    "p_top",
                    mother_lv,
                    false,
                    0);
  // body and beam tube
  G4LogicalVolume* lv_body = new G4LogicalVolume(sv_body,
                                                 GetG4NistMaterial("G4_Fe"),
                                                 "lv_body");
  new G4PVPlacement(0,
                    origin,
                    lv_body,
                    "p_body",
                    lv_top,
                    false,
                    0);
  
  // beam monitors : front and back
  //-----------------------------------------------------------------------------
  
  G4RotationMatrix* R_mon_front = 0;
  G4RotationMatrix* R_mon_back = 0;
  if (!looks_rect) // Rectangular edge
  {
    R_mon_front = new G4RotationMatrix(G4RotationMatrix::IDENTITY);
    R_mon_front->rotateY(-0.5 * bnd_angle);
    R_mon_front->rotateZ(dir_angle);
    R_mon_front->invert();
    R_mon_back = new G4RotationMatrix(G4RotationMatrix::IDENTITY);
    R_mon_back->rotateY(0.5 * bnd_angle);
    R_mon_back->rotateZ(dir_angle);
    R_mon_back->invert();
  }
  
  G4Box* sv_mon = new G4Box("smon", 
                            0.48 * tube_full_width,
                            0.48 * tube_full_height,
                            0.5 * mon_t);
  G4LogicalVolume* lv_mon_front 
    = new G4LogicalVolume(sv_mon,
                          mother_lv->GetMaterial(),
                          "lv_mon_front",
                          0,
                          new TrackerSD("RBi", R0, V0));
  lv_mon_front->SetVisAttributes(new G4VisAttributes(cRed));
  new G4PVPlacement(R_mon_front,
                    p.front() + 0.5 * mon_t * front,
                    lv_mon_front,
                    "pv_mon_front",
                    lv_top,
                    false,
                    0);
  G4LogicalVolume* lv_mon_back 
    = new G4LogicalVolume(sv_mon,
                          mother_lv->GetMaterial(),
                          "lv_mon_back",
                          0,
                          new TrackerSD("RBf", R_all, V_all));
  lv_mon_back->SetVisAttributes(new G4VisAttributes(cRed));
  new G4PVPlacement(R_mon_back,
                    p.back() - 0.5 * mon_t * back,
                    lv_mon_back,
                    "pv_mon_back",
                    lv_top,
                    false,
                    0);

}



