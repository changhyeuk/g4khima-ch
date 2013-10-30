#ifndef DipoleMagnet_h
#define DipoleMagnet_h 1

#include "globals.hh"
#include "BeamLineComponent.hh"

class DipoleMagnet : public BeamLineComponent
{
public:
  DipoleMagnet(const G4double tube_full_height,
               const G4double tube_full_width,
               const G4double bnd_angle,
               const G4double radii_curv,
               const G4double dir_angle,
               const G4double body_full_height,
               const G4double body_full_width,
               const char*    file_name);

  ~DipoleMagnet();
  
  void SetRectShape(){looks_rect = true;};
  
  DipoleMagnet* New();
  DipoleMagnet* NewScale(const G4double field_scale);

  G4RotationMatrix GetNextRotationMatrix(const G4RotationMatrix& R0);
  G4ThreeVector GetNextPosition(const G4ThreeVector&    V0,
                                const G4RotationMatrix& R0);

  void Generate(const G4RotationMatrix& R0,
                const G4ThreeVector&    V0,
                      G4LogicalVolume*  mother_lv);
  
  typedef std::vector<G4ThreeVector> v3s;
  typedef std::vector<G4ThreeVector>::iterator v3s_i;
  
private:
  G4double    tube_full_height;
  G4double    tube_full_width;
  G4double    body_full_height;
  G4double    body_full_width;

  G4double    bnd_angle;
  G4double    radii_curv;
  G4double    dir_angle;
  
  G4bool      looks_rect;
};

#endif
