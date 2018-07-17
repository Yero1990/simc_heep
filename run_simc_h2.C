void get_normfac(string ifile);


void run_simc_h2()
{


  string INDIR = "./infiles/";
  string OUTDIR = "./outfiles/";
  string WORKSIM="./worksim_voli/";

  //********** read input kinematics data file *************

  ifstream ikin;
  ofstream ofile;
  string ikin_file = "input_kinematics.data";
  string line;

  vector <string> ikin_vec;

  
  ikin.open(ikin_file);

  if (!ikin)
    {
      cerr << "Unable to open file";
      exit(1);   // call system to stop
    }

  while (ikin >> line)
    {
      
      //ignore commented input files
      if ( line[0]=='#')
	{
	  cout << "ignoring " + line << endl;
	  
	}

      //read kinematic input file
      else
	{
	  cout << "Reading " + line << endl;
	  ikin_vec.push_back(line);

	}
      
    }

  //************************************************************

  string CMD; // string command to be used as terminal commands
  //Loop over each kinematic input file
    for (int i = 0; i<ikin_vec.size(); i++)
      {
	string input_path = INDIR+ikin_vec[i];
	string ikin_file = ikin_vec[i];

	cout << "removing ./infiles/current.data . . . " << endl;

	gSystem->Exec("rm ./infiles/current.data"); 
	
	cout << "copy: " << input_path.c_str() << " to ./infiles/current.data " << endl; 
	//copy input file name to current.data
	gSystem->CopyFile(input_path.c_str(), "./infiles/current.data"); 

	//Run SIMC  
	gSystem->Exec("./run_simc.sh current.data");
	
	//extract normalization factor 
	get_normfac(OUTDIR+"current.hist");

	//convert to root file
	gSystem->Exec("root -l -q fmake_tree.C");
	    

	//replace '.data' with '.root' in file name
	//	cout << ikin_file << endl;
	size_t pos = ikin_file.find("data");
	
	
	ikin_file.replace(pos, std::string("data").length(), "root");   // 5 = length( $name )
	 
       
	string oldname = WORKSIM + "simc.root";
	string newname = WORKSIM + ikin_file;

	//define command to change file name 
	CMD = "mv " + oldname + " " + newname;
       
	//Execute command to change root file name
	gSystem->Exec(CMD.c_str());
	
	ofile << ikin_file << endl;;
	ofile.close();
	
	//EXIT ROOT
	//gSystem->Exit(kTRUE);
	
	
      }
    
  
}

//---------------------------------------
void get_normfac(string ifile)
{

  ifstream file;

  string line;
  string found;
  size_t pos;
  file.open (ifile); 

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
  ofstream ofile;
  ofile.open("normfact.data");
  ofile << found;
  ofile.close();
  
}




