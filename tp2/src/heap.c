#include "heap.h"

#include <stdio.h>
#include <stdlib.h>

#include "hospital.h"

typedef int (*heap_comparar_func_t)(const void*, const void*);

struct heap {
  void** elementos;
  size_t cantidad;
  size_t capacidad;
  heap_comparar_func_t comparador;
};

/*
 * Crea un heap vacio con el comparador recibido y capacidad 10
 *
 * Si no se pudo crear o el comparador es nulo devuelve NULL. 
*/
heap_t* heap_crear(heap_comparar_func_t comparador) {
  if (!comparador) return NULL;
  heap_t* heap = malloc(sizeof(heap_t));
  if (!heap) return NULL;
  heap->elementos = malloc(sizeof(void*) * 10);
  if (!heap->elementos) {
    free(heap);
    return NULL;
  }
  heap->cantidad = 0;
  heap->capacidad = 10;
  heap->comparador = comparador;
  return heap;
}

/*
 * Utiliza la funcion de comparacion del heap para verificar si el elemento en posicion
 * actual es menor al padre y de ser asi, intercambia los elementos y verifica esa posicion
 * recursivamente 
*/
void sift_up(void** vector, heap_comparar_func_t comparador, int pos_actual) {
  if (pos_actual <= 0) return;

  int pos_padre = (pos_actual - 1) / 2;
  void* actual = vector[pos_actual];
  void* padre = vector[pos_padre];

  if (comparador(actual, padre) < 0) {
    vector[pos_padre] = actual;
    vector[pos_actual] = padre;
    sift_up(vector, comparador, pos_padre);
  }
}

/*
 * Inserta al final el elemento recibido y luego utiliza sift_up para acomodar este elemento
 * en el heap
 * 
 * Si la capacidad del heap es insuficiente, se duplica su capacidad 
 * 
 * Devuelve NULL si el heap o puntero a elemento son nulos o hubo errores de memoria
*/
heap_t* heap_insertar(heap_t* heap, void* elemento) {
  if (!heap || !elemento) return NULL;

  if (heap->cantidad == heap->capacidad) {
    void** elementos_aux = realloc(heap->elementos, sizeof(void*) * heap->capacidad * 2);
    if (!elementos_aux) return NULL;

    heap->capacidad *= 2;
    heap->elementos = elementos_aux;
  }

  heap->elementos[heap->cantidad] = elemento;
  sift_up(heap->elementos, heap->comparador, (int)heap->cantidad);
  heap->cantidad++;

  return heap;
}

/*
 * Utiliza la funcino de comparacion para verificar si alguno de los hijos del elemento en posicion
 * son menores que el elemento en posicion actual y de ser asi, intercambia los elementos y verifica
 * esa posicion recursivamente
*/
void sift_down(void** vector, heap_comparar_func_t comparador, int tope, int pos_actual) {
  int pos_izquierda = pos_actual * 2 + 1;
  int pos_derecha = pos_actual * 2 + 2;

  if (pos_izquierda > tope) return;

  if (pos_derecha > tope)
    pos_derecha = pos_izquierda;

  int pos_nueva = comparador(vector[pos_derecha], vector[pos_izquierda]) < 0 ? pos_derecha : pos_izquierda;

  if (comparador(vector[pos_nueva], vector[pos_actual]) < 0) {
    void* aux = vector[pos_actual];
    vector[pos_actual] = vector[pos_nueva];
    vector[pos_nueva] = aux;
    sift_down(vector, comparador, tope, pos_nueva);
  }
}

/*
 * Extrae la raiz del heap, coloca en la raiz el ultimo elemento y luego utiliza sift_down para acomodar
 * el elemento en la posicion 0
 *
 * Devuelve NULL si el heap es nulo o no tiene elementos
*/
void* heap_extraer_raiz(heap_t* heap) {
  if (!heap || heap->cantidad == 0) return NULL;

  void* extraido = heap->elementos[0];
  heap->elementos[0] = heap->elementos[heap->cantidad - 1];
  sift_down(heap->elementos, heap->comparador, (int)heap->cantidad - 1, 0);
  heap->cantidad -= 1;

  return extraido;
}

/*
 * Devuelve la cantidad de elementos en el heap o 0 si el heap es nulo
*/
size_t heap_cantidad(heap_t* heap) {
  if (!heap) return 0;
  return heap->cantidad;
}

/*
* Libera de memoria el heap sin sus elementos
*/
void heap_destruir(heap_t* heap) {
  if (!heap) return;
  free(heap->elementos);
  free(heap);
}
