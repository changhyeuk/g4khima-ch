//*****************************************************************************
//  VirtualMonitor.cc
//    - this must be used with drift space.
//    - this does not occupy space, therefore this can be superposition with
//      other component!!
//    - using some trick, this can be top volume!!! but... not yet tested/
//                           - 2012-12-17, by Garam HAHN.
//*****************************************************************************

#include "VirtualMonitor.hh"
#include "TrackerSD.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

//=============================================================================
VirtualMonitor::VirtualMonitor(const G4double ixsize, 
                               const G4double iysize)
  :BeamLineComponent(VirtualMon), xsize(ixsize), ysize(iysize)
{
}

//=============================================================================
VirtualMonitor::~VirtualMonitor()
{

}

//=============================================================================
VirtualMonitor* VirtualMonitor::New()
{
    G4cout<<" x size :"<<xsize<<G4endl;
    
  return new VirtualMonitor(xsize, ysize);
}

//==========================================================================
G4RotationMatrix 
VirtualMonitor::GetNextRotationMatrix(const G4RotationMatrix& R0)
{
  return R0;
}

//==========================================================================
G4ThreeVector 
VirtualMonitor::GetNextPosition(const G4ThreeVector&    V0,
                                const G4RotationMatrix&)
{
  return V0;
}

//==========================================================================
void VirtualMonitor::Generate(const G4RotationMatrix& R0,
                              const G4ThreeVector&    V0,
                                    G4LogicalVolume*  mother_lv)
{
    std::cout << "V0 : " << V0 << std::endl;
    std::cout << "Vadd : " << R0 * (0.5 * mon_t * zhat) << std::endl;
  const G4ThreeVector Vmon = V0 + R0 * (0.5 * mon_t * zhat);
  G4Box* s_mon0 = new G4Box("smon", xsize, ysize, 0.5 * mon_t);
  G4LogicalVolume* l_mon0 = new G4LogicalVolume(s_mon0,
                                                mother_lv->GetMaterial(), 
                                                "lmon", 0,
                                                new TrackerSD("VM", R0, Vmon));
  l_mon0->SetVisAttributes(new G4VisAttributes(cRed));
  G4RotationMatrix* IR = new G4RotationMatrix(R0);
  IR->invert();
  new G4PVPlacement(IR, V0, l_mon0, "p_mon", mother_lv, false, 0);
}



