/*
 * Este prograba recibe el nombre de un archivo que contiene las posiciones y velocidades iniciales de
 * estrellas en una galaxia y lo evoluciona a través del tiempo (5 mil millones de años) e imprime
 * archivos del estado actual de la galaxia cada mil millones de años.
 * 
 * Creado por Camilo Rivera.  24/10/13
 * 
 * 
 * 
 */

#include <stdio.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
#include <math.h>
#include <stdlib.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_cblas.h>
#include <string.h>
#include <stdlib.h>

#define M pow(10,12)
#define G 4.302*pow(10,-6)


void leer_archivo(char *nombre,gsl_matrix *destino,int lineas, int *centros);
int count_lines(char *filename);
void print_matrixg(gsl_matrix *m, int n_x, int n_y);
void imprimir_matriz(gsl_matrix *Mat_inicial,int archivos, int filas);
void runge_4tta(double h,gsl_matrix *actual,gsl_matrix *siguiente,int *centros, int filas);
float hallar_relat(float dx,float dy,int eje);

int main(int argc, char **argv){
	/*Se crean índices para las iteraciones del programa 
	 * variable que almacena el número de estrellas
	 * número de centros de masa
	 * Las posiciones de los centros de masa en las matrices
	 * Un contador de archivos escritos*/
	int j,i,k,num_est,*num_CM,archivos=1;
	
	/*Se crean las matrices donde se guardarán los datos del archivo y las iteraciones*/
	gsl_matrix *Mat_inicial,*Mat_siguiente;
	
	
	/*Se define el paso para las iteraciones temporales
	 * el contador para dichas iteraciones (tiempo)*/
	double paso=0.0001,tiempo=0;
	
	/*Se definen variables de utilidad para las iteraciones
	 * (Aceleración en X y Y, distancias en X y Y, ...)*/
	 double a_x,a_y;
	 
	 
	/*Comprobación de argumentos*/
	if(argc!=2){
	printf("Se debe especificar el nombre del archivo donde se encuentran las condiciones iniciales\n");
	exit(1);
	}
	
	///////*Alocación de memoria*//////////
	
	//Primer se realiza el conteo de lineas para la alocación de memoria
	num_est=count_lines(argv[1]);
	//printf("%d\n",num_est);
	num_CM=malloc(sizeof(int)*2);
	/*Se inicializan las posiciones de las estrellas
	 * Es importante tener en cuenta de ahora en adelante que si hay una única
	 * galaxia, sólo se escribirá la primera entrada y la otra permanecerá en -1
	 * mientras que si hay dos galaxias se escribirán ambas entradas.*/
	num_CM[0]=-1;
	num_CM[1]=-1;
	
	Mat_inicial = gsl_matrix_alloc (num_est, 5);
	Mat_siguiente = gsl_matrix_alloc (num_est, 5);
	
	/*Se procede a la lectura del archivo y escritura de la matriz inicial*/
	leer_archivo(argv[1],Mat_inicial,num_est,num_CM);
	
	//print_matrixg(Mat_inicial,num_est,5);
	/*Se procede a iterar el método de Runge-Kutta y cada vez que pasen Mil millones
	 * de años se exporta un archivo. Al final del ciclo se exporta un último archivo*/
	gsl_matrix_set_zero (Mat_siguiente);
	for(tiempo=0;tiempo<5u;tiempo+=paso){
		runge_4tta(paso,Mat_inicial,Mat_siguiente,num_CM,num_est);
		if(tiempo>1u && archivos==1){
			imprimir_matriz(Mat_inicial,archivos,num_est);
			printf("Se exportó el primer archivo\n");
			archivos++;
		}
		if(tiempo>2u && archivos==2){
			imprimir_matriz(Mat_inicial,archivos,num_est);
			printf("Se exportó el segundo archivo\n");
			archivos++;
		}
		if(tiempo>3u && archivos==3){
			imprimir_matriz(Mat_inicial,archivos,num_est);
			printf("Se exportó el tercer archivo\n");
			archivos++;
		}
		if(tiempo>4u && archivos==4){
			imprimir_matriz(Mat_inicial,archivos,num_est);
			printf("Se exportó el cuarto archivo\n");
			archivos++;
		}
	}
	imprimir_matriz(Mat_inicial,archivos,num_est);
	printf("Se exportó el quinto archivo\n");
	printf("%d %d \n",num_CM[0],num_CM[1]);

	//////Liberación de espacio alocado///////
	gsl_matrix_free(Mat_inicial);
	gsl_matrix_free(Mat_siguiente);
	free(num_CM);
	return 1;
}

/*Conteo de líneas para asignación en memoria*/
int count_lines(char *filename){
	
  FILE *in;
  int n_lines;
  int c;
  if(!(in=fopen(filename, "r"))){
    printf("problem opening file %s\n", filename);
    exit(1);
  }

  n_lines = 0;
  do{
    c = fgetc(in);
    if(c=='\n'){
      n_lines++;
    }
  }while(c!=EOF);
  
  fclose(in);
  return n_lines;
}

