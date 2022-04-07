#include <string.h>

#include "pa2mm.h"
#include "src/hash.h"

void destruir_elemento(void* elemento) {
  free(elemento);
}

char* duplicar_elemento(const char* elemento) {
  char* auxiliar = malloc(strlen(elemento) + 1);
  strcpy(auxiliar, elemento);
  return auxiliar;
}

bool sumar_claves(hash_t* hash, const char* clave, void* aux) {
  int* suma = aux;
  *suma += (int)clave[0] - 96;
  return false;
}

bool sumar_claves_menor_c(hash_t* hash, const char* clave, void* aux) {
  int* suma = aux;
  *suma += (int)clave[0] - 96;
  if ((int)clave[0] - 96 == 3) {
    return true;
  }
  return false;
}

void pruebas_crear_y_destruir() {
  pa2m_nuevo_grupo("Crear y destruir");
  hash_t* hash = hash_crear(destruir_elemento, 3);
  pa2m_afirmar((hash != NULL), "Crear hash con capacidad de tres");
  pa2m_afirmar((hash_cantidad(hash) == 0), "Cantidad de elementos en hash es cero");
  hash_destruir(hash);
}

void pruebas_insertar_y_obtener() {
  pa2m_nuevo_grupo("Insertar y obtener");
  hash_t* hash = hash_crear(destruir_elemento, 3);
  pa2m_afirmar((hash != NULL), "Crear hash con capacidad de tres");
  pa2m_afirmar((hash_cantidad(hash) == 0), "Cantidad de elementos en hash es cero");
  pa2m_afirmar((hash_insertar(hash, "a", duplicar_elemento("primero")) == 0), "Insertar elemento a (primero)>");
  pa2m_afirmar((hash_insertar(hash, "b", duplicar_elemento("segundo")) == 0), "Insertar elemento b (segundo)>");
  pa2m_afirmar((hash_insertar(hash, "c", duplicar_elemento("tercero")) == 0),
               "Insertar elemento c (tercero) (deberia hacer rehash para el siguiente)");
  pa2m_afirmar((hash_insertar(hash, "d", duplicar_elemento("cuarto")) == 0), "Insertar elemento d (cuarto)");
  pa2m_afirmar((hash_cantidad(hash) == 4), "Cantidad de elementos en hash es 4");
  pa2m_afirmar((strcmp(hash_obtener(hash, "a"), "primero") == 0), "Obtener elemento a (primero)");
  pa2m_afirmar((strcmp(hash_obtener(hash, "b"), "segundo") == 0), "Obtener elemento b (segundo)");
  pa2m_afirmar((strcmp(hash_obtener(hash, "c"), "tercero") == 0), "Obtener elemento c (tercero)");
  pa2m_afirmar((strcmp(hash_obtener(hash, "d"), "cuarto") == 0), "Obtener elemento d (cuarto)");

  hash_destruir(hash);
}

void pruebas_insertar_misma_clave() {
  pa2m_nuevo_grupo("Insertar misma clave");
  hash_t* hash = hash_crear(destruir_elemento, 3);
  pa2m_afirmar((hash_insertar(hash, "a", duplicar_elemento("primero")) == 0), "Insertar elemento a (primero)>");
  pa2m_afirmar((hash_insertar(hash, "b", duplicar_elemento("segundo")) == 0), "Insertar elemento b (segundo)>");
  pa2m_afirmar((hash_insertar(hash, "c", duplicar_elemento("tercero")) == 0), "Insertar elemento c (tercero)");
  pa2m_afirmar((hash_insertar(hash, "d", duplicar_elemento("cuarto")) == 0), "Insertar elemento d (cuarto)");
  pa2m_afirmar((hash_cantidad(hash) == 4), "Cantidad de elementos en hash es 4");
  pa2m_afirmar((hash_insertar(hash, "b", duplicar_elemento("quinto")) == 0), "Insertar elemento b (quinto)>");
  pa2m_afirmar((hash_insertar(hash, "d", duplicar_elemento("sexto")) == 0), "Insertar elemento d (sexto)");
  pa2m_afirmar((hash_cantidad(hash) == 4), "Cantidad de elementos sigue siendo 4");
  pa2m_afirmar((strcmp(hash_obtener(hash, "a"), "primero") == 0), "Obtener elemento a (primero)");
  pa2m_afirmar((strcmp(hash_obtener(hash, "b"), "quinto") == 0), "Obtener elemento b (quinto)");
  pa2m_afirmar((strcmp(hash_obtener(hash, "c"), "tercero") == 0), "Obtener elemento c (tercero)");
  pa2m_afirmar((strcmp(hash_obtener(hash, "d"), "sexto") == 0), "Obtener elemento d (sexto)");

  hash_destruir(hash);
}

