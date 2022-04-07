#include <stdio.h>
#include <stdlib.h>

#include "pa2mm.h"
#include "src/cola.h"
#include "src/lista.h"
#include "src/pila.h"

void prueba_lista_crear_y_destruir() {
  pa2m_nuevo_grupo("Pruebas crear y destruir lista");
  lista_t* lista = lista_crear();
  pa2m_afirmar((lista != NULL), "Crear lista devuelve una lista");
  pa2m_afirmar((lista_tamanio(lista) == 0), "Lista vacia tiene 0 elementos");
  pa2m_afirmar((lista_primero(lista) == NULL), "Primer elemento nulo");
  pa2m_afirmar((lista_ultimo(lista) == NULL), "Ultimo elemento nulo");
  pa2m_afirmar((lista_vacia(lista)), "La Lista esta vacia");
  lista_destruir(lista);
}

void prueba_lista_insertar_elementos() {
  pa2m_nuevo_grupo("Pruebas agregar elementos a lista");
  lista_t* lista = lista_crear();
  char a = 'a', b = 'b', c = 'c';
  pa2m_afirmar((lista_insertar(lista, &a) != NULL), "Insertar elemento A en la lista");
  pa2m_afirmar((lista_insertar(lista, &b) != NULL), "Insertar segundo elemento B");
  pa2m_afirmar((lista_insertar(lista, &c) != NULL), "Insertar tercer elemento C");
  pa2m_afirmar((lista_tamanio(lista) == 3), "Lista tiene 3 elementos");
  pa2m_afirmar((lista_primero(lista) == &a && lista_ultimo(lista) == &c), "Primer elemento es el A y ultimo C");
  pa2m_afirmar((!lista_vacia(lista)), "La Lista no esta vacia");

  lista_destruir(lista);
}

void prueba_lista_obtener_elementos() {
  pa2m_nuevo_grupo("Pruebas obtener elementos de la lista");
  lista_t* lista = lista_crear();
  char a = 'a', b = 'b', c = 'c';
  pa2m_afirmar((lista_insertar(lista, &a) && lista_insertar(lista, &b) && lista_insertar(lista, &c)), "Nueva lista con tres elementos (A, B, C)");
  pa2m_afirmar((lista_elemento_en_posicion(lista, 0) == &a), "Elemento posicion 0 (primero) es A");
  pa2m_afirmar((lista_elemento_en_posicion(lista, 1) == &b), "Elemento posicion 1 (segundo) es B");
  pa2m_afirmar((lista_elemento_en_posicion(lista, 2) == &c), "Elemento posicion 2 (tercero) es C");
  pa2m_afirmar((lista_elemento_en_posicion(lista, 6) == NULL), "Elemento posicion 6 (septimo) es NULL");
  lista_destruir(lista);
}

void prueba_lista_insertar_elementos_en_posicion() {
  pa2m_nuevo_grupo("Pruebas insertar elementos en posiciones especificas");
  lista_t* lista = lista_crear();
  char a = 'a', b = 'b', c = 'c', d = 'd', e = 'e', f = 'f', g = 'g', h = 'h';
  pa2m_afirmar((lista_insertar(lista, &a) && lista_insertar(lista, &b) && lista_insertar(lista, &c)), "Nueva lista con tres elementos (A, B, C)");
  pa2m_afirmar((lista_insertar_en_posicion(lista, &d, 0)), "Insertar en el principio");
  pa2m_afirmar((lista_primero(lista) == &d && lista_elemento_en_posicion(lista, 1) == &a), "Primer elemento es D \n   y segundo elemento es A");
  pa2m_afirmar((lista_insertar_en_posicion(lista, &e, 2)), "Insertar en el medio (tercero)");
  pa2m_afirmar((lista_elemento_en_posicion(lista, 2) == &e && lista_elemento_en_posicion(lista, 3) == &b), "Tercer elemento es E\n   y cuarto elemento es B");
  pa2m_afirmar((lista_insertar_en_posicion(lista, &f, 4)), "Insertar en el ultimo usado (quinto)");
  pa2m_afirmar((lista_elemento_en_posicion(lista, 4) == &f && lista_elemento_en_posicion(lista, 5) == &c), "Quinto elemento es F\n   y sexto elemento es C");
  pa2m_afirmar((lista_insertar_en_posicion(lista, &g, 6)), "Insertar despues del ultimo con lista_insertar_en_posicion");
  pa2m_afirmar((lista_ultimo(lista) == &g), "Ultimo es G");
  pa2m_afirmar((lista_insertar_en_posicion(lista, &h, 2000)), "Insertar el el lugar 2000 con lista_insertar_en_posicion");
  pa2m_afirmar((lista_ultimo(lista) == &h), "Ultimo es H");
  pa2m_afirmar((lista_tamanio(lista) == 8), "Lista tiene 8 elementos");
  lista_destruir(lista);
}

