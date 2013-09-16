#ifndef AnalysisManager_hh
#define AnalysisManager_hh 1
//*****************************************************************************
//  AnalysisManager.hh
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

class TFile;
class TNtuple;
class TH3D;
class TH1D;

class AnalysisMessenger;
class PrimaryGeneratorMessenger;

#include "CaloHit.hh"
#include "TrackHit.hh"

#include "globals.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

class AnalysisManager
{
public:
  AnalysisManager();
  ~AnalysisManager();
  static AnalysisManager* GetInstance();
  static void Dispose();

private:
  static AnalysisManager* instance;
  AnalysisMessenger*      am;

  // for gun setting
  PrimaryGeneratorMessenger* pgm;

  // result file information
  G4String                path;
  G4String                timestr;
  G4String                dirpath;
  G4String                filename;
  TFile*                  aresult;

  TNtuple*                bullets;
  
  // result data structures
  // followings dont need to be deleted because of TFile
  // if you delete elements inside destructor of this class,
  // TFile will not save those elements.
  std::vector<G4String>   tuplenames;
  std::vector<G4String>   histonames;
  std::vector<G4String>   histonames1d;
  std::vector<TNtuple*>   tuples;
  std::vector<TH3D*>      histos;
  std::vector<TH1D*>      histos1d;
  // instead of root

public:
  void SetFilename(const G4String words);

  void FillBullet(const G4double ix, const G4double ixp,
                  const G4double iy, const G4double iyp,
                  const G4double iw);

  // booking must be done after registration of sensitive detector.
  // because of s_id searching.
  void BookTrackTuple(const G4String isdname);
  void BookCaloHisto3D(const G4String isdname,
                       const G4int inx,
                       const G4double ixmin, const G4double ixmax,
                       const G4int iny,
                       const G4double iymin, const G4double iymax,
                       const G4int inz,
                       const G4double izmin, const G4double izmax);
  // Chang Added
  void BookCaloHisto1D(const G4String isdname,
                       const G4int inz,
                       const G4double izmin, const G4double izmax);
  // Chang Added
  void FillTrackTuple(const G4String isdname, const TrackHitsCollection* hc);
  void FillCaloTuple(const G4String isdname, const CaloHitsCollection* hc);
  void FillCaloHisto3D(const G4String isdname, const CaloHitsCollection* hc);
  void FillCaloHisto1D(const G4String isdname, const CaloHitsCollection* hc);
};


#endif // ANALYSISMANAGER_HH
