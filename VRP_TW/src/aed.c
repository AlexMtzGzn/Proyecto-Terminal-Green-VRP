#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../include/aed.h"
#include "../include/estructuras.h"
#include "../include/vrp_tw_aco.h"
#include "../include/configuracion_vrp_tw.h"
#include "../include/control_memoria.h"
#include "../include/salida_datos.h"
#include "../include/configuracion_json.h"

double calcular_Distancia(struct vrp_configuracion *vrp, int cliente_origen, int cliente_destino)
{
   // Retornamos la distancia de los puntos
   return sqrt(pow((vrp->clientes[cliente_destino].coordenada_x - vrp->clientes[cliente_origen].coordenada_x), 2.0) + pow((vrp->clientes[cliente_destino].coordenada_y - vrp->clientes[cliente_origen].coordenada_y), 2.0));
}

void inicializar_Visibilidad(double **instancia_visibilidad, struct vrp_configuracion *vrp)
{
   // Recorre todos los clientes y calcula la visibilidad entre ellos
   for (int i = 0; i < vrp->num_clientes; i++)
   {
      for (int j = i + 1; j < vrp->num_clientes; j++)
      {
         // Calculamos la distancia entre los clientes i y j una sola vez
         double distancia = calcular_Distancia(vrp, i, j);

         // Si la distancia es mayor a 0, calculamos la visibilidad
         if (distancia > 0)
         {
            instancia_visibilidad[i][j] = 1.0 / distancia;
            instancia_visibilidad[j][i] = instancia_visibilidad[i][j]; // Aprovechamos la simetría
         }
         else
         {
            instancia_visibilidad[i][j] = 0.0; // Es 0.0
         }
      }
   }
}

void inicializar_Ventana_Tiempo(double **instancia_ventanas_tiempo, struct vrp_configuracion *vrp)
{
   // Recorre todos los clientes y calcula la ventana de tiempo entre ellos
   for (int i = 0; i < vrp->num_clientes; i++)
   {
      for (int j = i + 1; j < vrp->num_clientes; j++)
      {
         // Si i y j son diferentes, asigna la ventana de tiempo como 1 / tiempo final del cliente j
         if (calcular_Distancia(vrp, i, j) > 0)
         {
            instancia_ventanas_tiempo[i][j] = 1.0 / vrp->clientes[j].vt_final;
            instancia_ventanas_tiempo[j][i] = instancia_ventanas_tiempo[i][j]; // Aprovechamos la simetría
         }
         else
         {
            instancia_ventanas_tiempo[i][j] = 0.0; // Es 0.0
         }
      }
   }
}

void inicializar_Distancias(double **instancia_distancias, struct vrp_configuracion *vrp)
{
   // Recorre todos los clientes y calcula las distancias entre ellos
   for (int i = 0; i < vrp->num_clientes; i++)
   {
      for (int j = i + 1; j < vrp->num_clientes; j++)
      {
         // Calculamos la distancia entre los clientes i y j
         double distancia = calcular_Distancia(vrp, i, j);

         if (i != j)
         {
            instancia_distancias[i][j] = distancia;
            instancia_distancias[j][i] = distancia; // Aprovechamos la simetría
         }
         else
         {
            instancia_distancias[i][j] = 0.0; // Es 0.0
         }
      }
   }
}

void inicializar_Feromona(struct vrp_configuracion *vrp, double **instancia_feromona)
{
   // Recorre todos los clientes en la matriz de feromonas (fila i, columna j)
   for (int i = 0; i < vrp->num_clientes; i++)
   {
      for (int j = 0; j < vrp->num_clientes; j++)
      {
         // Si i y j son diferentes (es decir, no es la misma ciudad), se asigna un valor de feromona de 1.0
         if (i != j)
            instancia_feromona[i][j] = 1.0; // Hay que revisr si lo hacemo entre la ventana del tiempo final
         // Si i y j son iguales (es decir, es la misma ciudad), la feromona se establece en 0.0
         else
            instancia_feromona[i][j] = 0.0;
      }
   }
}

void evaluaFO_AED(struct individuo *ind, double **instancia_feromona, double **instancia_visibilidad, double **instancia_distancias, double **instancia_ventanas_tiempo, struct vrp_configuracion *vrp)
{
   // Inicializa las feromonas en la instancia
   inicializar_Feromona(vrp, instancia_feromona);
   // imprimir_instancia(instancia_feromonas,vrp,"INSTANCIA FEROMONAS");
   // Ejecuta el algoritmo de optimización con ventanas de tiempo (ACO) en el individuo
   vrp_tw_aco(vrp, ind, instancia_visibilidad, instancia_distancias, instancia_feromona, instancia_ventanas_tiempo);
}

