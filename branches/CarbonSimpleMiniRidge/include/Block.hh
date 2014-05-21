#ifndef Block_h
#define Block_h 1

#include "globals.hh"
#include "BeamLineComponent.hh"

class Block : public BeamLineComponent
{
public:
  Block(const G4double thickness,
        const G4double total_length,
        const char*    body_mat);

  Block(const G4double    thickness,
        const G4double total_length,
              G4Material* body_mat);

  ~Block();

  Block* New();
  Block* New(const G4double thickness);

  G4RotationMatrix GetNextRotationMatrix(const G4RotationMatrix& R0);
  G4ThreeVector GetNextPosition(const G4ThreeVector&    V0,
                                const G4RotationMatrix& R0);

  void Generate(const G4RotationMatrix& R0,
                const G4ThreeVector&    V0,
                      G4LogicalVolume*  mother_lv);

private:
  G4double    thickness;
  G4double    total_length;
  G4Material* body_mat;
};

#endif
