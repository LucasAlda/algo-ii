#include "lista.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 *  Busca el nodo en la posicion indicada.
 *  
 *  Devuelve el puntero al nodo o NULL en caso de no existir la lista
 *  o la posicion ser mayor al tamanio.
 */
nodo_t* lista_nodo_en_posicion(lista_t* lista, size_t posicion) {
  if (!lista || posicion > lista_tamanio(lista)) return NULL;

  size_t posicion_actual = 0;
  nodo_t* nodo_actual = lista->nodo_inicio;

  while (nodo_actual && posicion_actual < posicion) {
    nodo_actual = nodo_actual->siguiente;
    posicion_actual++;
  }

  return nodo_actual;
}

/**
 *  Crea un nodo reservando la memoria necesaria e inserta un elemento.
 *  Devuelve un puntero al nodo o NULL en caso de error.
 */
nodo_t* lista_crear_nodo(void* elemento) {
  nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
  if (!nuevo_nodo) return NULL;
  nuevo_nodo->elemento = elemento;
  nuevo_nodo->siguiente = NULL;
  return nuevo_nodo;
}

/**
 * Crea la lista reservando la memoria necesaria.
 * Devuelve un puntero a la lista creada o NULL en caso de error.
 */
lista_t* lista_crear() {
  return calloc(1, sizeof(lista_t));
}

/**
 * Inserta un elemento al final de la lista.
 *
 * Devuelve NULL si no pudo insertar el elemento a causa de un error, o la lista en caso de exito.
 */
lista_t* lista_insertar(lista_t* lista, void* elemento) {
  if (!lista) return NULL;
  nodo_t* nuevo_nodo = lista_crear_nodo(elemento);
  if (!nuevo_nodo) return NULL;

  if (lista_vacia(lista)) {
    lista->nodo_fin = nuevo_nodo;
    lista->nodo_inicio = nuevo_nodo;
  } else {
    lista->nodo_fin->siguiente = nuevo_nodo;
    lista->nodo_fin = nuevo_nodo;
  }
  lista->cantidad += 1;
  return lista;
}

/**
 * Inserta un elemento en la posicion indicada, donde 0 es insertar
 * como primer elemento y 1 es insertar luego del primer elemento.  
 * En caso de no existir la posicion indicada, lo inserta al final.
 *
 * Devuelve NULL si no pudo insertar el elemento a causa de un error, o la lista en caso de exito.
 */
lista_t* lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion) {
  if (!lista) return NULL;
  if (posicion >= lista_tamanio(lista)) return lista_insertar(lista, elemento);

  nodo_t* nuevo_nodo = lista_crear_nodo(elemento);
  if (!nuevo_nodo) return NULL;

  if (posicion > 0) {
    nodo_t* nodo_anterior = lista_nodo_en_posicion(lista, posicion - 1);
    nuevo_nodo->siguiente = nodo_anterior->siguiente;
    nodo_anterior->siguiente = nuevo_nodo;
  } else {
    nuevo_nodo->siguiente = lista->nodo_inicio;
    lista->nodo_inicio = nuevo_nodo;
  }

  lista->cantidad += 1;

  return lista;
}

/**
 * Quita de la lista el elemento que se encuentra en la ultima posición.
 *
 * Devuelve el elemento removido de la lista o NULL en caso de error.
 */
void* lista_quitar(lista_t* lista) {
  return lista_quitar_de_posicion(lista, lista_tamanio(lista) - 1);
}

/**
 * Quita de la lista el elemento que se encuentra en la posición
 * indicada, donde 0 es el primer elemento de la lista.
 *
 * En caso de no existir esa posición se intentará borrar el último
 * elemento.
 *
 * Devuelve el elemento removido de la lista o NULL en caso de error.
 *
 */
void* lista_quitar_de_posicion(lista_t* lista, size_t posicion) {
  size_t tamanio = lista_tamanio(lista);
  void* elemento_quitado = NULL;
  if (lista_vacia(lista)) return NULL;

  if (posicion > tamanio - 1) posicion = tamanio - 1;
  if (posicion > 0) {
    nodo_t* nodo_anterior = lista_nodo_en_posicion(lista, posicion - 1);
    nodo_t* nodo_a_borrar = nodo_anterior->siguiente;
    nodo_t* nodo_siguiente = nodo_a_borrar->siguiente;

    nodo_anterior->siguiente = nodo_siguiente;
    if (nodo_a_borrar) {
      elemento_quitado = nodo_a_borrar->elemento;
      free(nodo_a_borrar);
    }
    if (posicion == tamanio - 1) lista->nodo_fin = nodo_anterior;
  } else {
    nodo_t* nodo_a_borrar = lista->nodo_inicio;
    lista->nodo_inicio = nodo_a_borrar->siguiente;
    if (nodo_a_borrar) {
      elemento_quitado = nodo_a_borrar->elemento;
      free(nodo_a_borrar);
    }
    if (tamanio == 1) lista->nodo_fin = lista->nodo_inicio;
  }

  lista->cantidad -= 1;

  return elemento_quitado;
}

