#ifndef GlobalGridBField_HH_
#define GlobalGridBField_HH_ 1

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4ElectroMagneticField.hh"
#include "FieldTable.hh"

class GlobalGridBField : public G4ElectroMagneticField
{
public:
  GlobalGridBField();
  GlobalGridBField(const GlobalGridBField& r);
  ~GlobalGridBField();
  
  GlobalGridBField& operator=(const GlobalGridBField& p);

  void GetFieldValue(const G4double x[],
                           G4double f[] ) const;
  G4bool DoesFieldChangeEnergy() const {return false;};

private:
  FieldTable* ft;
};

#endif // GLOBALGRIDFIELD_HH_

