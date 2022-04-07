#include <stdio.h>

#include "pa2mm.h"
#include "src/abb.h"
#include "string.h"

int comparar_cosas(void* c1, void* c2) {
  if (!c1) return -1;
  if (!c2) return 1;
  return *(int*)c1 - *(int*)c2;
}

void destruir_int(void* dato) {
  free((int*)dato);
}

bool print_hasta(void* dato, void* extra, int hasta) {
  *(int*)extra += 1;
  if (*(int*)extra == 1) {
    printf("    Elementos: %d", *(int*)dato);
  } else {
    printf(", %d", *(int*)dato);
  }

  if (*(int*)dato == hasta) {
    printf("\n");
    return false;
  }
  return true;
}

bool print_inorden(void* dato, void* extra) {
  *(int*)extra += 1;
  printf("    Elemento: %d ", *(int*)dato);
  pa2m_afirmar((*(int*)dato == *(int*)extra), "");
  return true;
}

// recibe vector con primer elemento contador y luego los elementos con el orden correcto
bool print_preorden_postorden(void* dato, void* extra) {
  ((int*)extra)[0] += 1;
  printf("    Elemento: %d ", *(int*)dato);
  pa2m_afirmar((*(int*)dato == ((int*)extra)[((int*)extra)[0]]), "");
  return true;
}

bool print_hasta_e(void* dato, void* extra) {
  return print_hasta(dato, extra, 5);
}

bool print_hasta_d(void* dato, void* extra) {
  return print_hasta(dato, extra, 4);
}

bool print_hasta_c(void* dato, void* extra) {
  return print_hasta(dato, extra, 3);
}

bool print_hasta_b(void* dato, void* extra) {
  return print_hasta(dato, extra, 2);
}

bool print_hasta_a(void* dato, void* extra) {
  return print_hasta(dato, extra, 1);
}

bool print_false(void* dato, void* extra) {
  return print_hasta(dato, extra, 0);
}

void prueba_crear_y_destruir() {
  pa2m_nuevo_grupo("Pruebas crear y destruir");
  abb_t* arbol = abb_crear(comparar_cosas);
  pa2m_afirmar((arbol != NULL), "Crear arbol");
  pa2m_afirmar((abb_tamanio(arbol) == 0), "Hay 0 elementos");
  pa2m_afirmar((true), "Probar destruir arbol");
  abb_destruir(arbol);
}

void prueba_insertar_buscar() {
  pa2m_nuevo_grupo("Pruebas insertar y buscar");
  int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6;
  abb_t* arbol = abb_crear(comparar_cosas);

  pa2m_afirmar((abb_insertar(arbol, &c) != NULL), "Insertar C");
  pa2m_afirmar((abb_insertar(arbol, &b) != NULL), "Insertar B");
  pa2m_afirmar((abb_insertar(arbol, &e) != NULL), "Insertar E");
  pa2m_afirmar((abb_insertar(arbol, &d) != NULL), "Insertar D");
  pa2m_afirmar((abb_insertar(arbol, &a) != NULL), "Insertar A");
  pa2m_afirmar((abb_tamanio(arbol) == 5), "Hay 5 elementos");
  pa2m_afirmar((*(int*)abb_buscar(arbol, &a) == a), "Buscar A encuentra a A");
  pa2m_afirmar((*(int*)abb_buscar(arbol, &b) == b), "Buscar B encuentra a B");
  pa2m_afirmar((*(int*)abb_buscar(arbol, &c) == c), "Buscar C encuentra a C");
  pa2m_afirmar((*(int*)abb_buscar(arbol, &d) == d), "Buscar D encuentra a D");
  pa2m_afirmar((*(int*)abb_buscar(arbol, &e) == e), "Buscar E encuentra a E");
  pa2m_afirmar((abb_buscar(arbol, &f) == NULL), "Buscar F devuelve NULL");
  abb_destruir(arbol);
}

void prueba_destruir_todo() {
  pa2m_nuevo_grupo("Prueba destruir con destructor de elementos");
  abb_t* arbol = abb_crear(comparar_cosas);
  int *a = malloc(sizeof(int)), *b = malloc(sizeof(int)), *c = malloc(sizeof(int)), *d = malloc(sizeof(int)), *e = malloc(sizeof(int));
  *a = 1, *b = 2, *c = 3, *d = 4, *e = 5;
  abb_insertar(arbol, a);
  abb_insertar(arbol, b);
  abb_insertar(arbol, d);
  abb_insertar(arbol, e);
  abb_insertar(arbol, c);
  pa2m_afirmar((abb_tamanio(arbol) == 5), "Hay 5 elementos (A, B, D, E, C)");
  pa2m_afirmar((true), "Probar destruir arbol con sus elementos internos");
  abb_destruir_todo(arbol, destruir_int);
}

