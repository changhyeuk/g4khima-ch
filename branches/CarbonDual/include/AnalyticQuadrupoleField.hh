#ifndef AnalyticQuadrupoleField_hh_
#define AnalyticQuadrupoleField_hh_ 1
//*****************************************************************************
//  AnalyticQuadrupoleField.hh
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
class G4ElectroMagneticField;

class AnalyticQuadrupoleField: public G4ElectroMagneticField
{
public:
  AnalyticQuadrupoleField(const G4double          fgrad,
                          const G4RotationMatrix& R0,
                          const G4ThreeVector&    V0);
  ~AnalyticQuadrupoleField();
  
  AnalyticQuadrupoleField(const AnalyticQuadrupoleField& r);
  AnalyticQuadrupoleField& operator=(const AnalyticQuadrupoleField& p);
  
  void GetFieldValue(const G4double x[], G4double f[]) const;
  G4bool DoesFieldChangeEnergy() const {return false;};

private:
  G4double         fG; // focusing gradient
  G4RotationMatrix R0;
  G4ThreeVector    V0;

};

#endif
