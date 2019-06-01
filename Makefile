all:generador.exe Jaccard_v1.exe Jaccard_v2.exe Jaccard_v3.exe minhash.exe minhash_v1.cc minhash_v2.exe LSH.exe

Jaccard: Jaccard_v1.exe Jaccard_v2.exe Jaccard_v3.exe

Minhash: minhash.exe minhash_v1.exe minhash_v2.exe

Locale: LSH.exe

Generador: generador.exe

generador.exe: generador.cc
	g++ -std=c++11 -O3 -o  generador.exe generador.cc

Jaccard_v1.exe: Jaccard_v1.cc
	g++ -std=c++11 -O3 -o  Jaccard_v1.exe Jaccard_v1.cc

Jaccard_v2.exe: Jaccard_v2.cc
	g++ -std=c++11 -O3 -o  Jaccard_v2.exe Jaccard_v2.cc

Jaccard_v3.exe: Jaccard_v3.cc
	g++ -std=c++11 -O3 -o  Jaccard_v3.exe Jaccard_v3.cc

minhash.exe: minhash.cc
	g++ -std=c++11 -O3 -o  minhash.exe minhash.cc

minhash_v1.exe: minhash_v1.cc
	g++ -std=c++11 -O3 -o  minhash_v1.exe minhash_v1.cc

minhash_v2.exe: minhash_v2.cc
	g++ -std=c++11 -O3 -o  minhash_v2.exe minhash_v2.cc

LSH.exe: LSH.cc
	g++ -std=c++11 -O3 -o  LSH.exe LSH.cc

clean:
	rm *.exe
