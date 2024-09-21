#ifndef TSP_ANT_H
#define TSP_ANT_H
typedef struct
{
    double *ruta;
    double *tabu;
    double fitness;
}hormiga;

void caminar_hormiga(hormiga *hor, double **instancia_feromona, double **probabilidad, double **visibilidad, int tamanio_instancia);
void inicializar_ruleta(double ** instancia_feromona,double ** instacia_distancias, double **visibilidad, double **probabilidad, hormiga * hor, individuo * ind, int tamanio_instancia);
void ant_system(hormiga *hor, individuo *ind, double **instancia_distancias, double **instansia_feromona,double ** probabilidad, double ** visibilidad, int tamanio_instancia);
void inializacionHormiga(hormiga *hor, int tamanio_instancia, int numHormigas);
void inializacionHormiga(hormiga * hor, int tamanio_instancia,int numHormigas);
void aco_tsp(individuo * ind, double ** instancia_feromona,double ** instancia_distancias,int tamanio_instancia);

#endif // TSP_ANT_H