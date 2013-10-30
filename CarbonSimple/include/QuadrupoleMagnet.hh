#ifndef QuadrupoleMagnet_h
#define QuadrupoleMagnet_h 1
//*****************************************************************************
//  QuadrupoleMagnet.hh
//                           - 2012-10-02, by Garam HAHN.
//*****************************************************************************

#include "globals.hh"
#include "BeamLineComponent.hh"

class QuadrupoleMagnet: public BeamLineComponent
{
public:
  QuadrupoleMagnet(const G4double field_grad,
                   const G4double eff_length,
                   const G4double tube_r, 
                   const G4double body_r,
                   const char*    file_name);

  ~QuadrupoleMagnet();

  QuadrupoleMagnet* New();
  QuadrupoleMagnet* NewScale(const G4double field_scale = 1.0);
  QuadrupoleMagnet* NewGradient(const G4double field_grad);

  G4RotationMatrix GetNextRotationMatrix(const G4RotationMatrix& R0);
  G4ThreeVector GetNextPosition(const G4ThreeVector&    V0,
                                const G4RotationMatrix& R0);

  void Generate( const G4RotationMatrix& R0,
                 const G4ThreeVector&    V0,
                       G4LogicalVolume*  mother_lv );

private:
  G4double    field_grad;
  G4double    eff_length;

  G4double    tube_r;
  G4double    body_r;
};

#endif
