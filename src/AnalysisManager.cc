//*****************************************************************************
//  AnalysisManager.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "AnalysisMessenger.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "GaramGlobal.hh"
#include "BeamInfo.hh"
#include "AnalysisManager.hh"

#include <iostream>
#include <vector>

#include "G4SDManager.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

// for the Root
#include "TFile.h"
#include "TNtuple.h"
#include "TH3D.h"
#include "TH1D.h"
#include "float.h"
#include "math.h"

using namespace std;
AnalysisManager* AnalysisManager::instance = 0;

//==========================================================================
AnalysisManager::AnalysisManager()

{
  am = new AnalysisMessenger(this);
  timestr = garam::timeformat();
}

//==========================================================================
AnalysisManager::~AnalysisManager()
{
  aresult->Write();
  aresult->Close();
  delete aresult;
  delete am;
}

//==========================================================================
AnalysisManager* AnalysisManager::GetInstance()
{
  if (instance == 0)
  {
    instance = new AnalysisManager();
  }
  return instance;
}

//==========================================================================
void AnalysisManager::Dispose()
{
  if (instance != 0)
  {
    delete instance;
    instance = 0;
  }
}

//==========================================================================
void AnalysisManager::SetFilename(const G4String words)
{
  // get words
  std::vector<std::string> params = garam::ssplit(words);
  unsigned int n_params = params.size();
  
  // make result dirs.
  std::string rdir = "result";
  if (n_params != 1)
  {
    for (size_t i = 0; i < n_params - 1; ++i)
    {
      rdir.append("/" + params[i]);
    }
  }
  garam::mkdir_p(rdir.c_str());
  
  // make result root file.
  aresult = new TFile((rdir + "/" 
                       + params[n_params - 1] 
                       + "_" + timestr + ".root").c_str(),
                      "RECREATE");
  
  // by default, memory allocation for launched particle information!!
  bullets = new TNtuple("bullets", 
                        "Launched particle information",
                        "x:xp:y:yp:w");

}

//==========================================================================
void AnalysisManager::BookTrackTuple(const G4String isdname)
{ 
  // find tuple on existing list
  G4bool found_sdname = false;
  const G4int tuple_size = tuplenames.size();
  for (G4int i = 0; i < tuple_size; ++i)
  {
    if (isdname == tuplenames[i]) 
    {
      found_sdname = true;
      break;
    }
  }
  if (found_sdname)
  {
    //std::cout << "[AM]" << isdname << " already exist!!" << std::endl;
  }
  else
  {
    const char* n_chr = isdname.c_str();
    // MAXFieldNumber of TNtuple is 15
    TNtuple* tpl = new TNtuple(n_chr, n_chr, "pid:L:Z:A:I:tid:x:y:z:px:py:pz:w");
    tuplenames.push_back(isdname);
    tuples.push_back(tpl);
  }
}

//==========================================================================
void AnalysisManager::BookCaloHisto3D(const G4String isdname,
                                      const G4int    inx,
                                      const G4double ixmin,
                                      const G4double ixmax,
                                      const G4int    iny,
                                      const G4double iymin,
                                      const G4double iymax,
                                      const G4int    inz,
                                      const G4double izmin,
                                      const G4double izmax)
{
  // find tuple on existing list
  G4bool found_sdname = false;
  const G4int histo_size = histonames.size();
  for (G4int i = 0; i < histo_size; ++i)
  {
    if (isdname == histonames[i]) 
    {
      found_sdname = true;
      break;
    }
  }
  if (found_sdname)
  {
    //std::cout << "[AM]" << isdname << " already exist!!" << std::endl;
  }
  else
  {
    const char* n_chr = isdname.c_str();
    TH3D* h3 = new TH3D(n_chr, n_chr, 
                        inx, ixmin, ixmax,
                        iny, iymin, iymax,
                        inz, izmin, izmax);
    histonames.push_back(isdname);
    histos.push_back(h3);
  }

}

void AnalysisManager::BookCaloHisto1D(const G4String isdname,
                                      const G4int inz,
                                      const G4double izmin, const G4double izmax)
{
    // find tuple on existing list
    G4bool found_sdname = false;
    const G4int histo_size = histonames1d.size();
    for (G4int i = 0; i < histo_size; ++i)
    {
        if (isdname == histonames1d[i])
        {
            found_sdname = true;
            break;
        }
    }
    if (found_sdname)
    {
        //std::cout << "[AM]" << isdname << " already exist!!" << std::endl;
    }
    else
    {
        const char* n_chr = isdname.c_str();
        TH1D* h1 = new TH1D(n_chr, n_chr,inz, izmin, izmax);
        histonames1d.push_back(isdname);
        histos1d.push_back(h1);
    }
    

}
//==========================================================================
void AnalysisManager::FillBullet(const G4double ix, const G4double ixp,
                                 const G4double iy, const G4double iyp,
                                 const G4double iw)
{
  bullets->Fill(ix, ixp, iy, iyp, iw);
  // do nothing now...
}

//==========================================================================
void AnalysisManager::FillTrackTuple(const G4String isdname,
                                     const TrackHitsCollection* hc)
{
  G4bool found_sdname = false;
  const G4int tuple_size = tuplenames.size();
  G4int i = 0;
  for (; i < tuple_size; ++i)
  {
    if (isdname == tuplenames[i]) 
    {
      found_sdname = true;
      break;
    }
  }
  if (!found_sdname) return;
  // tuples[i] is the one we've found
  
  std::vector<TrackHit*>* hits = hc->GetVector();
  for (std::vector<TrackHit*>::iterator it = hits->begin();
       it != hits->end();  
       ++it)
  {
    tuples[i]->Fill((*it)->GetParticleId(),
                    (*it)->GetNStrangeQuark(),
                    (*it)->GetAtomicNumber(),
                    (*it)->GetAtomicMass(),
                    (*it)->GetIsomericExtEnergy(),
                    (*it)->GetTrackId(),
                    (*it)->GetPosition().x(),
                    (*it)->GetPosition().y(),
                    (*it)->GetPosition().z(),
                    (*it)->GetMomentum().x(),
                    (*it)->GetMomentum().y(),
                    (*it)->GetMomentum().z(),
                    (*it)->GetKineticEnergy());
  }
}

