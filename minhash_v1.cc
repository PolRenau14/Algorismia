#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <ctime>  
#include <algorithm>

using namespace std;




void creaSet(set<string>& s, string document) {

	string line; 
	ifstream font(document);
    if(not font.is_open()) {
     cout << "El la ruta: " << document << " no es correcte" << endl;
     exit(0);
    }
    set<string>::iterator it;
    
	if( font.is_open() ) {
		while( getline (font,line) ) {
			string aux ="";
			for(int i = 0; i < line.size(); ++i) {
				if(line[i] != ' ') aux += line[i];
				else {
                    if(aux != "") {
                        it= s.find(aux);
                        if(it == s.end() ) s.insert( aux );
                        aux="";
                    }
				}
			}
			if(aux != "") {
                it= s.find(aux);
                if(it == s.end() ) s.insert(aux );
            }
		}
	}
}

unsigned int hash1(string s)
{
    unsigned int hash = 5381;
    unsigned int c;

    for(int i = 0; i < s.size(); ++i) {
        c = s[i];
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
    }
}

unsigned int minHash( set<string>& s,int a, int b, int c) {
        set<string>::iterator it;
        
        unsigned int min;
        unsigned int aux;
        for( it = s.begin() ; it != s.end(); ++it) {
            aux = hash1(*it);
            aux = (a*aux+b)%c;
           if(it == s.begin()) min = aux;
           else if( min > aux ) min = aux; 
        }
        return min;
}


/* Calcula lo semblants que son tots els documents entre si, i ens mostra per pantalla, quins tenen 
 * una probabilitat de semblança superior a 75%. 
 * */
void calcSemblants(vector< vector<int> > &hashTable, char *argv[] ) {
    for(int i = 0; i < hashTable[i].size(); ++i) {
        for(int j = i+1; j< hashTable[i].size(); ++j ) {
            float sim=0;
                for(int k = 0; k < hashTable.size(); ++k) {
                    sim += hashTable[k][i] == hashTable[k][j];
                }
            sim = sim/hashTable.size();
            //if(sim > 0.75 ) cout << "La semblança es: " << sim << " Entre els documents " << argv[i+1] << " y " << argv[j+1] << endl;
        }
    }
}

void calculaHash(vector< set <string> > &paraules_document, vector< vector<int> > &m) {
    int c =  99839;
    for( int i = 0; i< m.size(); i++) {
            // creem cada funció de hash ( la mateixa per cada document) en cada iteraciio
            int a, b;
            a = rand(); b = rand();
            for( int j = 0; j < m[i].size(); ++j) 
                m[i][j] = minHash(paraules_document[j],a,b,c);            
    }
    
}

int main( int argc, char *argv[] ) {
    if(argc < 3 ){
            cout << "Error: la entrada no es adecuada => ./exe Rutadocument1 RutaDocument2 ... RutaDocument_n " << endl;
            exit(0);
    }
    vector< set<string> > paraules_document(argc-1);
    //cout << "Introdueixi el numero de les funcions de hash" << endl;
    int k=500;
    //cin >> k;
    
    const clock_t begin_time = clock();
    // Per cada document creem un set de totes les paraules que conté
    for(int i = 1; i < argc;++i) {
        set<string> s;
        creaSet(s,argv[i]);
        paraules_document[i-1] = s;
    }
    

    vector< vector<int> > m(k,vector<int>(argc-1));
    calculaHash(paraules_document,m);
    
    calcSemblants(m,argv);
    
    cout << float( clock() - begin_time) /CLOCKS_PER_SEC << endl;
    
}

    

    