void prueba_quitar() {
  pa2m_nuevo_grupo("Pruebas quitar");
  int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6;
  abb_t* arbol = abb_crear(comparar_cosas);
  abb_insertar(arbol, &c);
  abb_insertar(arbol, &b);
  abb_insertar(arbol, &e);
  abb_insertar(arbol, &d);
  abb_insertar(arbol, &a);
  abb_insertar(arbol, &f);

  pa2m_afirmar((abb_tamanio(arbol) == 6), "Hay 6 elementos (C, B, E, D, A, F)");

  pa2m_afirmar(((int*)abb_quitar(arbol, &d) == &d), "Quitar D devuelve D (ningun hijo)");
  size_t cantidad = 0;
  int* elementos[8];
  cantidad = abb_recorrer(arbol, POSTORDEN, (void**)elementos, 8);
  pa2m_afirmar((abb_tamanio(arbol) == 5), "Hay 5 elementos (C, B, E, A, F)");
  printf("Elemento en POSTORDEN: ");
  for (size_t i = 0; i < cantidad; i++) {
    printf("%i ", *(elementos[i]));
  }
  printf("\n");

  pa2m_afirmar(((int*)abb_quitar(arbol, &b) == &b), "Quitar B devuelve B (un hijo)");
  cantidad = abb_recorrer(arbol, POSTORDEN, (void**)elementos, 8);
  pa2m_afirmar((abb_tamanio(arbol) == 4), "Hay 4 elementos (C, E, A, F)");
  printf("Elemento en POSTORDEN: ");
  for (size_t i = 0; i < cantidad; i++) {
    printf("%i ", *(elementos[i]));
  }
  printf("\n");

  pa2m_afirmar(((int*)abb_quitar(arbol, &c) == &c), "Quitar C devuelve C (dos hijo y raiz)");
  cantidad = abb_recorrer(arbol, POSTORDEN, (void**)elementos, 8);
  pa2m_afirmar((abb_tamanio(arbol) == 3), "Hay 3 elementos (E, A, F)");
  printf("Elemento en POSTORDEN: ");
  for (size_t i = 0; i < cantidad; i++) {
    printf("%i ", *(elementos[i]));
  }
  printf("\n");

  abb_destruir(arbol);
}

void prueba_funcion_a_elemento_inorden() {
  pa2m_nuevo_grupo("Pruebas aplicar funcion a elementos INORDEN");
  int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6;
  abb_t* arbol = abb_crear(comparar_cosas);
  abb_insertar(arbol, &c);
  abb_insertar(arbol, &b);
  abb_insertar(arbol, &e);
  abb_insertar(arbol, &d);
  abb_insertar(arbol, &a);
  abb_insertar(arbol, &f);

  pa2m_afirmar((abb_tamanio(arbol) == 6), "Hay 6 elementos (C, B, E, D, A, F)");

  printf("Recorrer los elementos aplicando funcion\n");
  int aux = 0;
  size_t recorrido = abb_con_cada_elemento(arbol, INORDEN, print_inorden, &aux);
  pa2m_afirmar((recorrido == 6), "Recorre los 6 elementos en fila con el orden correcto\n");

  printf("Recorrer los elementos hasta E aplicando funcion\n");
  aux = 0;
  recorrido = abb_con_cada_elemento(arbol, INORDEN, print_hasta_e, &aux);
  pa2m_afirmar((recorrido == 5), "Recorre los primeros 5 elementos en fila\n");

  printf("Recorrer los elementos hasta D aplicando funcion\n");
  aux = 0;
  recorrido = abb_con_cada_elemento(arbol, INORDEN, print_hasta_d, &aux);
  pa2m_afirmar((recorrido == 4), "Recorre los primeros 4 elementos en fila\n");

  printf("Recorrer los elementos hasta C aplicando funcion\n");
  aux = 0;
  recorrido = abb_con_cada_elemento(arbol, INORDEN, print_hasta_c, &aux);
  pa2m_afirmar((recorrido == 3), "Recorre los primeros 3 elementos en fila\n");

  printf("Recorrer los elementos hasta B aplicando funcion\n");
  aux = 0;
  recorrido = abb_con_cada_elemento(arbol, INORDEN, print_hasta_b, &aux);
  pa2m_afirmar((recorrido == 2), "Recorre los primeros 2 elementos en fila\n");

  printf("Recorrer los elementos hasta A aplicando funcion\n");
  aux = 0;
  recorrido = abb_con_cada_elemento(arbol, INORDEN, print_hasta_a, &aux);
  pa2m_afirmar((recorrido == 1), "Recorre el primer elemento en fila\n");

  abb_destruir(arbol);
}

