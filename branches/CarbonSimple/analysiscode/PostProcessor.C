{
  // style initiation
  // Canvas
  gStyle->SetCanvasColor     (0);
  gStyle->SetCanvasBorderSize(10);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasDefH      (700);
  gStyle->SetCanvasDefW      (700);
  gStyle->SetCanvasDefX      (100);
  gStyle->SetCanvasDefY      (100);
  
  // Pads
  gStyle->SetPadColor       (0);
  gStyle->SetPadBorderSize  (10);
  gStyle->SetPadBorderMode  (0);
  gStyle->SetPadBottomMargin(0.22);
  gStyle->SetPadTopMargin   (0.15);
  gStyle->SetPadLeftMargin  (0.22);
  gStyle->SetPadRightMargin (0.17);
  gStyle->SetPadGridX       (1);
  gStyle->SetPadGridY       (1);
  gStyle->SetPadTickX       (1);
  gStyle->SetPadTickY       (1);
  
  // Frames
  gStyle->SetFrameFillStyle (0);
  gStyle->SetFrameFillColor (0);
  gStyle->SetFrameLineColor (1);
  gStyle->SetFrameLineStyle (0);
  gStyle->SetFrameLineWidth (1);
  gStyle->SetFrameBorderSize(10);
  gStyle->SetFrameBorderMode(0);
  
  // Histograms
  gStyle->SetHistFillColor(2);
  gStyle->SetHistFillStyle(0);
  gStyle->SetHistLineColor(1);
  gStyle->SetHistLineStyle(0);
  gStyle->SetHistLineWidth(2);
  gStyle->SetNdivisions(505);
  
  // Functions
  gStyle->SetFuncColor(1);
  gStyle->SetFuncStyle(0);
  gStyle->SetFuncWidth(2);
  
  // Various
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerColor(kBlack);
  gStyle->SetMarkerSize (1.2);
  
  gStyle->SetTitleSize  (0.050,"X");
  gStyle->SetTitleOffset(1.200,"X");
  gStyle->SetLabelOffset(0.004,"X");
  gStyle->SetLabelSize  (0.045,"X");
  gStyle->SetLabelFont  (42   ,"X");
  
  gStyle->SetStripDecimals(kFALSE);
  
  gStyle->SetTitleSize  (0.050,"Y");
  gStyle->SetTitleOffset(1.400,"Y");
  gStyle->SetLabelOffset(0.005,"Y");
  gStyle->SetLabelSize  (0.045,"Y");
  gStyle->SetLabelFont  (42   ,"Y");
  
  gStyle->SetTextSize   (0.055);
  gStyle->SetTextFont   (42);
  
  gStyle->SetStatFont   (42);
  gStyle->SetTitleFont  (42);
  gStyle->SetTitleFont  (42,"X");
  gStyle->SetTitleFont  (42,"Y");
  
  gStyle->SetOptStat(1);
  gStyle->SetPalette(1);
  

  //======================================================================
  // pick up root file from arguments
  //======================================================================
  const int argc = gApplication->Argc();
  const char **argv = gApplication->Argv();
  std::string filename;
  size_t root_pos;
  for (int i = 0; i < argc; ++i)
  {
    std::string argstr = argv[i];
    root_pos = argstr.find(".root");
    if (root_pos != std::string::npos)
    {
      filename.assign(argv[i]);
      break;
    }
  }
  std::cout << "Trying to open " << filename << std::endl;
  TFile* f = new TFile(filename.c_str());
  if (f->IsZombie())
  {
    std::cout << "An error was occurred!!" << std::endl;
    return true;
  }

  // too dangerous!! this overwrite file without mercy!!
  std::string fname_first;
  fname_first.assign(filename.c_str(), root_pos);
  TFile* fw = new TFile((fname_first + "_analysis.root").c_str(), "RECREATE");

  // meta information that can not be saved in newly generated root file.
  std::ofstream txt((fname_first + "_analysis.txt").c_str());
  const char* DELIM = ","; // CSV format
  txt << "name" << DELIM
      << "alpha_x" << DELIM
      << "beta_x" << DELIM
      << "gamma_x" << DELIM
      << "emit_rms_x" << DELIM
      << "alpha_y" << DELIM
      << "beta_y" << DELIM
      << "gamma_y" << DELIM
      << "emit_rms_y" << DELIM
      << "e_peak" << std::endl;

  
  TList* objs = f->GetListOfKeys();
  const int N = objs->GetEntries();

  for (int i = 0; i < N; ++i)
  {
    std::cout << i << " - " << objs->At(i)->GetName() << std::endl;
  }
  
  //=====================================================================
  const int nx = 100, xmin = -50.0, xmax = 50.0;
  const int nxp = 100, xpmin = -20.0, xpmax = 20.0;

  float emit_rms_x, emit_rms_y;
  float alpha_x, beta_x, gamma_x, alpha_y, beta_y, gamma_y;
  float pid, tid, L, Z, A, I, x, y, z, px, py, pz, w;
  int ipid, iL, iZ, iA, iI;
  TBranch *bpid, *bL, *bZ, *bA, *bI, *btid, *bx, *by, *bz, *bpx, *bpy, *bpz, *bw;
  float xp, yp;
  TBranch *bxp, *byp;
  TNtuple* tuple;
  std::string name, iname;
  
  // for emittance and twiss parameter calculation
  float x_ave = 0., y_ave = 0.;
  float x2_ave = 0., y2_ave = 0.;
  float xp_ave = 0., yp_ave = 0.;
  float xp2_ave = 0., yp2_ave = 0.;
  float xxp_ave = 0., yyp_ave = 0.;
  float xxp2_ave = 0., yyp2_ave = 0.;
  float x_rms = 0., y_rms = 0.;
  float xp_rms = 0., yp_rms = 0.;
  float xxp_rms = 0., yyp_rms = 0.;
  
  float e_mean, e_mean_c12, e_peak_c12;
  int count, count_c12;

  for (int i = 0; i < N; ++i)
  {
    std::string name = objs->At(i)->GetName();
    iname.assign(name.c_str(), 4); 
    
    x_ave = 0.;     y_ave = 0.;
    x2_ave = 0.;    y2_ave = 0.;
    xp_ave = 0.;    yp_ave = 0.;
    xp2_ave = 0.;   yp2_ave = 0.;
    xxp_ave = 0.;   yyp_ave = 0.;
    xxp2_ave = 0.;  yyp2_ave = 0.;
    x_rms = 0.;     y_rms = 0.;
    xp_rms = 0.;    yp_rms = 0.;
    xxp_rms = 0.;   yyp_rms = 0.;
     
    e_mean = 0.; e_mean_c12 = 0.; e_peak_c12 = 0.;
    count = 0;   count_c12 = 0;
    //====================================================================
    // Tracker SD 
    //====================================================================
    if (iname == "Trac") 
    {
      tuple = (TNtuple*)f->Get(name.c_str());
      tuple->SetBranchAddress("pid", &pid, &bpid);
      tuple->SetBranchAddress("L", &L, &bL);
      tuple->SetBranchAddress("Z", &Z, &bZ);
      tuple->SetBranchAddress("A", &A, &bA);
      tuple->SetBranchAddress("I", &I, &bI);
      tuple->SetBranchAddress("tid", &tid, &btid);
      tuple->SetBranchAddress("x", &x, &bx);
      tuple->SetBranchAddress("y", &y, &by);
      tuple->SetBranchAddress("z", &z, &bz);
      tuple->SetBranchAddress("px", &px, &bpx);
      tuple->SetBranchAddress("py", &py, &bpy);
      tuple->SetBranchAddress("pz", &pz, &bpz);
      tuple->SetBranchAddress("w", &w, &bw);
      
      // simply check mean energy first,
      // because requirement is less than 2% of e-spread
      for (int j = 0; j < tuple->GetEntries(); ++j)
      {
        bZ->GetEntry(j);
        bA->GetEntry(j);
        bw->GetEntry(j);
        iZ = Z; iA = A;
        if (iA == 12 && iZ == 6) // objective carbon particle
        {
          e_mean_c12 += w/A;
          count_c12++;
        }
      }
      e_mean_c12 /= (float)count_c12;

      // ================================================
      // now fit gaussian function to find e_peak_c12
      TH1D* h_epeak_c12 = new TH1D((name + "_epeak_c12").c_str(), "K",
                                   1000, e_mean_c12*0.5, e_mean_c12*1.5);
      for (int j = 0; j < tuple->GetEntries(); ++j)
      {
        bZ->GetEntry(j);
        bA->GetEntry(j);
        bw->GetEntry(j);
        iZ = Z; iA = A;
        if (iA == 12 && iZ == 6) h_epeak_c12->Fill(w/A);
      }
      
      double e_peak_center
        = h_epeak_c12->GetBinCenter(h_epeak_c12->GetMaximumBin());
      TF1* gaussfit = new TF1((name + "_gaussfit").c_str(), "gaus", 
                              e_peak_center * 0.95, e_peak_center * 1.05);
      h_epeak_c12->Fit(gaussfit, "R");
      double par[3];
      gaussfit.GetParameters(&par[0]);
      e_peak_c12 = par[1];
      
      // Book histograms
      TH2D* h_xphase_c12 = new TH2D((name + "_h_xphase_c12").c_str(), 
                                    "X-Xp Phase Space, Obj. 12C",
                                    nx, xmin, xmax, nxp, xpmin, xpmax);
      h_xphase_c12->SetXTitle("x [mm]");
      h_xphase_c12->SetYTitle("x' [mrad]");
      TH2D* h_yphase_c12 = new TH2D((name + "_h_yphase_c12").c_str(),
                                    "Y-Yp Phase Space, Obj. 12C",
                                    nx, xmin, xmax, nxp, xpmin, xpmax);
      h_yphase_c12->SetXTitle("y [mm]");
      h_yphase_c12->SetYTitle("y' [mrad]");
      TH2D* h_real_c12 = new TH2D((name + "_h_real_c12").c_str(),
                                  "Real Space, Obj. 12C",
                                  nx, xmin, xmax, nx, xmin, xmax);
      h_real_c12->SetXTitle("x [mm]");
      h_real_c12->SetYTitle("y [mm]");
      TH1D* h_w_c12 = new TH1D((name + "_h_w_c12").c_str(),
                               "Kinetic Energy Dist., Obj. 12C",
                               100, e_peak_c12*0.95, e_peak_c12*1.05);
      h_w_c12->SetXTitle("Kinetic Energy [MeV/u]");
      h_w_c12->SetYTitle("# particle [arb. unit]");

      /*
      TH2D* h_xphase_za = new TH2D((name + "_h_xphase_za").c_str(), 
                                   "X-Xp Phase Space, Z/A=1/2",
                                   nx, xmin, xmax, nxp, xpmin, xpmax);
      h_xphase_za->SetXTitle("x [mm]");
      h_xphase_za->SetYTitle("x' [mrad]");
      TH2D* h_yphase_za = new TH2D((name + "_h_yphase_za").c_str(),
                                   "Y-Yp Phase Space, Z/A=1/2",
                                   nx, xmin, xmax, nxp, xpmin, xpmax);
      h_yphase_za->SetXTitle("x [mm]");
      h_yphase_za->SetYTitle("x' [mrad]");
      */
      TH2D* h_real_za = new TH2D((name + "_h_real_za").c_str(),
                                 "Real Space, Z/A=1/2",
                                 nx, xmin, xmax, nx, xmin, xmax);
      h_real_za->SetXTitle("x [mm]");
      h_real_za->SetYTitle("y [mm]");
      /*
      TH2D* h_xphase_other = new TH2D((name + "_h_xphase_other").c_str(), 
                                      "X-Xp Phase Space, Other All",
                                      nx, xmin, xmax, nxp, xpmin, xpmax);
      h_xphase_other->SetXTitle("x [mm]");
      h_xphase_other->SetYTitle("x' [mrad]");
      TH2D* h_yphase_other = new TH2D((name + "_h_yphase_other").c_str(),
                                      "Y-Yp Phase Space, Other All",
                                      nx, xmin, xmax, nxp, xpmin, xpmax);
      h_yphase_other->SetXTitle("x [mm]");
      h_yphase_other->SetYTitle("x' [mrad]");
      TH2D* h_real_other = new TH2D((name + "_h_real_other").c_str(),
                                    "Real Space, Other All",
                                    nx, xmin, xmax, nx, xmin, xmax);
      h_real_other->SetXTitle("x [mm]");
      h_real_other->SetYTitle("y [mm]");
      */
      
      
      
      // fill up histograms
      count_c12 = 0;
      for (int j = 0; j < tuple->GetEntries(); ++j)
      {
        // locate branch address
        bpid->GetEntry(j);
        bL->GetEntry(j);
        bZ->GetEntry(j);
        bA->GetEntry(j);
        bI->GetEntry(j);
        btid->GetEntry(j);
        bx->GetEntry(j);
        by->GetEntry(j);
        bz->GetEntry(j);
        bpx->GetEntry(j);
        bpy->GetEntry(j);
        bpz->GetEntry(j);
        bw->GetEntry(j);

        xp = px / pz * 1000.0; // *1000.0 is rad -> mrad scalling
        yp = py / pz * 1000.0;
        
        ipid = pid; iL = L, iZ = Z; iA = A; iI = I;
        // PID : high energy physics, MC particle numbering scheme      
        //  with energy cut +-3% emperically good value.
        if (iA != 0 && iZ != 0)
        {
          if ((iA == 12 && iZ == 6) &&
              (w/A > e_peak_c12 * 0.98) &&
              (w/A < e_peak_c12 * 1.02)) // objective carbon particle
          {
            x_ave += x;    x2_ave += x * x;
            xp_ave += xp;  xp2_ave += xp * xp;
            y_ave += y;    y2_ave += y * y;
            yp_ave += yp;  yp2_ave += yp * yp;
            xxp_ave += x * xp; xxp2_ave += x * x * xp * xp;
            yyp_ave += y * yp; yyp2_ave += y * y * yp * yp;
            count_c12++;
            h_xphase_c12->Fill(x, xp);
            h_yphase_c12->Fill(y, yp);
            h_real_c12->Fill(x, y);
            h_w_c12->Fill(w/A);
          }
          else
          {
            if (Z/A == 0.5)
            {
              //h_xphase_za->Fill(x, xp);
              //h_yphase_za->Fill(y, yp);
              h_real_za->Fill(x, y);
            }
            /*
            h_xphase_other->Fill(x, xp);
            h_yphase_other->Fill(y, yp);
            h_real_other->Fill(x, y);
            */
          }
        }
        else // elementary particles
        {
          // do nothing now
        }
        
      } // end of fill up, loop for TrackerSD


      float fcount_c12 = count_c12;
      x_ave   = x_ave   / fcount_c12;
      xp_ave  = xp_ave  / fcount_c12;
      y_ave   = y_ave   / fcount_c12;
      yp_ave  = yp_ave  / fcount_c12;
      x2_ave  = x2_ave  / fcount_c12;
      xp2_ave = xp2_ave / fcount_c12;
      xxp_ave = xxp_ave / fcount_c12;
      y2_ave  = y2_ave  / fcount_c12;
      yp2_ave = yp2_ave / fcount_c12;
      yyp_ave = yyp_ave / fcount_c12;
      x_rms = sqrt(x2_ave - x_ave*x_ave);
      y_rms = sqrt(y2_ave - y_ave*y_ave);
      xp_rms = sqrt(xp2_ave - xp_ave*xp_ave);
      yp_rms = sqrt(yp2_ave - yp_ave*yp_ave);
      xxp_rms = xxp_ave - x_ave*xp_ave;
      yyp_rms = yyp_ave - y_ave*yp_ave;
      
      emit_rms_x = sqrt(x2_ave*xp2_ave - xxp_ave*xxp_ave);
      emit_rms_y = sqrt(y2_ave*yp2_ave - yyp_ave*yyp_ave);
      beta_x  = (x_rms  * x_rms)  / emit_rms_x;
      gamma_x = (xp_rms * xp_rms) / emit_rms_x;
      alpha_x = - xxp_rms/emit_rms_x;
      beta_y  = (y_rms  * y_rms)  / emit_rms_y;
      gamma_y = (yp_rms * yp_rms) / emit_rms_y;
      alpha_y = - yyp_rms / emit_rms_y;
      // emit calc done.
      txt << name << DELIM
          << alpha_x << DELIM
          << beta_x << DELIM
          << gamma_x << DELIM
          << emit_rms_x << DELIM
          << alpha_y << DELIM
          << beta_y << DELIM
          << gamma_y << DELIM
          << emit_rms_y << DELIM
          << e_peak_c12 << std::endl;
    }
    
    
    
    //====================================================================
    // Bullets
    //====================================================================
    else if (iname == "bull")
    {
      // mapping variable to field data
      tuple = (TNtuple*)f->Get(name.c_str());
      tuple->SetBranchAddress("x", &x, &bx);
      tuple->SetBranchAddress("xp", &xp, &bxp);
      tuple->SetBranchAddress("y", &y, &by);
      tuple->SetBranchAddress("yp", &yp, &byp);
      tuple->SetBranchAddress("w", &w, &bw);
      
      // simply check mean energy first,
      float e_mean = 0.;
      for (int j = 0; j < tuple->GetEntries(); ++j)
      { 
        bw->GetEntry(j);
        e_mean += w;
      }
      e_mean /= (float)tuple->GetEntries();

      //===========================================================
      TH2D* h_xphase = new TH2D((name + "_h_xphase").c_str(), 
                                "X-Xp Phase Space",
                                nx, xmin, xmax, nxp, xpmin, xpmax);
      h_xphase->SetXTitle("x [mm]");
      h_xphase->SetYTitle("x' [mrad]");
      TH2D* h_yphase = new TH2D((name + "_h_yphase").c_str(),
                                "Y-Yp Phase Space",
                                nx, xmin, xmax, nxp, xpmin, xpmax);
      h_yphase->SetXTitle("y [mm]");
      h_yphase->SetYTitle("y' [mrad]");
      TH2D* h_real = new TH2D((name + "_h_real").c_str(),
                              "Real Space",
                              nx, xmin, xmax, nx, xmin, xmax);
      h_real->SetXTitle("x [mm]");
      h_real->SetYTitle("y [mm]");
      TH1D* h_w = new TH1D((name + "_h_w").c_str(),
                           "Kinetic Energy Dist.",
                           nx, e_mean*0.9, e_mean*1.1);
      h_w->SetXTitle("Kinetic Energy [MeV/u]");

      
      // to calc mean_energy, emittance, twiss
      for (int j = 0; j < tuple->GetEntries(); ++j)
      {
        // locate branch address
        bx->GetEntry(j);
        by->GetEntry(j);
        bxp->GetEntry(j);
        byp->GetEntry(j);
        bw->GetEntry(j);

        xp *= 1000.0; // *1000.0 is mrad unit conversion
        yp *= 1000.0;
        
        x_ave += x;    x2_ave += x * x;
        xp_ave += xp;  xp2_ave += xp * xp;
        y_ave += y;    y2_ave += y * y;
        yp_ave += yp;  yp2_ave += yp * yp;
        xxp_ave += x * xp; xxp2_ave += x * x * xp * xp;
        yyp_ave += y * yp; yyp2_ave += y * y * yp * yp;
        count++;
        
        h_xphase->Fill(x, xp);
        h_yphase->Fill(y, yp);
        h_real->Fill(x, y);
        h_w->Fill(w);
        
      }
      float fcount = tuple->GetEntries();
      x_ave   = x_ave   / fcount;
      xp_ave  = xp_ave  / fcount;
      y_ave   = y_ave   / fcount;
      yp_ave  = yp_ave  / fcount;
      x2_ave  = x2_ave  / fcount;
      xp2_ave = xp2_ave / fcount;
      xxp_ave = xxp_ave / fcount;
      y2_ave  = y2_ave  / fcount;
      yp2_ave = yp2_ave / fcount;
      yyp_ave = yyp_ave / fcount;
      x_rms = sqrt(x2_ave - x_ave*x_ave);
      y_rms = sqrt(y2_ave - y_ave*y_ave);
      xp_rms = sqrt(xp2_ave - xp_ave*xp_ave);
      yp_rms = sqrt(yp2_ave - yp_ave*yp_ave);
      xxp_rms = xxp_ave - x_ave*xp_ave;
      yyp_rms = yyp_ave - y_ave*yp_ave;
      
      emit_rms_x = sqrt(x2_ave*xp2_ave - xxp_ave*xxp_ave);
      emit_rms_y = sqrt(y2_ave*yp2_ave - yyp_ave*yyp_ave);
      beta_x  = (x_rms  * x_rms)  / emit_rms_x;
      gamma_x = (xp_rms * xp_rms) / emit_rms_x;
      alpha_x = - xxp_rms/emit_rms_x;
      beta_y  = (y_rms  * y_rms)  / emit_rms_y;
      gamma_y = (yp_rms * yp_rms) / emit_rms_y;
      alpha_y = - yyp_rms / emit_rms_y;
      // emit calc done.
      txt << name << DELIM
          << alpha_x << DELIM
          << beta_x << DELIM
          << gamma_x << DELIM
          << emit_rms_x << DELIM
          << alpha_y << DELIM
          << beta_y << DELIM
          << gamma_y << DELIM
          << emit_rms_y << DELIM
          << e_mean << std::endl;
    }
    
    else if (iname == "Calo")
    {
      // CalorimeterSD
    }
    // call another function? or do it here?
    // here would be good.
  
  }
  fw->Write();
  fw->Close();
  f->Close();
  txt.close();
  exit();
}
