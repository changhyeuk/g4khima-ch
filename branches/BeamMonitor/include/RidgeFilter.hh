#ifndef RidgeFilter_hh_
#define RidgeFilter_hh_
//*****************************************************************************
//  RidgeFilter.hh
//                           - 2012-12-07, by Garam HAHN.
//*****************************************************************************

#include "globals.hh"
#include "BeamLineComponent.hh"

class RidgeFilter: public BeamLineComponent
{
public:
  RidgeFilter(const G4double ixsize,
              const G4double iysize,
              const char*    ibody_nistname,
              const char*    ibg_nistname);
  RidgeFilter(const G4double    ixsize,
              const G4double    iysize,
                    G4Material* ibody_mat,
                    G4Material* ibg_mat);
  ~RidgeFilter();

  G4RotationMatrix GetNextRotationMatrix(const G4RotationMatrix& R0);
  G4ThreeVector GetNextPosition(const G4ThreeVector&    V0,
                                const G4RotationMatrix& R0);

  void Generate(const G4RotationMatrix& R0,
                const G4ThreeVector&    V0,
                      G4LogicalVolume*  mother_lv);

public:
  RidgeFilter* New();
  void Add2DCrxPoint(const G4double ix, const G4double iz);
  void AddSpace(const G4double igap);

private:
  G4double xsize;
  G4double ysize;
  G4double zsize;
  G4double gapsize;
  std::vector<G4double> xs;
  std::vector<G4double> zs;
  G4Material* body_mat;
  G4Material* bg_mat;
};

#endif