void prueba_funcion_a_elemento_preorden() {
  pa2m_nuevo_grupo("Pruebas aplicar funcion a elementos PREORDEN");
  int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6;
  abb_t* arbol = abb_crear(comparar_cosas);
  abb_insertar(arbol, &c);
  abb_insertar(arbol, &b);
  abb_insertar(arbol, &e);
  abb_insertar(arbol, &d);
  abb_insertar(arbol, &a);
  abb_insertar(arbol, &f);

  pa2m_afirmar((abb_tamanio(arbol) == 6), "Hay 6 elementos (C, B, E, D, A, F)");

  printf("Recorrer los elementos aplicando funcion\n");
  int aux_vector[7] = {0, 3, 2, 1, 5, 4, 6};
  size_t recorrido = abb_con_cada_elemento(arbol, PREORDEN, print_preorden_postorden, aux_vector);
  pa2m_afirmar((recorrido == 6), "Recorre los 6 elementos en fila con el orden correcto\n");

  printf("Recorrer los elementos hasta E aplicando funcion\n");
  int aux = 0;
  recorrido = abb_con_cada_elemento(arbol, PREORDEN, print_hasta_e, &aux);
  pa2m_afirmar((recorrido == 4), "Recorre los primeros 4 elementos en fila\n");

  printf("Recorrer los elementos hasta D aplicando funcion\n");
  aux = 0;
  recorrido = abb_con_cada_elemento(arbol, PREORDEN, print_hasta_d, &aux);
  pa2m_afirmar((recorrido == 5), "Recorre los primeros 5 elementos en fila\n");

  printf("Recorrer los elementos hasta C aplicando funcion\n");
  aux = 0;
  recorrido = abb_con_cada_elemento(arbol, PREORDEN, print_hasta_c, &aux);
  pa2m_afirmar((recorrido == 1), "Recorre el primer elemento en fila\n");

  printf("Recorrer los elementos hasta B aplicando funcion\n");
  aux = 0;
  recorrido = abb_con_cada_elemento(arbol, PREORDEN, print_hasta_b, &aux);
  pa2m_afirmar((recorrido == 2), "Recorre los primeros 2 elementos en fila\n");

  printf("Recorrer los elementos hasta A aplicando funcion\n");
  aux = 0;
  recorrido = abb_con_cada_elemento(arbol, PREORDEN, print_hasta_a, &aux);
  pa2m_afirmar((recorrido == 3), "Recorre los primeros 3 elementos en fila\n");

  abb_destruir(arbol);
}

void prueba_funcion_a_elemento_postorden() {
  pa2m_nuevo_grupo("Pruebas aplicar funcion a elementos POSTORDEN");
  int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6;
  abb_t* arbol = abb_crear(comparar_cosas);
  abb_insertar(arbol, &c);
  abb_insertar(arbol, &b);
  abb_insertar(arbol, &e);
  abb_insertar(arbol, &d);
  abb_insertar(arbol, &a);
  abb_insertar(arbol, &f);

  pa2m_afirmar((abb_tamanio(arbol) == 6), "Hay 6 elementos (C, B, E, D, A, F)");

  printf("Recorrer los elementos aplicando funcion\n");
  int aux_vector[7] = {0, 1, 2, 4, 6, 5, 3};
  size_t recorrido = abb_con_cada_elemento(arbol, POSTORDEN, print_preorden_postorden, aux_vector);
  pa2m_afirmar((recorrido == 6), "Recorre los 6 elementos en fila con el orden correcto\n");

  printf("Recorrer los elementos hasta E aplicando funcion\n");
  int aux = 0;
  recorrido = abb_con_cada_elemento(arbol, POSTORDEN, print_hasta_e, &aux);
  pa2m_afirmar((recorrido == 5), "Recorre los primeros 5 elementos en fila\n");

  printf("Recorrer los elementos hasta D aplicando funcion\n");
  aux = 0;
  recorrido = abb_con_cada_elemento(arbol, POSTORDEN, print_hasta_d, &aux);
  pa2m_afirmar((recorrido == 3), "Recorre los primeros 3 elementos en fila\n");

  printf("Recorrer los elementos hasta C aplicando funcion\n");
  aux = 0;
  recorrido = abb_con_cada_elemento(arbol, POSTORDEN, print_hasta_c, &aux);
  pa2m_afirmar((recorrido == 6), "Recorre los primeros 6 elementos en fila\n");

  printf("Recorrer los elementos hasta B aplicando funcion\n");
  aux = 0;
  recorrido = abb_con_cada_elemento(arbol, POSTORDEN, print_hasta_b, &aux);
  pa2m_afirmar((recorrido == 2), "Recorre los primeros 2 elementos en fila\n");

  printf("Recorrer los elementos hasta A aplicando funcion\n");
  aux = 0;
  recorrido = abb_con_cada_elemento(arbol, POSTORDEN, print_hasta_a, &aux);
  pa2m_afirmar((recorrido == 1), "Recorre los primeros 1 elementos en fila\n");

  abb_destruir(arbol);
}

