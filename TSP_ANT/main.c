// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algoritmo_evolutivo_diferencial.h"

int main(int argc, char *argv[])
{
    int tamanio_instancia = atoi(argv[1]);
    char archivo_instancia[20];
    strcpy(archivo_instancia, argv[2]);
    algoritmo_evolutivo_diferencial(tamanio_instancia, archivo_instancia);
    return 0;
}
