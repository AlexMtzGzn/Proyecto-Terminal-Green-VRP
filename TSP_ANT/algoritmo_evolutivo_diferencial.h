#ifndef ALGORITMO_EVOLUTIVO_DIFERENCIAL_H
#define ALGORITMO_EVOLUTIVO_DIFERENCIAL_H

typedef struct
{
    double alpha;       // Influencia de la feromona
    double beta;        // Influencia del costo
    double rho;         // Tasa de evaporación de la feromona
    int numHormigas;    // Número de hormigas
    int numIteraciones; // Número de iteraciones
    double fitness;     // Valor de fitness         // Ruta solo para la ultima iteracion de la genracion
} individuo;

typedef struct {
    individuo mejor_individuo; // Estructura con los parámetros del individuo
    double mejor_fitness;      // Valor del fitness del mejor individuo
} mejor_individuo_t;


void inializacion_instancia_feromona(double **instancia_feromona, int tamanio_instancia, double alpha);
void evaluaFO(individuo *ind, double **instancia_feromona, double **instancia_distancias,int tamanio_instancia,int bandera);
double generaAleatorio(double minimo, double maximo);
void inicializaPoblacion(individuo *objetivo, int poblacion);
void construyeRuidosos(individuo *objetivo, individuo *ruidoso, int poblacion);
void construyePrueba(individuo *objetivo, individuo *ruidoso, individuo *prueba, int poblacion);
void imprimeIndividuo(individuo ind);
void imprimePoblacion(individuo *objetivo, int poblacion);
void leer_instancia(double **instancia_distancias, int tamanio_instancia, char *archivo_instancia);
void imprimir_instancia(double ** instancia, int tamanio_instancia);
void algoritmo_evolutivo_diferencial(int poblacion,int generaciones,int tamanio_instancia, char *archivo_instancia);

#endif // ALGORITMO_EVOLUTIVO_DIFERENCIAL_H
