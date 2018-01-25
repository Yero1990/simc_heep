//script to setup links to run SIMC from another directory


int setup_simc()
{
  
  
  //Adjust directories to your needs
  TString SIMCDIR = "~/HallC/deut_simc/";
  
  //setup symbolic links to SIMC directory
  TString link_simc = "ln -s " + SIMCDIR+"simc";
  TString link_hms = "ln -s " + SIMCDIR+"hms";
  TString link_shms = "ln -s " + SIMCDIR+"shms";
  TString link_fmake_tree = "ln -s " + SIMCDIR+"root/fmake_tree";
  TString link_h2theory = "ln -s " + SIMCDIR+"h2.theory";
  TString link_runsimc = "ln -s " + SIMCDIR+"run_simc.sh";
  TString link_nml_data = "ln -s " + SIMCDIR+"nml_default.data";
  
  system(link_simc);
  system(link_hms);
  system(link_shms);
  system(link_fmake_tree);
  system(link_h2theory);
  system(link_runsimc);
  system(link_nml_data);

  
  //Set up local directories2
  
  
  
  //Create SIMC  Directories if they dont exist
  const char *dir_infiles = "mkdir ./infiles";
  const char *dir_worksim = "mkdir ./worksim";
  const char *dir_outfiles = "mkdir ./outfiles";
  const char *dir_err = "mkdir ./err";
  const char *dir_log = "mkdir ./log";

       
  system(dir_infiles);
  system(dir_worksim);
  system(dir_outfiles);
  system(dir_err);
  system(dir_log);
      
  
  
  return 0;
  
}