void pruebas_quitar() {
  pa2m_nuevo_grupo("Quitar elementos");
  hash_t* hash = hash_crear(destruir_elemento, 3);
  pa2m_afirmar((hash_insertar(hash, "a", duplicar_elemento("primero")) == 0), "Insertar elemento a (primero)>");
  pa2m_afirmar((hash_insertar(hash, "b", duplicar_elemento("segundo")) == 0), "Insertar elemento b (segundo)>");
  pa2m_afirmar((hash_insertar(hash, "c", duplicar_elemento("tercero")) == 0), "Insertar elemento c (tercero)");
  pa2m_afirmar((hash_insertar(hash, "c", duplicar_elemento("cuarto")) == 0), "Insertar elemento c (cuarto)");
  pa2m_afirmar((hash_quitar(hash, "a") == 0), "Quitar elemento a");
  pa2m_afirmar((hash_quitar(hash, "a") == -1), "Quitar elemento a (de nuevo) devuelve -1");
  pa2m_afirmar((hash_quitar(hash, "r") == -1), "Quitar elemento r devuelve -1");
  pa2m_afirmar((hash_quitar(hash, "c") == 0), "Quitar elemento c");
  pa2m_afirmar((hash_quitar(hash, "c") == -1), "Quitar elemento c devuelve -1");
  pa2m_afirmar((hash_cantidad(hash) == 1), "Cantidad de elementos en hash es 1");

  hash_destruir(hash);
}

void pruebas_cambiar_clave() {
  pa2m_nuevo_grupo("Cambiar clave");
  hash_t* hash = hash_crear(destruir_elemento, 3);
  char* clave_heap = malloc(sizeof(char) * 2);
  strcpy(clave_heap, "a");
  pa2m_afirmar((hash_insertar(hash, clave_heap, duplicar_elemento("primero")) == 0), "Insertar elemento con clave 'a' almacenada en heap  (primero)>");
  free(clave_heap);
  pa2m_afirmar((true), "Liberar clave del heap");
  pa2m_afirmar(hash_obtener(hash, "a") != NULL, "Obtener elemento a (primero)");
  hash_destruir(hash);
}

void pruebas_iterador_interno() {
  pa2m_nuevo_grupo("Iterador Interno");
  hash_t* hash = hash_crear(destruir_elemento, 3);
  pa2m_afirmar((hash_insertar(hash, "a", duplicar_elemento("primero")) == 0), "Insertar elemento a (primero)>");
  pa2m_afirmar((hash_insertar(hash, "b", duplicar_elemento("segundo")) == 0), "Insertar elemento b (segundo)>");
  pa2m_afirmar((hash_insertar(hash, "c", duplicar_elemento("tercero")) == 0), "Insertar elemento c (tercero)");
  pa2m_afirmar((hash_insertar(hash, "d", duplicar_elemento("cuarto")) == 0), "Insertar elemento d (cuarto)");
  int sumatoria = 0;
  int recorridos = hash_con_cada_clave(hash, sumar_claves, &sumatoria);
  pa2m_afirmar((recorridos != 0), "Recorren las claves sumando su orden en el abecedario");
  pa2m_afirmar((recorridos == 4), "Recorridos los 4 elementos");
  pa2m_afirmar((sumatoria == 10), "Sumatoria de las claves es 10");
  sumatoria = 0;
  recorridos = hash_con_cada_clave(hash, sumar_claves_menor_c, &sumatoria);
  pa2m_afirmar((recorridos != 0), "Recorren las claves sumando su orden en el abecedario hasta llegar a c");
  pa2m_afirmar((recorridos == 3), "Recorridos 3 elementos");
  pa2m_afirmar((sumatoria == 6), "Sumatoria de las claves es 6");

  hash_destruir(hash);
}

void prueba_nulls() {
  pa2m_nuevo_grupo("Prueba con NULLs");
  hash_t* hash = hash_crear(NULL, 3);
  pa2m_afirmar((hash != NULL), "Crear hash con destructor NULL");
  pa2m_afirmar((hash_insertar(hash, "a", NULL) == 0), "Insertar elemento NULL");
  pa2m_afirmar((hash_insertar(NULL, "a", "1") == -1), "Insertar en un hash NULL devuelve -1");
  pa2m_afirmar((hash_insertar(hash, NULL, "1") == -1), "Insertar clave NULL devuelve -1");
  pa2m_afirmar((hash_obtener(NULL, "a") == NULL), "Buscar clave en hash NULL devuelve NULL");
  pa2m_afirmar((hash_obtener(hash, NULL) == NULL), "Buscar clave NULL devuelve NULL");
  pa2m_afirmar((hash_contiene(hash, NULL) == false), "Contiene clave NULL devuelve false");
  pa2m_afirmar((hash_cantidad(NULL) == 0), "Hash NULL tiene 0 elementos");
  pa2m_afirmar((hash_quitar(NULL, "a") == -1), "Quitar de hash NULL devuelve -1");
  pa2m_afirmar((hash_quitar(hash, NULL) == -1), "Quitar clave NULL devuelve -1");

  hash_destruir(hash);
}

int main() {
  pruebas_crear_y_destruir();
  pruebas_insertar_y_obtener();
  pruebas_insertar_misma_clave();
  pruebas_quitar();
  pruebas_cambiar_clave();
  pruebas_iterador_interno();
  prueba_nulls();

  return pa2m_mostrar_reporte();
}