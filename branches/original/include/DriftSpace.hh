#ifndef DriftSpace_h
#define DriftSpace_h 1

//*****************************************************************************
//  DriftSpace.hh
//                           - 2012-11-17, by Garam HAHN.
//*****************************************************************************

#include "globals.hh"
#include "BeamLineComponent.hh"

class DriftSpace: public BeamLineComponent
{
public:
  DriftSpace(const G4double length);
  ~DriftSpace();
  
  DriftSpace* New();
  DriftSpace* New(const G4double length);
  
  G4RotationMatrix GetNextRotationMatrix(const G4RotationMatrix& R0);
  G4ThreeVector GetNextPosition(const G4ThreeVector&    V0,
                                const G4RotationMatrix& R0);

  void Generate(const G4RotationMatrix& R0,
                const G4ThreeVector&    V0,
                      G4LogicalVolume*  mother_lv);


private:
  G4double  length;

};

#endif
