//  Slab.hh
#ifndef Slab_h
#define Slab_h 1

#include "Globals.hh"
#include "BeamLineComponent.hh"

class Slab : public BeamLineComponent
{
public:
    Slab(const G4double HalfX,
         const G4double HalfY,
         const G4double HalfZ,
         const char*    body_mat);
    
    Slab(const G4double HalfX,
         const G4double HalfY,
         const G4double HalfZ,
         G4Material* mat);
    
    ~Slab();
    
    Slab* New();
    Slab* New(const G4double HalfZ);
    
    G4RotationMatrix GetNextRotationMatrix(const G4RotationMatrix& R0);
    G4ThreeVector GetNextPosition(const G4ThreeVector&    V0,
                                  const G4RotationMatrix& R0);
    
    void Generate(const G4RotationMatrix& R0,
                  const G4ThreeVector&    V0,
                  G4LogicalVolume*  mother_lv);
    
private:
    
    G4double HalfX;
    G4double HalfY;
    G4double HalfZ;
    G4Material* body_mat;
    
};


#endif