void prueba_lista_quitar_elementos() {
  pa2m_nuevo_grupo("Pruebas quitar elementos de la lista");
  lista_t* lista = lista_crear();
  char a = 'a', b = 'b', c = 'c', d = 'd', e = 'e';
  pa2m_afirmar((lista_insertar(lista, &a) && lista_insertar(lista, &b) && lista_insertar(lista, &c) && lista_insertar(lista, &d) && lista_insertar(lista, &e)), "Nueva lista con cinco elementos (A, B, C, D, E)");
  pa2m_afirmar((lista_quitar_de_posicion(lista, 2) == &c), "Eliminar elemento posicion 2 (C)");
  pa2m_afirmar((lista_elemento_en_posicion(lista, 2) == &d && lista_tamanio(lista) == 4), "Elemento posicion 2 (tercero) es D y cantidad ahora es 4");
  pa2m_afirmar((lista_quitar_de_posicion(lista, 0) == &a), "Eliminar elemento posicion 0 (inicio)");
  pa2m_afirmar((lista_elemento_en_posicion(lista, 0) == &b && lista_primero(lista) == &b && lista_tamanio(lista) == 3), "Elemento posicion 0 (primero) es B y cantidad es 3");
  pa2m_afirmar((lista_quitar(lista) == &e), "Eliminar ultimo elemento (E)");
  pa2m_afirmar((lista_ultimo(lista) == &d && lista_tamanio(lista) == 2), "Ultimo elemento es D y cantidad es 2");
  pa2m_afirmar((lista_quitar_de_posicion(lista, 2000) == &d), "Eliminar elemento posicion 2000 elimina el ultimo (D)");
  pa2m_afirmar((lista_primero(lista) == &b && lista_ultimo(lista) == &b && lista_tamanio(lista) == 1), "Unico elemento es B");
  pa2m_afirmar((lista_quitar_de_posicion(lista, 2000) == &b), "Eliminar unico elemento (B) con posicion mayor a 0");
  pa2m_afirmar((lista_ultimo(lista) == NULL && lista_ultimo(lista) == NULL && lista_tamanio(lista) == 0), "Lista vacia");
  lista_insertar(lista, &a);
  pa2m_afirmar((lista_quitar_de_posicion(lista, 0) == &a), "Agrega un elemento y elimina unico elemento (A) con posicion 0");
  pa2m_afirmar((lista_ultimo(lista) == NULL && lista_ultimo(lista) == NULL && lista_tamanio(lista) == 0), "Lista vacia");
  lista_insertar(lista, &a);
  pa2m_afirmar((lista_quitar(lista) == &a), "Agrega un elemento y elimina unico elemento (A) con lista_quitar");
  pa2m_afirmar((lista_ultimo(lista) == NULL && lista_ultimo(lista) == NULL && lista_tamanio(lista) == 0), "Lista vacia");

  lista_destruir(lista);
}

bool sumar_nuevo_elemento(void* elemento, void* resultado_actual) {
  *((int*)resultado_actual) += *((int*)elemento);
  return true;
}

bool sumar_nuevo_elemento_menor_tres(void* elemento, void* resultado_actual) {
  if (*((int*)elemento) >= 3) return false;
  *((int*)resultado_actual) += *((int*)elemento);
  return true;
}