/*Función que realiza la escritura del archivo en una matriz GSL y devuelve el número de CM*/
void leer_archivo(char *nombre,gsl_matrix *destino,int lineas,int *centros){
	
	FILE *in;
	int i,j, cont=0;
	float *aux;
	if(!(aux = malloc(sizeof(float)*lineas*5))){
		fprintf(stderr, "Problem with memory allocation");
		exit(1);		
	}
	in = fopen(nombre,"r"); 
	if(!in){
		printf("Hubo un problema abriendo el archivo %s\n", nombre);
		exit(1);
	}
	/*Se escriben los datos del archivo en la matriz auxiliar*/ 
	for(i=0;i<lineas;i++){
		fscanf(in, "%f %f %f %f %f \n", &aux[i*5], &aux[i*5+1],&aux[i*5+2],&aux[i*5+3],&aux[i*5+4]); 
	}
	fclose(in);
	
	/*Se escriben los archivos de la matrix auxiliar en la matriz GSL*/
	for(i=0;i<lineas;i++){
		for(j=0;j<5;j++){
			gsl_matrix_set (destino,i,j, (double)aux[5*i+j]);
		}
		if(aux[5*i]<0){
			centros[cont]=i;
			cont++;
		}
	}
	
	free(aux);
}

/*Método auxiliar para imprimir matrices GSL*/
void print_matrixg(gsl_matrix *m, int n_x, int n_y){
	
	int i,j;
	fprintf(stdout, "\n");
	/*Prints to screen*/
	for(i=0;i<n_x;i++){
		for(j=0;j<n_y;j++){
			fprintf(stdout, " %f ",gsl_matrix_get(m, i, j));
		}
		fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");

}

/*Función que exporta los archivos de una matriz GSL*/
void imprimir_matriz(gsl_matrix *Mat_inicial,int archivos, int filas){
	/*Se define el nombre base de los archivos que se van a exportar*/
	 FILE *in1;
	 int i,j;
	 char *nombre, *result, *num;
	 result=malloc(sizeof(char)*50);
	 nombre=malloc(sizeof(char)*50);
	 num=malloc(sizeof(char)*50);
	 nombre="Estado";
	 sprintf(num, "%d", archivos);
	 strcpy(result, nombre);
	 strcat(result, num);
	 in1 = fopen(result,"w"); 
	/*A continuación se recorre la matriz GSL y se imprimen sus valores en el archivo
	 * con nombre "Estado de galaxia"+archivos*/
	 for(i=0;i<filas;i++){
		for(j=0;j<5;j++){
			fprintf(in1, "%f ",gsl_matrix_get(Mat_inicial, i, j));
		}
		fprintf(in1, "\n");
	}
	fprintf(in1, "\n");
	fclose(in1);
	free(result);
	 
}

/*Función que halla aceleraciones dadas unas distancias relativas. Si recibe
 * un 0 en el último parámetro devuelve la aceleración en x
 * Si recibe un 1, devuelve la aceleración en el eje y*/
float hallar_relat(float dx,float dy,int eje){
	//Valores relativos de los puntos a considerar fuerzas
	float radio2,cos_theta, sin_theta, at, ax, ay;
	
	radio2=pow(dx,2)+pow(dy,2);
	cos_theta=dx/sqrt(radio2);
	sin_theta=dy/sqrt(radio2);
	at=(float)-1*G*M/radio2;
	ax=at*cos_theta;
	ay=at*sin_theta;
	
	if(eje==0){
		return ax;
	}
	else{
		return ay;
	}
		
}

/*Función que realiza una iteración de Runge-kutta de 4 orden*/
void runge_4tta(double h,gsl_matrix *actual,gsl_matrix *siguiente,int *centros, int filas){
	int i,j,k;
	//Valores para uso en la iteración de runge-kutta
	float ax,ay;
	//Valores relativos de los puntos a considerar fuerzas
	float delta_x, delta_y;
	//Variables auxiliares para la iteracipon
	float vx,vy,xaux,yaux;
	//Variables al inicio del intervalo
	float vox,voy,xo,yo;
	
	/*Parámetros de Runge-Kutta. Parámetros con K corresponden a posición y Parámetros con L a velocidad
	 * El los vectores tiene 5 posiciones, las 4 primeras corresponden a los 4 valores de runge-kutta
	 * mientras que la última corresponde al total*/
	float *kx,*ky,*lx,*ly;
	
	/*Esta función realiza una iteración sobre toda la matriz, realizando el cálculo
	 * sobre "siguiente" y luego copiando todos los datos sobre actual.
	 * Se tienen los índices de los centros para revisar con facilidad
	 * las distancias y las fuerzas.
	 * h es el paso temporal que se va a realizar. 
	 * Se deben realizar dos Runge-kutta para cada fila de la matriz por lo cual se itera
	 * sobre las filas, evitando aquellas que están dentro de los índices de *centros.
	 * Si en *centros uno de los índices es negativo, es porque sólo hay un CM; una galaxia.
	*/
	
	//Se realiza la alocación de memoria para los vectores
	kx=malloc(sizeof(float)*5);
	ky=malloc(sizeof(float)*5);
	lx=malloc(sizeof(float)*5);
	ly=malloc(sizeof(float)*5);
	
	//Se recorren todos las filas de la matriz
	for(i=0;i<filas;i++){
		
		
		ax=0;
		ay=0;
		/*El siguiente loop es para hallar la aceleración total que siente la masa actual al ir directamente 
		 * a los índices de los centros de masa*/
		for(j=0;j<2;j++){
			//Se verifica que el centro de masa actual exista y que no sea el índice de la masa actual
			if(centros[j]!=-1 && centros[j]!=i){
				//Se hallan las variables relativas a dicho punto
				delta_x=(float)gsl_matrix_get(actual,centros[j],1)-gsl_matrix_get(actual,i,1);
				delta_y=(float)gsl_matrix_get(actual,centros[j],2)-gsl_matrix_get(actual,i,2);
				ax+=hallar_relat(delta_x,delta_y,0);
				ay+=hallar_relat(delta_x,delta_y,1);
			}
		}
		
		vox=gsl_matrix_get(actual,i,3);
		voy=gsl_matrix_get(actual,i,4);
		xo=gsl_matrix_get(actual,i,1);
		yo=gsl_matrix_get(actual,i,2);
		
		xaux=xo;
		yaux=yo;
		vx=vox;
		vy=voy;
		
		//Método de Runge-Kutta
		
		//K1
		kx[0]=vx*h;
		ky[0]=vy*h;
		lx[0]=ax*h;
		ly[0]=ay*h;
		
		//Primer paso
		xaux=xo+0.5*kx[0];
		yaux=yo+0.5*ky[0];
		vx=vox+0.5*lx[0];
		vy=voy+0.5*lx[0];
		ax=0;
		ay=0;
		for(j=0;j<2;j++){
			if(centros[j]!=-1 && centros[j]!=i){
				delta_x=gsl_matrix_get(actual,centros[j],1)-xaux;
				delta_y=gsl_matrix_get(actual,centros[j],2)-yaux;
				ax= ax + hallar_relat(delta_x,delta_y,0);
				ay= ay + hallar_relat(delta_x,delta_y,1);
			}
		}
		
		//K2
		kx[1]=vx*h;
		ky[1]=vy*h;
		lx[1]=ax*h;
		ly[1]=ay*h;
		
		//Segundo Paso
		xaux=xo+0.5*kx[1];
		yaux=yo+0.5*ky[1];
		vx=vox+0.5*lx[1];
		vy=voy+0.5*lx[1];
		ax=0;
		ay=0;
		for(j=0;j<2;j++){
			if(centros[j]!=-1 && centros[j]!=i){
				delta_x=gsl_matrix_get(actual,centros[j],1)-xaux;
				delta_y=gsl_matrix_get(actual,centros[j],2)-yaux;
				ax+=hallar_relat(delta_x,delta_y,0);
				ay+=hallar_relat(delta_x,delta_y,1);
			}
		}
		
		//K3
		kx[2]=vx*h;
		ky[2]=vy*h;
		lx[2]=ax*h;
		ly[2]=ay*h;
		
		//Tercer Paso
		xaux=xo+kx[2];
		yaux=yo+ky[2];
		vx=vox+lx[2];
		vy=voy+lx[2];
		ax=0;
		ay=0;
		for(j=0;j<2;j++){
			if(centros[j]!=-1 && centros[j]!=i){
				delta_x=gsl_matrix_get(actual,centros[j],1)-xaux;
				delta_y=gsl_matrix_get(actual,centros[j],2)-yaux;
				ax+=hallar_relat(delta_x,delta_y,0);
				ay+=hallar_relat(delta_x,delta_y,1);
			}
		}
		
		//K4
		kx[3]=vx*h;
		ky[3]=vy*h;
		lx[3]=ax*h;
		ly[3]=ay*h;
		
		//K
		kx[4]=(kx[0]+2*kx[1]+2*kx[2]+kx[3])/6;
		ky[4]=(ky[0]+2*ky[1]+2*ky[2]+ky[3])/6;
		lx[4]=(lx[0]+2*lx[1]+2*lx[2]+lx[3])/6;
		ly[4]=(ly[0]+2*ly[1]+2*ly[2]+ly[3])/6;
		
		//Cuarto Paso
		xaux=xo+kx[4];
		yaux=yo+ky[4];
		vx=vox+lx[4];
		vy=voy+lx[4];
		
		gsl_matrix_set (siguiente,i,1,xaux);
		gsl_matrix_set (siguiente,i,2,yaux);
		gsl_matrix_set (siguiente,i,3,vx);
		gsl_matrix_set (siguiente,i,4,vy);	
		gsl_matrix_set(siguiente,i,0,gsl_matrix_get(actual,i,0));
	}
	
	/*Una vez se terminan de escribir los datos del nuevo instante de tiempo
	 * de la matriz siguiente, se copia la información en la matriz actual*/
	gsl_matrix_memcpy (actual, siguiente);
	gsl_matrix_set_zero (siguiente);
	
	free(kx);
	free(ky);
	free(lx);
	free(ly);
	
}

