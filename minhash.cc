#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <functional>
#include <cmath>
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

void leeDocs(int argc, char *argv[], int k, vector<string>& paraules) {
        for( int i = 1; i < argc; ++i) {
                string s="";
                leeTodo(argv[i], s);
                paraules.push_back(s); // afegim per cada document tot el document passat a string
        }
}



void afegeixParaules(vector<string> &vs, string& s ,int k) {
        for( int i = 0; i < s.size()-k +1 ; ++i ) {
            string aux = "";
            vs.push_back(aux.append(s,i,k));
        }        
}

int getsingleMax(vector<vector<string> > &ksingle) {
        int max = ksingle[0].size();
        for(int i = 1; i < ksingle.size(); ++i )  {
                if(ksingle[i].size() > max) max = ksingle[i].size();
        }
        return max;
}

/*
 * al set<int> propi, tenim els hash de les paraules
 * */
void setDoc(vector<string> &s, set<int> &propi, int max) {
    hash<string> h;
    for(int i = 0; i < s.size(); ++i) {
        int hashDet = h(s[i])%max; 
        // como maximo un valor de ksing max.
        if( propi.find(hashDet) == propi.end()) propi.insert(hashDet);
    }
}

void noRepGen( set<int> &noRep, vector<set<int> > paraulesDocument) {
        set<int>::iterator it;
        for( int i = 0; i < paraulesDocument.size();++i ) {
            for( it = paraulesDocument[i].begin(); it != paraulesDocument[i].end();++it) {
                if( noRep.find(*it) == noRep.end() ) noRep.insert(*it);
            }
        }
}

void generaCM(int argc, char *argv[] , int k, vector<vector<int> > &caracteristica) {
        vector<string> paraules; 
        leeDocs(argc,argv,k,paraules); // Pas previ lectura de documents, tot el docu pasat a string en un vector de strings ( cada string un docu. ) tamany de paraules igual al nombre de documents
        //PAs 1 generem Ksingles
        vector<vector<string> > ksingles;
        for( int i = 0; i < paraules.size(); ++i) {
                vector<string> aux;
                afegeixParaules(aux,paraules[i], k);
                ksingles.push_back(aux); // afegim els ksingles de cada document
        }
        
        int ksingleMax = getsingleMax(ksingles);
        
        vector<set<int> > paraulesDocument;
        for(int i = 0; i < ksingles.size(); ++i) {
            set<int> propi;
            setDoc(ksingles[i],propi,ksingleMax);
            paraulesDocument.push_back(propi);
        }
        
        set<int> noRep;
        noRepGen(noRep,paraulesDocument);       
        caracteristica = vector<vector<int> >(noRep.size(),vector<int>(paraulesDocument.size(),0));
        
        set<int>::iterator it;
        int index =0;
        for(it = noRep.begin(); it != noRep.end();++it) {
            for(int i = 0; i < paraulesDocument.size(); ++i ) {
                    if(paraulesDocument[i].find(*it) != paraulesDocument[i].end() ) {
                        caracteristica[index][i]=1;
                    }
            }
            ++index;
        }
        
}

void generaSM(vector<vector<int> > &caracteristica, vector<vector<int> > &signature,int numFhash) {
    vector< pair<int,int> > hashFunctions(numFhash);
    
    for( int hf = 0; hf < numFhash; ++hf) {
        int a = rand()%100; int b = rand()%100;
        pair<int,int> parell = pair<int,int>(a,b);
        hashFunctions[hf]=parell;
    }
    
    int numDocs = caracteristica[0].size();
    signature = vector<vector<int> > (numFhash, vector<int>(numDocs, caracteristica.size()));
    for (int r = 0; r < caracteristica.size(); r++) {
        for (int c = 0; c < numDocs; c++) {
            if (caracteristica[r][c]==1) {
               for (int h = 0; h < numFhash; h++) {
                    int hashValue = (hashFunctions[h].first*r + hashFunctions[h].second)%caracteristica.size();
                   signature[h][c] = min( hashValue , signature[h][c] );
               }
            }
        }
    }   
}

double jaccardSimilarity(int d1, int d2,vector<vector<int> > &signature) {
    double value = 0;
    for (int k = 0; k < signature.size(); k++) {
        if (signature[k][d1] == signature[k][d2]) value++;
    }
    value = value / signature.size();
    return value;
}


void generateCandidates(vector<vector<int> > &signature  , char *argv[] ,double threshold, int &total) {

    //processing candidates;
    int ndocs = signature[0].size();
    for (int c = 0; c < ndocs-1; c++) {
        for (int k = c+1; k < ndocs; k++) {
            float comp = jaccardSimilarity(c,k,signature);
            if (comp > threshold) cout << argv[c +1] << " es similar amb " << argv[k+1] << " en un " << comp <<endl;
        }
    }

}

int main(int argc, char *argv[]) {
    if(argc < 3 ){
            cout << "Error: la entrada no es adecuada => ./exe Rutadocument1 RutaDocument2 ... RutaDocument_n " << endl;
            exit(0);
    }
    int k, numFhash; double threshold;k=5; numFhash=5000; threshold=0.75;
   cout << "Introdueixi el valor de divisions de las paraules (k) "<< endl; cin >> k;
   cout << "Introdueixi el nombre de funcions de Hash" << endl; cin >> numFhash;
   cout << "Introdueixi el valor del threshold  "<< threshold << endl; cin >> threshold;
   
    
    const clock_t begin_time = clock();
    vector<vector<int> > caracteristica;
    generaCM(argc,argv,k,caracteristica); // Generem la matriu de Caracteristics => Paraules caracteristiques(no repetides tamany k) * nº de documents
    
    vector<vector<int> >signature;
    generaSM(caracteristica,signature,numFhash); int total=0;
    generateCandidates(signature,argv,threshold,total);
     cout << float( clock() - begin_time) /CLOCKS_PER_SEC << endl;
     
    
    
    
    
}
