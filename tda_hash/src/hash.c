#include "hash.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct casillero {
  void* elemento;
  char* clave;
  struct casillero* siguiente;
} casillero_t;

struct hash {
  size_t capacidad;
  size_t cantidad;
  casillero_t** casilleros;
  hash_destruir_dato_t destructor_elemento;
};

/*
 * Recibe una clave y la capacidad de la tabla donde se almacenará y devuelve un entero que  
 * representa el indice en el que debe estar ubicado el elemento.
 */
size_t hash_funcion_hash(const char* clave, size_t capacidad) {
  size_t sumatoria = 0;

  size_t tamanio_clave = strlen(clave);
  for (size_t i = 0; i < tamanio_clave / 2; i++) {
    sumatoria += clave[i] * clave[tamanio_clave - 1 - 1];
  }

  return sumatoria % capacidad;
}

/*
 * Reserva en el heap un casillero con la clave y el elemento pasados por parámetro.
 * Devuelve un puntero al casillero creado.
 */
casillero_t* hash_crear_casillero(const char* clave, void* elemento) {
  casillero_t* casillero = calloc(1, sizeof(casillero_t));
  if (!casillero) return NULL;
  casillero->clave = malloc(strlen(clave) + 1);
  if (!casillero) {
    free(casillero);
    return NULL;
  }
  casillero->elemento = elemento;
  strcpy(casillero->clave, clave);
  casillero->siguiente = NULL;

  return casillero;
}

/*
 * se encarga de almacenar en el casillero la clave y elementos recibidos.
 * En caso de que el casillero esta vacio, se crea un nuevo casillero y se almacena en el.
 * En caso de que el casillero recibido ya contenga un elemento con la misma clave, se 
 *  destruye el elemento y se reemplaza.
 * En caso de que el casillero recibido contenga un elemento con una clave distinta, se 
 *  verifica recursivamente con el casillero siguiente al recibido.
 */

casillero_t* hash_agregar_casillero(hash_t* hash, casillero_t* casillero,
                                    const char* clave, void* elemento) {
  if (!casillero) {
    casillero = hash_crear_casillero(clave, elemento);
    hash->cantidad++;
    return casillero;
  }

  if (strcmp(casillero->clave, clave) == 0) {
    if (hash->destructor_elemento) hash->destructor_elemento(casillero->elemento);
    casillero->elemento = elemento;
    return casillero;
  }

  casillero->siguiente = hash_agregar_casillero(hash, casillero->siguiente, clave, elemento);

  return casillero;
}

/*
 * Se encarga de liberar la memoria de un casillero, incluyendo la clave y el elemento
 * (mediante el destructor recibido por parametro proveniente del hash).
 */
void hash_eliminar_casillero(casillero_t* casillero, hash_destruir_dato_t destruir_elemento) {
  if (!casillero) return;

  if (destruir_elemento) destruir_elemento(casillero->elemento);
  free(casillero->clave);
  free(casillero);
}

/*
 * Se encarga de crear una tabla del doble de capacidad que recibe por parametro.
 * Luego recorre los elementos de la tabla anterior para almacenarlos en la nueva tabla
 * con sus nuevas posiciones debido al incremento de tamanio, y luego se destruye la
 * tabla anterior. 
 * Reemplaza la tabla anterior por la nueva. 
*/
void hash_funcion_rehash(hash_t* hash) {
  size_t capacidad_nueva = hash->capacidad * 2;
  casillero_t** casilleros_nuevos = calloc(capacidad_nueva, sizeof(casillero_t*));
  if (!casilleros_nuevos) return;

  hash->cantidad = 0;
  for (size_t i = 0; i < hash->capacidad; i++) {
    casillero_t* casillero_actual = hash->casilleros[i];
    while (casillero_actual) {
      size_t casillero_nuevo = hash_funcion_hash(casillero_actual->clave, capacidad_nueva);
      casilleros_nuevos[casillero_nuevo] =
          hash_agregar_casillero(hash, casilleros_nuevos[casillero_nuevo], casillero_actual->clave,
                                 casillero_actual->elemento);

      casillero_t* casillero_siguiente = casillero_actual->siguiente;
      hash_eliminar_casillero(casillero_actual, NULL);
      casillero_actual = casillero_siguiente;
    }
  }

  free(hash->casilleros);
  hash->casilleros = casilleros_nuevos;
  hash->capacidad = capacidad_nueva;
}

/*
 * Crea el hash reservando la memoria necesaria para el.
 * Destruir_elemento es un destructor que se utilizará para liberar
 * los elementos que se eliminen del hash.  Capacidad indica la
 * capacidad inicial con la que se crea el hash. La capacidad inicial
 * no puede ser menor a 3. Si se solicita una capacidad menor, el hash
 * se creará con una capacidad de 3.
 *
 * Devuelve un puntero al hash creado o NULL en caso de no poder
 * crearlo.
 */
hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad_inicial) {
  hash_t* hash = malloc(sizeof(hash_t));
  if (!hash) return NULL;

  if (capacidad_inicial < 3) capacidad_inicial = 3;

  hash->casilleros = calloc(capacidad_inicial, sizeof(casillero_t*));
  if (!hash->casilleros) {
    free(hash);
    return NULL;
  }

  hash->capacidad = capacidad_inicial;
  hash->cantidad = 0;
  hash->destructor_elemento = destruir_elemento;
  return hash;
}

/*
 * Inserta un elemento en el hash asociado a la clave dada.
 *
 * Nota para los alumnos: Recordar que si insertar un elemento provoca
 * que el factor de carga exceda cierto umbral, se debe ajustar el
 * tamaño de la tabla para evitar futuras colisiones.
 *
 * Devuelve 0 si pudo guardarlo o -1 si no pudo.
 */
int hash_insertar(hash_t* hash, const char* clave, void* elemento) {
  if (!hash || !clave) return -1;

  size_t posicion_casillero = hash_funcion_hash(clave, hash->capacidad);

  casillero_t* casillero_auxiliar =
      hash_agregar_casillero(hash, hash->casilleros[posicion_casillero], clave, elemento);
  if (!casillero_auxiliar) return -1;
  hash->casilleros[posicion_casillero] = casillero_auxiliar;

  if (hash->cantidad > hash->capacidad * 0.75) hash_funcion_rehash(hash);
  return 0;
}

/*
 * Quita un elemento del hash e invoca la funcion destructora
 * pasandole dicho elemento.
 * Devuelve 0 si pudo eliminar el elemento o -1 si no pudo.
 */
int hash_quitar(hash_t* hash, const char* clave) {
  if (!hash || !clave) return -1;

  size_t posicion_casillero = hash_funcion_hash(clave, hash->capacidad);
  casillero_t* casillero = hash->casilleros[posicion_casillero];
  if (!casillero) return -1;

  if (strcmp(casillero->clave, clave) == 0) {
    hash->casilleros[posicion_casillero] = casillero->siguiente;
    hash_eliminar_casillero(casillero, hash->destructor_elemento);
    hash->cantidad--;
    return 0;
  }

  casillero_t* anterior = casillero;
  casillero = casillero->siguiente;
  while (casillero) {
    if (strcmp(casillero->clave, clave) == 0) {
      anterior->siguiente = casillero->siguiente;
      hash_eliminar_casillero(casillero, hash->destructor_elemento);
      hash->cantidad--;
      return 0;
    }

    anterior = casillero;
    casillero = casillero->siguiente;
  }

  return -1;
}

/*
 * Devuelve un elemento del hash con la clave dada o NULL si dicho
 * elemento no existe (o en caso de error).
 */
void* hash_obtener(hash_t* hash, const char* clave) {
  if (!hash || !clave) return NULL;

  size_t posicion_casillero = hash_funcion_hash(clave, hash->capacidad);

  casillero_t* casillero = hash->casilleros[posicion_casillero];
  while (casillero) {
    if (strcmp(casillero->clave, clave) == 0) {
      return casillero->elemento;
    }
    casillero = casillero->siguiente;
  }

  return NULL;
}

/*
 * Devuelve true si el hash contiene un elemento almacenado con la
 * clave dada o false en caso contrario (o en caso de error).
 */
bool hash_contiene(hash_t* hash, const char* clave) {
  if (hash_obtener(hash, clave)) return true;
  return false;
}

/*
 * Devuelve la cantidad de elementos almacenados en el hash o 0 en
 * caso de error.
 */
size_t hash_cantidad(hash_t* hash) {
  if (!hash) return 0;
  return hash->cantidad;
}

/*
 * Destruye el hash liberando la memoria reservada y asegurandose de
 * invocar la funcion destructora con cada elemento almacenado en el
 * hash.
 */
void hash_destruir(hash_t* hash) {
  if (!hash) return;

  for (size_t i = 0; i < hash->capacidad; i++) {
    casillero_t* casillero = hash->casilleros[i];
    while (casillero) {
      casillero_t* siguiente = casillero->siguiente;
      hash_eliminar_casillero(casillero, hash->destructor_elemento);
      casillero = siguiente;
    }
  }

  free(hash->casilleros);
  free(hash);
}

/*
 * Recorre cada una de las claves almacenadas en la tabla de hash e
 * invoca a la función funcion, pasandole como parámetros el hash, la
 * clave en cuestión y el puntero auxiliar.
 *
 * Mientras que queden mas claves o la funcion retorne false, la
 * iteración continúa. Cuando no quedan mas claves o la función
 * devuelve true, la iteración se corta y la función principal
 * retorna.
 *
 * Devuelve la cantidad de claves totales iteradas (la cantidad de
 * veces que fue invocada la función) o 0 en caso de error.
 *
 */
size_t hash_con_cada_clave(hash_t* hash,
                           bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux) {
  if (!hash || !funcion) return 0;

  size_t cantidad_claves = 0;
  for (size_t i = 0; i < hash->capacidad; i++) {
    casillero_t* casillero = hash->casilleros[i];
    while (casillero) {
      cantidad_claves++;
      if (funcion(hash, casillero->clave, aux)) return cantidad_claves;
      casillero = casillero->siguiente;
    }
  }

  return cantidad_claves;
}