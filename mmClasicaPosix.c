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
# 	  - Paralelismo con Posix Threads
#****************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "Funciones.h"


// Declarar las matrices y el mutex
pthread_mutex_t MM_mutex;
static double MEM_CHUNK[DATA_SIZE];
double *mA, *mB, *mC;


int main(int argc, char *argv[]){
	// Verificar que se ingresen los argumentos correctos
	if (argc < 3){
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		exit(0);	
	}
	// Obtener los argumentos de la línea de comandos
    int SZ = atoi(argv[1]); 
    int n_threads = atoi(argv[2]); 
	// Declarar las variables para los punteros a las matrices
    pthread_t p[n_threads];
	// Declarar las variables para los atributos y el mutex de los hilos
    pthread_attr_t atrMM;

	mA = MEM_CHUNK;
	mB = mA + SZ*SZ;
	mC = mB + SZ*SZ;

	//Incializar matrices con valores aleatorios
	iniMatrixPosix(SZ);
	//Imprimir matrices
	impMatrixPosix(SZ, mA);
	impMatrixPosix(SZ, mB);

	//Iniciar medición de tiempo
	InicioMuestra();
	
 	//Mutex para sincronizar hilos
	pthread_mutex_init(&MM_mutex, NULL);
	//Inicializar atributos de los hilos
	pthread_attr_init(&atrMM);
	//Establecer el estado de los hilos
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);
	//Crear hilos y asignarles tareas
    for (int j=0; j<n_threads; j++){
			//Crear estructura para pasar parámetros a la función de multiplicación de matrices
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
		//Asignar valores a la estructura
		datos->idH = j;
		datos->nH  = n_threads;
		datos->N   = SZ;
			//Crear hilo y asignarle la función de multiplicación de matrices
        pthread_create(&p[j],&atrMM,multiMatrixPosix,(void *)datos);
	}
	//Esperar a que todos los hilos terminen
    for (int j=0; j<n_threads; j++)
			//Esperar a que el hilo termine
        pthread_join(p[j],NULL);
	//Finalizar medición de tiempo
	FinMuestra();
	//Imprimir matriz resultante
	impMatrixPosix(SZ, mC);
	//Finalizar atributos y mutex
	pthread_attr_destroy(&atrMM);
	pthread_mutex_destroy(&MM_mutex);
	pthread_exit (NULL);
}

