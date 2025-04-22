#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "../include/vrp_sa.h"
#include "../include/estructuras.h"
#include "../include/lista_flota.h"

void evaluaFO_SA(struct individuo *ind, struct vrp_configuracion *vrp, double **instancia_distancias)
{
    ind->metal->fitness_solucion_vecina = 0.0;

    struct nodo_vehiculo *vehiculo_actual = ind->metal->solucion_vecina->cabeza;

    while (vehiculo_actual != NULL)
    {
        double fitness_vehiculo = 0.0;

        lista_ruta *ruta = vehiculo_actual->vehiculo->ruta;
        if (ruta == NULL)
        {
            vehiculo_actual = vehiculo_actual->siguiente;
            continue;
        }

        nodo_ruta *nodo_actual = ruta->cabeza;

        while (nodo_actual != NULL && nodo_actual->siguiente != NULL)
        {
            int cliente_actual = nodo_actual->cliente;
            int cliente_siguiente = nodo_actual->siguiente->cliente;

            fitness_vehiculo += instancia_distancias[cliente_actual][cliente_siguiente];

            nodo_actual = nodo_actual->siguiente;
        }

        vehiculo_actual->vehiculo->fitness_vehiculo = fitness_vehiculo;
        ind->metal->fitness_solucion_vecina += fitness_vehiculo;
        vehiculo_actual = vehiculo_actual->siguiente;
    }
}

bool moverClienteVehiculo(struct individuo *ind, struct vrp_configuracion *vrp)
{
    nodo_vehiculo *vehiculo_origen = NULL;
    nodo_vehiculo *vehiculo_destino = NULL;

    if (ind->hormiga->vehiculos_necesarios <= 1)
        return false;

    int intentos = 10;

    while (intentos--)
    {
        int id_vehiculo_origen = (rand() % ind->hormiga->vehiculos_necesarios) + 1;
        int id_vehiculo_destino;
        do
        {
            id_vehiculo_destino = (rand() % ind->hormiga->vehiculos_necesarios) + 1;
        } while (id_vehiculo_origen == id_vehiculo_destino);

        vehiculo_origen = ind->metal->solucion_vecina->cabeza;
        while (vehiculo_origen && vehiculo_origen->vehiculo->id_vehiculo != id_vehiculo_origen)
            vehiculo_origen = vehiculo_origen->siguiente;

        vehiculo_destino = ind->metal->solucion_vecina->cabeza;
        while (vehiculo_destino && vehiculo_destino->vehiculo->id_vehiculo != id_vehiculo_destino)
            vehiculo_destino = vehiculo_destino->siguiente;

        if (!vehiculo_origen || !vehiculo_destino)
            continue;

        if (vehiculo_origen->vehiculo->clientes_contados == 0)
            continue;

        int pos_cliente = rand() % vehiculo_origen->vehiculo->clientes_contados;

        nodo_ruta *prev = vehiculo_origen->vehiculo->ruta->cabeza;
        if (!prev)
            continue;

        nodo_ruta *actual = prev->siguiente;
        if (!actual)
            continue;

        for (int i = 0; i < pos_cliente && actual != NULL; i++)
        {
            prev = actual;
            actual = actual->siguiente;
        }

        if (!actual || actual->cliente == 0)
            continue;

        int id_cliente = actual->cliente;

        if (id_cliente < 0 || id_cliente >= vrp->num_clientes)
            continue;

        double demanda = vrp->clientes[id_cliente].demanda_capacidad;

        if ((vehiculo_destino->vehiculo->capacidad_acumulada + demanda) > vehiculo_destino->vehiculo->capacidad_maxima)
            continue;

        if (prev)
        {
            prev->siguiente = actual->siguiente;
        }
        else
        {
            vehiculo_origen->vehiculo->ruta->cabeza = actual->siguiente;
        }

        if (actual == vehiculo_origen->vehiculo->ruta->cola)
        {
            vehiculo_origen->vehiculo->ruta->cola = prev;
        }

        vehiculo_origen->vehiculo->capacidad_acumulada -= demanda;
        vehiculo_origen->vehiculo->clientes_contados--;

        nodo_ruta *penultimo = vehiculo_destino->vehiculo->ruta->cabeza;
        if (!penultimo)
        {
            free(actual);
            return false;
        }

        while (penultimo->siguiente && penultimo->siguiente != vehiculo_destino->vehiculo->ruta->cola)
        {
            penultimo = penultimo->siguiente;
        }

        actual->siguiente = vehiculo_destino->vehiculo->ruta->cola;
        penultimo->siguiente = actual;

        vehiculo_destino->vehiculo->capacidad_acumulada += demanda;
        vehiculo_destino->vehiculo->clientes_contados++;

        return true;
    }

    return false;
}