void prueba_lista_iterador_externo_en_for(lista_t* lista) {
  size_t i = 0;
  lista_iterador_t* it = NULL;
  for (it = lista_iterador_crear(lista);
       lista_iterador_tiene_siguiente(it);
       lista_iterador_avanzar(it)) {
    pa2m_afirmar((*(int*)lista_iterador_elemento_actual(it) == *(int*)lista_elemento_en_posicion(lista, i)), "El elemento dado por el iterador_externo es correcto");
    i++;
  }

  pa2m_afirmar((i == lista_tamanio(lista)), "Iterador externo recorre correctamente todos los elementos");
  pa2m_afirmar((lista_iterador_avanzar(it) == false), "lista_iterador_avanzar devuelve false cuando ya llego al final");
  pa2m_afirmar((lista_iterador_elemento_actual(it) == NULL), "lista_iterador_elemento_actual devuelve NULL cuando ya paso el final");
  lista_iterador_destruir(it);
}

void prueba_lista_iterador_externo() {
  pa2m_nuevo_grupo("Pruebas de iterador externo");
  lista_t* lista = lista_crear();
  int* numeros = malloc(5 * sizeof(int));
  if (!numeros) return;
  numeros[0] = 0, numeros[1] = 1, numeros[2] = 2;

  pa2m_afirmar((lista_insertar(lista, &(numeros[0])) && lista_insertar(lista, &(numeros[1])) && lista_insertar(lista, &(numeros[2]))), "Nueva lista con cinco elementos (0,1,2)");

  lista_iterador_t* it = NULL;
  pa2m_afirmar((it = lista_iterador_crear(lista)), "Crear iterador externo de una lista");
  pa2m_afirmar(lista_iterador_tiene_siguiente(it), "Lista tiene primer elemento");
  pa2m_afirmar((*(int*)lista_iterador_elemento_actual(it) == numeros[0]), "El elemento 1 es correcto");
  pa2m_afirmar(lista_iterador_avanzar(it), "Pudo avanzar al siguiente (segundo)");
  pa2m_afirmar(lista_iterador_tiene_siguiente(it), "Lista tiene segundo elemento");
  pa2m_afirmar((*(int*)lista_iterador_elemento_actual(it) == numeros[1]), "El elemento 2 es correcto");
  pa2m_afirmar(lista_iterador_avanzar(it), "Pudo avanzar al siguiente (tercero)");
  pa2m_afirmar(lista_iterador_tiene_siguiente(it), "Lista tiene segundo elemento");
  pa2m_afirmar((*(int*)lista_iterador_elemento_actual(it) == numeros[2]), "El elemento 2 es correcto");
  pa2m_afirmar((lista_iterador_avanzar(it) == false), "Avanzar al cuarto elemento devuelve false por no haber mas");
  pa2m_afirmar((lista_iterador_tiene_siguiente(it) == false), "Lista no tiene mas elementos");
  pa2m_afirmar((lista_iterador_elemento_actual(it) == NULL), "El elemento actual es NULL");

  lista_iterador_destruir(it);

  pa2m_nuevo_grupo("Pruebas de iterador externo en for");
  prueba_lista_iterador_externo_en_for(lista);

  free(numeros);
  lista_destruir(lista);
}

void prueba_lista_iterador_interno() {
  pa2m_nuevo_grupo("Pruebas de iterador interno");
  lista_t* lista = lista_crear();
  int* numeros = malloc(5 * sizeof(int));
  if (!numeros) return;
  numeros[0] = 0, numeros[1] = 1, numeros[2] = 2, numeros[3] = 3, numeros[4] = 4;

  pa2m_afirmar((lista_insertar(lista, &(numeros[0])) && lista_insertar(lista, &(numeros[1])) && lista_insertar(lista, &(numeros[2])) && lista_insertar(lista, &(numeros[3])) && lista_insertar(lista, &(numeros[4]))), "Nueva lista con cinco elementos (1,2,3,4,5)");

  int sumatoria = 0;
  size_t elementos_recorridos = 0;

  pa2m_afirmar((elementos_recorridos = lista_con_cada_elemento(lista, sumar_nuevo_elemento, &sumatoria)), "Recorrer la lista sumando los valores");
  pa2m_afirmar((elementos_recorridos == 5), "Los elementos recorridos son 5");
  pa2m_afirmar((sumatoria == 10), "La sumatoria de los valores es 10");
  sumatoria = 0;
  pa2m_afirmar((elementos_recorridos = lista_con_cada_elemento(lista, sumar_nuevo_elemento_menor_tres, &sumatoria)), "Recorrer la lista sumando los valores");
  pa2m_afirmar((elementos_recorridos == 3), "Los elementos recorridos son 3");
  pa2m_afirmar((sumatoria == 3), "La sumatoria de los valores es 3");

  free(numeros);
  lista_destruir(lista);
}

