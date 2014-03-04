#ifndef VirtualMonitor_hh_
#define VirtualMonitor_hh_
//*****************************************************************************
//  VirtualMonitor.hh
//                           - 2012-12-17, by Garam HAHN.
//*****************************************************************************

#include "globals.hh"
#include "BeamLineComponent.hh"

class VirtualMonitor: public BeamLineComponent
{
public:
  VirtualMonitor(const G4double ixsize, const G4double iysize);
  ~VirtualMonitor();
  
  VirtualMonitor* New();

  G4RotationMatrix GetNextRotationMatrix(const G4RotationMatrix& R0);
  G4ThreeVector GetNextPosition(const G4ThreeVector&    V0,
                                const G4RotationMatrix& R0);

  void Generate(const G4RotationMatrix& R0,
                const G4ThreeVector&    V0,
                      G4LogicalVolume*  mother_lv);


private:
  G4double xsize;
  G4double ysize;


private:
  
};

#endif
