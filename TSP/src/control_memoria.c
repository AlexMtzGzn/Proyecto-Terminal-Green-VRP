#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../include/estructuras.h"
#include "../include/salida_datos.h"
#include "../include/lista_ruta.h"

/*=========================*/
/*== MEMORIA BÁSICA ======*/
/*=========================*/

// Matriz cuadrada (e.g., instancia)
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
            for (int j = 0; j < i; j++) free(instancia[j]);
            free(instancia);
            exit(EXIT_FAILURE);
        }
    }
    return instancia;
}

void liberar_instancia(double **instancia, int tamanio_instancia)
{
    for (int i = 0; i < tamanio_instancia; i++)
        free(instancia[i]);
    free(instancia);
}

// Arreglo de enteros
int *asignar_memoria_arreglo_int(int tamanio_arreglo)
{
    int *arreglo = (int *)malloc(sizeof(int) * tamanio_arreglo);
    if (arreglo == NULL)
    {
        imprimir_mensaje("No se pudo asignar memoria para el arreglo entero");
        exit(EXIT_FAILURE);
    }
    return arreglo;
}

void liberar_memoria_arreglo_int(int *arreglo)
{
    free(arreglo);
}

// Arreglo de doubles
double *asignar_memoria_arreglo_double(int tamanio_arreglo)
{
    double *arreglo = (double *)malloc(sizeof(double) * tamanio_arreglo);
    if (arreglo == NULL)
    {
        imprimir_mensaje("No se pudo asignar memoria para el arreglo double");
        exit(EXIT_FAILURE);
    }
    return arreglo;
}

void liberar_memoria_arreglo_double(double *arreglo)
{
    free(arreglo);
}

/*=========================*/
/*== ESTRUCTURA: RANGOS ==*/
/*=========================*/

struct rangos *asignar_memoria_rangos()
{
    struct rangos *rango = (struct rangos *)malloc(sizeof(struct rangos));
    if (rango == NULL)
    {
        imprimir_mensaje("Error: No se pudo asignar memoria para los rangos");
        exit(EXIT_FAILURE);
    }
    return rango;
}

void liberar_rangos(struct rangos *rango)
{
    free(rango);
}

/*=============================*/
/*== ESTRUCTURA: INDIVIDUOS ==*/
/*=============================*/

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

void liberar_individuos(struct individuo *ind, int num_poblacion, bool tipo)
{
    if (tipo)
        for (int i = 0; i < num_poblacion; i++)
            liberar_lista_ruta(ind[i].hormiga->ruta);
    free(ind);
}

/*===============================*/
/*== ESTRUCTURA: CONFIGURACIÓN ==*/
/*===============================*/

struct tsp_configuracion *asignar_memoria_tsp_configuracion()
{
    struct tsp_configuracion *tsp = (struct tsp_configuracion *)malloc(sizeof(struct tsp_configuracion));
    if (tsp == NULL)
    {
        imprimir_mensaje("Error: No se pudo asignar memoria para la configuración tsp");
        exit(EXIT_FAILURE);
    }
    return tsp;
}

void liberar_memoria_tsp_configuracion(struct tsp_configuracion *tsp)
{
    if (tsp != NULL)
    {
        free(tsp->clientes);
        tsp->clientes = NULL;
        free(tsp);
    }
}

/*===========================*/
/*== ESTRUCTURA: CLIENTES ==*/
/*===========================*/

struct cliente *asignar_memoria_clientes(struct tsp_configuracion *tsp)
{
    struct cliente *cliente = (struct cliente *)malloc(tsp->num_clientes * sizeof(struct cliente));
    if (cliente == NULL)
    {
        imprimir_mensaje("Error: No se pudo asignar memoria para los clientes");
        exit(EXIT_FAILURE);
    }
    return cliente;
}

/*==========================*/
/*== ESTRUCTURA: HORMIGA ==*/
/*==========================*/

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

void reiniciar_hormiga(struct hormiga *hormiga, struct individuo *ind, struct tsp_configuracion *tsp)
{
    for (int i = 0; i < tsp->num_clientes; i++)
    {
        hormiga->tabu[i] = 0;
        hormiga->posibles_clientes[i] = 0;
        hormiga->probabilidades[i] = 0.0;
    }

    hormiga->tabu_contador = 0;
    hormiga->posibles_clientes_contador = 0;
    hormiga->suma_probabilidades = 0.0;
    hormiga->fitness_global = 0.0;

    vaciar_lista_ruta(hormiga->ruta);
    insertar_cliente_ruta(hormiga, &tsp->clientes[0]);
}

void liberar_memoria_hormiga(struct hormiga *hormiga)
{
    liberar_memoria_arreglo_int(hormiga->tabu);
    liberar_memoria_arreglo_int(hormiga->posibles_clientes);
    liberar_memoria_arreglo_double(hormiga->probabilidades);
    liberar_lista_ruta(hormiga->ruta);
}

/*=========================*/
/*== ESTRUCTURA: METAL ====*/
/*=========================*/

struct metal *asignar_memoria_metal()
{
    struct metal *metal = (struct metal *)malloc(sizeof(struct metal));
    if (metal == NULL)
    {
        imprimir_mensaje("No se pudo asignar memoria para la estructura metal");
        exit(EXIT_FAILURE);
    }
    return metal;
}

void liberar_memoria_metal(struct individuo *ind)
{
    if (ind->metal->solucion_inicial)
        liberar_lista_ruta(ind->metal->solucion_inicial);
    
    if (ind->metal->solucion_vecina)
        liberar_lista_ruta(ind->metal->solucion_vecina);
    
    if (ind->metal->mejor_solucion)
        liberar_lista_ruta(ind->metal->mejor_solucion);

    free(ind->metal);
}

/*==========================*/
/*== ESTRUCTURA: RUTA ======*/
/*==========================*/

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
