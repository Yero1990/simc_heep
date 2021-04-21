#include "utilities.h"

//void get_normfac(string ifile);

void run_simc_h2(string ikin_filename)
{
  ofstream ofile;


  string INDIR = "./infiles/";
  string OUTDIR = "./outfiles/";
  string WORKSIM="./worksim_voli/";
  string WORKDISK="./SIMC/";
  //********** read input kinematics data file *************


  //************************************************************

  string CMD; // string command to be used as terminal commands
  
  string input_path = INDIR+ikin_filename;
  
  
  //Run SIMC  
  string cmd1 ="./run_simc.sh "+ikin_filename;
  gSystem->Exec(cmd1.c_str());

  //replace '.data' with '.hist' in file name
  size_t pos = ikin_filename.find("data");
  string hfname = ikin_filename.replace(pos, std::string("data").length(), "hist");   // 5 = length( $name )
  
  cout << "data file name: " << ikin_filename << endl;
  cout << "hist file name: " << hfname << endl;
  
  string hfname_path = "outfiles/"+hfname;
  //extract normalization factor 

  Double_t normfac = stod(split(FindString("normfac",hfname_path.c_str())[0], '=')[1]);
  string kin_ext = split(ikin_filename, '.')[0];
  string normfact_name = "normfact_"+kin_ext+".data";
  ofile.open(normfact_name, std::fstream::out);
  ofile << normfac << endl;
  ofile.close();

  pos = ikin_filename.find("hist");
  string kfname = ikin_filename.replace(pos, std::string("hist").length(), "data");

  //convert to root file
  string cmd = Form("root -l -b -q \"fmake_tree.C(\\\"%s\\\")\"", kfname.c_str());
  gSystem->Exec(cmd.c_str());
  
  //Delete normfac datafile, as it is not needed anymore
  cmd = "rm "+normfact_name;
  gSystem->Exec(cmd.c_str());

  pos = ikin_filename.find("data");
  
  ikin_filename.replace(pos, std::string("data").length(), "root");   // 5 = length( $name )
  
  
  string oldname = WORKSIM + ikin_filename;
  string newname = WORKDISK + ikin_filename;
  
  //define command to change file name 
  CMD = "mv " + oldname + " " + newname;
  
  //Execute command to change root file name
  gSystem->Exec(CMD.c_str());
  
  ofile << ikin_filename << endl;;
  ofile.close();
  
 
}

/*
//---------------------------------------
void get_normfac(string ifile)
{

  ifstream file;
  ofstream ofile;
  string line;
  string found;
  size_t pos;




  file.open (ifile.c_str()); 

  //find the line containing normfac
  while (getline(file, line))    //6 being the 6th line
    {
      pos = line.find("normfac");
      if (pos==19){
      found = line.substr(pos);
      }
    }

  found.erase (0, 14);  //remove trailing spaces

  //write to file

  
}
*/



