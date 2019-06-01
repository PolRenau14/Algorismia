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
        if( s.size()< k  ) {
            cout << "Error: el tamany de la k supera el valor de caracters " << s.size() << endl;
            exit(0);        
        }
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


void generateCandidates(int b, int r , vector<vector<int> > &signature  , char *argv[], double threshold , int & total) {
    //Genera els candidat per al LSH
    hash<string> h; //to hash all rows of all bands.
    int ndocs = signature[0].size();
    vector <vector<int> > buckets(b, vector<int> (ndocs)); //vector[i] -> vector<int> on cada int és el bucket de la fila r

    for (int band = 0; band < b; band++) {
        for (int c = 0; c < ndocs; c++) {
            char aux[r];
            for (int row = 0; row < r; row++) {
                aux[row] = signature[band*r+row][c];
            }
            //at this point, a single row is computed.
            string aux1 = aux;
            buckets[band][c] = h(aux1);
        }
    }
map<pair<int,int>,double> candidates;
    //processing candidates;
    for (int band = 0; band < b; band++) {
        for (int c = 0; c < ndocs-1; c++) {
            for (int k = c+1; k < ndocs; k++) {
                if (buckets[band][c] == buckets[band][k]) {
                    pair<int,int> p;
                    p.first = c;
                    p.second = k;
                    candidates[p]= 0;
                }
            }
        }
    }

    std::map<pair<int,int>,double>::iterator it = candidates.begin();

    while (it != candidates.end()) {
        it->second = jaccardSimilarity(it->first.first, it->first.second,signature);
        if(it->second > threshold) cout << argv[it->first.first +1] << " es similar amb " << argv[it->first.second+1]  << " en un " << it->second <<endl;
        it++;
    }

}

int main(int argc, char *argv[]) {
    if(argc < 3 ){
            cout << "Error: la entrada no es adecuada => ./exe Rutadocument1 RutaDocument2 ... RutaDocument_n " << endl;
            exit(0);
    }
    int k, bandesTam,rTam; double threshold,t_in; k=5; bandesTam=100; rTam =5;
    cout << "Introdueixi el valor de divisions de las paraules (k) "<< endl; cin >> k;
    cout << "Introdueixi el nombre de funcions de bandes" << endl; cin >> bandesTam;
    cout << "Introdueixi les files per banda" << endl; cin >> rTam;
    double b = bandesTam;
    double r = rTam;
    threshold = 0.76;//pow((1/b),(1/r)); //t ~ (1/b)^(1/r) 
    
    cout << "Introdueixi el valor del threshold (per defecte usarem el auto calculat, seleccioneu 0 per el autocalculat) "<< threshold << endl; cin >> t_in;
    if( t_in != 0) threshold=t_in;
    const clock_t begin_time = clock();
    vector<vector<int> > caracteristica;
    generaCM(argc,argv,k,caracteristica); // Generem la matriu de Caracteristics => Paraules caracteristiques(no repetides tamany k) * nº de documents
    
    vector<vector<int> >signature;
    generaSM(caracteristica,signature,bandesTam*rTam);int total=0;
    generateCandidates(bandesTam,rTam,signature,argv, threshold,total);
     cout << float( clock() - begin_time) /CLOCKS_PER_SEC << endl;
     cout << "El total es " << total << endl;
    
    
    
    
}