void prueba_lista_manejo_de_nulls() {
  pa2m_nuevo_grupo("Pruebas manejo de Nulls");
  lista_t* lista = lista_crear();
  char a = 'a';
  pa2m_afirmar((lista_insertar(NULL, &a) == NULL), "Insertar elemento A en una lista NULL devuelve NULL");
  pa2m_afirmar((lista_insertar(lista, NULL) != NULL), "Insertar elemento NULL en una lista");
  pa2m_afirmar((lista_tamanio(NULL) == 0), "Tamanio de lista NULL devuelve 0");
  pa2m_afirmar((lista_primero(NULL) == NULL), "Primer elemento de lista NULL devuelve NULL");
  pa2m_afirmar((lista_ultimo(NULL) == NULL), "Ultimo elemento de lista NULL devuelve NULL");
  pa2m_afirmar((lista_vacia(NULL)), "Lista NULL esta vacia");
  pa2m_afirmar((lista_elemento_en_posicion(NULL, 4) == NULL), "Elemento de Lista NULL devuelve NULL");
  pa2m_afirmar((lista_quitar_de_posicion(NULL, 4) == NULL && lista_quitar(NULL) == NULL), "Quitar de Lista NULL devuelve NULL");
  pa2m_afirmar((lista_iterador_crear(NULL) == NULL), "Crear iterador de lista nula devuelve NULL");
  pa2m_afirmar((lista_iterador_tiene_siguiente(NULL) == false), "lista_iterador_tiene_siguiente a iterador nulo devuelve false");
  pa2m_afirmar((lista_iterador_avanzar(NULL) == false), "lista_iterador_avanzar en iterador nulo devuelve false");
  pa2m_afirmar((lista_iterador_elemento_actual(NULL) == NULL), "lista_iterador_elemento_actual en iterador nulo devuelve NULL");
  pa2m_afirmar((!lista_con_cada_elemento(lista, NULL, &a)), "Pasar funcion nula a lista_con_cada_elemento devuelve NULL");
  pa2m_afirmar((!lista_con_cada_elemento(NULL, NULL, NULL)), "Pasar valores nulos a lista_con_cada_elemento devuelve NULL");
  lista_destruir(lista);
  lista_destruir(NULL);
  lista_iterador_destruir(NULL);
}

void prueba_lista_manejo_de_lista_vacia() {
  pa2m_nuevo_grupo("Pruebas manejo de lista vacia");
  lista_t* lista = lista_crear();
  pa2m_afirmar((lista_elemento_en_posicion(lista, 6) == NULL), "Elemento en 7ma posicion de lista vacia es NULL");
  pa2m_afirmar((lista_quitar_de_posicion(lista, 6) == NULL), "Eliminar elemento en 7ma posicion de lista vacia da error");
  pa2m_afirmar((lista_quitar(lista) == NULL), "Eliminar elemento en 7ma posicion de lista vacia da error");
  printf("\nFor con iterador externo en lista vacia:\n");
  prueba_lista_iterador_externo_en_for(lista);
  size_t sumatoria = 0;
  pa2m_afirmar((lista_con_cada_elemento(lista, sumar_nuevo_elemento, &sumatoria) == 0), "Recorrer la lista con iterador interno recorre 0 elementos");
  lista_destruir(lista);
}

void pruebas_lista() {
  printf(
      "\x1b[33;1m"
      "\n===========================");
  pa2m_nuevo_grupo(
      "=====  Pruebas lista  =====");
  prueba_lista_crear_y_destruir();
  prueba_lista_insertar_elementos();
  prueba_lista_obtener_elementos();
  prueba_lista_insertar_elementos_en_posicion();
  prueba_lista_quitar_elementos();
  prueba_lista_iterador_externo();
  prueba_lista_iterador_interno();
  prueba_lista_manejo_de_nulls();
  prueba_lista_manejo_de_lista_vacia();
}