//==========================================================================
void AnalysisManager::FillCaloHisto3D(const G4String isdname,
                                      const CaloHitsCollection* hc)
{
  G4bool found_sdname = false;
  const G4int histo_size = histonames.size();
  G4String thishisto = isdname + "_dose";
  G4int i_dose = 0;
  for (; i_dose < histo_size; ++i_dose)
  {
    if (thishisto == histonames[i_dose]) 
    {
      found_sdname = true;
      break;
    }
  }
  if (!found_sdname) return;

  found_sdname = false;
  thishisto = isdname + "_letP";
  G4int i_letP = 0;
  for (; i_letP < histo_size; ++i_letP)
  {
    if (thishisto == histonames[i_letP]) 
    {
      found_sdname = true;
      break;
    }
  }
  if (!found_sdname) return;

  found_sdname = false;
  thishisto = isdname + "_letH";
  G4int i_letH = 0;
  for (; i_letH < histo_size; ++i_letH)
  {
    if (thishisto == histonames[i_letH]) 
    {
      found_sdname = true;
      break;
    }
  }
  if (!found_sdname) return;

  found_sdname = false;
  thishisto = isdname + "_letA";
  G4int i_letA = 0;
  for (; i_letA < histo_size; ++i_letA)
  {
    if (thishisto == histonames[i_letA]) 
    {
      found_sdname = true;
      break;
    }
  }
  if (!found_sdname) return;

  std::vector<CaloHit*>* hits = hc->GetVector();
  for (std::vector<CaloHit*>::iterator it = hits->begin();
       it != hits->end();  
       ++it)
  {
    CaloHit* hit = *it;
    histos[i_dose]->Fill(hit->GetPosition().x(),
                         hit->GetPosition().y(),
                         hit->GetPosition().z(),
                         hit->GetEdeposit());
    histos[i_letP]->Fill(hit->GetPosition().x(),
                         hit->GetPosition().y(),
                         hit->GetPosition().z(),
                         hit->GetDedx());
    histos[i_letH]->Fill(hit->GetPosition().x(),
                         hit->GetPosition().y(),
                         hit->GetPosition().z(),
                         hit->GetDedx());
    histos[i_letA]->Fill(hit->GetPosition().x(),
                         hit->GetPosition().y(),
                         hit->GetPosition().z(),
                         hit->GetDedx());
  }
}

void AnalysisManager::FillCaloHisto1D(const G4String isdname,
                                      const CaloHitsCollection* hc)
{
    G4bool found_sdname = false;
    const G4int histo_size = histonames1d.size();
    G4String thishisto = isdname + "_dose_1D";
    G4int i_dose = 0;
    for (; i_dose < histo_size; ++i_dose)
    {
        if (thishisto == histonames1d[i_dose])
        {
            found_sdname = true;
            break;
        }
    }
    if (!found_sdname) return;
    
    found_sdname = false;
    thishisto = isdname + "_letP_1D";
    G4int i_letP = 0;
    for (; i_letP < histo_size; ++i_letP)
    {
        if (thishisto == histonames1d[i_letP])
        {
            found_sdname = true;
            break;
        }
    }
    if (!found_sdname) return;
    
    found_sdname = false;
    thishisto = isdname + "_letH_1D";
    G4int i_letH = 0;
    for (; i_letH < histo_size; ++i_letH)
    {
        if (thishisto == histonames1d[i_letH])
        {
            found_sdname = true;
            break;
        }
    }
    if (!found_sdname) return;
    
    found_sdname = false;
    thishisto = isdname + "_letA_1D";
    G4int i_letA = 0;
    for (; i_letA < histo_size; ++i_letA)
    {
        if (thishisto == histonames1d[i_letA])
        {
            found_sdname = true;
            break;
        }
    }
    if (!found_sdname) return;
    
    std::vector<CaloHit*>* hits = hc->GetVector();
    for (std::vector<CaloHit*>::iterator it = hits->begin();
         it != hits->end();
         ++it)
    {
        CaloHit* hit = *it;
        // Collect hit only inside diameter of virtural cylinder
        
        G4double radi = sqrt(pow(hit->GetPosition().x(),2)+
                             pow(hit->GetPosition().y(),2)); // Unit : mm

        if (radi <= 150) //200 )
        {
            histos1d[i_dose]->Fill(hit->GetPosition().z(),
                                   hit->GetEdeposit());
            
            if ( hit->IsPrimary() == 0 )
            {
                if (isnan(hit->GetDedx()))
                {
                    //G4cout<<isnan(hit->GetDedx()) <<" Not A Number "<<G4endl;
                }
                else
                {
                    histos1d[i_letP]->Fill(hit->GetPosition().z(),
                                           hit->GetDedx());
                    histos1d[i_letH]->Fill(hit->GetPosition().z(),
                                           hit->GetDedx());
                    histos1d[i_letA]->Fill(hit->GetPosition().z(),
                                           hit->GetDedx());
                }
            }
            else
            {}
        }
        else
        {}
    }
}
