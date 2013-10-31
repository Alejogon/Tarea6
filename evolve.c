//
//  evolve.c
//  
//
//  Created by Alejandro Gonzalez && Camilo Rivera on 26/10/13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_linalg.h>


#define G 7.2*pow(10,16)
#define M 1
#define dt 0.001

//Definir las funciones derivadas


//funciones para la evolucion de la masa 1
double func_primec1_1(t, x,v){
	return v;
	}
double func_primec1x_2(t,x,v,xc1,xc2,yc1,yc2, M2){
	double teta, r2, a;
	
	teta = atan((y2-y1)/(x2-x1));
	r2 = pow((x2-x1),2) + pow((y2-y1),2);

	a = G*M2/r2*cos(teta);
	return a;
	}
double func_primec1y_2(t,x,v,xc1,xc2,yc1,yc2, M2){
	double teta, r2, a;
	teta = atan((y2-y1)/(x2-x1));
	r2 = pow((x2-x1),2) + pow((y2-y1),2);

	a = G*M2/r2*sin(teta);
	return a;
	}


//funciones para la evolucion de la masa 2
double func_primec2_1(t, x,v){
	return v;
	}
double func_primec2x_2(t,x,v, y, xc1,xc2,yc1,yc2, M1){
	double teta, r2, a;    
 	teta = atan((y1-y2)/(x1-x2));
 	r2 = pow((x2-x1),2) + pow((y2-y1),2);
 	    
 	a = G*M2/r2*cos(teta);
 	return a;
	}

double func_primec2y_2(t,x,v, y, xc1, xc2, yc1, yc2, M1){
	double teta, r2, a;
 	teta = atan((y1-y2)/(x1-x2));
 	r2 = pow((x2-x1),2) + pow((y2-y1),2);
	a = G*M2/r2*sin(teta);
 	return a;
	}



//funciones para la evolucion de cada punto

double  func_prime_1(t, x, v){
	return v;
	}

double func_primex_2(t, xi, v, yi, x1, y1, x2,y2, M1){
	double teta1, r21, r22, teta2, ax1, ax2;

	teta1 = atan((y1-yi)/(x1-xi));
	teta2 = atan((y2-yi)/(x2-xi));
	
	r21 = pow((x1-xi),2)+pow((y1-yi),2);
	r22 = pow((x2-xi),2)+pow((y2-yi),2);

	ax1 = G*M1/r21*cos(teta1);
	ax2 = G*M2/r22*cos(teta2);

	return ax1 + ax2;
	}

double func_primey_2(t, yi, v, xi, x1, y1, x2,y2, M1){
	double teta1, r21, r22, teta2, ay1, ay2;
 
	teta1 = atan((y1-yi)/(x1-xi));
	teta2 = atan((y2-yi)/(x2-xi));
   
	r21 = pow((x1-xi),2)+pow((y1-yi),2);
	r22 = pow((x2-xi),2)+pow((y2-yi),2);
    
	ay1 = G*M1/r21*sin(teta1);
	ay2 = G*M2/r22*sin(teta2);

	return ay1 + ay2;
	}




	

