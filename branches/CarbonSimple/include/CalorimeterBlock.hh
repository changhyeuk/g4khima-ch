#ifndef CalorimeterBlock_hh_
#define CalorimeterBlock_hh_
//*****************************************************************************
//  CalorimeterBlock.hh
//                           - 2012-12-07, by Garam HAHN.
//*****************************************************************************

#include "globals.hh"
#include "BeamLineComponent.hh"

class CalorimeterBlock: public BeamLineComponent
{
public:
  CalorimeterBlock(const G4double ixsize,
                   const G4double iysize,
                   const G4double izsize,
                   const char*    ibody_mat);
  CalorimeterBlock(const G4double ixsize,
                   const G4double iysize,
                   const G4double izsize,
                   G4Material*    ibody_mat);
  ~CalorimeterBlock();

  G4RotationMatrix GetNextRotationMatrix(const G4RotationMatrix& R0);
  G4ThreeVector GetNextPosition(const G4ThreeVector&    V0,
                                const G4RotationMatrix& R0);

  void Generate(const G4RotationMatrix& R0,
                const G4ThreeVector&    V0,
                      G4LogicalVolume*  mother_lv);
  
  //void SetDICOMs();

private:
  G4double xsize;
  G4double ysize;
  G4double zsize;
  G4Material* body_mat;
  
  
};

#endif
