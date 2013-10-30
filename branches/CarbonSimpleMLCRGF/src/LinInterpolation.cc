//*****************************************************************************
//  LinInterpolation.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "LinInterpolation.hh"

#include <iostream>
#include <fstream>

//===================================================================================
LinInterpolation::LinInterpolation(const char* fname,
                                   const int   lskip,
                                   const int   lread,
                                   const double lunit,
                                   const double funit)
{
  if (lread == 0) 
  {
    // do nothing now...
  }
  
  cout << " reading : " << fname << endl;
  strcpy(filename, fname);

  ifstream grid_t(fname, ifstream::in);

  // �Ʒ� �� ������ ��� seek�� ��ü ����
  const int LSIZE = 256;
  char line[LSIZE];
  for (int i = 0; i < lskip; ++i)
    grid_t.getline(line, LSIZE);

  cout << "  check ";
  int count = 0;
  std::vector<double> xx, yy, zz;
  double ix, iy, iz, ibx, iby, ibz;
  while (grid_t >> ix >> iy >> iz >> ibx >> iby >> ibz)
  {
    ix  *= lunit;
    iy  *= lunit;
    iz  *= lunit;
    ibx *= funit;
    iby *= funit;
    ibz *= funit;
    if (xx.end() == find(xx.begin(), xx.end(), ix)) xx.push_back(ix);
    if (yy.end() == find(yy.begin(), yy.end(), iy)) yy.push_back(iy);
    if (zz.end() == find(zz.begin(), zz.end(), iz)) zz.push_back(iz);
    if ((++count % 10000) == 0) cout << ".";
  }

  // sorting
  sort(xx.begin(), xx.end());
  sort(yy.begin(), yy.end());
  sort(zz.begin(), zz.end());
  // assignment
  xmin = xx.front();
  xmax = xx.back();
  ymin = yy.front();
  ymax = yy.back();
  zmin = zz.front();
  zmax = zz.back();
  NX = xx.size();
  NY = yy.size();
  NZ = zz.size();


  //check all delta is same
  if ((NX < 3) || (NY < 3) || (NZ < 3))
  {
    grid_t.close();
    //cout << "exit1" << endl;
    return;
  }

  bool reading_err = false;
  double delta_x = xx[1] - xx[0];
  for (vector<double>::iterator ii = xx.begin() +2; ii != xx.end(); ++ii)
  {
    if ((*ii - *(ii-1)) != delta_x)
    {
      reading_err = true;
      break;
    }
  }

  double delta_y = yy[1] - yy[0];
  for (vector<double>::iterator ii = yy.begin() +2; ii != yy.end(); ++ii)
  {
    if ((*ii - *(ii-1)) != delta_y)
    {
      reading_err = true;
      break;
    }
  }

  double delta_z = zz[1] - zz[0];
  for (vector<double>::iterator ii = zz.begin() +2; ii != zz.end(); ++ii)
  {
    if ((*ii - *(ii-1)) != delta_z)
    {
      reading_err = true;
      break;
    }
  }

  dx = delta_x;
  dy = delta_y;
  dz = delta_z;

  //----------------------------------------------------------------------------
  // ��̴� �ⲯ�ؾ� double �� 134,217,727 �� �ۿ��� �� �����. 32bit default!
  // �޸𸮸� ����� �� �ٷ�� ������ ���������� �� �𸣰�����...
  // ������ ������ ���͸� ���°� �´�. �� ũ�� ������...
  if (reading_err || (NX * NY * NZ > 130000000))
  {
    grid_t.close();
    return;
  }

  grid_t.clear();
  grid_t.seekg(0);

  for (int i = 0; i < lskip; ++i)
  {
    grid_t.getline(line, LSIZE);
  }

  //----------------------------------------------------------------------------
  // memory allocation
  cout << endl << "  memory allocated" << endl;
  int i, j, k;
  Bx = new double** [NX];
  By = new double** [NX];
  Bz = new double** [NX];
  for (i = 0; i < NX; ++i)
  {
    Bx[i] = new double* [NY];
    By[i] = new double* [NY];
    Bz[i] = new double* [NY];
    for (j = 0; j < NY; ++j)
    {
      Bx[i][j] = new double [NZ];
      By[i][j] = new double [NZ];
      Bz[i][j] = new double [NZ];
    }
  }

  x = new double [NX];
  for (i = 0; i < NX; ++i) x[i] = xx[i];
  y = new double [NY];
  for (j = 0; j < NY; ++j) y[j] = yy[j];
  z = new double [NZ];
  for (k = 0; k < NZ; ++k) z[k] = zz[k];

  //----------------------------------------------------------------------------
  // put data
  const int ten_percent = count / 10 - 1;
  int load_count = 0;
  int load_percent = 0;
  cout << "  loading ";
  while (grid_t >> ix >> iy >> iz >> ibx >> iby >> ibz)
  {
    ix  *= lunit;
    iy  *= lunit;
    iz  *= lunit;
    ibx *= funit;
    iby *= funit;
    ibz *= funit;
    for (i = 0; i < NX; ++i) if (xx[i] == ix) break;
    for (j = 0; j < NY; ++j) if (yy[j] == iy) break;
    for (k = 0; k < NZ; ++k) if (zz[k] == iz) break;
    Bx[i][j][k] = ibx;
    By[i][j][k] = iby;
    Bz[i][j][k] = ibz;
    if ((++load_count % ten_percent) == 0)
    {
      load_percent += 10;
      cout << load_percent << "% ";
    }
  }
  grid_t.close();
  cout << endl;
  cout << "   field was loaded successfully" << endl;
  cout << "   lattice points = "
       << NX << " x " << NY << " x " << NZ << endl;
  cout << "   check range"
       << " x (" << x[0] << ", " << x[NX-1] << "), "
       << " y (" << y[0] << ", " << y[NY-1] << "), "
       << " z (" << z[0] << ", " << z[NZ-1] << ")" << endl;

}

