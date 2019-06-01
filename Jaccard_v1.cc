#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <ctime>

using namespace std;

void rellena(map<string,int>& m, string document, int& cont) {

	string line; 
	ifstream font(document);
    if(not font.is_open()) {
     cout << "El la ruta: " << document << " no es correcte" << endl;
     exit(0);
    }
    map<string,int>::iterator it;
    
	if( font.is_open() ) {
		while( getline (font,line) ) {
			string aux ="";
			for(int i = 0; i < line.size(); ++i) {
				if(line[i] != ' ') aux += line[i];
				else if(aux != "") {
                        ++cont; // afegim una paraula.
                        it= m.find(aux);
                        if(it == m.end() ) m.insert( pair<string,int>(aux,1) );
                        else {
                        int auxI = it->second+1; 
                        it->second = auxI;
                        }
                        aux="";
                }
				
			}
			if(aux != "") {
                ++cont;
                it= m.find(aux);
                if(it == m.end() ) m.insert( pair<string,int>(aux,1) );
                else {
                    int auxI = it->second;
                    m.erase(it);
                    m.insert(it,pair<string,int>(aux,auxI+1) );
                }
            }
		}
	}
}


void jaccard(string t1, vector<string>& t2){

    map<string,int>::iterator it;
    map<string,int>::iterator it2;
    
    for ( int i = 0; i < t2.size(); ++i) { // comparem aquest document amb tota la resta que hi ha per devant, anteriors no cal ja que han estat comparats anteriorment.
        map<string,int> map1; 
        int cont1=0;
        
        rellena(map1, t1,cont1);
    
        int total =0; int inter =0;  int cont2=0;
        map<string,int> map2;
        
        rellena(map2, t2[i],cont2);
        
        for (it=map1.begin(); it!=map1.end(); ++it){
            it2 = map2.find(it->first);
            if( it2 != map2.end() ) {
                int eq = min(it->second, it2->second);
                inter += eq;
                cont1 -= eq; cont2 -= eq;
            }
        }/*
        total = inter + cont1 + cont2;
        cout << "Comparació entre: " << t1 << " i " << t2[i] << endl;
        float f = (float)inter/(float)total;
        cout << "La divisió es: " << f <<endl<< endl;*/
    }

    
}


int main(int argc, char *argv[]) {
    const clock_t begin_time = clock();
    
    if(argc < 3 ){
            cout << "Error: la entrada no es adecuada => ./exe Rutadocument1 RutaDocument2 ... RutaDocument_n " << endl;
            exit(0);
    }
    for( int k = 1; k < argc-1; ++k) { // l'ultim no tenim amb que comparar-lo.
        string t1 = "";
        t1 = argv[k]; 
        vector<string> t2;
        int j = k+1;
        for ( int i = 0 ; i < argc - j; ++i) t2.push_back(argv[i+j]); 
        jaccard(t1,t2);
    }
        
    cout << float( clock() - begin_time) /CLOCKS_PER_SEC << endl;
}

