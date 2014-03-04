//*****************************************************************************
//  BeamInfo.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "BeamInfo.hh"
#include "GaramGlobal.hh"

//=============================================================================
BeamInfo::BeamInfo()
  :alpx(0.), betx(0.), emitx(0.),
   alpy(0.), bety(0.), emity(0.),
   e_mean(0.), es_ratio(0.)
{}

//=============================================================================
BeamInfo::BeamInfo(const double ix, const double iemitx,
                   const double iy, const double iemity,
                   const double ie_mean, const double ies_ratio)
{
  SetXXe(ix, iemitx);
  SetYYe(iy, iemity);
  SetEnergyAndEnergyRatio(ie_mean, ies_ratio);
}

//=============================================================================
BeamInfo::BeamInfo(const double ialpx, const double ibetx, const double iemitx,
                   const double ialpy, const double ibety, const double iemity,
                   const double ie_mean, const double ies_ratio)
{
  SetXabe(ialpx, ibetx, iemitx);
  SetYabe(ialpy, ibety, iemity);
  SetEnergyAndEnergyRatio(ie_mean, ies_ratio);
}

//=============================================================================
BeamInfo::~BeamInfo() {}


// Getters __________________________________________________________________
//=============================================================================
double BeamInfo::X() const {return sqrt(betx * emitx);}

//=============================================================================
double BeamInfo::XInt() const {return sqrt(emitx / Gamx());}

//=============================================================================
double BeamInfo::Xp() const {return sqrt(Gamx() * emitx);}

//=============================================================================
double BeamInfo::XpInt() const {return sqrt(emitx / betx);}

//=============================================================================
double BeamInfo::Y() const {return sqrt(bety * emity);}

//=============================================================================
double BeamInfo::YInt() const {return sqrt(emity / Gamy());}

//=============================================================================
double BeamInfo::Yp() const {return sqrt(Gamy() * emity);}

//=============================================================================
double BeamInfo::YpInt() const { return sqrt(emity / bety);}

//=============================================================================
double BeamInfo::Alpx() const {return alpx;}

//=============================================================================
double BeamInfo::Betx() const {return betx;}

//=============================================================================
double BeamInfo::Gamx() const {return (alpx * alpx + 1.0) / betx;}

//=============================================================================
double BeamInfo::Emitx() const {return emitx;}

//=============================================================================
double BeamInfo::Alpy() const {return alpy;}

//=============================================================================
double BeamInfo::Bety() const {return bety;}

//=============================================================================
double BeamInfo::Gamy() const {return (alpy * alpy + 1.0) / bety;}

//=============================================================================
double BeamInfo::Emity() const {return emity;}

//=============================================================================
double BeamInfo::MeanEnergy() const {return e_mean;}

//=============================================================================
double BeamInfo::EsRatio() const {return es_ratio;}

//=============================================================================
double BeamInfo::MsRatio() const {return garam::dpop(e_mean, es_ratio);}

//=============================================================================
double BeamInfo::Slopx() const {return -alpx / betx;}

//=============================================================================
double BeamInfo::Slopy() const {return -alpy / bety;}


// Setters __________________________________________________________________
//=============================================================================
void BeamInfo::SetXXe(const double ix, const double iemitx)
{
  alpx = 0.;
  betx = ix * ix / emitx;
  emitx = iemitx;
}

//=============================================================================
void BeamInfo::SetYYe(const double iy, const double iemity)
{
  alpy = 0.;
  bety = iy * iy / emity;
  emity = iemity;
}

//=============================================================================
void BeamInfo::SetXabe(const double ialpx,
                       const double ibetx,
                       const double iemitx)
{
  alpx = ialpx;
  betx = ibetx;
  emitx = iemitx;
}

//=============================================================================
void BeamInfo::SetYabe(const double ialpy,
                       const double ibety,
                       const double iemity)
{
  alpy = ialpy;
  bety = ibety;
  emity = iemity;
}

//=============================================================================
void BeamInfo::SetEnergyAndEnergyRatio(const double ie_mean,
                                       const double ies_ratio)
{
  e_mean = ie_mean;
  es_ratio = ies_ratio;
}

//=============================================================================
void BeamInfo::SetEnergyAndMomentumRatio(const double ie_mean,
                                         const double ies_ratio)
{
  e_mean = ie_mean;
  es_ratio = garam::dwow(e_mean, ies_ratio);
}



