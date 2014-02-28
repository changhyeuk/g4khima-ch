#ifndef AnalyticDipoleField_hh_
#define AnalyticDipoleField_hh_ 1
//*****************************************************************************
//  AnalyticDipoleField.hh
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
class G4ElectroMagneticField;

class AnalyticDipoleField: public G4ElectroMagneticField
{
public:
  AnalyticDipoleField(const G4double fldstr,
                      const G4RotationMatrix& R,
                      const G4ThreeVector& V);
  AnalyticDipoleField(const G4double w_per_u,    // [KE/u]
                      const G4double q_over_a,   // [Q/A]
                      const G4double radii_curv,
                      const G4RotationMatrix& R,
                      const G4ThreeVector& V);
  ~AnalyticDipoleField();
  
  AnalyticDipoleField(const AnalyticDipoleField& r);
  AnalyticDipoleField& operator=(const AnalyticDipoleField& r);
  
  void GetFieldValue(const G4double x[], G4double f[] ) const;
  G4bool   DoesFieldChangeEnergy() const {return false;};
  
private:
  G4double         fB;
  G4RotationMatrix R0;
  G4ThreeVector    V0;
};

#endif
