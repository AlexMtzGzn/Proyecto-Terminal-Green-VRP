#include <stdio.h>
#include <stdlib.h>
#include "AED.h"
#include "TSP_ACO.h"
#include "entrada_salida_datos.h"
#include "control_memoria.h"

void inializacion_instancia_feromona(double **instancia_feromonas, int tamanio_instancia, individuo *ind)
{
   for (int i = 0; i < tamanio_instancia; i++)
      for (int j = 0; j < tamanio_instancia; j++)
      {
         if (i == j)
            instancia_feromonas[i][j] = 0.0;
         else
            instancia_feromonas[i][j] = ind->alpha;
      }
}

void evaluaFO_AED(individuo *ind, double **instancia_feromonas, double **instancia_distancias, int tamanio_instancia)
{
   inializacion_instancia_feromona(instancia_feromonas, tamanio_instancia, ind);

   // Podemos imprimir matriz de Feromonas
   // imprimir_instancia(instancia_feromonas,tamanio_instancia,"Instancia de Feromonas");
   tsp_aco(ind, instancia_feromonas, instancia_distancias, tamanio_instancia);
}

double generaAleatorio(double minimo, double maximo)
{
   double aleatorio = (double)rand() / RAND_MAX;
   return minimo + aleatorio * (maximo - minimo);
}

void construyeRuidosos(individuo *objetivo, individuo *ruidoso, int poblacion)
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

void construyePrueba(individuo *objetivo, individuo *ruidoso, individuo *prueba, int poblacion)
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

void seleccion(individuo *objetivo, individuo *prueba, int poblacion)
{
   for (int i = 0; i < poblacion; ++i)
      if (objetivo[i].fitness > prueba[i].fitness)
         objetivo[i] = prueba[i];
}

void inicializaPoblacion(individuo *objetivo, int poblacion, int tamanio_instancia)
{
   for (int i = 0; i < poblacion; ++i)
   {
      objetivo[i].alpha = generaAleatorio(0.1, 2.0);
      objetivo[i].beta = generaAleatorio(1.5, 2.5);
      objetivo[i].rho = generaAleatorio(0.0, 1.0);
      objetivo[i].numHormigas = (int)generaAleatorio(10, 20);
      objetivo[i].numIteraciones = (int)generaAleatorio(20, 50);
   }
}

void aed(int poblacion, int generaciones, int tamanio_instancia, char *archivo_instancia)
{

   individuo *objetivo, *ruidoso, *prueba;

   objetivo = asignar_memoria_individuos(poblacion);
   ruidoso = asignar_memoria_individuos(poblacion);
   prueba = asignar_memoria_individuos(poblacion);

   double **instancia_distancias, **instancia_feromonas;

   instancia_distancias = (double **)malloc(tamanio_instancia * sizeof(double *));
   instancia_feromonas = (double **)malloc(tamanio_instancia * sizeof(double *));
   for (int i = 0; i < tamanio_instancia; i++)
   {
      instancia_distancias[i] = (double *)malloc(tamanio_instancia * sizeof(double));
      instancia_feromonas[i] = (double *)malloc(tamanio_instancia * sizeof(double));
   }

   leer_instancia(instancia_distancias, tamanio_instancia, archivo_instancia);

   // Podemos imprimir matriz de distancias
   // imprimir_instancia(instancia_distancias,tamanio_instancia,"Instancia de Distancias");

   inicializaPoblacion(objetivo, poblacion, tamanio_instancia);
   // Podemos imprimir el objetivo n
   // printf("\nPoblacion Inicial\n");
   // imprimir_individuo(objetivo, tamanio_instancia, poblacion, false);

   for (int i = 0; i < generaciones; i++)
   {
      construyeRuidosos(objetivo, ruidoso, poblacion);
      construyePrueba(objetivo, ruidoso, prueba, poblacion);

      for (int j = 0; j < poblacion; ++j)
      {
         evaluaFO_AED(&objetivo[j], instancia_feromonas, instancia_distancias, tamanio_instancia);
         evaluaFO_AED(&prueba[j], instancia_feromonas, instancia_distancias, tamanio_instancia);
      }

      // imprimir_individuo(objetivo, tamanio_instancia, poblacion, true);
      // imprimir_individuo(prueba, tamanio_instancia, poblacion, true);
      seleccion(objetivo, prueba, poblacion);

      if (i == generaciones - 1)
      {

         for (int j = 0; j < poblacion; ++j)
            evaluaFO_AED(&objetivo[j], instancia_feromonas, instancia_distancias, tamanio_instancia);

         // Imprime todos los individuos de la ultima generacion
         // printf("\n\nUltima Generacion De La Poblacion:\n");
         // imprimir_individuo(objetivo, tamanio_instancia, poblacion, true);
      }
   }

   int indice_mejor = 0;
   for (int j = 1; j < poblacion; ++j)
      if (objetivo[j].fitness < objetivo[indice_mejor].fitness)
         indice_mejor = j;

   // Podemos imprimir las mejores soluciones
   printf("\n\nLas Mejores Soluciones De La Ultima Generacion\n");
   imprimir_individuo(objetivo, tamanio_instancia, poblacion, true);

   liberar_instancia(instancia_distancias, tamanio_instancia);
   liberar_instancia(instancia_feromonas, tamanio_instancia);
   liberar_individuos(objetivo, true);
   liberar_individuos(prueba, true);
   liberar_individuos(ruidoso, false);
}
