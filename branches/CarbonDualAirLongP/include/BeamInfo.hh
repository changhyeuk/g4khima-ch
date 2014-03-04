#ifndef BeamInfo_hh_
#define BeamInfo_hh_ 1
//*****************************************************************************
//  BeamInfo.hh
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

class BeamInfo {
public:
  BeamInfo();
  BeamInfo(const double ix, const double iemitx,
           const double iy, const double iemity,
           const double ie_mean, const double ies_ratio = 0.);
  BeamInfo(const double ialpx, const double ibetx, const double iemitx,
           const double ialpy, const double ibety, const double iemity,
           const double ie_mean, const double ies_ratio = 0.);
  ~BeamInfo();

  // Getters __________________________________________________________________
  // Units : you must choose any of following unit set and devide by them
  //         when you use getter method
  //   set 1. x[mm] xp[mrad] beta[mm/mrad = m] gamma[mrad/mm = m^-1]
  //   set 2. x[m]  xp[rad]  beta[m/rad = m]   gamma[rad/m = m^-1]
  // Common use : emit[mm*mrad]
  double X() const;
  double XInt() const;
  double Xp() const;
  double XpInt() const;
  double Y() const;
  double YInt() const;
  double Yp() const;
  double YpInt() const;
  double Alpx() const;
  double Betx() const;
  double Gamx() const;
  double Emitx() const;
  double Alpy() const;
  double Bety() const;
  double Gamy() const;
  double Emity() const;

  double MeanEnergy() const;
  double EsRatio() const;
  double MsRatio() const;

  double Slopx() const;
  double Slopy() const;

  // Setters __________________________________________________________________
  // Units : you must choose any of following unit set and multiply them
  //         when you use setter method
  //   set 1. x[mm] xp[mrad] beta[mm/mrad = m] gamma[mrad/mm = m^-1]
  //   set 2. x[m]  xp[rad]  beta[m/rad = m]   gamma[rad/m = m^-1]
  // Common use : emit[mm*mrad]
  void SetXXe(const double ix, const double iemitx);
  void SetYYe(const double iy, const double iemity);
  void SetXabe(const double ialpx, const double ibetx, const double iemitx);
  void SetYabe(const double ialpy, const double ibety, const double iemity);
  void SetEnergyAndEnergyRatio(const double ie_mean,
                               const double ies_ratio = 0.);
  void SetEnergyAndMomentumRatio(const double ie_mean,
                                 const double ims_ratio = 0.);

private:
  double alpx;
  double betx;
  double emitx;
  double alpy;
  double bety;
  double emity;
  double e_mean;
  double es_ratio;
};


#endif // RMSBEAMINFO_HH_
