//
//  Tarea6_1.c
//  
//
//  Created by Alejandro Gonzalez && Camilo Rivera on 21/10/13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define pi 3.14159

int main(int argc, char **argv){
    int n, j, total, i, indice;
    double x0, y0, vxc, vyc; // Datos de entrada
    double teta, alpha, rc, rj, xij, yij, vxij, vyij, G, vj;
    FILE *output;
    
    
    // Toma los parametros de entrada
    x0 = atof(argv[1]);
    y0 = atof(argv[2]);
    vxc = atof(argv[3]);
    vyc = atof(argv[4]);
    
    
    G = 7.2 * pow(10,16);
    //printf("Xo = %f \n" , x0);
    //printf("Yo = %f \n", y0);
    //printf("Vxc = %f \n", vxc);
    //printf("Vyc = %f \n", vyc);
    
    //crear el archivo de salida
    output = fopen ("CondicionesIniciales.txt", "w");
    fprintf (output, "-1 %f %f %f %f\n", x0, y0, vxc, vyc);
    
    //Calculamos el radio y el angulo del centro
    rc = pow(x0,2)+ pow (y0,2);
    rc = pow(rc,0.5);
    teta = atan(y0/x0);
    
    //Recorremos las orbitas con j
    total = 0;
    indice = 0;
    for (j=1; j<=5; j++){
        rj = 10*j;
        n = 6 * (j+1);
        vj = pow((G/rj),0.5);
        
        //printf("\n\n\n orbita: %d\n", j);
        for(i=0; i<n; i++){
            alpha = (2*pi/n)*i;
            xij = x0 + rj*cos(alpha);
            yij = y0 + rj*sin(alpha);
            indice++;
            vxij = vxc + vj*cos(((pi/2)+alpha));
            vyij = vyc + vj*sin(((pi/2)+alpha));
            fprintf(output, "%d %f %f %f %f\n", indice, xij, yij, vxij, vyij);
        }
        
        //printf("rj: %f\n",rj);
        total = total +n;
        //printf ("Orbita: %d \nRadio: %f \nNumero de estrellas: %d \n", j,rj,n);
        //printf ("Total estrellas: %d\n", total);
        
    }
    
}
