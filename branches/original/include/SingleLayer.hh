#ifndef SINGLELAYER_HH_
#define SINGLELAYER_HH_

#include "G4UImessenger.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

struct SingleLayer 
{
    SingleLayer(const char* m, G4double t)
    {
      material  = m;
      thickness = t;
      rangecut  = 1.0 * mm;
      steplimit = 0.5 * mm;
    };
    
    SingleLayer(const char* m, G4double t, G4double cut, G4double stp)
    {
      material  = m;
      thickness = t;
      rangecut  = cut;
      steplimit = stp;
    };


    const SingleLayer& operator=(const SingleLayer& r)
    {
      material  = r.material;
      thickness = r.thickness;
      steplimit = r.steplimit;
      rangecut  = r.rangecut;
      return *this;
    };
    
    G4Material* GetG4material()
    {
      try 
      { return G4NistManager::Instance()->FindOrBuildMaterial(material, false); }
      catch (int i)
      { return G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR", false); }
    };
    
    G4String material;
    G4double thickness;
    G4double steplimit;
    G4double rangecut;
};
typedef std::vector<SingleLayer> TargetLayers;

#endif /* SINGLELAYER_HH_ */

