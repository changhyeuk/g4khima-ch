//*****************************************************************************
//  FieldTable.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "FieldTable.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

#include <list>
#include <vector>
#include <algorithm>

FieldTable* FieldTable::instance = 0;

//===================================================================================
FieldTable::FieldTable()
{
  ftable = new std::list<LinInterpolation*>();
  v0s = new std::vector<G4ThreeVector>();
  r0s = new std::vector<G4RotationMatrix>();
  scales = new std::vector<double>();
  frefs = new std::vector<LinInterpolation*>();
}

//===================================================================================
FieldTable::~FieldTable()
{
  for (std::list<LinInterpolation*>::iterator it = ftable->begin();
       it != ftable->end(); ++it)
  {
    delete (*it);
  }
  delete v0s;
  delete r0s;
  delete scales;
  delete frefs;
}

//===================================================================================
FieldTable* FieldTable::GetInstance()
{
  if (instance == 0)
  {
    instance = new FieldTable();
  }
  return instance;
}

//===================================================================================
void FieldTable::Dispose()
{
  if (instance != 0)
  {
    delete instance;
    instance = 0;
  }
}

//===================================================================================
LinInterpolation* FieldTable::GetIntp(const char* fname)
{
  for (std::list<LinInterpolation*>::iterator it = ftable->begin();
       it != ftable->end(); ++it)
  {
    if ((*it)->IsNamed(fname))
    {
      return (*it);
    }
  }
  return 0;
}

//===================================================================================
void FieldTable::RegisterLinIntpolation(const char*  fname,
                                        const int    lskip,
                                        const int    lread,
                                        const double lunit,
                                        const double funit)
{
  for (std::list<LinInterpolation*>::iterator it = ftable->begin();
       it != ftable->end(); ++it)
  {
    if ((*it)->IsNamed(fname)) return;
  }
  LinInterpolation* k = new LinInterpolation(fname, lskip, lread, lunit, funit);
  ftable->push_back(k);
  cout << fname << " registered successfully..." << endl;
}

//===================================================================================
void FieldTable::AddFieldEntry(const char*             filename,
                               const G4ThreeVector&    V0,
                               const G4RotationMatrix& R0,
                               const double            scale)
{
  LinInterpolation* lntp = GetIntp(filename);
  if (lntp)
  {
    v0s->push_back(V0);
    r0s->push_back(R0);
    scales->push_back(scale);
    frefs->push_back(lntp);
    cout << r0s->size() << "-th of " << filename << endl;
  }

}

//===================================================================================
void FieldTable::GetGlobalB(const G4double x[],
                                  G4double f[])
{
  G4double xl[3];
  G4double bl[3];
  G4ThreeVector Xloc;
  G4ThreeVector Bglob;
  f[0] = 0.; f[1] = 0. ;f[2] = 0. ;f[3] = 0. ;f[4] = 0. ;f[5] = 0.;
  std::vector<G4RotationMatrix>::iterator  ir = r0s->begin();
  std::vector<G4ThreeVector>::iterator     iv = v0s->begin();
  std::vector<double>::iterator            is = scales->begin();
  std::vector<LinInterpolation*>::iterator il = frefs->begin();
  for (; ir != r0s->end(); ++ir, ++iv, ++is, ++il)
  {
    Xloc = (*ir).inverse() * (G4ThreeVector(x[0], x[1], x[2]) - (*iv));
    xl[0] = Xloc.x();
    xl[1] = Xloc.y();
    xl[2] = Xloc.z();
    if ((*il)->Get(xl, bl))
    {
      Bglob = (*ir) * G4ThreeVector(bl[0], bl[1], bl[2]);
      f[0] += (*is) * Bglob.x();
      f[1] += (*is) * Bglob.y();
      f[2] += (*is) * Bglob.z();
    }
  }
}


