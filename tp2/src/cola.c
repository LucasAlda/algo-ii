#include "cola.h"

#include "lista.h"

/**
 * Crea una cola vacía y la devuelve.
 *
 * En caso de error devuelve NULL
 */
cola_t* cola_crear() {
  return (cola_t*)lista_crear();
}

/**
 * Encola un elemento en la cola.
 *
 * Devuelve la cola o NULL en caso de error.
 */
cola_t* cola_encolar(cola_t* cola, void* elemento) {
  return (cola_t*)lista_insertar((lista_t*)cola, elemento);
}

/**
 * Desencola un elemento de la cola y lo devuelve.
 *
 * Devuelve NULL en caso de error.
 */
void* cola_desencolar(cola_t* cola) {
  return lista_quitar_de_posicion((lista_t*)cola, 0);
}

/**
 * Devuelve el elemento en el frente de la cola o NULL en caso de que no exista.
 */
void* cola_frente(cola_t* cola) {
  return lista_primero((lista_t*)cola);
}

/**
 * Devuelve la cantidad de elementos de la cola o 0 si no existe.
 */
size_t cola_tamanio(cola_t* cola) {
  return lista_tamanio((lista_t*)cola);
}

/**
 * Devuelve true si la cola tiene 0 elementos.
 */
bool cola_vacia(cola_t* cola) {
  return lista_vacia((lista_t*)cola);
}

/**
 * Libera la memoria reservada por la cola.
 */
void cola_destruir(cola_t* cola) {
  lista_destruir((lista_t*)cola);
}
