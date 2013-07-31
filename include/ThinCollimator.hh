#ifndef ThinCollimator_hh_
#define ThinCollimator_hh_
//*****************************************************************************
//  ThinCollimator.hh
//                           - 2012-12-07, by Garam HAHN.
//*****************************************************************************

// One axis resizable collimator, not really thin collimator.
//  the name "thin" means that length of this collimator is not included
//  when beam line length calculation, but this occupy space!!
// For example,
//  if made beam line structure is like Drift2m-ThinCol1cm-Drift2m,
//  total beam line length is 4 m, not 4.01 m.
//  but, collimator will occupy 0.5 cm of pre-Drift and 0.5 cm of post-Drift.
// Exclusion of length calculation is aimed to ease beamline tunning.

#include "globals.hh"
#include "BeamLineComponent.hh"

class ThinCollimator : public BeamLineComponent
{
public:
  ThinCollimator(const G4double ihalf_gap,
                 const G4double ithickness,
                 const G4double ix_size,
                 const G4double iy_size,
                 const G4double irot_angle = 0.,
                 const G4String inist_name = "G4_Ta"); // tantalum default
  ~ThinCollimator();

  ThinCollimator* New();
  ThinCollimator* NewGap(const G4double ihalf_gap,
                         const G4double irot_angle = 0.);
  
  void SetMaterial(G4Material* m){col_mat = m;};

  G4RotationMatrix GetNextRotationMatrix(const G4RotationMatrix& R0);
  G4ThreeVector GetNextPosition(const G4ThreeVector&    V0,
                                const G4RotationMatrix& R0);

  void Generate(const G4RotationMatrix& R0,
                const G4ThreeVector&    V0,
                      G4LogicalVolume*  mother_lv);

private:
  G4double    half_gap;
  G4double    z_length;
  G4double    x_length;
  G4double    y_length;
  G4double    rot_angle;
  G4Material* col_mat;
  
};

#endif
