//
//  RangeShifter.hh
//  projg4khima
//
//  Created by Kim Chang Hyeuk on 13. 10. 1..
//
//

#ifndef RangeShifter_h
#define RangeShifter_h 1

#include "Globals.hh"
#include "BeamLineComponent.hh"

class RangeShifter : public BeamLineComponent
{
public:
    RangeShifter(const G4double iHalfX,
                 const G4double iHalfY,
                 const G4double iHalfZ,
                 const G4int    iRSFN,
                 const char*    ibg_name,
                 const char*    ibody_name);
    
    RangeShifter(const G4double iHalfX,
                 const G4double iHalfY,
                 const G4double iHalfZ,
                 const G4int    iRSFN,
                       G4Material* ibg_mat,
                       G4Material* ibody_mat);
    
    ~RangeShifter();
    
    RangeShifter* New();
    
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
    G4int    RSFN;
    G4Material* bg_mat;
    G4Material* body_mat;
    G4double RSF_p;
    G4double RSF_t;
    
};


#endif
