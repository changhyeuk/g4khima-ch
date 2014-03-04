#ifndef GridBField_HH_
#define GridBField_HH_ 1

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4ElectroMagneticField.hh"
class LinInterpolation;

class GridBField : public G4ElectroMagneticField
{
public:
  GridBField(      LinInterpolation* lintp, 
             const double            scale_factor,
             const G4RotationMatrix& R0,
             const G4ThreeVector&    V0);
  GridBField(const GridBField& r);
  ~GridBField();
  
  GridBField& operator=(const GridBField& p);

  void GetFieldValue(const G4double x[],
                           G4double f[] ) const;
  G4bool DoesFieldChangeEnergy() const {return false;};

private:
  LinInterpolation* lintp;
  G4double          scale_factor;
  G4RotationMatrix  R0;
  G4ThreeVector     V0;
};

#endif // GRIDFIELD_HH_