double generaAleatorio(double minimo, double maximo)
{
   // Genera un número aleatorio entre 0 y 1, luego lo escala al rango deseado
   return minimo + ((double)rand() / RAND_MAX) * (maximo - minimo);
}

void construyeRuidosos(struct individuo *objetivo, struct individuo *ruidoso, int poblacion)
{
   // Recorre cada individuo de la población ruidosa
   for (int i = 0; i < poblacion; ++i)
   {
      // Selecciona tres índices aleatorios diferentes de la población
      int aleatorio1 = rand() % poblacion;
      int aleatorio2 = rand() % poblacion;
      int aleatorio3 = rand() % poblacion;

      // Asegura que los índices sean distintos entre sí
      while (aleatorio1 == aleatorio2 || aleatorio2 == aleatorio3 || aleatorio1 == aleatorio3)
      {
         aleatorio1 = rand() % poblacion;
         aleatorio2 = rand() % poblacion;
         aleatorio3 = rand() % poblacion;
      }

      // Genera un individuo ruidoso como una combinación de tres individuos aleatorios
      ruidoso[i].alpha = objetivo[aleatorio1].alpha + 0.5 * (objetivo[aleatorio2].alpha - objetivo[aleatorio3].alpha);
      ruidoso[i].beta = objetivo[aleatorio1].beta + 0.5 * (objetivo[aleatorio2].beta - objetivo[aleatorio3].beta);
      ruidoso[i].gamma = objetivo[aleatorio1].gamma + 0.5 * (objetivo[aleatorio2].gamma - objetivo[aleatorio3].gamma);
      ruidoso[i].rho = objetivo[aleatorio1].rho + 0.5 * (objetivo[aleatorio2].rho - objetivo[aleatorio3].rho);
      ruidoso[i].numHormigas = objetivo[aleatorio1].numHormigas + (int)(0.5 * (objetivo[aleatorio2].numHormigas - objetivo[aleatorio3].numHormigas));
      ruidoso[i].numIteraciones = objetivo[aleatorio1].numIteraciones + (int)(0.5 * (objetivo[aleatorio2].numIteraciones - objetivo[aleatorio3].numIteraciones));

      // Limita los valores de los parámetros para que estén dentro de un rango válido
      // Ajusta los valores de alpha dentro del rango permitido [1.0, 2.5]
      if (ruidoso[i].alpha > 2.5)
         ruidoso[i].alpha = 2.5;

      if (ruidoso[i].alpha < 1.0)
         ruidoso[i].alpha = 1.0;

      // Ajusta los valores de beta dentro del rango permitido [2.5, 4.5]
      if (ruidoso[i].beta > 4.5)
         ruidoso[i].beta = 4.5;

      if (ruidoso[i].beta < 2.5)
         ruidoso[i].beta = 2.5;

      // Ajusta los valores de gamma dentro del rango permitido [0.7, 1.5]
      if (ruidoso[i].gamma > 1.5)
         ruidoso[i].gamma = 1.5;

      if (ruidoso[i].gamma < 0.7)
         ruidoso[i].gamma = 0.7;

      // Ajusta los valores de rho dentro del rango permitido [0.5, 0.7]
      if (ruidoso[i].rho > 0.7)
         ruidoso[i].rho = 0.7;

      if (ruidoso[i].rho < 0.5)
         ruidoso[i].rho = 0.5;

      // Ajusta el número de hormigas dentro del rango permitido [30, 45]
      if (ruidoso[i].numHormigas > 45)
         ruidoso[i].numHormigas = 45;

      if (ruidoso[i].numHormigas < 30)
         ruidoso[i].numHormigas = 30;

      // Ajusta el número de iteraciones dentro del rango permitido [150, 300]
      if (ruidoso[i].numIteraciones > 300)
         ruidoso[i].numIteraciones = 300;

      if (ruidoso[i].numIteraciones < 150)
         ruidoso[i].numIteraciones = 150;
   }
}

