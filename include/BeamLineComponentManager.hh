#ifndef BeamLineComponentManager_h
#define BeamLineComponentManager_h 1

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class BeamLineComponent;
class G4Materail;

class BeamLineComponentManager
{
public:
  BeamLineComponentManager(const G4double energy_per_nucleon,
                           const G4double Q_over_A = 1.0,
                           const char*    nistname = "G4_Galactic",
                           const G4double world_margin = 1.0 * m);
  ~BeamLineComponentManager();

  void Add(BeamLineComponent* blc);

  G4VPhysicalVolume* GenerateVolume();

  G4double GetQoA(){ return q_over_a; };
  G4double GetEnergyPerNucleon(){ return e_over_u; };

  void SetBGMaterial(G4Material* ibg_mat);
  void SetBGMaterial(const char* inistname);
  
  void SetWorldMargin(const G4double iw_margin);
  
  static void SetInitRotationMatrix(const G4RotationMatrix& iR);
  static G4RotationMatrix GetInitRotationMatrix();
  
  static void SetInitVector(const G4ThreeVector& iV);
  static G4ThreeVector GetInitVector();

private:
  static G4RotationMatrix         R_init;
  static G4ThreeVector            V_init;
  std::vector<BeamLineComponent*> devices;
  G4double q_over_a;
  G4double e_over_u;
  G4Material* bg_mat;
  G4double world_margin;
};

#endif