void prueba_pila_crear_y_destruir() {
  pa2m_nuevo_grupo("Pruebas crear y destruir pila");
  pila_t* pila = pila_crear();
  pa2m_afirmar((pila != NULL), "Crear pila devuelve una pila");
  pa2m_afirmar((pila_tamanio(pila) == 0), "El tamanio de la pila es cero");
  pa2m_afirmar((pila_vacia(pila) == true), "La pila esta vacia");
  pa2m_afirmar((pila_tope(pila) == NULL), "Tope es NULL");
  pila_destruir(pila);
}

void prueba_pila_apilar_y_tope() {
  pa2m_nuevo_grupo("Pruebas pila apilar y ver tope");
  pila_t* pila = pila_crear();
  int a = 0, b = 1, c = 2;
  pa2m_afirmar((pila_apilar(pila, &a)), "Apilar elemento A");
  pa2m_afirmar((pila_tamanio(pila) == 1), "El tamanio de la pila es uno");
  pa2m_afirmar((pila_tope(pila) == &a), "El tope de la pila es A");
  pa2m_afirmar((pila_apilar(pila, &b)), "Apilar elemento B");
  pa2m_afirmar((pila_tamanio(pila) == 2), "El tamanio de la pila es dos");
  pa2m_afirmar((pila_tope(pila) == &b), "El tope de la pila es B");
  pa2m_afirmar((pila_apilar(pila, &c)), "Apilar elemento C");
  pa2m_afirmar((pila_tamanio(pila) == 3), "El tamanio de la pila es tres");
  pa2m_afirmar((pila_tope(pila) == &c), "El tope de la pila es C");
  pa2m_afirmar((pila_apilar(pila, NULL)), "Apilar elemento NULL");
  pa2m_afirmar((pila_tamanio(pila) == 4), "El tamanio de la pila es cuatro");
  pa2m_afirmar((pila_tope(pila) == NULL), "El tope de la cola es NULL");
  pa2m_afirmar((pila_vacia(pila) == false), "La pila no esta vacia");
  pila_destruir(pila);
}

void prueba_pila_desapilar() {
  pa2m_nuevo_grupo("Pruebas pila desapilar");
  pila_t* pila = pila_crear();
  int a = 0, b = 1, c = 2;
  pa2m_afirmar((pila_apilar(pila, &a) && pila_apilar(pila, &b) && pila_apilar(pila, &c)), "Apilar elementos A, B, C");
  pa2m_afirmar((pila_tamanio(pila) == 3), "El tamanio de la pila es tres");
  pa2m_afirmar((pila_tope(pila) == &c), "El tope de la pila es C");
  pa2m_afirmar((pila_desapilar(pila) == &c), "Desapilar devuelve elemento C");
  pa2m_afirmar((pila_tamanio(pila) == 2), "El tamanio de la pila es dos");
  pa2m_afirmar((pila_tope(pila) == &b), "El tope de la pila es B");
  pa2m_afirmar((pila_desapilar(pila) == &b), "desapilar devuelve elemento B");
  pa2m_afirmar((pila_tamanio(pila) == 1), "El tamanio de la pila es uno");
  pa2m_afirmar((pila_tope(pila) == &a), "El tope de la pila es A");
  pa2m_afirmar((pila_desapilar(pila) == &a), "desapilar devuelve elemento A");
  pa2m_afirmar((pila_tamanio(pila) == 0), "El tamanio de la pila es NULL");
  pa2m_afirmar((pila_tope(pila) == NULL), "El tope de la pila es NULL");
  pa2m_afirmar((pila_desapilar(pila) == NULL), "desapilar devuelve elemento NULL");
  pa2m_afirmar((pila_tamanio(pila) == 0), "El tamanio de la pila es NULL");
  pa2m_afirmar((pila_tope(pila) == NULL), "El tope de la pila es NULL");

  pila_destruir(pila);
}

void pruebas_pila() {
  printf(
      "\x1b[33;1m"
      "\n==========================");
  pa2m_nuevo_grupo(
      "=====  Pruebas pila  =====");
  prueba_pila_crear_y_destruir();
  prueba_pila_apilar_y_tope();
  prueba_pila_desapilar();
}

