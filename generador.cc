#include <iostream>
#include <vector>
#include <ctime>  
#include <algorithm>
#include <fstream>
#include <string>

using namespace std;



vector<int> index(50);

/* NO SABEM SI ES NECESARI.
string purga(string s) {
	
	if(s[s.size()-1] =='.' ) s.pop_back();
	return s;
}
*/


/*Pre: name -> nom del fitxer
 * Post: genera un fitxer de nom (name) on te les paraules de S desordenades (document original) 
 * */

void escriu(string name,vector<string> &s){
	srand ( unsigned ( std::time(0) ) );
	random_shuffle(s.begin(),s.end() );
	ofstream fs(name);
	for(int i =0; i < s.size();++i) fs << s[i] << " " ;
	fs.close();
}


/*Pre: tenir un fitxer de entrada
 * Post: a S guardarem cada paraula en una posició del vector de strings S.
 * 
 * 
 * */

void rellena(vector<string> &s) {
	int p =0;
	string line; 
        ifstream font("font50.txt");
	if( font.is_open() ) {
		while( getline (font,line) ) {
			string aux ="";
			for(int i = 0; i < line.size(); ++i) {
				if(line[i] != ' ') aux += line[i];
				else {
					s.push_back(aux); aux =""; ++p;	
				}
			}	
			s.push_back(aux);
		}
	}
}

int main() {
    
        vector<string> s;
	rellena(s);
	for(int i = 0; i < index.size(); ++i) index[i]=i;	
	for (int j = 1; j <= 20; ++j) {
		string iterator = to_string(j);
		string name = "font50_" + iterator + ".txt";
		escriu(name,s);
	}


}
