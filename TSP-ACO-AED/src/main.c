#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "AED.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int poblacion = atoi(argv[1]);
    int generaciones = atoi(argv[2]);
    int tamanio_instancia = atoi(argv[3]);
    char archivo_instancia[200];
    snprintf(archivo_instancia, sizeof(archivo_instancia), "../Instancias/%s", argv[4]);
    aed(poblacion, generaciones, tamanio_instancia, archivo_instancia);
    return 0;
}