void construyePrueba(struct individuo *objetivo, struct individuo *ruidoso, struct individuo *prueba, int poblacion)
{
   // Itera sobre todos los individuos en la población.
   for (int i = 0; i < poblacion; ++i)
   {
      // Genera un número aleatorio en el rango [0,1].
      double aleatorio = (double)rand() / RAND_MAX;

      // Con una probabilidad del 50%, selecciona el individuo de la población objetivo.
      if (aleatorio <= 0.5)
         prueba[i] = objetivo[i];
      // En caso contrario, selecciona el individuo de la población ruidosa.
      else
         prueba[i] = ruidoso[i];
   }
}

void seleccion(struct individuo *objetivo, struct individuo *prueba, int poblacion)
{
   // Recorremos la población para comparar cada individuo de ambas poblaciones.
   for (int i = 0; i < poblacion; ++i)
      // Si el fitness del individuo en la población objetivo es mayor que en la población de prueba,
      // se actualiza el individuo en la población objetivo por el de la población de prueba.
      if (objetivo[i].fitness > prueba[i].fitness)
         objetivo[i] = prueba[i];
}

void inicializaPoblacion(struct individuo *objetivo, int poblacion)
{
   for (int i = 0; i < poblacion; ++i)
   {
      // Asignamos valores aleatorios dentro de los nuevos rangos recomendados
      objetivo[i].alpha = generaAleatorio(1.0, 2.5);              // alpha: entre 1.0 y 2.5
      objetivo[i].beta = generaAleatorio(2.5, 4.5);               // beta: entre 2.5 y 4.5
      objetivo[i].gamma = generaAleatorio(0.7, 1.5);              // gamma: entre 0.7 y 1.5
      objetivo[i].rho = generaAleatorio(0.5, 0.7);                // rho: entre 0.5 y 0.7
      objetivo[i].numHormigas = (int)generaAleatorio(30, 45);     // numHormigas: entre 30 y 45
      objetivo[i].numIteraciones = (int)generaAleatorio(150, 300); // numIteraciones: entre 150 y 300
   }
}

