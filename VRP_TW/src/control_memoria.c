#include <stdlib.h>
#include <stdbool.h>
#include "../include/estructuras.h"
#include "../include/lista_flota.h"
#include "../include/salida_datos.h"

/*Funciones para asignación y liberación de memoria para arreglos y matrices*/

/**
 * Asigna memoria para una matriz de doble puntero (instancia) de tamaño [tamanio_instancia x tamanio_instancia].
 * Si no puede asignarse, termina el programa con un mensaje de error.
 */
double **asignar_memoria_instancia(int tamanio_instancia)
{
    double **instancia = (double **)malloc(tamanio_instancia * sizeof(double *));
    if (instancia == NULL)
    {
        imprimir_mensaje("Error: No se pudo asignar memoria para las filas de la instancia");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < tamanio_instancia; i++)
    {
        instancia[i] = (double *)malloc(tamanio_instancia * sizeof(double));
        if (instancia[i] == NULL)
        {
            imprimir_mensaje("No se pudo asignar memoria para la columna");
            // Liberar memoria ya asignada antes de salir
            for (int j = 0; j < i; j++)
                free(instancia[j]);
            free(instancia);
            exit(EXIT_FAILURE);
        }
    }

    return instancia;
}

/**
 * Asigna memoria para un arreglo de enteros de tamaño 'tamanio_arreglo'.
 * Si no puede asignarse, termina el programa con un mensaje de error.
 */
int *asignar_memoria_arreglo_int(int tamanio_arreglo)
{
    int *arreglo = (int *)malloc(sizeof(int) * (tamanio_arreglo));
    if (arreglo == NULL)
    {
        imprimir_mensaje("No se pudo asignar memoria para el arreglo entero");
        exit(EXIT_FAILURE);
    }
    return arreglo;
}

/**
 * Asigna memoria para un arreglo de tipo double de tamaño 'tamanio_arreglo'.
 * Si no puede asignarse, termina el programa con un mensaje de error.
 */
double *asignar_memoria_arreglo_double(int tamanio_arreglo)
{
    double *arreglo = (double *)malloc(sizeof(double) * (tamanio_arreglo));
    if (arreglo == NULL)
    {
        imprimir_mensaje("No se pudo asignar memoria para el arreglo double");
        exit(EXIT_FAILURE);
    }
    return arreglo;
}

/**
 * Libera la memoria del arreglo de enteros.
 */
void liberar_memoria_arreglo_int(int *arreglo)
{
    free(arreglo);
}

/**
 * Libera la memoria del arreglo de tipo double.
 */
void liberar_memoria_arreglo_double(double *arreglo)
{
    free(arreglo);
}

/**
 * Libera la memoria de la instancia (matriz) de tamaño 'tamanio_instancia'.
 */
void liberar_instancia(double **instancia, int tamanio_instancia)
{
    for (int i = 0; i < tamanio_instancia; i++)
        free(instancia[i]);
    free(instancia);
}

/*Funciones para la estructura del individuo*/

/**
 * Asigna memoria para una cantidad de individuos definida por 'poblacion'.
 * Si no puede asignarse, termina el programa con un mensaje de error.
 */
struct individuo *asignar_memoria_individuos(int poblacion)
{
    struct individuo *individuo = (struct individuo *)malloc(sizeof(struct individuo) * poblacion);
    if (individuo == NULL)
    {
        imprimir_mensaje("Error: No se pudo asignar memoria para los individuos");
        exit(EXIT_FAILURE);
    }
    return individuo;
}

/**
 * Libera la memoria de un arreglo de individuos.
 * Si 'tipo' es verdadero, también libera la memoria de las flotas de los individuos.
 */
void liberar_individuos(struct individuo *ind, int num_poblacion, bool tipo)
{
    if (tipo)
        for (int i = 0; i < num_poblacion; i++)
            liberar_lista_vehiculos(ind[i].hormiga->flota);

    free(ind);
}

/*Funciones para la estructura de configuración VRP*/

/**
 * Asigna memoria para la estructura 'vrp_configuracion'.
 * Si no puede asignarse, termina el programa con un mensaje de error.
 */
struct vrp_configuracion *asignar_memoria_vrp_configuracion()
{
    struct vrp_configuracion *vrp = (struct vrp_configuracion *)malloc(sizeof(struct vrp_configuracion));
    if (vrp == NULL)
    {
        imprimir_mensaje("Error: No se pudo asignar memoria para la configuración VRP");
        exit(EXIT_FAILURE);
    }
    return vrp;
}

/**
 * Libera la memoria de la estructura 'vrp_configuracion'.
 */
void liberar_memoria_vrp_configuracion(struct vrp_configuracion *vrp)
{
    if (vrp != NULL)
    {
        free(vrp->clientes);
        vrp->clientes = NULL;
        free(vrp);
    }
}

/*Funciones para la estructura de clientes*/

/**
 * Asigna memoria para los clientes de acuerdo al número de clientes definido en la configuración VRP.
 * Si no puede asignarse, termina el programa con un mensaje de error.
 */
struct cliente *asignar_memoria_clientes(struct vrp_configuracion *vrp)
{
    struct cliente *cliente = (struct cliente *)malloc(vrp->num_clientes * sizeof(struct cliente));
    if (cliente == NULL)
    {
        imprimir_mensaje("Error: No se pudo asignar memoria para los clientes");
        exit(EXIT_FAILURE);
    }
    return cliente;
}