bool comparar_vectores(int** vector1, int** vector2, size_t tamanio) {
  for (size_t i = 0; i < tamanio; i++) {
    if (*(vector1[i]) != *(vector2[i])) {
      return false;
    }
  }
  return true;
}

void prueba_recorrer() {
  pa2m_nuevo_grupo("Pruebas recorrido");
  int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6;
  abb_t* arbol = abb_crear(comparar_cosas);
  abb_insertar(arbol, &c);
  abb_insertar(arbol, &b);
  abb_insertar(arbol, &e);
  abb_insertar(arbol, &d);
  abb_insertar(arbol, &a);
  abb_insertar(arbol, &f);

  pa2m_afirmar((abb_tamanio(arbol) == 6), "Hay 6 elementos (C, B, E, D, A, F)");
  size_t cantidad = 0;
  int* elementos_falta_espacio[5];
  int* elementos_igual_espacio[6];
  int* elementos_sobra_espacio[8];
  int* inorden_deberia_quedar[6] = {&a, &b, &c, &d, &e, &f};
  int* preorden_deberia_quedar[6] = {&c, &b, &a, &e, &d, &f};
  int* postorden_deberia_quedar[6] = {&a, &b, &d, &f, &e, &c};

  pa2m_nuevo_grupo("INORDEN");
  cantidad = abb_recorrer(arbol, INORDEN, (void**)elementos_falta_espacio, 5);
  pa2m_afirmar((comparar_vectores(elementos_falta_espacio, inorden_deberia_quedar, cantidad)), "Recorrer a vector menor tamanio (el vector queda exactamente A, B, C, D, E)");
  pa2m_afirmar((cantidad == 5), "La cantidad de elementos guardados es 5");

  printf("\n");
  cantidad = abb_recorrer(arbol, INORDEN, (void**)elementos_igual_espacio, 6);
  pa2m_afirmar((comparar_vectores(elementos_igual_espacio, inorden_deberia_quedar, cantidad)), "Recorrer a vector igual tamanio (el vector queda exactamente A, B, C, D, E, F)");
  pa2m_afirmar((cantidad == 6), "La cantidad de elementos guardados es 6");

  printf("\n");
  cantidad = abb_recorrer(arbol, INORDEN, (void**)elementos_sobra_espacio, 8);
  pa2m_afirmar((comparar_vectores(elementos_sobra_espacio, inorden_deberia_quedar, cantidad)), "Recorrer a vector mayor tamanio (el vector queda exactamente A, B, C, D, E, F)");
  pa2m_afirmar((cantidad == 6), "La cantidad de elementos guardados es 6");

  pa2m_nuevo_grupo("PREORDEN");
  cantidad = abb_recorrer(arbol, PREORDEN, (void**)elementos_falta_espacio, 5);
  pa2m_afirmar((comparar_vectores(elementos_falta_espacio, preorden_deberia_quedar, cantidad)), "Recorrer a vector menor tamanio (el vector queda exactamente C, B, A, E, D)");
  pa2m_afirmar((cantidad == 5), "La cantidad de elementos guardados es 5");

  printf("\n");
  cantidad = abb_recorrer(arbol, PREORDEN, (void**)elementos_igual_espacio, 6);
  pa2m_afirmar((comparar_vectores(elementos_igual_espacio, preorden_deberia_quedar, cantidad)), "Recorrer a vector igual tamanio (el vector queda exactamente C, B, A, E, D, F)");
  pa2m_afirmar((cantidad == 6), "La cantidad de elementos guardados es 6");

  printf("\n");
  cantidad = abb_recorrer(arbol, PREORDEN, (void**)elementos_sobra_espacio, 8);
  pa2m_afirmar((comparar_vectores(elementos_sobra_espacio, preorden_deberia_quedar, cantidad)), "Recorrer a vector mayor tamanio (el vector queda exactamente C, B, A, E, D, F)");
  pa2m_afirmar((cantidad == 6), "La cantidad de elementos guardados es 6");

  pa2m_nuevo_grupo("POSTORDEN");
  cantidad = abb_recorrer(arbol, POSTORDEN, (void**)elementos_falta_espacio, 5);
  pa2m_afirmar((comparar_vectores(elementos_falta_espacio, postorden_deberia_quedar, cantidad)), "Recorrer a vector menor tamanio (el vector queda exactamente A, B, D, F, E)");
  pa2m_afirmar((cantidad == 5), "La cantidad de elementos guardados es 5");

  printf("\n");
  cantidad = abb_recorrer(arbol, POSTORDEN, (void**)elementos_igual_espacio, 6);
  pa2m_afirmar((comparar_vectores(elementos_igual_espacio, postorden_deberia_quedar, cantidad)), "Recorrer a vector igual tamanio (el vector queda exactamente A, B, D, F, E, C)");
  pa2m_afirmar((cantidad == 6), "La cantidad de elementos guardados es 6");

  printf("\n");
  cantidad = abb_recorrer(arbol, POSTORDEN, (void**)elementos_sobra_espacio, 8);
  pa2m_afirmar((comparar_vectores(elementos_sobra_espacio, postorden_deberia_quedar, cantidad)), "Recorrer a vector mayor tamanio (el vector queda exactamente A, B, D, F, E, C)");
  pa2m_afirmar((cantidad == 6), "La cantidad de elementos guardados es 6");

  abb_destruir(arbol);
}

