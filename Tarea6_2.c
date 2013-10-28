//
//  Tarea6_2.c
//  
//
//  Created by Alejandro Gonzalez && Camilo Rivera on 26/10/13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_linalg.h>


#define G 1
#define M 1
#define dt 0.001

//Definir las funciones derivadas

double func_prime_1(t, x_1, x_2){
	return (x_2);
	}
double  func_prime_2(t, x_1, x_2, r2){
	return (G*M/r2);
	}

int main(int argc, char **argv){

	FILE *ci, *out1, *out2, *out3, *out4, *out5;
	int i, j, lineas, c, c2;   
	double tiempo;
	double x, xc, yc, vxc, vyc, xi, yi, vxi, vyi, r2; 
	double xt,vxt,yt,vyt;
	double k11x, k11y, k12x, k12y, t1, x1, vx1, y1, vy1, k21x, k22x, k21y, k22y;
	double t2, x2, vx2, y2, vy2, t3, vy3, k31x, x3, k32x, vx3, k32y, k31y, y3;
	double k41x, k41y, k42x, k42y, kprom1x, kprom1y, kprom2x, kprom2y;
	char text;

	
	
    //abrir el archivo de condiciones iniciales
    
    ci = fopen(argv[1], "r");
    if (!ci){
        printf("problemas al abrir el archivo:  %s\n", argv[1]);
        exit(1);
	}
    else {
        printf("el archivo se abrio exitosamente\n");
	}
    
	//Contador de lineas
	rewind(ci);
	lineas = 0;
	while((text = fgetc(ci)) != EOF){
		if(text == '\n'){
			lineas ++;
			}
		}

	printf("el numero total de lineas es %d \n", lineas);

	//abrir los archivos de salida
	out1 = fopen("mil_millones.dat","w");
	out2 = fopen("dosmil_millones.dat","w");
	out3 = fopen("tresmil_millones.dat","w");
	out4 = fopen("cuatromil_millones.dat","w");
	out5 = fopen("cincomil_millones.dat","w");



    // Declarar la matriz de condiciones iniciales
    
	

    gsl_matrix * condiciones_iniciales = gsl_matrix_alloc(lineas,5);
    
    // Llena la matriz con los datos del archivo
    
    rewind(ci);
    
    
    gsl_matrix_fscanf ( ci, condiciones_iniciales);
    
// imprime la matriz de datos (para verificar)

/*    for (i=0; i<lineas; i++){
	for (j=0; j<5; j++){
		x = gsl_matrix_get(condiciones_iniciales, i,j);
		printf("%lf ",x);
		}
	printf("\n");
	} 
*/
// Recorrer la matriz de condiciones iniciales y calcular la evolucion para cada punto
	c = 0;
	c2 = 0;
	printf("Numero de lineas: %d \n", lineas);
	for (i=0; i<lineas; i++){
		//evolucion del centro
		if(gsl_matrix_get(condiciones_iniciales, i,0)== -1){
			xc = gsl_matrix_get(condiciones_iniciales, i,1);
			yc= gsl_matrix_get(condiciones_iniciales, i,2);
			vxc = gsl_matrix_get(condiciones_iniciales, i,3);
			vyc = gsl_matrix_get(condiciones_iniciales, i,1);
			xc = xc + (vxc*dt);
			yc = yc + (vyc*dt);
			
			fprintf(out1, "%d %lf %lf %lf %lf\n", -1,xc,yc,vxc,vyc);
			fprintf(out2, "%d %lf %lf %lf %lf\n", -1,xc,yc,vxc,vyc);
			fprintf(out3, "%d %lf %lf %lf %lf\n", -1,xc,yc,vxc,vyc);
			fprintf(out4, "%d %lf %lf %lf %lf\n", -1,xc,yc,vxc,vyc);
			fprintf(out5, "%d %lf %lf %lf %lf\n", -1,xc,yc,vxc,vyc);


			}
		
		else{
			xc = gsl_matrix_get(condiciones_iniciales, 0,1);
			yc= gsl_matrix_get(condiciones_iniciales, 0,2);

			xi = gsl_matrix_get(condiciones_iniciales, i,1);
			yi = gsl_matrix_get(condiciones_iniciales, i,2);
			vxi = gsl_matrix_get(condiciones_iniciales, i,3);
			vyi = gsl_matrix_get(condiciones_iniciales, i,4);
			r2 = pow((xi-xc),2) + pow((yi-yc),2);
			

			xt = xi;
			vxt = vxi;
			
			yt = yi;
			vyt = vyi;


			//c2++;
			//printf("%d \n", c2);
			c=0;
			for (tiempo = 0; tiempo <=5; tiempo+= dt){
				c++;
				//printf("%d \n", c);
				k11x = func_prime_1(tiempo,xt,vxt);
				k12x = func_prime_2(tiempo,xt,vxt, r2);
				
				k11y = func_prime_1(tiempo,yt,vyt);
				k12y = func_prime_2(tiempo,yt,vyt, r2);

				//Primer paso
				t1 = tiempo + (dt/2.0);
				x1 = xt + (dt/2.0)*k11x;
				vx1 = vxt + (dt/2.0)*k12x;

				y1 = yt + (dt/2.0)*k11y;
				vy1 = vyt + (dt/2.0)*k12y;

				k21x = func_prime_1(t1, x1, vx1);
				k21y = func_prime_1(t1, y1, vy1);

				k22x = func_prime_2(t1, x1, vx1, r2);
				k22y = func_prime_2(t1, y1, vy1, r2);


				//Segundo paso
				t2 = tiempo + (dt/2.0);
				x2 = xt + (dt/2.0)*k21x;
				vx2 = vxt + (dt/2.0)*k22x;
			
				y2 = yt + (dt/2.0)*k21y;
				vy2 = vyt + (dt/2.0)*k22y;

				k31x = func_prime_1(t2, x2, vx2);
				k31y = func_prime_1(t2, y2, vy2);

				k32x = func_prime_2(t2, x2, vx2, r2);
				k32y = func_prime_2(t2, y2, vy2, r2);

				//Tercer Paso
				t3 = tiempo + dt;
				x3 = xt + dt*k31x;
				vx3 = vxt + dt*k32x;

				y3 = yt + dt*k31y;
				vy3 = vyt + dt*k32y;

				k41x = func_prime_1(t3, x3, vx3);
				k41y = func_prime_1(t3, y3, vy3);

				k42x = func_prime_2(t3, x3, vx3, r2);
				k42y = func_prime_2(t3, y3, vy3, r2);
				
				//Cuarto paso
				kprom1x = (1.0/6.0)*(k11x + 2.0*k21x + 2.0*k31x + k41x);
				kprom1y = (1.0/6.0)*(k11y + 2.0*k21y + 2.0*k31y + k41y);

				kprom2x	= (1.0/6.0)*(k12x + 2.0*k22x + 2.0*k32x + k42x);
				kprom2y = (1.0/6.0)*(k12y + 2.0*k22y + 2.0*k32y + k42y);

				xt = xt + dt * kprom1x;
				vxt = vxt + dt * kprom2x;

				yt = yt + dt * kprom1y;
				vyt = yt + dt * kprom2y;
				
				if (c == 1000){
					// imprimir los datos en el archivo 1
					fprintf(out1, "%d %lf %lf %lf %lf\n", i,xt,yt,vxt,vyt);
					}
				if (c == 2000){
					// imprimir los datos en el archivo 2
					fprintf(out2, "%d %lf %lf %lf %lf\n", i,xt,yt,vxt,vyt);
					//c2++;
					//printf("%d \n", c2);
					}
				if (c == 3000){
					// imprimir los datos en el archivo 3
					fprintf(out3, "%d %lf %lf %lf %lf\n", i,xt,yt,vxt,vyt);
					}
				if (c == 4000){
					// imprimir los datos en el archivo 4
					fprintf(out4, "%d %lf %lf %lf %lf\n", i,xt,yt,vxt,vyt);
					}
				if (c == 5000){
					// imprimir los datos en el archivo 5
					fprintf(out5, "%d %lf %lf %lf %lf\n", i,xt,yt,vxt,vyt);
					}

				}
					

			}
		
		}		
	
}


