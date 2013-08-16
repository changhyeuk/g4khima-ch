//*****************************************************************************
//  GaramGlobal.cc
//                           - 2012-11-21, by Garam HAHN.
//*****************************************************************************

#include "GaramGlobal.hh"

#include <iostream>
#include <sstream>
#include <ctime>
#include <locale>
#include <exception>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

using namespace std;

namespace garam
{
  //============================================================================
  vector<string>& ssplit(const string &s,
                         char delim,
                         vector<string> &elems)
  {
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
      elems.push_back(item);
    }
    return elems;
  }

  //============================================================================
  vector<string> ssplit(const string &s, char delim)
  {
    vector<string> elems;
    return ssplit(s, delim, elems);
  }

  //============================================================================
  vector<double> ssplit_to_doubles(const string &s, char delim)
  {
    vector<string> elems;
    ssplit(s, delim, elems);
    vector<double> darr;
    for(vector<string>::iterator it = elems.begin();
        it != elems.end();
        ++it)
    {
      darr.push_back(to_double(*it));
    }
    return darr;
  }

  //============================================================================
  vector<int> ssplit_to_ints(const string &s, char delim)
  {
    vector<string> elems;
    ssplit(s, delim, elems);
    vector<int> iarr;
    for(vector<string>::iterator it = elems.begin();
        it != elems.end();
        ++it)
    {
      iarr.push_back(to_int(*it));
    }
    return iarr;
  }

  //============================================================================
  int to_int(const string s)
  {
    stringstream ss(s);
    int int_a = 0;
    ss >> int_a;
    return int_a;
  }

  //============================================================================
  double to_double(const string s)
  {
    stringstream ss(s);
    double double_a = 0;
    ss >> double_a;
    return double_a;
  }
  
  //============================================================================
  const std::string timeformat()
  {
    //std::locale::global(std::locale("en_US.utf8"));
    char buff[16];
    time_t t = time(NULL);
    strftime(buff, 16, "%Y%m%d-%H%M%S", localtime(&t));
    return string(buff);
  }
  
  //============================================================================
  const char* mkdir_p(const char* dirname)
  {
    string sdir = string(dirname) + "/";
    int last_pos = -1;
    size_t pos = sdir.find("/", last_pos+1);
    while (pos != string::npos)
    {
      if ((last_pos + 1) == (int) pos)
      {
        sdir.erase(pos, 1);
      }
      else
      {
        last_pos = int(pos);
        if (!opendir(sdir.c_str()))
        {
          mkdir(sdir.substr(0, int(pos)).c_str(), 0755);
          cout << sdir.substr(0, int(pos)) << " was created." << endl;
        }
      }
      pos = sdir.find("/", last_pos+1);
    }
    return sdir.c_str();
  }
  
}

