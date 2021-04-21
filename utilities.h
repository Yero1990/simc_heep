#ifndef UTIL_FUNC
#define UTIL_FUNC 

#include <iostream>
#include <string>
#include <algorithm>


//----------------------------------UTILITIES FUNCTIONS--------------------------------------
//__________________________________________________
int get_total_lines(string rlist="")
{

  //Open a file and replace a string 
  fstream fs;
  ofstream ofs;
  ifstream ifs;

  string line;
  int cnt=0;

  //input runlist
  ifs.open(rlist);


  while(getline(ifs, line))
    {
      cnt++;
  
      //cout << "line counter: " << cnt << endl;

    }

  return cnt;
}


//_____________________________________________________________________________
string getString(char x)
{
  //method to convert a character to a string
  string s(1,x);
  return s;
}

//__________________________________________________________________
vector <string> split(string str, char del=':')
{

  //method to split a string into a vetor of strings separated by a delimiter del
  //Returns a vector of strings, whose elements are separated by the delimiter del.

  string temp1, temp2;

  vector <string> parse_word;
  int del_pos;  //delimiter position
    
    for (int i=0; i < str.size(); i++){

      //Get position of delimiter
      if(str[i]==del){
	del_pos = i;
      }

    }

    for (int i=0; i < str.size(); i++){

      //append char to a string for char left of the delimiter
      if(i<del_pos){
	temp1.append(getString(str[i]));
      }      

      //append char to a string for char right of the delimiter
      else if(i>del_pos){
	temp2.append(getString(str[i]));
      }

    }
    parse_word.push_back(temp1);
    parse_word.push_back(temp2);
    
    return parse_word;
}

//_______________________________________________________________________________________
vector <string> FindString(string keyword, string fname)
{

  //Method: Finds string keyword in a given txt file. 
  //Returns the lines (stored in a vector) in which the keyword was found. Lines are counted from 0. 
  
  ifstream ifile(fname);

  vector <string> line_found; //vector to store in which lines was the keyword found
  
  int line_cnt = 0;
  string line;                  //line string to read
  
  int found = -1; //position of found keyword

  while(getline(ifile, line))
    {
        
      found = line.find(keyword);
      
      if(found<0||found>1000){found=-1;} //not found

      if(found!=-1){
	
	line_found.push_back(line);
	

      } //end if statement
    
      line_cnt++;
    } //end readlines

  return line_found;

}//end test.C


//--------Functions for removing leading/trailing white spaces----------
std::string& ltrim(std::string& s)
{
  auto it = std::find_if(s.begin(), s.end(),
			 [](char c) {
			   return !std::isspace<char>(c, std::locale::classic());
			 });
  s.erase(s.begin(), it);
  return s;
}

std::string& rtrim(std::string& s)
{
  auto it = std::find_if(s.rbegin(), s.rend(),
						[](char c) {
			   return !std::isspace<char>(c, std::locale::classic());
			 });
  s.erase(it.base(), s.end());
  return s;
}

std::string& trim(std::string& s)
{
  return ltrim(rtrim(s));
}
//-----------------------------------------------------------------------------

#endif 
