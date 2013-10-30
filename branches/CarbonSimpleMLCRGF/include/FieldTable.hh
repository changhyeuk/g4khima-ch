#ifndef FieldTable_HH_
#define FieldTable_HH_ 1

#include "globals.hh"
#include <list>
#include <vector>
#include "LinInterpolation.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class FieldTable
{
public:
  FieldTable();
  ~FieldTable();
  static FieldTable* GetInstance();
  static void Dispose();
  LinInterpolation* GetIntp(const char* fname);
  void RegisterLinIntpolation(const char*  filename,
                              const int    line_skip_first = 8,
                              const int    line_read = 0,
                              const double length_unit = cm,
                              const double field_unit = gauss);
  void AddFieldEntry(const char* filename,
                     const G4ThreeVector& V0,
                     const G4RotationMatrix& R0,
                     const double scale);
  void GetGlobalB(const G4double x[],
                        G4double f[]);

private:
  static FieldTable* instance;
  std::list<LinInterpolation*>* ftable;

  std::vector<G4ThreeVector>*      v0s;
  std::vector<G4RotationMatrix>*   r0s;
  std::vector<double>*             scales;
  std::vector<LinInterpolation*>*  frefs;

};


#endif // FieldTable_HH_