int main(int argc, char **argv){

	int i, j, lineas, c, c2, ct;   
	double tiempo;
	double x, xc1, yc1, vxc1, vyc1, M1, xc2, yc2, vxc2, vyc2, M2, xi, yi, vxi, vyi, r2; 
	double xt,vxt,yt,vyt;
	double k11ix, k11iy, k12ix, k12iy, t1, x1, vx1, y1, vy1, k21ix, k22ix, k21iy, k22iy;
	double k11c1x, k12c1x, k11c1y, k12c1y, k21c1x, k22c1x, k21c1y, k21c1y;
	double k31c1x, k32c1x, k31c1y, k32c1y, k41c1c, k42c1x, k42c1y, k42c1y;
        double k11c2x, k12c2x, k11c2y, k12c2y, k21c2x, k22c2x, k21c2y, k21c2y;
        double k31c2x, k32c2x, k31c2y, k32c2y, k41c2c, k42c2x, k42c2y, k42c2y;


	double t2, x2, vx2, y2, vy2, t3, vy3, k31ix, x3, k32ix, vx3, k32iy, k31iy, y3;
	double k41ix, k41iy, k42ix, k42iy, kprom1x, kprom1y, kprom2x, kprom2y;
	char text;
	int centros[10];
	//Variables del primer centro de masa
	double xc1, yc1, vxc1, vyc1, M1;
	//Variables del segundo centro de masa
	double xc2, yc2, vxc2, vyc2, M2;
	
	
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
    

	// Inicializamos los datos de los centros de masa

	xc1 = yc1 = vxc1 = vyc1 = M1 = xc2 = yc2 = vxc2 = vyc2 = M2 = 0;

	//recorremos la matriz para determinar los datos de los centros de masa recibidos como parametro en el archivo
	c = 0;
	for(i=0; i<lineas; i++){
		if ( gsl_matrix_get(condiciones_iniciales, i, 0) == -1){
			
			centros[c] = i;	
			c++;
			}
		}
	M1 = M; 
 	xc1 = gsl_matrix_get(condiciones_iniciales, 0, 1);
        yc1 = gsl_matrix_get(condiciones_iniciales, 0, 2); 
	vxc1 = gsl_matrix_get(condiciones_iniciales, 0, 3);
	vyc1 = gsl_matrix_get(condiciones_iniciales, 0, 4);
	
	if( c ==  2){
		M2 = M;
		xc2 = gsl_matrix_get(condiciones_iniciales, centros[1], 1);
		yc2 = gsl_matrix_get(condiciones_iniciales, centros[1], 2);
		vxc2 = gsl_matrix_get(condiciones_iniciales, centros[1], 3);
		vyc2 = gsl_matrix_get(condiciones_iniciales, centros[1], 4);

		}

	for(i=0; i<= lineas; i++){
		M1 = M;
		xc1 = gsl_matrix_get(condiciones_iniciales, 0, 1);
		yc1 = gsl_matrix_get(condiciones_iniciales, 0, 2);
		vxc1 = gsl_matrix_get(condiciones_iniciales, 0, 3);
		yc1 = gsl_matrix_get(condiciones_iniciales, 0, 4);
		if( c ==  2){
			M2 = M;
			xc2 = gsl_matrix_get(condiciones_iniciales, centros[1], 1);
			yc2 = gsl_matrix_get(condiciones_iniciales, centros[1], 2);
			vxc2 = gsl_matrix_get(condiciones_iniciales, centros[1], 3);
			vyc2 = gsl_matrix_get(condiciones_iniciales, centros[1], 4);
			}
		ct = 0;
		for(tiempo = 0; tiempo <= 5; tiempo+=dt){
			ct ++;
			//evolucion de los centros de masa
			//centro 1
			k11c1x = func_primec1_1(tiempo, xc1, vxc1);
			k12c1x = func_primec1x_2(tiempo, xc1, vxc1, xc1, xc2, yc1, yc2, M2);
			k11c1y = func_primec1_1(tiempo, yc1, vyc1);
			k12c1y = func_primec1y_2(tiempo, yc1, vyc1, xc1, xc2, yc1, yc2, M2);
			//primer paso
			t1 = tiempo + (dt/2.0);
			x1 = xc1 + (dt/2.0)*k11c1x;
			vx1 = vxc1 + (dt/2.0)*k12c1x;
			k21c1x = func_primec1_1(t1, x1, vx1);
			k22c1x = func_primec1x_2(t1, x1, vx1,  xc1, xc2, yc1, yc2, M2);

			y1 = yc1 + (dt/2.0)*k11c1y;
			vy1 = vyc1 + (dt/2.0)*k12c1y;
			k21c1y = func_primec1_1(t1, y1, vy1);
			k22c1y = func_primec1y_2(t1, y1, vy1,  xc1, xc2, yc1, yc2, M2);
			//segundo paso
			t2 = tiempo + (dt/2.0);
			x2 = xc1 + (dt/2.0)*k21c1x;
			vx2 = vxc1 + (dt/2.0)*k22c1x;
			k31c1x = func_primec1_1(t2, x2, vx2);
			k32c1x = func_primec1x_2(t2, x2, vx2,  xc1, xc2, yc1, yc2, M2);

			y2 = yc1 +(dt/2.0)*k21c1y;
			vy2 = vyc1 + (dt/2.0)*k22c1y;
			k31c1y = func_primec1_1(t2, y2, vy2);
			k32c1y = func_primec1y_2(t2, y2, vy2,  xc1, xc2, yc1, yc2, M2);

			//tercer paso
			t3 = tiempo + dt;
			x3 = xc1 + dt*k31c1x;
			vx3 = vxc1 + dt*k32c1x;
			k41c1x = func_primec1_1(t3, x3, vx3);
			k42c1x = func_primec1x_2(t3, x3, vx3,  xc1, xc2, yc1, yc2, M2);

			y3 = yc1 + dt*k31c1y;
			vy3 = vyc1 + dt*k32c1y;
			k41c1y = func_primec1_1(t3, y3, vy3);
			k42c1y = func_primec1y_2(t3, y3, vy3,  xc1, xc2, yc1, yc2, M2);

			//cuarto paso
			kprom1x = (1.0/1.6)*(k11c1x + 2.0*k21c1x + 2.0*k31c1x + k41c1x);
			kprom2x = (1.0/1.6)*(k12c1x + 2.0*k22c1x + 2.0*k32c1x + k42c1x);
			kprom1y = (1.0/1.6)*(k11c1y + 2.0*k21c1y + 2.0*k31c1y + k41c1y); 
			kprom2y = (1.0/1.6)*(k12c1y + 2.0*k22c1y + 2.0*k32c1y + k42c1y);
			
			xc1 = xc1 + dt*kprom1x;
			vxc1 = vxc1 + dt*kprom2x;

			yc1 = yc1 + dt*kprom1y;
			vyc1 = vyc1 + dt*kprom2y;

			//centro 2

			k11c2x = func_primec2_1(tiempo, xc2, vxc2);
			k12c2x = func_primec2x_2(tiempo, xc2, vxc2,  xc1, xc2, yc1, yc2, M2);
			k11c2y = func_primec2_1(tiempo, yc2, vyc2);
			k12c2y = func_primec2y_2(tiempo, yc2, vyc2,  xc1, xc2, yc1, yc2, M2);
			//primer paso
			t1 = tiempo + (dt/2.0);
			x1 = xc2 + (dt/2.0)*k11c2x;
			vx1 = vxc2 + (dt/2.0)*k12c2x;
			k21c2x = func_primec2_1(t1, x1, vx1);
			k22c2x = func_primec2x_2(t1, x1, vx1,  xc1, xc2, yc1, yc2, M2);

			y1 = yc2 + (dt/2.0)*k11c2y;
			vy1 = vyc2 + (dt/2.0)*k12c2y;
			k21c2y = func_primec2_1(t1, y1, vy1);
			k22c2y = func_primec2y_2(t1, y1, vy1,  xc1, xc2, yc1, yc2, M2);
			
			//segundo paso
			t2 = tiempo + (dt/2.0);
			x2 = xc2 + (dt/2.0)*k21c2x;
			vx2 = vxc2 + (dt/2.0)*k22c2x;
			k31c2x = func_primec2_1(t2, x2, vx2);
			k32c2x = func_primec2x_2(t2, x2, vx2,  xc1, xc2, yc1, yc2, M2);

			y2 = yc2 +(dt/2.0)*k21c2y;
			vy2 = vyc2 + (dt/2.0)*k22c2y;
			k31c2y = func_primec2_1(t2, y2, vy2);
			k32c2y = func_primec2y_2(t2, y2, vy2,  xc1, xc2, yc1, yc2, M2);

                        //tercer paso
			t3 = tiempo + dt;
			x3 = xc2 + dt*k31c2x;
			vx3 = vxc2 + dt*k32c2x;
			k41c2x = func_primec2_1(t3, x3, vx3);
			k42c2x = func_primec2x_2(t3, x3, vx3, xc1, xc2, yc1, yc2, M2);

			y3 = yc2 + dt*k31c2y;
			vy3 = vyc2 + dt*k32c2y;
			k41c2y = func_primec2_1(t3, y3, vy3);
			k42c2y = func_primec2y_2(t3, y3, vy3,  xc1, xc2, yc1, yc2, M2);
			//cuarto paso
			kprom1x = (1.0/1.6)*(k11c2x + 2.0*k21c2x + 2.0*k31c2x + k41c2x);
			kprom2x = (1.0/1.6)*(k12c2x + 2.0*k22c2x + 2.0*k32c2x + k42c2x);
			kprom1y = (1.0/1.6)*(k11c2y + 2.0*k21c2y + 2.0*k31c2y + k41c2y);
			kprom2y = (1.0/1.6)*(k12c2y + 2.0*k22c2y + 2.0*k32c2y + k42c2y);

			xc2 = xc2 + dt*kprom1x;
			vxc2 = vxc2 + dt*kprom2x;

			yc2 = yc2 + dt*kprom1y;
			vyc2 = vyc2 + dt*kprom2y;

			//evolucion del punto i
			xi = gsl_matrix_get(condiciones_iniciales, i, 1);
			xi = gsl_matrix_get(condiciones_iniciales, i, 1);
			yi = gsl_matrix_get(condiciones_iniciales, i, 2);
			vxi = gsl_matrix_get(condiciones_iniciales, i, 3);
			vyi = gsl_matrix_get(condiciones_iniciales, i, 4);

			k11ix = func_prime_1(tiempo, xi, vxi);
			k12ix = func_primex_2(tiempo, xi, vxi, yi, xc1, xc2, yc1, yc2, M1);
			k11iy = func_prime_1(tiempo, yc2, vyc2);
			k12iy = func_primey_2(tiempo, xi, vxi, yi, xc1, xc2, yc1, yc2, M1);
			//primer paso
			t1 = tiempo + (dt/2.0);
			x1 = xi + (dt/2.0)*k11ix;
			vx1 = vxi + (dt/2.0)*k12ix;

			y1 = yi + (dt/2.0)*k11iy;
			vy1 = vyi + (dt/2.0)*k12iy;

			k21ix = func_prime_1(t1, x1, vx1);
			k22ix = func_primex_2(t1, x1, vx1, y1, xc1, xc2, yc1, yc2, M1);
 
			k21iy = func_prime_1(t1, x1, vx1);
			k22iy = func_primey_2(t1, y1, vy1, x1, xc1, xc2, yc1, yc2, M1);

			//segundo paso
			t2 = tiempo + (dt/2.0);
			x2 = xi + (dt/2.0)*k21ix;
			vx2 = vxi + (dt/2.0)*k22ix;
			y2 = yi +(dt/2.0)*k21iy;
			vy2 = vyi + (dt/2.0)*k22iy;


			k31ix = func_prime_1(t2, x2, vx2);
			k32ix = func_primex_2(t2, x2, vx2, y2, xc1, xc2, yc1, yc2, M1);

			
			k31iy = func_prime_1(t2, y2, vy2);
			k32iy = func_primey_2(t2, y2, vy2, x2, xc1, xc2, yc1, yc2, M1);

			//tercer paso
			t3 = tiempo + dt;
			x3 = xc2 + dt*k31ix;
			vx3 = vxi + dt*k32ix;
			k41ix = func_prime_1(t3, x3, vx3);
			k42ix = func_primex_2(t3, x3, vx3);
 
			y3 = yi + dt*k31iy;
			vy3 = vyi + dt*k32iy;
			k41iy = func_prime_1(t3, y3, vy3);
			k42iy = func_primey_2(t3, y3, vy3);
			//cuarto paso
			kprom1x = (1.0/1.6)*(k11ix + 2.0*k21ix + 2.0*k31ix + k41ix);
			kprom2x = (1.0/1.6)*(k12ix + 2.0*k22ix + 2.0*k32ix + k42ix);
			kprom1y = (1.0/1.6)*(k11iy + 2.0*k21iy + 2.0*k31iy + k41iy);
			kprom2y = (1.0/1.6)*(k12iy + 2.0*k22iy + 2.0*k32iy + k42iy);

			xi = xi + dt*kprom1x;
			vxi = vxi + dt*kprom2x;

			yi = yi + dt*kprom1y;
			vyi = vyi + dt*kprom2y;
			
			//imprimir las evolucion en los archivos correspondientes si se cumple el tiempo requerido
			// mil millones de años	
			//imprimir la evolucion del centro 1 
			if(ct == 1000 && i=centros[0]){
				fprintf(out1, "%d %lf %lf %lf %lf\n",-1 ,xc1,yc1,vxc1,vyc1);

				}
			//imprimir la evolucion del centro 2
			if(ct == 1000 && i=centros[1] 
				fprintf(out1, "%d %lf %lf %lf %lf\n",-1 ,xc2,yc2,vxc2,vyc2);
				}
			//imprimir la evolucion del punto i
			if(ct == 1000 && i!=centros[0] && i!=centros[1]){
				fprintf(out1, "%d %lf %lf %lf %lf\n", i,xi,yi,vxi,vyi);
				}

			//dos mil millones de años
			if(ct == 2000 && i=centros[0]){
				fprintf(out2, "%d %lf %lf %lf %lf\n",-1 ,xc1,yc1,vxc1,vyc1);
				}
			//imprimir la evolucion del centro 2
			if(ct == 2000 && i=centros[1]
				fprintf(out2, "%d %lf %lf %lf %lf\n",-1 ,xc2,yc2,vxc2,vyc2);
				}
			//imprimir la evolucion del punto i
			if(ct == 2000 && i!=centros[0] && i!=centros[1]){
				fprintf(out2, "%d %lf %lf %lf %lf\n", i,xi,yi,vxi,vyi);
				}

			//tres mil millones

                        if(ct == 3000 && i=centros[0]){
                                fprintf(out3, "%d %lf %lf %lf %lf\n",-1 ,xc1,yc1,vxc1,vyc1);
                                }   
                        //imprimir la evolucion del centro 2
                        if(ct == 3000 && i=centros[1]
                                fprintf(out3, "%d %lf %lf %lf %lf\n",-1 ,xc2,yc2,vxc2,vyc2);
                                }   
                        //imprimir la evolucion del punto i
                        if(ct == 3000 && i!=centros[0] && i!=centros[1]){
                                fprintf(out3, "%d %lf %lf %lf %lf\n", i,xi,yi,vxi,vyi);
                                }   


                        //cuatro mil millones de años
                        if(ct == 4000 && i=centros[0]){
                                fprintf(out4, "%d %lf %lf %lf %lf\n",-1 ,xc1,yc1,vxc1,vyc1);
                                }   
                        //imprimir la evolucion del centro 2
                        if(ct == 4000 && i=centros[1]
                                fprintf(out4, "%d %lf %lf %lf %lf\n",-1 ,xc2,yc2,vxc2,vyc2);
                                }   
                        //imprimir la evolucion del punto i
                        if(ct == 4000 && i!=centros[0] && i!=centros[1]){
                                fprintf(out4, "%d %lf %lf %lf %lf\n", i,xi,yi,vxi,vyi);
                                }   

                        //cinco mil millones de años
                        if(ct == 5000 && i=centros[0]){
                                fprintf(out5, "%d %lf %lf %lf %lf\n",-1 ,xc1,yc1,vxc1,vyc1);
                                }   
                        //imprimir la evolucion del centro 2
                        if(ct == 5000 && i=centros[1]
                                fprintf(out5, "%d %lf %lf %lf %lf\n",-1 ,xc2,yc2,vxc2,vyc2);
                                }   
                        //imprimir la evolucion del punto i
                        if(ct == 5000 && i!=centros[0] && i!=centros[1]){
                                fprintf(out5, "%d %lf %lf %lf %lf\n", i,xi,yi,vxi,vyi);
                                }   


			}
		}

	}