bool intercambiarClientes(struct individuo *ind, struct vrp_configuracion *vrp)
{
    int vehiculo_aleatorio;
    nodo_vehiculo *vehiculo_actual = NULL;

    int intentos = 10;

    while (intentos--)
    {
        vehiculo_aleatorio = (rand() % ind->hormiga->vehiculos_necesarios) + 1;

        vehiculo_actual = ind->metal->solucion_vecina->cabeza;
        while (vehiculo_actual != NULL)
        {
            if (vehiculo_actual->vehiculo->id_vehiculo == vehiculo_aleatorio)
                break;
            vehiculo_actual = vehiculo_actual->siguiente;
        }

        if (vehiculo_actual == NULL || vehiculo_actual->vehiculo->clientes_contados < 2)
            continue;

        break;
    }

    if (vehiculo_actual == NULL || vehiculo_actual->vehiculo->clientes_contados < 2)
        return false;

    int cliente1_idx, cliente2_idx;
    do
    {
        cliente1_idx = rand() % vehiculo_actual->vehiculo->clientes_contados;
        cliente2_idx = rand() % vehiculo_actual->vehiculo->clientes_contados;
    } while (cliente1_idx == cliente2_idx);

    nodo_ruta *nodo1 = vehiculo_actual->vehiculo->ruta->cabeza->siguiente;
    nodo_ruta *nodo2 = vehiculo_actual->vehiculo->ruta->cabeza->siguiente;

    for (int i = 0; i < cliente1_idx && nodo1 != NULL; i++)
        nodo1 = nodo1->siguiente;
    for (int i = 0; i < cliente2_idx && nodo2 != NULL; i++)
        nodo2 = nodo2->siguiente;

    if (nodo1 == NULL || nodo2 == NULL)
        return false;

    int temp = nodo1->cliente;
    nodo1->cliente = nodo2->cliente;
    nodo2->cliente = temp;

    return true;
}


// Genera una solución vecina a partir de la solución actual
void generar_vecino(struct individuo *ind, struct vrp_configuracion *vrp)
{
    if (ind->metal->solucion_vecina) {
        liberar_lista_vehiculos(ind->metal->solucion_vecina);
    }

    ind->metal->solucion_vecina = copiar_lista_vehiculos(ind->metal->solucion_actual);
}