void prueba_nulls() {
  pa2m_nuevo_grupo("Prueba de NULLs");
  abb_t* arbol = abb_crear(comparar_cosas);
  int elemento = 1;
  int* elementos[3];
  pa2m_afirmar((abb_crear(NULL) == NULL), "Crear arbol con comparador NULL devuelve NULL");
  pa2m_afirmar((abb_insertar(NULL, &elemento) == NULL), "Insertar elemento a arbol NULL devuelve NULL");
  pa2m_afirmar((abb_insertar(arbol, NULL) != NULL), "Insertar elemento nulo es valido");
  pa2m_afirmar((abb_tamanio(arbol) == 1), "Tamanio de arbol con el NULL insertado es 1");
  pa2m_afirmar((abb_buscar(NULL, &elemento) == NULL), "Obtener elemento de arbol NULL devuelve NULL");
  pa2m_afirmar((abb_quitar(arbol, NULL) == NULL), "Eliminar elemento NULL");
  pa2m_afirmar((abb_tamanio(NULL) == 0), "Tamanio de arbol con el NULL eliminado es 0 nuevamente");
  pa2m_afirmar((abb_vacio(NULL) == true), "Arbol NULL esta vacio");
  pa2m_afirmar((abb_tamanio(NULL) == 0), "Arbol NULL tamanio cero");
  pa2m_afirmar((abb_con_cada_elemento(NULL, INORDEN, print_hasta_a, NULL) == 0), "Funcion a cada elemento de arbol null devuelve 0");
  pa2m_afirmar((abb_con_cada_elemento(arbol, INORDEN, NULL, NULL) == 0), "Funcion null a cada elemento de arbol devuelve 0");
  pa2m_afirmar((abb_recorrer(NULL, INORDEN, (void**)elementos, 3) == 0), "Recorrer arbol null devuelve 0");
  pa2m_afirmar((abb_recorrer(arbol, INORDEN, NULL, 3) == 0), "Recorrer arbol a vector null devuelve 0");
  abb_destruir(NULL);
  pa2m_afirmar((true), "Destruir arbol Null no falla");
  abb_destruir_todo(NULL, destruir_int);
  pa2m_afirmar((true), "Destruir todo a arbol Null no falla");
  abb_destruir_todo(arbol, NULL);
  pa2m_afirmar((true), "Destruir todo a arbol con destructor null borra los nodos");
}

int main() {
  pa2m_nuevo_grupo("Pruebas de ABB");
  prueba_crear_y_destruir();
  prueba_insertar_buscar();
  prueba_destruir_todo();
  prueba_quitar();
  prueba_funcion_a_elemento_inorden();
  prueba_funcion_a_elemento_preorden();
  prueba_funcion_a_elemento_postorden();
  prueba_recorrer();
  prueba_nulls();

  return pa2m_mostrar_reporte();
}
