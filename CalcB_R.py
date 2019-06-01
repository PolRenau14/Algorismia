import random
import subprocess
import sys
import string
from subprocess import Popen, PIPE

file = open("Sortida_Jaccard.txt","w")
dispersos = ["./a.out", "./Textos/Disp_1.txt", "./Textos/Disp_2.txt", "./Textos/Disp_3.txt", "./Textos/Disp_4.txt", "./Textos/Disp_5.txt", "./Textos/Disp_6.txt", "./Textos/Disp_7.txt", "./Textos/Disp_8.txt", "./Textos/Disp_9.txt", "./Textos/Disp_10.txt"]
casiidentics = ["./a.out", "./Textos/Casi_1.txt", "./Textos/Casi_2.txt", "./Textos/Casi_3.txt", "./Textos/Casi_4.txt", "./Textos/Casi_5.txt", "./Textos/Casi_6.txt", "./Textos/Casi_7.txt", "./Textos/Casi_8.txt"]
parafscanviats = ["./a.out", "./Textos/Parafs_1.txt", "./Textos/Parafs_2.txt", "./Textos/Parafs_3.txt", "./Textos/Parafs_4.txt", "./Textos/Parafs_5.txt", "./Textos/Parafs_6.txt", "./Textos/Parafs_7.txt", "./Textos/Parafs_8.txt"]
random = ["./a.out", "./Textos/Harry_4.txt", "./Textos/Harry_5.txt", "./Textos/Harry_6.txt", "./Textos/Harry_7.txt", "./Textos/Harry_8.txt", "./Textos/Harry_9.txt", "./Textos/Harry_10.txt", "./Textos/Harry_11.txt","./Textos/Harry_17.txt", "./Textos/Harry_19.txt"]

for y in range(1, 5):
	paths = None
	nom = None
	if y == 1:
		paths = dispersos
		nom = " amb els 10 documents molt dispersos entre ells"
	elif y == 2:
		paths = casiidentics
		nom = " amb els 8 documents casi identics"
	elif y == 3:
		paths = parafscanviats
		nom = " amb els 8 documents casi identics pero una mica desordenats"
	elif y == 4:
		paths = random
		nom = " amb 10 documents Grans diferents"
	file.write("La comparacio de LSH" + nom + ":\n")
	subprocess.call(["g++","LSH.cc"])
	tmp = subprocess.Popen(paths,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
	stdout,stderr = tmp.communicate()
	file.write(stdout)
	print(stdout)


file.close()

	#a = input() llegeix de la linia de comandos
	#print(a)
	#file2.append(line)   #append serveix per escriure al final del fitxer

	
