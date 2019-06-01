#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <ctime>  
#include <algorithm>
#include <set>
#include <ctime>
using namespace std;



/*Pre: mapa tam es el nombre de documents totals, id es el document que estem tractant
 * Post: En mapa tenim totes les paraules de Doument, a part com a value, tenim un vector que referencia quin document te quina paraula.
 * 
 * */
void creaMapa(map<string, vector<int> > &mapa , string document,int tam,int id) {

	string line; 
	ifstream font(document);
    if(not font.is_open()) {
     cout << "El la ruta: " << document << " no es correcte" << endl;
     exit(0);
    }
    map<string,vector<int> >::iterator it;
    
	if( font.is_open() ) {
		while( getline (font,line) ) {
			string aux ="";
			for(int i = 0; i < line.size(); ++i) {
                if(line[i] != ' ') aux += line[i];
                else if(aux != "") {
                        it= mapa.find(aux);
                        if(it == mapa.end() ) {
                            vector<int> v(tam,0);
                            v[id] =1;
                            mapa.insert( pair<string, vector<int> >(aux,v)  );
                        }
                        else {
                            it->second[id]=1; // posem a 1 per indicar que esta                            
                        }
                        aux="";
                    }
            }
			if(aux != "") {
                    it= mapa.find(aux);
                    if(it == mapa.end() ) {
                        vector<int> v(tam,0);
                        v[id] =1;
                        mapa.insert( pair<string, vector<int> >(aux,v) );
                    }
                    else {
                        it->second[id]=1; // posem a 1 per indicar que esta                            
                    }
            }
		}
	}
}

/*Pre: x es la fila que hem de fer hashing, c es el nombre total de paraules. 
 * Post: retorna un valor entre 0- c-1 de x. apliant hash
 * */
int hashing(int a, int b, int c, int x) {return(a*x+b)%c;}

/* Omple la taula de hash, on per cada document aplicarem un k hashos, i a la matriu, tindrem la primera fila 
 * per cada document, que a la matriu boleanMatrix
 * Es 1 ( la paraula existeix ) aplicant el hash r 
 * */
void ompleHashTable(vector< vector<int> > &boleanMatrix, vector< vector<int> > &hashTable ) {
    int c = boleanMatrix.size(); // nombre total de paraules
     for(int r = 0; r < hashTable.size(); ++r) {
        int a, b; // Hash value
        a = rand()%100; b = rand()%100; 
        
        for( int i = 0; i < boleanMatrix[0].size(); ++i) {
            int min = c;
            for(int j = 0; j < c; ++j) {
                int aux = hashing(a,b,c,j);
                if(boleanMatrix[j][i] == 1 and  aux < min ) min = aux;
            }
            hashTable[r][i] = min;
        }
    }   
}

/* Calcula lo semblants que son tots els documents entre si, i ens mostra per pantalla, quins tenen 
 * una probabilitat de semblança superior a 75%.  cost constant relament.
 * */
void calcSemblants(vector< vector<int> > &hashTable, char *argv[] ) {
    
    for(int i = 0; i < hashTable[0].size(); ++i) {
        for(int j = i+1; j< hashTable[0].size(); ++j ) {
            float sim=0;
                for(int k = 0; k < hashTable.size(); ++k) {
                    sim += hashTable[k][i] == hashTable[k][j];
                }
            sim = sim/hashTable.size();
           // if(sim > 0.75 ) cout << "La semblança es: " << sim << " Entre els documents "<< argv[i+1] << " y " << argv[j+1] << endl;
        }
    }
}

int main( int argc, char *argv[] ) {
    if(argc < 3 ){
            cout << "Error: la entrada no es adecuada => ./exe Rutadocument1 RutaDocument2 ... RutaDocument_n " << endl;
            exit(0);
    }
    
    //cout << "Introdueixi el numero de les funcions de hash" << endl;
    int k=500;
    //cin >> k;
    
    
    const clock_t begin_time = clock();
    // Per cada document creem un set de totes les paraules que conté
    
    map<string, vector<int> > s;   
    map<string, vector<int> >::iterator it;
   
    for(int i = 1; i < argc;++i) creaMapa(s,argv[i], argc -1,i-1);
    
    
    vector< vector<int> > boleanMatrix;
    
    //Construim la matriu boleana
   for(it = s.begin(); it != s.end(); ++it) boleanMatrix.push_back(it->second);
   
    vector< vector<int> > hashTable(k,vector<int>(boleanMatrix[0].size())); 
   
    ompleHashTable(boleanMatrix,hashTable);
   calcSemblants(hashTable,argv);
   
    cout << float( clock() - begin_time) /CLOCKS_PER_SEC << endl;
   
}

    

    


