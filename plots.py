# encoding: UTF-8
import matplotlib.pyplot as plt
import numpy as np
import sys, string, os
from pylab import *

#Primero se lee la cantidad de archivos que se deben graficar
n_args = len(sys.argv)
#Se crea la figura donde se dibujarán las posiciones
fig = plt.figure()
for i in range(n_args-1):
	#Se extrae uno a uno los archivos (la primera posición es el llamado de la función)
	mat=np.loadtxt(sys.argv[i+1])
	#Se Crea el subplot correspondinte
	a=fig.add_subplot(1,n_args-1,i)
	#Se grafica la posición de todos los puntos
	a.scatter(mat[:,1],mat[:,2],color='blue')
	#Se coloca el título del plot
	tt=a.set_title(sys.argv[i+1])
	#SE hallan los índices con números negativos en la 1 columna (CM)
	b=[k for k,v in enumerate(mat[:,0]) if v <0]
	#Se recorren los índices anteriores y se grafican en rojo
	for m in b:
		a.scatter(mat[m,1],mat[m,2],color='red')
plt.axis('equal')
plt.show()
