#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "aed.h"
#include "configuracion_vrp_tw.h"
#include "vrp_tw_aco.h"
#include "control_memoria.h"

double calcular_distancia(struct vrp_configuracion *vrp, int cliente_origen, int cliente_destino)
{
   return sqrt(pow((vrp->clientes[cliente_destino].cordenada_x - vrp->clientes[cliente_origen].cordenada_x), 2.0) + pow((vrp->clientes[cliente_destino].cordenada_y - vrp->clientes[cliente_origen].cordenada_y), 2.0));
}

void inicializar_Visibilidad(double **instancia_visibilidad, struct vrp_configuracion *vrp)
{
   for (int i = 0; i < vrp->num_clientes; i++)
   {
      for (int j = 0; j < vrp->num_clientes; j++)
      {
         if (i != j)
            instancia_visibilidad[i][j] = 1.0 / calcular_distancia(vrp, i, j);

         else
            instancia_visibilidad[i][j] = 0.0;
      }
   }
}

void inicializar_Distancias(double **instancia_distancias, struct vrp_configuracion *vrp)
{
   for (int i = 0; i < vrp->num_clientes; i++)
   {
      for (int j = 0; j < vrp->num_clientes; j++)
      {
         if (i != j)
            instancia_distancias[i][j] = calcular_distancia(vrp, i, j);

         else
            instancia_distancias[i][j] = 0.0;
      }
   }
}

void inicializar_Feromona(struct vrp_configuracion *vrp, double **instancia_feromona, struct individuo *ind)
{
   for (int i = 0; i < vrp->num_clientes; i++)
      for (int j = 0; j < vrp->num_clientes; j++)
      {
         if (i != j)
            instancia_feromona[i][j] = ind->alpha;
         else
            instancia_feromona[i][j] = 0.0;
      }
}

void evaluaFO_AED(struct individuo *ind, double **instancia_feromona, double **instancia_visibilidad,double **instancia_distancias,struct vrp_configuracion *vrp)
{
   inicializar_Feromona(vrp, instancia_feromona, ind);
   vrp_tw_aco(vrp, ind, instancia_visibilidad,instancia_distancias,instancia_feromona);
}

double generaAleatorio(double minimo, double maximo)
{
   double aleatorio = (double)rand() / RAND_MAX;
   return minimo + aleatorio * (maximo - minimo);
}

void construyeRuidosos(struct individuo *objetivo, struct individuo *ruidoso, int poblacion)
{
   for (int i = 0; i < poblacion; ++i)
   {
      int aleatorio1 = rand() % poblacion;
      int aleatorio2 = rand() % poblacion;
      int aleatorio3 = rand() % poblacion;

      while (aleatorio1 == aleatorio2 || aleatorio2 == aleatorio3 || aleatorio1 == aleatorio3)
      {
         aleatorio1 = rand() % poblacion;
         aleatorio2 = rand() % poblacion;
         aleatorio3 = rand() % poblacion;
      }

      ruidoso[i].alpha = objetivo[aleatorio1].alpha + 0.5 * (objetivo[aleatorio2].alpha - objetivo[aleatorio3].alpha);
      ruidoso[i].beta = objetivo[aleatorio1].beta + 0.5 * (objetivo[aleatorio2].beta - objetivo[aleatorio3].beta);
      ruidoso[i].gamma = objetivo[aleatorio1].gamma + 0.5 * (objetivo[aleatorio2].gamma - objetivo[aleatorio3].gamma);
      ruidoso[i].rho = objetivo[aleatorio1].rho + 0.5 * (objetivo[aleatorio2].rho - objetivo[aleatorio3].rho);
      ruidoso[i].numHormigas = objetivo[aleatorio1].numHormigas + (int)(0.5 * (objetivo[aleatorio2].numHormigas - objetivo[aleatorio3].numHormigas));
      ruidoso[i].numIteraciones = objetivo[aleatorio1].numIteraciones + (int)(0.5 * (objetivo[aleatorio2].numIteraciones - objetivo[aleatorio3].numIteraciones));

      if (ruidoso[i].alpha > 2.0)
         ruidoso[i].alpha = 2.0;

      if (ruidoso[i].alpha < 0.1)
         ruidoso[i].alpha = 0.1;

      if (ruidoso[i].beta > 2.5)
         ruidoso[i].beta = 2.5;

      if (ruidoso[i].beta < 1.5)
         ruidoso[i].beta = 1.5;

      if (ruidoso[i].gamma > 2.0)
         ruidoso[i].gamma = 2.0;

      if (ruidoso[i].gamma < 0.5)
         ruidoso[i].gamma = 0.5;

      if (ruidoso[i].rho > 1.0)
         ruidoso[i].rho = 1.0;

      if (ruidoso[i].rho < 0.0)
         ruidoso[i].rho = 0.0;

      if (ruidoso[i].numHormigas > 20)
         ruidoso[i].numHormigas = 20;

      if (ruidoso[i].numHormigas < 10)
         ruidoso[i].numHormigas = 10;

      if (ruidoso[i].numIteraciones > 50)
         ruidoso[i].numIteraciones = 50;

      if (ruidoso[i].numIteraciones < 20)
         ruidoso[i].numIteraciones = 20;
   }
}