/**
 * Devuelve el elemento en la posicion indicada, donde 0 es el primer
 * elemento.
 *
 * Si no existe dicha posicion devuelve NULL.
 */
void* lista_elemento_en_posicion(lista_t* lista, size_t posicion) {
  nodo_t* nodo_en_posicion = lista_nodo_en_posicion(lista, posicion);
  if (!nodo_en_posicion) return NULL;
  return nodo_en_posicion->elemento;
}

/**
 * Devuelve el primer elemento de la lista o NULL si la lista se
 * encuentra vacía o no existe.
 */
void* lista_primero(lista_t* lista) {
  if (!lista || lista_vacia(lista)) return NULL;
  return lista->nodo_inicio->elemento;
}

/**
 * Devuelve el último elemento de la lista o NULL si la lista se
 * encuentra vacía o no existe.
 */
void* lista_ultimo(lista_t* lista) {
  if (!lista || lista_vacia(lista)) return NULL;
  return lista->nodo_fin->elemento;
}

/**
 * Devuelve true si la lista está vacía (o no existe) o false en caso contrario.
 */
bool lista_vacia(lista_t* lista) {
  return !lista || lista->cantidad == 0;
}

/**
 * Devuelve la cantidad de elementos almacenados en la lista.
 * Una lista que no existe no puede tener elementos.
 */
size_t lista_tamanio(lista_t* lista) {
  if (!lista) return 0;
  return lista->cantidad;
}

/**
 * Libera la memoria reservada por la lista.
 */
void lista_destruir(lista_t* lista) {
  if (!lista) return;
  nodo_t* nodo_actual = lista->nodo_inicio;
  while (nodo_actual && nodo_actual != lista->nodo_fin) {
    nodo_t* siguiente_nodo = nodo_actual->siguiente;
    free(nodo_actual);
    nodo_actual = siguiente_nodo;
  }
  free(nodo_actual);
  free(lista);
}

/**
 * Crea un iterador para una lista. El iterador creado es válido desde
 * el momento de su creación hasta que no haya mas elementos por
 * recorrer o se modifique la lista iterada (agregando o quitando
 * elementos de la lista). 
 *
 * Al momento de la creación, el iterador queda listo para devolver el
 * primer elemento utilizando lista_iterador_elemento_actual.
 *
 * Devuelve el puntero al iterador creado o NULL en caso de error.
 */
lista_iterador_t* lista_iterador_crear(lista_t* lista) {
  if (!lista) return NULL;

  lista_iterador_t* nuevo_iterador = malloc(sizeof(lista_iterador_t));
  if (!nuevo_iterador) return NULL;

  nuevo_iterador->lista = lista;
  nuevo_iterador->corriente = lista->nodo_inicio;
  return nuevo_iterador;
}

/**
 * Devuelve true si hay mas elementos sobre los cuales iterar o false
 * si no hay mas.
 */
bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador) {
  if (!iterador) return NULL;
  return iterador->corriente != NULL;
}

/**
 * Avanza el iterador al siguiente elemento.
 * Devuelve true si pudo avanzar el iterador o false en caso de
 * que no queden elementos o en caso de error.
 *
 * Una vez llegado al último elemento, si se invoca a
 * lista_iterador_elemento_actual, el resultado siempre será NULL.
 */
bool lista_iterador_avanzar(lista_iterador_t* iterador) {
  if (!iterador || !(iterador->corriente)) return false;
  iterador->corriente = iterador->corriente->siguiente;
  return iterador->corriente != NULL;
}

/**
 * Devuelve el elemento actual del iterador o NULL en caso de que no
 * exista dicho elemento o en caso de error.
 */
void* lista_iterador_elemento_actual(lista_iterador_t* iterador) {
  if (!iterador || !(iterador->corriente)) return NULL;
  return iterador->corriente->elemento;
}

/**
 * Libera la memoria reservada por el iterador.
 */
void lista_iterador_destruir(lista_iterador_t* iterador) {
  free(iterador);
}

/**
 * Iterador interno. Recorre la lista e invoca la funcion con cada elemento de
 * la misma como primer parámetro. Dicha función puede devolver true si se deben
 * seguir recorriendo elementos o false si se debe dejar de iterar elementos.
 *
 * El puntero contexto se pasa como segundo argumento a la función del usuario.
 *
 * La función devuelve la cantidad de elementos iterados o 0 en caso de error.
 */
size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void* contexto) {
  if (!lista || !funcion) return 0;

  size_t posicion_actual = 0;
  nodo_t* nodo_actual = lista->nodo_inicio;

  while (nodo_actual && posicion_actual < lista->cantidad) {
    if (!funcion(nodo_actual->elemento, contexto)) return posicion_actual;
    nodo_actual = nodo_actual->siguiente;
    posicion_actual++;
  }

  return lista->cantidad;
}