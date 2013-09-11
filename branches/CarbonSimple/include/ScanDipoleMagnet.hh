#ifndef ScanDipoleMagnet_hh_
#define ScanDipoleMagnet_hh_
//*****************************************************************************
//  ScanDipoleMagnet.hh
//                           - 2012-12-07, by Garam HAHN.
//*****************************************************************************

#include "globals.hh"
#include "BeamLineComponent.hh"

class ScanDipoleMagnet : public BeamLineComponent
{
public:
  ScanDipoleMagnet(const G4double tube_full_height,
                   const G4double tube_full_width,
                   const G4double body_full_height,
                   const G4double body_full_width,
                   const G4double eff_length,
                   const G4double field_strength,
                   const G4double field_exp_scale,
                   const char*    body_mat);
  ScanDipoleMagnet(const G4double    tube_full_height,
                   const G4double    tube_full_width,
                   const G4double    body_full_height,
                   const G4double    body_full_width,
                   const G4double    eff_length,
                   const G4double    field_strength,
                   const G4double    field_exp_scale,
                         G4Material* body_mat);
  ~ScanDipoleMagnet();
  
  ScanDipoleMagnet* New();
  ScanDipoleMagnet* New(const G4double B);
  ScanDipoleMagnet* New(const G4double field_scale,
                        const char*    filename);

  G4RotationMatrix GetNextRotationMatrix(const G4RotationMatrix& R0);
  G4ThreeVector GetNextPosition(const G4ThreeVector&    V0,
                                const G4RotationMatrix& R0);

  void Generate(const G4RotationMatrix& R0,
                const G4ThreeVector&    V0,
                      G4LogicalVolume*  mother_lv);
  
private:
  G4double    tube_full_height;
  G4double    tube_full_width;
  
  G4double    body_full_height;
  G4double    body_full_width;
  G4double    eff_length;

  G4double    field_strength;
  G4double    field_exp_scale;
  
  G4Material* body_mat;
};

#endif
