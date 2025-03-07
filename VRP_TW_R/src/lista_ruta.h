#ifndef LISTA_RUTA_H
#define LISTA_RUTA_H

#include <stdbool.h>

typedef struct nodo_ruta {
    int cliente;
    struct nodo_ruta *siguiente;
} nodo_ruta;

typedef struct lista_ruta {
    nodo_ruta *cabeza;
    nodo_ruta *cola;
} lista_ruta;

struct nodo_ruta *crear_nodo_ruta(struct cliente *cliente);
bool es_vacia_lista_ruta(struct lista_ruta *ruta);
void insertar_cliente_ruta(struct vehiculo *vehiculo, struct cliente *cliente);

#endif /*lista_ruta.h*/