void construyePrueba(struct individuo *objetivo, struct individuo *ruidoso, struct individuo *prueba, int poblacion)
{
   for (int i = 0; i < poblacion; ++i)
   {
      double aleatorio = (double)rand() / RAND_MAX;
      if (aleatorio <= 0.5)
         prueba[i] = objetivo[i];
      else
         prueba[i] = ruidoso[i];
   }
}

void seleccion(struct individuo *objetivo, struct individuo *prueba, int poblacion)
{
   for (int i = 0; i < poblacion; ++i)
      if (objetivo[i].fitness > prueba[i].fitness)
         objetivo[i] = prueba[i];
}

void inicializaPoblacion(struct individuo *objetivo, int poblacion)
{
   for (int i = 0; i < poblacion; ++i)
   {
      objetivo[i].alpha = generaAleatorio(0.1, 2.0);
      objetivo[i].beta = generaAleatorio(1.5, 2.5);
      objetivo[i].gamma = generaAleatorio(0.5, 2.0);
      objetivo[i].rho = generaAleatorio(0.0, 1.0);
      objetivo[i].numHormigas = (int)generaAleatorio(10, 20);
      objetivo[i].numIteraciones = (int)generaAleatorio(20, 50);
   }
}

int aed_vrp_tw(int num_poblacion, int num_generaciones, char *archivo_instancia)
{
   struct individuo *objetivo = asignar_memoria_individuos(num_poblacion); // Asignamos memoria para el arreglo objetivo
   struct individuo *ruidoso = asignar_memoria_individuos(num_poblacion);  // Asignamos memoria para el arreglo ruidoso
   struct individuo *prueba = asignar_memoria_individuos(num_poblacion);   // Asiganamos memoria para el arreglo prueba
   vrp_configuracion *vrp = leer_instancia(archivo_instancia);             // Mandamo a leer la instancia y a retormamos en un apuntador structura vrp_configuracion

   double **instancia_visibilidad = asignar_memoria_instancia(vrp->num_clientes);
   double **instancia_feromonas = asignar_memoria_instancia(vrp->num_clientes);
   double **instancia_distancias = asignar_memoria_instancia(vrp->num_clientes);

   inicializar_Visibilidad(instancia_visibilidad, vrp);
   inicializar_Distancias(instancia_distancias,vrp);

   for (int i = 0; i < num_generaciones; i++)
   {
      construyeRuidosos(objetivo, ruidoso, num_poblacion);
      construyePrueba(objetivo, ruidoso, prueba, num_poblacion);

      for (int j = 0; j < num_poblacion; ++j)
      {
         evaluaFO_AED(&objetivo[j], instancia_feromonas, instancia_visibilidad,instancia_distancias, vrp);
         evaluaFO_AED(&prueba[j], instancia_feromonas, instancia_visibilidad, instancia_distancias, vrp);
      }

      seleccion(objetivo, prueba, num_poblacion);
   }

   liberar_instancia(instancia_feromonas, vrp->num_clientes);
   liberar_instancia(instancia_visibilidad, vrp->num_clientes);
   liberar_memoria_vrp_configuracion(vrp);

   return 0;
}