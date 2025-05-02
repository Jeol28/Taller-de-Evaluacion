/***************************************************************
#         		Pontificia Universidad Javeriana
#     Autor: J. Corredor
#     Comentarios y modificaciones: Jorge Enrique Olaya Liévano
#     Fecha: Mayo 2025
#     Materia: Sistemas Operativos
#     Taller de Evaluación de Rendimiento
#     Tema: 
#     - Programación Modular en C
# 	  - Programa Multiplicación de Matrices algoritmo clásico
# 	  - Paralelismo con Fork
#****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include "Funciones.h"


int main(int argc, char *argv[]) {
    // Verificar que se ingresen los argumentos correctos
	if (argc < 3) {
		printf("\n \t\tUse: $./nom_ejecutable Size Hilos \n");
		exit(0);
	}
    // Obtener los argumentos de la línea de comandos
	int N      = (int) atoi(argv[1]);
	int num_P  = (int) atoi(argv[2]);
    //Reservar memoria para las matrices
    double *matA = (double *) calloc(N*N, sizeof(double));
    double *matB = (double *) calloc(N*N, sizeof(double));
    double *matC = (double *) calloc(N*N, sizeof(double));

    // Inicializar la semilla para la generación de números aleatorios
    srand(time(0)); 
    // Inicializar las matrices con valores aleatorios
    iniMatrixFork(matA, matB, N);
    // Imprimir las matrices iniciales
    impMatrixFork(matA, N);
    impMatrixFork(matB, N);

    // Calcular el número de filas que cada proceso debe calcular
    int rows_per_process = N/ num_P;

    // Crear los procesos hijos y asignarles las filas correspondientes
	InicioMuestra();
    for (int i = 0; i < num_P; i++) {
        pid_t pid = fork();

        // Verificar si el fork fue exitoso
        if (pid == 0) { // Proceso hijo
            // Calcular las filas que debe calcular este proceso hijo
            int start_row = i * rows_per_process;
            // Calcular la fila final que debe calcular este proceso hijo
            int end_row = (i == num_P - 1) ? N : start_row + rows_per_process;

            // Multiplicar las matrices para las filas asignadas a este proceso hijo
			multiMatrixFork(matA, matB, matC, N, start_row, end_row); 
            
			if(N<9){
                //Imprimir la parte calculada por el proceso hijo
           		printf("\nChild PID %d calculated rows %d to %d:\n", getpid(), start_row, end_row-1);
            	for (int r = start_row; r < end_row; r++) {
                	for (int c = 0; c < N; c++) {
                    	printf(" %f ", matC[N*r+c]);
                	}
                	printf("\n");
            	}
			}
            // Salir del proceso hijo
            exit(0); 
        } else if (pid < 0) {
            // Si el fork falló, imprimir un mensaje de error y salir
            perror("fork failed");
            exit(1);
        }
    }
    // Padre espera a que todos los procesos hijos terminen
    for (int i = 0; i < num_P; i++) {
        wait(NULL);
    }

    // Calcular el tiempo transcurrido y mostrarlo
	FinMuestra(); 

    // Liberar la memoria reservada para las matrices
	free(matA);
	free(matB);
	free(matC);

    return 0;
}
