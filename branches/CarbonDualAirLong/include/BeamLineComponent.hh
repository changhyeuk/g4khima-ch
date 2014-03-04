#ifndef BeamLineComponent_h
#define BeamLineComponent_h 1

#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Colour.hh"

#include "BeamLineComponentManager.hh"

class TrackerSD;
class CalorimeterSD;
class GridBField;

enum BLCType
{
  None = -1,
  Drift,
  Dipole,
  ScanDipole,
  Quadrupole,
  Solenoid,
  DummyBlock,
  BLWaterPhantom,
  BLSlab,
  BLRangeShifter,
  ThinCol,
  VirtualMon,
  Calo,
  MLC,  //to do
  Ridge, //to do
  GasBox //to do
};


class BeamLineComponent
{
public:
  BeamLineComponent(const BLCType itype);
  ~BeamLineComponent();

public:
  virtual G4RotationMatrix GetNextRotationMatrix(const G4RotationMatrix& priv_rotm) = 0;
  virtual G4ThreeVector GetNextPosition(const G4ThreeVector& priv_posv,
                                        const G4RotationMatrix& priv_rotm) = 0;

  virtual void Generate(const G4RotationMatrix& priv_rotm,
                        const G4ThreeVector&    priv_posv,
                              G4LogicalVolume*  mother_lv ) = 0;

  BLCType GetType() const { return type; };
  void SetBeamLineComponentManager(BeamLineComponentManager* i_blcm)  { blcm = i_blcm; };
  void SetFieldScale(const G4double field_scale_factor);

protected:
  BeamLineComponentManager* blcm;
  BLCType      type;
  G4double     field_scale_factor;
  G4String     filename;
  //char         filename[100];

  static G4ThreeVector xhat;
  static G4ThreeVector yhat;
  static G4ThreeVector zhat;
  static G4ThreeVector origin;

  static G4Material*   mVacuum;

  static G4Colour      cRed;
  static G4Colour      cOrange;
  static G4Colour      cYellow;
  static G4Colour      cGreen;
  static G4Colour      cBlue;
  static G4Colour      cSkyBlue;
  static G4Colour      cPurple;

  static G4double      mon_t;
  static G4double      mon_step;
  static G4int         mon_id;

public:  
  static G4Material* GetG4NistMaterial(const char* nistname);
};


#endif
