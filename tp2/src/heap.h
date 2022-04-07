#include <stdlib.h>

#ifndef HEAP_H_
#define HEAP_H_

typedef int (*heap_comparar_func_t)(const void*, const void*);

typedef struct heap heap_t;

/*
 * Crea un heap vacio con el comparador recibido y capacidad 10
 *
 * Si no se pudo crear o el comparador es nulo devuelve NULL. 
*/
heap_t* heap_crear(heap_comparar_func_t comparador);

/*
 * Inserta al final el elemento recibido y luego utiliza sift_up para acomodar este elemento
 * en el heap
 * 
 * Si la capacidad del heap es insuficiente, se duplica su capacidad 
 * 
 * Devuelve NULL si el heap o puntero a elemento son nulos o hubo errores de memoria
*/
heap_t* heap_insertar(heap_t* heap, void* elemento);

/*
 * Extrae la raiz del heap, coloca en la raiz el ultimo elemento y luego utiliza sift_down para acomodar
 * el elemento en la posicion 0
 *
 * Devuelve NULL si el heap es nulo o no tiene elementos
*/
void* heap_extraer_raiz(heap_t* heap);

/*
 * Devuelve la cantidad de elementos en el heap o 0 si el heap es nulo
*/
size_t heap_cantidad(heap_t* heap);

/*
* Libera de memoria el heap sin sus elementos
*/
void heap_destruir(heap_t* heap);

#endif  // HEAP_H_
