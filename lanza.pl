#!/usr/bin/perl
#**************************************************************
#         		Pontificia Universidad Javeriana
#     Autor: J. Corredor
#     Comentarios y modificaciones: Jorge Enrique Olaya Liévano
#     Fecha: Febrero 2024
#     Materia: Sistemas Operativos
#     Tema: Taller de Evaluación de Rendimiento
#     Fichero: script automatización ejecución por lotes 
#****************************************************************/

#Devuelve la ruta del directorio actual donde se corre el script
$Path = `pwd`;
chomp($Path);
#Define el nombre del ejecutable
$Nombre_Ejecutable = "MM_ejecutable";
#Se crea un arreglo que contiene los tamaños de matrices a probar
@Size_Matriz = ("200","300");
#Se crea un arreglo con el numero de hilos a usar
@Num_Hilos = (1,2);
#Se define el numero de veces que se va a ejecutar cada ejecución
$Repeticiones = 2;

#Inicia un bucle que recorre cada tamaño de matriz en la lista @Size_Matriz
foreach $size (@Size_Matriz){
#Inicia bulve que recorre cada matriz con cada numero de hilos en la lista @Num_Hilos
	foreach $hilo (@Num_Hilos) {
	#Define el nombre del archivo de salida
		$file = "$Path/$Nombre_Ejecutable-".$size."-Hilos-".$hilo.".dat";
		#Se realiza el proceso hasta el numero de veces definido en $Repeticiones
		for ($i=0; $i<$Repeticiones; $i++) {
#system("$Path/$Nombre_Ejecutable $size $hilo  >> $file");
			printf("$Path/$Nombre_Ejecutable $size $hilo \n");
		}

	}
}
