GCC = gcc
CFLAGS = -lm
FOPENMP = -fopenmp -O3
POSIX = -lpthread

modulo = Funciones.c

PROGRAMAS = mmClasicaOpenMP mmClasicaFork mmClasicaPosix

ALL: $(PROGRAMAS)


mmClasicaPosix:
	$(GCC) $(CFLAGS) $(modulo) $@.c -o $@ $(POSIX)

mmClasicaFork:
	$(GCC) $(CFLAGS) $(modulo) $@.c -o $@ 

mmClasicaOpenMP:
	$(GCC) $(CFLAGS) $(modulo) $@.c -o $@ $(FOPENMP)

clean:
	$(RM) $(PROGRAMAS) 
