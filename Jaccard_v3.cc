#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <ctime>

using namespace std;

/* Pre: p es una string
 * Post: retorna la string p on només hi ha lletres a - z en minuscules i majusculas, la resta ha estat eliminat ( comes , simbols puntuació, espais en blanc ...) 
 * */
string purga(string p) {
    
    string aux = "";  
    for(int i = 0; i < p.size(); ++i ) {
        if( ( p[i] >= 65 and  p[i] <= 90) or ( p[i] >= 97 and p[i] <= 122) ) {
            aux.push_back(p[i]);
        }
    }
    return aux;
}


/*Pre: la ruta es correcte (document)
 * 
 * Post posa tot lo que hi ha en el document en la string s.
 * */
void leeTodo(string document, string& s) {
	string line; 
	ifstream font(document);
	if( font.is_open() ) {
		while( getline (font,line) ) {
            s += purga(line);
		}
	}
	else {
     cout << "Error: el fitxer "<< document << " no s'ha pogut obrir." << endl;
     exit(0);        
    }
}


/* Pre: string s conte tot el document que conte el document, se es buit k es la longitud de les substrings.
 * Post: retorna totes les substrings de tamany k en el string s.
 * */
void genKsingle(string s, set<string>& se , int k ) {
        for( int i = 0; i < s.size()-k +1 ; ++i ) {
            string aux = "";
            se.insert(aux.append(s,i,k) );
        }    
}


/* Pre: se1 i se2, son els sets complets de document1 i document2.
 * Post: Mostra per pantalla la unio, la intersecció, i la divisió.
 * */
void compara(set<string>& se1, set<string>& se2) {
    set<string>::iterator it;
    set<string>::iterator it2;
    
    int inter, unio; inter=unio=0;
    for(it = se1.begin(); it != se1.end(); ++it) {
        it2 = se2.find(*it);
        if(it2 != se2.end() ) {
            ++inter;
            se1.erase(it); se2.erase(it2);
        }
    }
    unio = inter + se1.size() + se2.size();
    //cout << "La divisio es: " << (float)inter/(float)unio << endl;
}

/* Pre: document 1 ha de ser ruta valida, i tots els strings del document2 han de ser valids
 * Post: Mostra la comparacio de document 1 amb tots els documents que estan a document2.
 * */
void jaccard(string document1, vector<string>& document2,int k) {

    for ( int i = 0; i < document2.size(); ++i) {
        string s1,s2; s1 = s2 ="";
        
        leeTodo(document1,s1);
        leeTodo(document2[i],s2);
        
        set<string> se1;
        set<string> se2;
        if( k > s1.size() or s2.size() < k ) {
            cout << "Error: no es poden generar substrings de tamany " << k << ", ja que un dels documents no compleix que #caracters_document >= "<< k << endl;
            exit(0);
        }
        genKsingle(s1,se1,k);
        genKsingle(s2,se2,k);
        //cout << "Comparació entre: " << document1 << " i " << document2[i] << endl;
        compara(se1,se2);
        //cout << endl;
    }
    
}

int main(int argc, char *argv[]) {
     if(argc < 3 ){
            cout << "Error: la entrada no es adecuada => ./exe Rutadocument1 RutaDocument2 " << endl;
            exit(0);
    }   
   // cout << "Introdueixi el valor de K" << endl;
    int k=5; //cin >>k;
    
    const clock_t begin_time = clock();
    for( int q = 1; q < argc-1; ++q) { // l'ultim no tenim amb que comparar-lo.
        string document1 = "";
        document1 = argv[q]; 
        vector<string> document2;
        int j = q+1;
        for ( int i = 0 ; i < argc - j; ++i) document2.push_back(argv[i+j]); 
        jaccard(document1,document2,k);
    }
    cout << float( clock() - begin_time) /CLOCKS_PER_SEC << endl;
}
