#ifndef CONFIGURACION_VRP_TW_H
#define CONFIGURACION_VRP_TW_H

// Incluimos las bibliotecas necesarias para trabajar con las estructuras de datos del VRP
#include "../includes/estructuras.h"
#include <cjson/cJSON.h>


// Declaramos prototipos de funciones que gestionan la configuración de la instancia VRP con ventanas de tiempo (VRPTW)

// Función para leer una instancia de VRP desde un archivo CSV, llenando la estructura vrp_configuracion
void leemos_csv(vrp_configuracion *vrp, char *archivo_instancia);

// Función para crear un archivo CSV a partir de los datos de la estructura vrp_configuracion
void creamos_csv(vrp_configuracion *vrp, char *archivo_instancia);

// Función para leer una instancia de VRP desde un archivo de texto, llenando la estructura vrp_configuracion
void leemos_txt(vrp_configuracion *vrp, char *ruta);

// Función para leer una instancia de VRP desde un archivo de instancia dado, devolviendo un puntero a la configuración VRP
vrp_configuracion *leer_instancia(char *archivo_instancia);

cJSON *generar_ruta_coordenadas(lista_ruta *ruta, cliente *clientes);
cJSON *vehiculo_a_json(vehiculo *v, cliente *clientes);
cJSON *individuo_a_json(individuo *ind, cliente *clientes);
void guardar_json_en_archivo(individuo *ind, vrp_configuracion *vrp, char *archivo_instancia);


#endif // CONFIGURACION_VRP_TW_H
