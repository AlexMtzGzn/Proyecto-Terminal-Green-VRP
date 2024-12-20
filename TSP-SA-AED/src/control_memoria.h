#ifndef CONTROL_MEMORIA_H
#define CONTROL_MEMORIA_H
#include <stdbool.h>
#include "AED.h"
#include "TSP_SA.h"

double **asignar_memoria_instancia(int tamanio_instancia);
individuo *asignar_memoria_individuos(int poblacion);
solucion *asignar_memoria_soluciones();
generacion * asignar_memoria_generaciones (int poblacion, int generaciones);
int *asignar_memoria_ruta(int tamanio_instancia);
void liberar_instancia(double **instancia, int tamanio_instancia);
void liberar_ruta(int *ruta);
void liberar_individuos(individuo *ind, bool bandera);
void liberar_soluciones(solucion *solucion, bool bandera);
void liberar_generaciones(generacion *generacion);


#endif // CONTROL_MEMORIA_H
