#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/aed.h"

int main(int argc, char *argv[])
{
    srand(time(NULL)); // Inicializamos la semilla
    (void)argc;
    int num_poblacion = atoi(argv[1]);     // Ingresa el tamaño de la poblacion
    int num_generaciones = atoi(argv[2]);  // Ingresa el numero de generacones
    int tamanio_instancia = atoi(argv[3]); // Ingresamos el numero del tamanio de la instancia
    /*Aqui estamos verificando que el numero de la poblacion no sea menor a 3*/
    if (num_poblacion < 3)
    {
        do
        {
            printf("\nLa poblacion debe ser mayor a 2 !!!\nIngresa de nuevo el valor de la poblacion: ");
            scanf("%d", &num_poblacion);

        } while (num_poblacion < 3);
    }

    if (tamanio_instancia != 25 || tamanio_instancia != 50 || tamanio_instancia != 100)
    {
        do
        {
            printf("\nEl tamanio de la instancia debe ser 25,50 o 100 !!!\nIngresa de nuevo el tamanio de la instancia: ");
            scanf("%d", &tamanio_instancia);

        } while (tamanio_instancia != 25 || tamanio_instancia != 50 || tamanio_instancia != 100);
    }
    char archivo_instancia[100];                                           // Declaramos nombre de la instacia a leer
    snprintf(archivo_instancia, sizeof(archivo_instancia), "%s", argv[3]); // Copiamos el nombre de la instacia a leer a del parametro argv[3];
    aed_vrp_tw(num_poblacion, num_generaciones, archivo_instancia);        // Enviamos al como parametro el numero de la poblacion , numero de generaciones , y el archivo de la instancia
    return 0;
}