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
#****************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "Funciones.h"


//Variables globales
double *mA, *mB, *mC;


// Funcion para iniciar la medición del tiempo
void InicioMuestra(){
	// Obtener el tiempo actual y almacenarlo en la variable inicio
	gettimeofday(&inicio, (void *)0);
}

// Funcion para finalizar la medición del tiempo y calcular el tiempo transcurrido
void FinMuestra(){
	// Obtener el tiempo actual y almacenarlo en la variable fin
	gettimeofday(&fin, (void *)0);
	// Calcular el tiempo transcurrido
	fin.tv_usec -= inicio.tv_usec;
	fin.tv_sec  -= inicio.tv_sec;
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec);
	// Imprimir el tiempo transcurrido
	printf("%9.0f \n", tiempo);
}



//Funcion para imprimir matrices
void impMatrixFork(double *matrix, int D) {
	// Verificar si el tamaño de la matriz es menor que 9
	if (D < 9) {
			// Imprimir la matriz
			printf("\nImpresión	...\n");
			for (int i = 0; i < D*D; i++, matrix++) {
			if(i%D==0) printf("\n");
							printf(" %f ", *matrix);
					}
				printf("\n ");
		}
}

// Funcion para imprimir matrices
void impMatrixOpenMP(size_t *matrix, int D){
	printf("\n");
	// Verificar si el tamaño de la matriz es menor que 9
	if(D < 9){
			// Imprimir la matriz
		for(int i=0; i<D*D; i++){
			if(i%D==0) printf("\n");
			printf("%zu ", matrix[i]);
		}
		printf("\n**-----------------------------**\n");
	}
}

//Funcion para imprimir matrices
void impMatrixPosix(int sz, double *matriz){
	// Verificar si el tamaño es menor que 12
	if(sz < 12){
			// Imprimir la matriz
				for(int i = 0; i < sz*sz; i++){
						if(i%sz==0) printf("\n");
								printf(" %.3f ", matriz[i]);
			}	
			printf("\n>-------------------->\n");
	}
}

// Funcion para inicializar matriz con valores aleaotorios
void iniMatrixOpenMP(size_t *m1, size_t *m2, int D){
	// Iterar sobre los elementos de las matrices y asignarles valores
	for(int i=0; i<D*D; i++, m1++, m2++){
		*m1 = i*2; // (size_t) rand()%10;	
		*m2 = i+2; // (size_t) rand()%10;	
	}
}

// Funcion para inicializar matriz con valores aleaotorios
void iniMatrixPosix(int SZ){ 
	// Iterar sobre los elementos de las matrices y asignarles valores aleatorios
	for(int i = 0; i < SZ*SZ; i++){
			mA[i] = rand() % 10; 
			mB[i] = rand() % 10;
			mC[i] = 0; 
		}	
}

// Funcion para inicializar matriz con valores aleaotorios
void iniMatrixFork(double *mA, double *mB, int D){
	// Iterar sobre los elementos de las matrices y asignarles valores aleatorios
	for (int i = 0; i < D*D; i++, mA++, mB++){
						*mA = rand() % 10; 
						*mB = rand() % 10; 
				}
}

// Funcion para multiplicar matrices
void *multiMatrixPosix(void *variables){
	// Obtener los parámetros pasados a la función
	struct parametros *data = (struct parametros *)variables;
	// Obtener el identificador del hilo, el número de hilos y el tamaño de la matriz
	int idH = data->idH;
	int nH  = data->nH;
	int N   = data->N;
	// Calcular el rango de filas que debe calcular este hilo
	int ini = (N/nH)*idH;
	int fin = (N/nH)*(idH+1);
	//Iterar sobre las filas asignadas
		for (int i = ini; i < fin; i++){
				for (int j = 0; j < N; j++){
						// Declarar variables para almacenar los punteros a las matrices y la suma temporal
			double *pA, *pB, sumaTemp = 0.0;
			pA = mA + (i*N); 
			pB = mB + j;
						// Iterar sobre las columnas y calcular la suma temporal
						for (int k = 0; k < N; k++, pA++, pB+=N){
								// Sumar el producto de los elementos correspondientes
				sumaTemp += (*pA * *pB);
			}
						// Almacenar la suma temporal en la matriz resultante
			mC[i*N+j] = sumaTemp;
		}
	}
	pthread_exit(NULL);
}

// Funcion para multiplicar matrices
void multiMatrixFork(double *mA, double *mB, double *mC, int D, int filaI, int filaF) {
	// Declarar variables para almacenar la suma y los punteros a las matrices
	double Suma, *pA, *pB;
	// Iterar sobre las filas asignadas
		for (int i = filaI; i < filaF; i++) {
				for (int j = 0; j < D; j++) {
						// Inicializar la suma y los punteros
			Suma = 0;
			pA = mA+i*D;
			pB = mB+j;
						// Iterar sobre las columnas y calcular la suma
						for (int k = 0; k < D; k++, pA++, pB+=D) {
								// Sumar el producto de los elementos correspondientes
				Suma += *pA * *pB;	
						}
						// Almacenar la suma en la matriz resultante
			mC[i*D+j] = Suma;
				}
		}
}

// Funcion para multiplicar matrices
void multiMatrixOpenMP(size_t *mA, size_t *mB, size_t *mC, int D){
	// Declarar variables para almacenar la suma y los punteros a las matrices
	size_t Suma, *pA, *pB;

	//Crea varios hilos. Cada hilo ejecuta una sección del código en paralelo.
	#pragma omp parallel
	{
		// Le indica a OpenMP que el bucle for debe dividirse automáticamente entre los hilos disponibles.
	#pragma omp for
		// Iterar sobre las filas de la matriz
	for(int i=0; i<D; i++){
		for(int j=0; j<D; j++){
				// Inicializar la suma y los punteros
			pA = mA+i*D;	
			pB = mB+j; //pB = mB+(j*D);	
			Suma = 0.0;
				// Iterar sobre las columnas y calcular la suma
			for(int k=0; k<D; k++, pA++, pB+=D){
					// Sumar el producto de los elementos correspondientes
				Suma += *pA * *pB;
			}
				// Almacenar la suma en la matriz resultante
			mC[i*D+j] = Suma;
		}
	}
	}
}