#include "split.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Recibe un string valido (finalizado en '\0') del cual se va a sustraer la parte deseada, 
 * y dos insigned ints de inicio y final (no incluido) de caracteres a sustraer
 * 
 * La funcion guarda en el heap un string valido sustraido del recibido, arrancando en "inicio" hasta "final" (no incluido)
 * o devuelve NULL en caso de fallo reservando la memoria
 */

char* sustraer_string(const char* string, size_t inicio, size_t final) {
  char* sustraccion = malloc((final - inicio + 1) * sizeof(char));
  if (!sustraccion) return NULL;
  for (size_t i = 0; i < final - inicio; i++) {
    sustraccion[i] = string[inicio + i];
  }
  sustraccion[final - inicio] = '\0';
  return sustraccion;
}

/*
 * Recibe un string valido (finalizado en '\0'), el char separador que se va utilizar para separar, y puntero a cantidad_substrings y longitud 
 * 
 * La funcion analiza el string guardando en los punteros recibidos el largo del string y la cantidad de substrings que se van a 
 * generar con el separador elegido
 *
 */
void contar_separadores_y_longitud(const char* string, char separador, size_t* cantidad_substrings, size_t* longitud) {
  while (string[*longitud] != '\0') {
    if (string[*longitud] == separador) (*cantidad_substrings)++;
    (*longitud)++;
  }
}

/*
 * Recibe un string valido (finalizado en '\0'), el char separador que se va utilizar para separar y una posicion inicial del string
 * 
 * La funcion busca en el string el siguente separador a partir de la posicion incial, en caso de no encontrar va a devolver el ultimo elemento del string
 *
 */
size_t encontrar_siguiente_separador(const char* string, char separador, size_t posicion_actual) {
  while (string[posicion_actual] != '\0' && string[posicion_actual] != separador) posicion_actual++;
  return posicion_actual;
}

/*
 * Recibe un vector con strings creado por el split y libera cada uno de los strings del heap y luego libera el vector
 * 
 */
void eliminar_string_vector(char** string_vector) {
  size_t i = 0;
  while (string_vector[i]) {
    free(string_vector[i]);
    i++;
  }

  free(string_vector);
}

/*
 * Recibe un string y un separador y devuelve un vector dinámico que contenga
 * los substrings delimitados por el separador. Al final del vector debe haber
 * un string NULL para saber que es el final del mismo.
 *
 * Si el string es nulo o no hay memoria disponible, se debe devolver NULL.
 *
 * Tanto el vector dinámico devuelto como los strings almacenados en el mismo
 * deben reservados con malloc/calloc y es el usuario quien se debe encargar de
 * liberar la memoria.
 *
 */
char** split(const char* string, char separador) {
  size_t longitud = 0;
  size_t cantidad_elementos = 1;
  size_t posicion_siguente_separador = 0;
  size_t principio_string_actual = 0;

  if (string == NULL) return NULL;

  contar_separadores_y_longitud(string, separador, &cantidad_elementos, &longitud);
  char** string_vector = malloc((cantidad_elementos + 1) * sizeof(char*));

  if (!string_vector) return NULL;

  for (size_t substring_actual = 0; substring_actual < cantidad_elementos; substring_actual++) {
    posicion_siguente_separador = encontrar_siguiente_separador(string, separador, posicion_siguente_separador);

    char* sustraccion = sustraer_string(string, principio_string_actual, posicion_siguente_separador);

    if (!sustraccion) {
      eliminar_string_vector(string_vector);
      return NULL;
    }

    string_vector[substring_actual] = sustraccion;

    posicion_siguente_separador++;
    principio_string_actual = posicion_siguente_separador;
  }

  string_vector[cantidad_elementos] = NULL;

  return string_vector;
}
