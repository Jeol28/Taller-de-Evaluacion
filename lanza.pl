#!/usr/bin/perl
#**************************************************************
# Pontificia Universidad Javeriana
# Autor: J. Corredor
# Modificado por: 
# Fecha: Mayo 2025
# Materia: Sistemas Operativos
# Tema: Evaluación de Rendimiento (Fork, POSIX, OpenMP)
#**************************************************************/

use strict;
use warnings;

# -------------------------------------------------
# Limpiar archivos generados en ejecuciones previas
# -------------------------------------------------
unlink glob("mmClasica*Hilos-*.dat");             # Archivos de resultados individuales
unlink "tabla_metodos.txt";                       # Tabla completa
unlink "plot_metodos.plt";                        # Script de gnuplot general
unlink glob("tabla_*.txt");                       # Tablas filtradas por tamaño
unlink glob("plot_*.plt");                        # Scripts de gnuplot individuales
unlink glob("grafico_*.png");                     # Gráficos PNG generados

# Ruta del directorio actual
my $Path = `pwd`;
chomp($Path);

# Ejecutables
my $Nombre_Ejecutable1 = "mmClasicaFork";
my $Nombre_Ejecutable2 = "mmClasicaPosix";
my $Nombre_Ejecutable3 = "mmClasicaOpenMP";

# Parámetros del experimento
my @Size_Matriz = ("500", "1000", "2000");
my @Num_Hilos   = (1, 2, 4, 8, 16);
my $Repeticiones = 30;

# Crear archivo tipo tabla
my $tabla = "$Path/tabla_metodos.txt";
open(my $tabla_out, '>', $tabla) or die "No se pudo crear $tabla: $!";
print $tabla_out "config\tfork\topenmp\tposix\n";  # Cabecera

# Bucle para ejecutar y calcular promedios
foreach my $size (@Size_Matriz) {
		foreach my $hilo (@Num_Hilos) {

				# Archivos de salida por método
				my %archivos = (
						"fork"   => "$Path/$Nombre_Ejecutable1-${size}-Hilos-${hilo}.dat",
						"posix"  => "$Path/$Nombre_Ejecutable2-${size}-Hilos-${hilo}.dat",
						"openmp" => "$Path/$Nombre_Ejecutable3-${size}-Hilos-${hilo}.dat",
				);

				# Ejecutar 30 repeticiones por cada configuración
				for (my $i = 0; $i < $Repeticiones; $i++) {
						system("$Path/$Nombre_Ejecutable1 $size $hilo >> $archivos{fork}");
						system("$Path/$Nombre_Ejecutable2 $size $hilo >> $archivos{posix}");
						system("$Path/$Nombre_Ejecutable3 $size $hilo >> $archivos{openmp}");
				}

				# Calcular promedios
				my %promedios;
				foreach my $metodo (keys %archivos) {
						my $archivo = $archivos{$metodo};
						my $prom = `awk 'NF > 0 {sum += \$1; count++} END {if (count > 0) print sum / count}' $archivo`;
						chomp($prom);
						$promedios{$metodo} = $prom;
				}

				# Escribir fila en la tabla
				my $config = "${size}x${hilo}";
				print $tabla_out "$config\t$promedios{fork}\t$promedios{openmp}\t$promedios{posix}\n";
		}
}

close($tabla_out);
print "\n✅ Tabla generada correctamente: $tabla\n";

# -------------------------
# Generar tres gráficas, una por tamaño
# -------------------------

foreach my $size (@Size_Matriz) {
		# Crear archivo temporal filtrado solo para este tamaño
		my $tabla_filtrada = "$Path/tabla_${size}.txt";
		open(my $in, '<', $tabla) or die "No se pudo leer $tabla";
		open(my $outf, '>', $tabla_filtrada) or die "No se pudo crear $tabla_filtrada";

		my $header = <$in>;
		print $outf $header;  # Escribe la cabecera

		while (my $line = <$in>) {
				if ($line =~ /^${size}x/) {
						print $outf $line;
				}
		}
		close($in);
		close($outf);

		# Comando gnuplot embebido
		my $grafico = "$Path/grafico_${size}.png";
		my $plt_file = "$Path/plot_${size}.plt";

		my $gnuplot_cmd = <<"GNUPLOT";
set terminal png size 1000,600
set output '$grafico'
set datafile separator "\\t"
set title 'Rendimiento con matriz ${size}x${size}'
set xlabel 'Número de hilos'
set ylabel 'Tiempo promedio (s)'
set xtics rotate by -45 font ",8"
set key outside
set grid
plot \\
		'$tabla_filtrada' using 2:xtic(1) with linespoints title 'Fork', \\
		'' using 3:xtic(1) with linespoints title 'OpenMP', \\
		'' using 4:xtic(1) with linespoints title 'POSIX'
GNUPLOT

		# Guardar y ejecutar script gnuplot
		open(my $plt, '>', $plt_file) or die "No se puede crear $plt_file";
		print $plt $gnuplot_cmd;
		close($plt);

		system("gnuplot $plt_file");
		print "✅ Gráfico generado: $grafico\n";
}