void sa(struct vrp_configuracion *vrp, struct individuo *ind, double **instancia_distancias)
{
    double temperatura = ind->temperatura_inicial;
    double delta;
    bool aceptado = false;

    ind->metal->mejor_solucion = copiar_lista_vehiculos(ind->metal->solucion_inicial);
    if (!ind->metal->mejor_solucion) return;
    ind->metal->fitness_mejor_solucion = ind->metal->fitness_solucion_inicial;

    ind->metal->solucion_actual = copiar_lista_vehiculos(ind->metal->solucion_inicial);
    if (!ind->metal->solucion_actual) return;
    ind->metal->fitness_solucion_actual = ind->metal->fitness_solucion_inicial;

    int iteracion_total = 0;
    int cambios_aceptados = 0;

    while (temperatura > ind->temperatura_final)
    {
        int cambios_en_ciclo = 0;

        for (int i = 0; i < ind->numIteracionesSA; i++)
        {
            iteracion_total++;

            generar_vecino(ind, vrp);
            if (!ind->metal->solucion_vecina) continue;

            if (ind->hormiga->vehiculos_necesarios > 1)
            {
                double prob = (double)rand() / RAND_MAX;
                double factor_control = 0.7;
                if (prob < factor_control * (1.0 - (temperatura / ind->temperatura_inicial))) {
                    aceptado = intercambiarClientes(ind, vrp);
                } else {
                    aceptado = moverClienteVehiculo(ind, vrp);
                }
            }
            else
            {
                aceptado = intercambiarClientes(ind, vrp);
            }

            if (!aceptado) continue;

            evaluaFO_SA(ind, vrp, instancia_distancias);

            delta = ind->metal->fitness_solucion_vecina - ind->metal->fitness_solucion_actual;

            bool aceptar = false;
            if (delta < 0) {
                aceptar = true;
            } else {
                double prob_aceptacion = exp(-delta / temperatura);
                double aleatorio = (double)rand() / RAND_MAX;
                if (aleatorio < prob_aceptacion) {
                    aceptar = true;
                }
            }

            if (aceptar)
            {
                liberar_lista_vehiculos(ind->metal->solucion_actual);
                ind->metal->solucion_actual = copiar_lista_vehiculos(ind->metal->solucion_vecina);
                ind->metal->fitness_solucion_actual = ind->metal->fitness_solucion_vecina;
                cambios_aceptados++;
                cambios_en_ciclo++;

                if (ind->metal->fitness_solucion_actual < ind->metal->fitness_mejor_solucion)
                {
                    liberar_lista_vehiculos(ind->metal->mejor_solucion);
                    ind->metal->mejor_solucion = copiar_lista_vehiculos(ind->metal->solucion_actual);
                    ind->metal->fitness_mejor_solucion = ind->metal->fitness_solucion_actual;
                }
            }
        }

        temperatura *= ind->factor_enfriamiento;
    }

    liberar_lista_vehiculos(ind->metal->solucion_actual);
}

void inicializar_metal(struct individuo *ind)
{
    ind->metal = (struct metal *)malloc(sizeof(struct metal));
    if (ind->metal == NULL) {
        exit(EXIT_FAILURE);
    }

    ind->metal->solucion_vecina = NULL;
    ind->metal->solucion_actual = NULL;
    ind->metal->mejor_solucion = NULL;
    ind->metal->fitness_solucion_actual = 0.0;
    ind->metal->fitness_solucion_vecina = 0.0;
    ind->metal->fitness_mejor_solucion = 0.0;

    ind->metal->solucion_inicial = copiar_lista_vehiculos(ind->hormiga->flota);
    if (ind->metal->solucion_inicial == NULL) {
        free(ind->metal);
        exit(EXIT_FAILURE);
    }

    ind->metal->fitness_solucion_inicial = ind->hormiga->fitness_global;
}

void liberar_memoria_metal(struct individuo *ind)
{
    if (ind->metal) {
        if (ind->metal->solucion_vecina) {
            liberar_lista_vehiculos(ind->metal->solucion_vecina);
        }

        if (ind->metal->solucion_inicial) {
            liberar_lista_vehiculos(ind->metal->solucion_inicial);
        }

        if (ind->metal->mejor_solucion) {
            liberar_lista_vehiculos(ind->metal->mejor_solucion);
        }

        free(ind->metal);
        ind->metal = NULL;
    }
}

void vrp_sa(struct vrp_configuracion *vrp, struct individuo *ind, double **instancia_distancias)
{
    inicializar_metal(ind);

    sa(vrp, ind, instancia_distancias);

    if (ind->metal->fitness_mejor_solucion < ind->hormiga->fitness_global)
    {
        ind->fitness = ind->metal->fitness_mejor_solucion;
        ind->hormiga->fitness_global = ind->metal->fitness_mejor_solucion;

        if (ind->hormiga->flota)
            liberar_lista_vehiculos(ind->hormiga->flota);

        ind->hormiga->flota = copiar_lista_vehiculos(ind->metal->mejor_solucion);
    }

    liberar_memoria_metal(ind);
}