void aed_vrp_tw(int num_poblacion, int num_generaciones, int tamanio_instancia, char *archivo_instancia)
{
   clock_t timepo_inicial, timepo_final;
   timepo_inicial = clock();
   char respuesta;                                                                // Respuesta
   struct individuo *objetivo = asignar_memoria_individuos(num_poblacion);        // Asignamos memoria para el arreglo objetivo
   struct individuo *ruidoso = asignar_memoria_individuos(num_poblacion);         // Asignamos memoria para el arreglo ruidoso
   struct individuo *prueba = asignar_memoria_individuos(num_poblacion);          // Asiganamos memoria para el arreglo prueba
   struct individuo *resultado = asignar_memoria_individuos(1);                   // Asignamos memoria para el arreglo de resultados
   vrp_configuracion *vrp = leer_instancia(archivo_instancia, tamanio_instancia); // Mandamo a leer la instancia y a retormamos en un apuntador structura vrp_configuracion

   double **instancia_visibilidad = asignar_memoria_instancia(vrp->num_clientes);     // Generamos memoria para la instancia de la visibilidad
   double **instancia_feromonas = asignar_memoria_instancia(vrp->num_clientes);       // Generamos memoria para la instancia de la feromona
   double **instancia_distancias = asignar_memoria_instancia(vrp->num_clientes);      // Generamos memoria para la instancia de la las distancias
   double **instancia_ventanas_tiempo = asignar_memoria_instancia(vrp->num_clientes); // Generamos memoria para la instancia de las ventanas de tiempo

   inicializar_Distancias(instancia_distancias, vrp);          // Inicializamos las distancias
   inicializar_Visibilidad(instancia_visibilidad, vrp);        // Inicializamos las visibilidad
   inicializar_Ventana_Tiempo(instancia_ventanas_tiempo, vrp); // Inicializmos las ventanas de tiempo
   inicializar_Feromona(vrp, instancia_feromonas);             // Inicializamos la feromona
   inicializaPoblacion(objetivo, num_poblacion);               // Inicializamos la poblacion

   // Aqui podemos imprimir las instancias
   // imprimir_instancia(instancia_distancias,vrp,"INSTANCIA DISTANCIAS");
   // imprimir_instancia(instancia_feromonas,vrp,"INSTANCIA FEROMONAS");
   // imprimir_instancia(instancia_ventanas_tiempo,vrp,"INSTANCIA VENTANAS DE TIEMPO");
   // imprimir_instancia(instancia_visibilidad,vrp,"INSTANCIA VISIBILIDAD");

   // Inicializamos la estructura de resultados
   resultado->fitness = INFINITY;
   resultado->hormiga = asignar_memoria_hormigas(1);
   // Evaluamos la función objetivo para cada individuo de la población inicial
   for (int i = 0; i < num_poblacion; ++i) // Iniciamos la funcion objetivo con el objetivo
      evaluaFO_AED(&objetivo[i], instancia_feromonas, instancia_visibilidad, instancia_distancias, instancia_ventanas_tiempo, vrp);
   // Encontramos el mejor individuo de la población inicial
   for (int i = 0; i < num_poblacion; i++)
   {
      if (objetivo[i].fitness < resultado->fitness)
      {
         resultado->alpha = objetivo[i].alpha;
         resultado->beta = objetivo[i].beta;
         resultado->gamma = objetivo[i].gamma;
         resultado->rho = objetivo[i].rho;
         resultado->numHormigas = objetivo[i].numHormigas;
         resultado->numIteraciones = objetivo[i].numIteraciones;
         recuperamos_mejor_hormiga(resultado, objetivo[i].hormiga);
      }
   }

   // Inicializamos ya las generaciones
   for (int i = 0; i < num_generaciones; i++)
   {
      construyeRuidosos(objetivo, ruidoso, num_poblacion);       // Contruimos Ruidosos
      construyePrueba(objetivo, ruidoso, prueba, num_poblacion); // Contruimos Prueba
                                                                 // Evaluamos la función objetivo para cada individuo de prueba
      for (int j = 0; j < num_poblacion; ++j)                    // Mandamos a evaluar la funcion objetivo de prueba{
         evaluaFO_AED(&prueba[j], instancia_feromonas, instancia_visibilidad, instancia_distancias, instancia_ventanas_tiempo, vrp);

      for (int i = 0; i < num_poblacion; i++)
      {
         // Actualizamos el mejor resultado si encontramos uno mejor
         if (prueba[i].fitness < resultado->fitness)
         {
            resultado->alpha = prueba[i].alpha;
            resultado->beta = prueba[i].beta;
            resultado->gamma = prueba[i].gamma;
            resultado->rho = prueba[i].rho;
            resultado->numHormigas = prueba[i].numHormigas;
            resultado->numIteraciones = prueba[i].numIteraciones;
            recuperamos_mejor_hormiga(resultado, prueba[i].hormiga);
         }
      }
      // Realizamos la selección de la siguiente generación
      seleccion(objetivo, prueba, num_poblacion); // Hacemos la seleccion
   }

   timepo_final = clock();
   double minutos = (((double)(timepo_final - timepo_inicial)) / CLOCKS_PER_SEC) / 60.0;

   vrp->tiempo_ejecucion = ceil(minutos);
   vrp->archivo_instancia = archivo_instancia;

   // Imprimimos la meojor homriga
   imprimir_mejor_hormiga(resultado->hormiga, resultado);
   printf("\nEl tiempo de ejecución es: %.2f minutos\n", minutos);

   printf("\n¿Quieres imprimir el archivo JSON (s/n)? ");
   scanf(" %c", &respuesta);

   if (respuesta == 's' || respuesta == 'S')
      guardar_json_en_archivo(resultado, vrp, archivo_instancia);

   liberar_instancia(instancia_feromonas, vrp->num_clientes);       // Liberemos la memoria de la instancia feromona
   liberar_instancia(instancia_visibilidad, vrp->num_clientes);     // Liberemos la memoria de la instancia visibilidad
   liberar_instancia(instancia_distancias, vrp->num_clientes);      // Liberemos la memoria de la instancia distancias
   liberar_instancia(instancia_ventanas_tiempo, vrp->num_clientes); // Liberemos la memoria de la instancia ventanas de tiempo
   liberar_individuos(objetivo, num_poblacion, true);               // Liberemos la memoria del objetivo
   liberar_individuos(prueba, num_poblacion, true);                 // Liberemos la memoria de la prueba
   liberar_individuos(ruidoso, num_poblacion, false);               // Liberemos la memoria del ruidoso
   liberar_individuos(resultado, 1, true);                          // Liberemos los resultado
   liberar_memoria_vrp_configuracion(vrp);                          // Liberemos la memoria del vrp
}