/*Funciones para la estructura de hormiga*/

/**
 * Asigna memoria para un arreglo de hormigas de tamaño 'numHormigas'.
 * Si no puede asignarse, termina el programa con un mensaje de error.
 */
struct hormiga *asignar_memoria_hormigas(int numHormigas)
{
    struct hormiga *hormiga = (struct hormiga *)malloc(sizeof(struct hormiga) * numHormigas);
    if (hormiga == NULL)
    {
        imprimir_mensaje("Error: No se pudo asignar memoria para hormigas.");
        exit(EXIT_FAILURE);
    }
    return hormiga;
}

/**
 * Libera la memoria asociada a las hormigas, incluyendo las estructuras dentro de cada hormiga.
 */
void liberar_memoria_hormiga(struct hormiga *hormiga, struct individuo *ind)
{
    for (int i = 0; i < ind->numHormigas; i++)
    {
        liberar_memoria_arreglo_int(hormiga[i].tabu);
        liberar_memoria_arreglo_int(hormiga[i].posibles_clientes);
        liberar_memoria_arreglo_double(hormiga[i].probabilidades);
        liberar_lista_vehiculos(hormiga[i].flota);
    }
    free(hormiga);
}
/**
 * Reinicia la información de una hormiga, incluyendo sus arreglos internos y flota de vehículos.
 */
void reiniciar_hormiga(struct hormiga *hormiga, struct vrp_configuracion *vrp)
{
    for (int i = 0; i < vrp->num_clientes; i++)
    {
        hormiga->tabu[i] = 0;
        hormiga->posibles_clientes[i] = 0;
        hormiga->probabilidades[i] = 0.0;
    }

    hormiga->tabu_contador = 0;
    hormiga->posibles_clientes_contador = 0;
    hormiga->suma_probabilidades = 0.0;
    hormiga->fitness_global = 0.0;
    liberar_lista_vehiculos(hormiga->flota);
    hormiga->vehiculos_necesarios = 0;
    inserta_vehiculo_flota(hormiga, vrp, hormiga->vehiculos_necesarios + 1);
    hormiga->vehiculos_necesarios++;
}

/*Funciones para la estructura de ruta*/

/**
 * Asigna memoria para una nueva lista de rutas.
 * Si no puede asignarse, termina el programa con un mensaje de error.
 */
struct lista_ruta *asignar_memoria_lista_ruta()
{
    struct lista_ruta *nueva_lista = (struct lista_ruta *)malloc(sizeof(struct lista_ruta));
    if (nueva_lista == NULL)
    {
        imprimir_mensaje("Error: No se pudo asignar memoria para la lista de rutas.");
        exit(EXIT_FAILURE);
    }

    nueva_lista->cabeza = NULL;
    nueva_lista->cola = NULL;

    return nueva_lista;
}

/**
 * Asigna memoria para un nuevo nodo de ruta.
 * Si no puede asignarse, termina el programa con un mensaje de error.
 */
struct nodo_ruta *asignar_memoria_nodo_ruta()
{
    struct nodo_ruta *nodo_nuevo = (struct nodo_ruta *)malloc(sizeof(struct nodo_ruta));
    if (!nodo_nuevo)
    {
        imprimir_mensaje("Error: No se pudo asignar memoria para nodo_ruta");
        exit(EXIT_FAILURE);
    }
    return nodo_nuevo;
}

/*Funciones para la estructura de vehículos*/

/**
 * Asigna memoria para una lista de vehículos.
 * Si no puede asignarse, termina el programa con un mensaje de error.
 */
lista_vehiculos *asignar_memoria_lista_vehiculos()
{
    lista_vehiculos *nueva_lista = (lista_vehiculos *)malloc(sizeof(lista_vehiculos));
    if (nueva_lista == NULL)
    {
        imprimir_mensaje("Error: No se pudo asignar memoria para la lista de vehículos.");
        exit(EXIT_FAILURE);
    }

    nueva_lista->cabeza = NULL;
    nueva_lista->cola = NULL;

    return nueva_lista;
}

/**
 * Asigna memoria para un nuevo vehículo.
 * Si no puede asignarse, termina el programa con un mensaje de error.
 */
struct vehiculo *asignar_memoria_vehiculo()
{
    struct vehiculo *vehiculo = (struct vehiculo *)malloc(sizeof(struct vehiculo));
    if (!vehiculo)
    {
        imprimir_mensaje("Error: No se pudo asignar memoria para el vehiculo.");
        exit(EXIT_FAILURE);
    }
    return vehiculo;
}

/**
 * Asigna memoria para un nuevo nodo de vehículo.
 * Si no puede asignarse, termina el programa con un mensaje de error.
 */
struct nodo_vehiculo *asignar_memoria_nodo_vehiculo()
{
    struct nodo_vehiculo *nodo_vehiculo = (struct nodo_vehiculo *)malloc(sizeof(struct nodo_vehiculo));
    if (!nodo_vehiculo)
    {
        imprimir_mensaje("Error: No se pudo asignar memoria para nodo_vehiculo.");
        exit(EXIT_FAILURE);
    }
    return nodo_vehiculo;
}
