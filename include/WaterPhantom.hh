// Water Phantom.hh
#ifndef WaterPhantom_h
#define WaterPhantom_h 1

#include "Globals.hh"
#include "BeamLineComponent.hh"

class WaterPhantom : public BeamLineComponent
{
public:

    WaterPhantom(const G4double HalfX,
                 const G4double HalfY,
                 const G4double HalfZ,
                 const char*    body_mat);

    WaterPhantom(const G4double HalfX,
                 const G4double HalfY,
                 const G4double HalfZ,
                       G4Material* mat);

   
    ~WaterPhantom();
    
    WaterPhantom* New();
    WaterPhantom* New(const G4double HalfZ); 
    
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
