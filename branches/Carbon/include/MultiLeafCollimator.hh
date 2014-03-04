#ifndef MultiLeafCollimator_hh_
#define MultiLeafCollimator_hh_
//*****************************************************************************
//  MultiLeafCollimator.hh
//                           - 2012-12-07, by Garam HAHN.
//*****************************************************************************

#include "globals.hh"
#include "BeamLineComponent.hh"

class MultiLeafCollimator: public BeamLineComponent
{
public:
  MultiLeafCollimator(const G4double ixsize,
                      const G4double iysize,
                      const G4double izsize,
                      const char*    ibody_nistname,
                      const char*    ibg_nistname);
  MultiLeafCollimator(const G4double    ixsize,
                      const G4double    iysize,
                      const G4double    izsize,
                            G4Material* ibody_mat,
                            G4Material* ibg_mat);
  ~MultiLeafCollimator();

  G4RotationMatrix GetNextRotationMatrix(const G4RotationMatrix& R0);
  G4ThreeVector GetNextPosition(const G4ThreeVector&    V0,
                                const G4RotationMatrix& R0);

  void Generate(const G4RotationMatrix& R0,
                const G4ThreeVector&    V0,
                      G4LogicalVolume*  mother_lv);

public:
  MultiLeafCollimator* New();
  void DefineLeaf(const G4int    inleaf,
                  const G4double ithick);
  void SetLeaf(const G4int    iid,
               const G4double ilp,
               const G4double irp);
  
private:
  G4bool   calc_done;
  G4double xsize;
  G4double ysize;
  G4double zsize;
  std::vector<G4int>    nleaf; // number of identical leafs
  std::vector<G4double> tleaf; // each leaf thickness
  std::vector<G4double> lu;    // left leaf upper
  std::vector<G4double> ll;    // left leaf lower
  std::vector<G4double> ru;    // right leaf upper
  std::vector<G4double> rl;    // right leaf lower
  G4Material* body_mat;
  G4Material* bg_mat;
};

#endif
