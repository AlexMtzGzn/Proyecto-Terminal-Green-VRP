#ifndef ENTRADA_SALIDA_DATOS_H
#define ENTRADA_SALIDA_DATOS_H
#include <stdbool.h>
#include "AED.h"
#include "TSP_ACO.h"

void leer_instancia(double **instancia_distancias, int tamanio_instancia, char *archivo_instancia);
void imprimir_instancia(double **matriz_instancia, int tamanio_instancia, char *texto_instancia);
void imprimir_individuo(individuo *ind, int tamanio_instancia, int poblacion, bool bandera);
void imprimir_ind(individuo *ind, int tamanio_instancia, int poblacion);
void imprimir_ruta_hormiga(hormiga *hor, int tamanio_instancia, int n_hormiga, int n_iteracion);

#endif // ENTRADA_SALIDA_DATOS_H