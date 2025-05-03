
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
# 	  - Paralelismo con OpenMP
#****************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#include "Funciones.h"


int main(int argc, char *argv[]){
	// Verificar que se ingresen los argumentos correctos
	if(argc < 3){
		printf("\n Use: $./clasicaOpenMP SIZE Hilos \n\n");
		exit(0);
	}

  // Obtener los argumentos de la línea de comandos
	int N = atoi(argv[1]);
	int TH = atoi(argv[2]);
	size_t *matrixA  = (size_t *)calloc(N*N, sizeof(size_t));
	size_t *matrixB  = (size_t *)calloc(N*N, sizeof(size_t));
	size_t *matrixC  = (size_t *)calloc(N*N, sizeof(size_t));
	// Inicializar la semilla para la generación de números aleatorios
	srand(time(NULL));

	// Establecer el número de hilos para OpenMP
	omp_set_num_threads(TH);
	// Inicializar las matrices con valores aleatorios
	iniMatrixOpenMP(matrixA, matrixB, N);
	// Imprimir las matrices iniciales
	impMatrixOpenMP(matrixA, N);
	impMatrixOpenMP(matrixB, N);
	// Multiplicar las matrices y medir el tiempo transcurrido
	InicioMuestra();
	multiMatrixOpenMP(matrixA, matrixB, matrixC, N);
	FinMuestra();
	// Imprimir la matriz resultante
	impMatrixOpenMP(matrixC, N);

	/*Liberación de Memoria*/
	free(matrixA);
	free(matrixB);
	free(matrixC);
	
	return 0;
}