//===================================================================================
LinInterpolation::~LinInterpolation()
{
  delete [] x;
  delete [] y;
  delete [] z;

  int i, j;
  for (i = 0; i < NX; ++i)
  {
    for (j = 0; j < NY; ++j)
    {
      delete [] Bx[i][j];
      delete [] By[i][j];
      delete [] Bz[i][j];
    }
    delete [] Bx[i];
    delete [] By[i];
    delete [] Bz[i];
  }
  delete [] Bx;
  delete [] By;
  delete [] Bz;
}

//===================================================================================
bool LinInterpolation::Get(const double pos[],
                                 double B[]) const
{
  // if the point is outside, return 0
  if ((pos[0] <= xmin) || (xmax < pos[0]) ||
      (pos[1] <= ymin) || (ymax < pos[1]) ||
      (pos[2] <= zmin) || (zmax < pos[2])) return false;

  //cout << "begin intp" << endl;
  // find local position
  int i, j, k;
  for (i = NX - 2; i != 0; --i) if (x[i] < pos[0]) break;
  for (j = NY - 2; j != 0; --j) if (y[j] < pos[1]) break;
  for (k = NZ - 2; k != 0; --k) if (z[k] < pos[2]) break;

  const double x_dx = (pos[0] - x[i]) / dx;
  const double y_dy = (pos[1] - y[j]) / dy;
  const double z_dz = (pos[2] - z[k]) / dz;

  double Bv000x = Bx[i][j][k];
  double Bv000y = By[i][j][k];
  double Bv000z = Bz[i][j][k];
  double Bv001x = Bx[i][j][k+1];
  double Bv001y = By[i][j][k+1];
  double Bv001z = Bz[i][j][k+1];
  double Bv010x = Bx[i][j+1][k];
  double Bv010y = By[i][j+1][k];
  double Bv010z = Bz[i][j+1][k];
  double Bv011x = Bx[i][j+1][k+1];
  double Bv011y = By[i][j+1][k+1];
  double Bv011z = Bz[i][j+1][k+1];
  double Bv100x = Bx[i+1][j][k];
  double Bv100y = By[i+1][j][k];
  double Bv100z = Bz[i+1][j][k];
  double Bv101x = Bx[i+1][j][k+1];
  double Bv101y = By[i+1][j][k+1];
  double Bv101z = Bz[i+1][j][k+1];
  double Bv110x = Bx[i+1][j+1][k];
  double Bv110y = By[i+1][j+1][k];
  double Bv110z = Bz[i+1][j+1][k];
  double Bv111x = Bx[i+1][j+1][k+1];
  double Bv111y = By[i+1][j+1][k+1];
  double Bv111z = Bz[i+1][j+1][k+1];

  // 4 points - z elimination
  Bv000x += z_dz * (Bv001x - Bv000x);
  Bv000y += z_dz * (Bv001y - Bv000y);
  Bv000z += z_dz * (Bv001z - Bv000z);
  Bv010x += z_dz * (Bv011x - Bv010x);
  Bv010y += z_dz * (Bv011y - Bv010y);
  Bv010z += z_dz * (Bv011z - Bv010z);
  Bv100x += z_dz * (Bv101x - Bv100x);
  Bv100y += z_dz * (Bv101y - Bv100y);
  Bv100z += z_dz * (Bv101z - Bv100z);
  Bv110x += z_dz * (Bv111x - Bv110x);
  Bv110y += z_dz * (Bv111y - Bv110y);
  Bv110z += z_dz * (Bv111z - Bv110z);

  // 2 points - y elimination
  Bv000x += y_dy * (Bv010x - Bv000x);
  Bv000y += y_dy * (Bv010y - Bv000y);
  Bv000z += y_dy * (Bv010z - Bv000z);
  Bv100x += y_dy * (Bv110x - Bv100x);
  Bv100y += y_dy * (Bv110y - Bv100y);
  Bv100z += y_dy * (Bv110z - Bv100z);

  Bv000x += x_dx * (Bv100x - Bv000x);
  Bv000y += x_dx * (Bv100y - Bv000y);
  Bv000z += x_dx * (Bv100z - Bv000z);

  B[0] = Bv000x;
  B[1] = Bv000y;
  B[2] = Bv000z;
  //cout << "f(" << pos[0] << ", " << pos[1] << ", " << pos[2] << ")="
  //     << "(" << Bv000x << ", " << Bv000y << ", " << Bv000z << ")\n";
  return true;
}

//===================================================================================
bool LinInterpolation::IsNamed(const char* fname) const
{
  return (string(filename) == fname);
}

//===================================================================================
LinInterpolation& LinInterpolation::operator=(const LinInterpolation& p)
{
  if (&p == this) return *this;
  *this = p;
  return *this;
}

//===================================================================================
LinInterpolation::LinInterpolation(const LinInterpolation& r)
{
  Bx = r.Bx;
  By = r.By;
  Bz = r.Bz;
  strcpy(filename, r.filename);
  dx = r.dx;
  dy = r.dy;
  dz = r.dz;
  x = r.x;
  y = r.y;
  z = r.z;
  NX = r.NX;
  NY = r.NY;
  NZ = r.NZ;
}

