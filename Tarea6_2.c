//
//  Tarea6_2.c
//  
//
//  Created by Alejandro Gonzalez && Camilo Riveraon 26/10/13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_linalg.h>


#define G 1
#define M 1


int main(int argc, char **argv){

    FILE *ci, *out1, *out2, out3, out4;
    
    //abrir el archivo de condiciones iniciales
    
    ci = fopen(argv[1], "r");
    if (!ci){
        printf("problemas al abrir el archivo:  %s\n", argv[1]);
        exit(1);
    }
    else {
        printf("el archivo se abrio exitosamente\n");
    }
    
    // Declarar la matriz de condiciones iniciales
    
    gsl_matrix * condiciones_iniciales = gsl_matrix_alloc(121,5);
    
    // Llena la matriz con los datos del archivo
    
    rewind(ci);
    
    
    gsl_matrix_fscanf ( ci, condiciones_iniciales);
    
    
}