void prueba_cola_crear_y_destruir() {
  pa2m_nuevo_grupo("Pruebas crear y destruir cola");
  cola_t* cola = cola_crear();
  pa2m_afirmar((cola != NULL), "Crear cola devuelve una cola");
  pa2m_afirmar((cola_tamanio(cola) == 0), "El tamanio de la cola es cero");
  pa2m_afirmar((cola_vacia(cola) == true), "La cola esta vacia");
  pa2m_afirmar((cola_frente(cola) == NULL), "Tope es NULL");
  cola_destruir(cola);
}

void prueba_cola_encolar_y_frente() {
  pa2m_nuevo_grupo("Pruebas cola encolar y frente");
  cola_t* cola = cola_crear();
  int a = 0, b = 1, c = 2;
  pa2m_afirmar((cola_encolar(cola, &a)), "Encolar elemento A");
  pa2m_afirmar((cola_tamanio(cola) == 1), "El tamanio de la cola es uno");
  pa2m_afirmar((cola_frente(cola) == &a), "El tope de la cola es A");
  pa2m_afirmar((cola_encolar(cola, &b)), "Encolar elemento B");
  pa2m_afirmar((cola_tamanio(cola) == 2), "El tamanio de la cola es dos");
  pa2m_afirmar((cola_frente(cola) == &a), "El tope de la cola es A");
  pa2m_afirmar((cola_encolar(cola, &c)), "Encolar elemento C");
  pa2m_afirmar((cola_tamanio(cola) == 3), "El tamanio de la cola es tres");
  pa2m_afirmar((cola_frente(cola) == &a), "El tope de la cola es A");
  pa2m_afirmar((cola_encolar(cola, NULL)), "Encolar elemento NULL");
  pa2m_afirmar((cola_tamanio(cola) == 4), "El tamanio de la cola es cuatro");
  pa2m_afirmar((cola_frente(cola) == &a), "El tope de la cola es A");
  pa2m_afirmar((cola_vacia(cola) == false), "La cola no esta vacia");
  cola_destruir(cola);
}

void prueba_cola_desacolar() {
  pa2m_nuevo_grupo("Pruebas cola desencolar");
  cola_t* cola = cola_crear();
  int a = 0, b = 1, c = 2;
  pa2m_afirmar((cola_encolar(cola, &a) && cola_encolar(cola, &b) && cola_encolar(cola, &c)), "Encolar elementos A, B, C");
  pa2m_afirmar((cola_tamanio(cola) == 3), "El tamanio de la cola es tres");
  pa2m_afirmar((cola_frente(cola) == &a), "El tope de la cola es A");
  pa2m_afirmar((cola_desencolar(cola) == &a), "Desencolar devuelve elemento A");
  pa2m_afirmar((cola_tamanio(cola) == 2), "El tamanio de la cola es dos");
  pa2m_afirmar((cola_frente(cola) == &b), "El tope de la cola es B");
  pa2m_afirmar((cola_desencolar(cola) == &b), "Desencolar devuelve elemento B");
  pa2m_afirmar((cola_tamanio(cola) == 1), "El tamanio de la cola es uno");
  pa2m_afirmar((cola_frente(cola) == &c), "El tope de la cola es C");
  pa2m_afirmar((cola_desencolar(cola) == &c), "Desencolar devuelve elemento C");
  pa2m_afirmar((cola_tamanio(cola) == 0), "El tamanio de la cola es NULL");
  pa2m_afirmar((cola_frente(cola) == NULL), "El tope de la cola es NULL");
  pa2m_afirmar((cola_desencolar(cola) == NULL), "Desencolar devuelve elemento NULL");
  pa2m_afirmar((cola_tamanio(cola) == 0), "El tamanio de la cola es NULL");
  pa2m_afirmar((cola_frente(cola) == NULL), "El tope de la cola es NULL");

  cola_destruir(cola);
}

void pruebas_cola() {
  printf(
      "\x1b[33;1m"
      "\n==========================");
  pa2m_nuevo_grupo(
      "=====  Pruebas cola  =====");
  prueba_pila_crear_y_destruir();
  prueba_cola_encolar_y_frente();
  prueba_cola_desacolar();
}

int main() {
  pruebas_lista();
  pruebas_pila();
  pruebas_cola();

  return pa2m_mostrar_reporte();
}
