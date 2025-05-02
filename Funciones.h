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

#ifndef FUNCIONES_H_
#define FUNCIONES_H_
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>


// Definir el tamaño de los datos
#define DATA_SIZE (1024*1024*64*3)

extern double *mA, *mB, *mC;



// Estructura para pasar los parámetros a la función de multiplicación de matrices
struct parametros{
	int nH;
	int idH;
	int N;
};

// Variables para medir el tiempo de ejecución
struct timeval inicio, fin;

// Firmas de las funciones
void InicioMuestra();
void FinMuestra();
void iniMatrixOpenMP(size_t *m1, size_t *m2, int D);
void iniMatrixPosix(int SZ);
void iniMatrixFork(double *mA, double *mB, int D);
void impMatrixOpenMP(size_t *matrix, int D);
void impMatrixPosix(int sz, double *matriz);
void impMatrixFork(double *matriz, int D);
void multiMatrixFork(double *mA, double *mB, double *mC, int D, int filaI, int filaF);
void *multiMatrixPosix(void *variables);
void multiMatrixOpenMP(size_t *mA, size_t *mB, size_t *mC, int D);

#endif