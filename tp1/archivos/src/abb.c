#include "abb.h"

#include <stddef.h>
#include <stdlib.h>

typedef struct iterador_recorrer {
  size_t posicion;
  size_t tamanio;
  void** datos;
} iterador_recorrer_t;
/*
 * Recibe un puntero a elemento devuelve un nodo de abb que lo contiene o NULL si falla.
 */
nodo_abb_t* nodo_abb_crear(void* elemento) {
  nodo_abb_t* nodo_abb_nuevo = calloc(1, sizeof(nodo_abb_t));
  if (!nodo_abb_nuevo) return NULL;
  nodo_abb_nuevo->elemento = elemento;
  return nodo_abb_nuevo;
}

/*
 * Inserta un elemento en un abb con nodo como raiz utilizando la funcion comparador para encontrar el lugar donde debe insertarse.
 * Devuelve el nodo recibido actualizado o NULL en caso de fallo.
 */
nodo_abb_t* abb_insertar_en_nodo(nodo_abb_t* nodo, void* elemento, abb_comparador comparador) {
  if (!nodo) return nodo_abb_crear(elemento);
  int comparacion = comparador(elemento, nodo->elemento);

  if (comparacion <= 0) {
    nodo_abb_t* nuevo_nodo = abb_insertar_en_nodo(nodo->izquierda, elemento, comparador);
    if (!nuevo_nodo) return NULL;
    nodo->izquierda = nuevo_nodo;
  } else {
    nodo_abb_t* nuevo_nodo = abb_insertar_en_nodo(nodo->derecha, elemento, comparador);
    if (!nuevo_nodo) return NULL;
    nodo->derecha = nuevo_nodo;
  }

  return nodo;
}

/*
 * Elimina un nodo con uno o ningun hijo devolviendo su hijo en el caso de que existista.
 */
void* abb_quitar_elemento_un_o_cero_hijo(nodo_abb_t* nodo) {
  nodo_abb_t* hijo = nodo->izquierda ? nodo->izquierda : nodo->derecha;
  free(nodo);
  return hijo;
}

/*
 * Elimina un nodo con dos hijos devolviendo la nueva raiz del arbol del nodo eliminado, el cual seria el predecesor inorden del nodo eliminado.
 * En caso de que el predecesor tuviera un hijo izquierdo, este se coloca como derecho de su padre al menos que su padre sea el nodo en 
 * cuestion, donde se mantiene como hijo izquierdo.
 */
void* abb_quitar_elemento_dos_hijos(nodo_abb_t* nodo) {
  nodo_abb_t* nodo_predecesor = nodo->izquierda;
  nodo_abb_t* nodo_predecesor_padre = NULL;

  while (nodo_predecesor->derecha) {
    nodo_predecesor_padre = nodo_predecesor;
    nodo_predecesor = nodo_predecesor->derecha;
  }

  nodo->elemento = nodo_predecesor->elemento;

  if (nodo_predecesor_padre) {
    nodo_predecesor_padre->derecha = nodo_predecesor->izquierda;
  } else {
    nodo->izquierda = nodo_predecesor->izquierda;
  }

  free(nodo_predecesor);
  return nodo;
}

/*
 * Elimina un nodo devolviendo la nueva raiz de su arbol inferior o NULL en caso de no tener hijos.
 */
void* abb_quitar_de_nodo(nodo_abb_t* nodo, abb_comparador comparador, void* elemento, void** elemento_borrado) {
  if (!nodo) return NULL;
  int comparacion = comparador(elemento, nodo->elemento);
  if (comparacion == 0) {
    *elemento_borrado = nodo->elemento;
    if (nodo->izquierda && nodo->derecha) {
      return abb_quitar_elemento_dos_hijos(nodo);
    } else {
      return abb_quitar_elemento_un_o_cero_hijo(nodo);
    }
  } else if (comparacion < 0) {
    nodo->izquierda = abb_quitar_de_nodo(nodo->izquierda, comparador, elemento, elemento_borrado);
  } else {
    nodo->derecha = abb_quitar_de_nodo(nodo->derecha, comparador, elemento, elemento_borrado);
  }
  return nodo;
}

/*
 * Busca un elemento en el arbol que tiene a nodo como raiz utilizando la funcion comparador.
 * Devuelve el elemento o NULL en caso de no encontrarlo.
 */
void* abb_buscar_en_nodo(nodo_abb_t* nodo, void* elemento, abb_comparador comparador) {
  if (!nodo) return NULL;
  int comparacion = comparador(elemento, nodo->elemento);
  if (comparacion == 0)
    return nodo->elemento;
  else if (comparacion < 0)
    return abb_buscar_en_nodo(nodo->izquierda, elemento, comparador);
  return abb_buscar_en_nodo(nodo->derecha, elemento, comparador);
}

/*
 * Recorre el arbol INORDEN e invoca la funcion con cada elemento almacenado en el mismo
 * como primer parámetro. El puntero aux se pasa como segundo parámetro a la
 * función. Si la función devuelve false, se finaliza el recorrido aun si quedan
 * elementos por recorrer. Si devuelve true se sigue recorriendo mientras queden
 * elementos. 
 * Devuelve la cantidad de veces que se aplicó la función.
 */
size_t abb_recorrido_inorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* aux, bool* continuar) {
  if (!nodo) return 0;
  size_t cantidad = 0;

  cantidad += abb_recorrido_inorden(nodo->izquierda, funcion, aux, continuar);
  if (*continuar == false) return cantidad;

  bool retorno = funcion(nodo->elemento, aux);
  cantidad++;
  if (!retorno) {
    *continuar = false;
    return cantidad;
  }

  cantidad += abb_recorrido_inorden(nodo->derecha, funcion, aux, continuar);
  return cantidad;
}

/*
 * Recorre el arbol PREORDEN e invoca la funcion con cada elemento almacenado en el mismo
 * como primer parámetro. El puntero aux se pasa como segundo parámetro a la
 * función. Si la función devuelve false, se finaliza el recorrido aun si quedan
 * elementos por recorrer. Si devuelve true se sigue recorriendo mientras queden
 * elementos. 
 * Devuelve la cantidad de veces que se aplicó la función.
 */
size_t abb_recorrido_preorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* aux, bool* continuar) {
  if (!nodo) return 0;
  size_t cantidad = 1;

  bool retorno = funcion(nodo->elemento, aux);
  if (!retorno) {
    *continuar = false;
    return cantidad;
  }

  cantidad += abb_recorrido_preorden(nodo->izquierda, funcion, aux, continuar);
  if (!*continuar) return cantidad;

  cantidad += abb_recorrido_preorden(nodo->derecha, funcion, aux, continuar);
  return cantidad;
}

/*
 * Recorre el arbol POSTORDEN e invoca la funcion con cada elemento almacenado en el mismo
 * como primer parámetro. El puntero aux se pasa como segundo parámetro a la
 * función. Si la función devuelve false, se finaliza el recorrido aun si quedan
 * elementos por recorrer. Si devuelve true se sigue recorriendo mientras queden
 * elementos. 
 * Devuelve la cantidad de veces que se aplicó la función.
 */
size_t abb_recorrido_postorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* aux, bool* continuar) {
  if (!nodo) return 0;
  size_t cantidad = 0;

  cantidad += abb_recorrido_postorden(nodo->izquierda, funcion, aux, continuar);
  if (!*continuar) return cantidad;

  cantidad += abb_recorrido_postorden(nodo->derecha, funcion, aux, continuar);
  if (!*continuar) return cantidad;

  bool retorno = funcion(nodo->elemento, aux);
  cantidad++;
  if (!retorno) *continuar = false;

  return cantidad;
}

/*
 * Almacena un elemento en el vector de un iterador. 
 * Devuelve true si queda espacio en el vector o falso si está lleno. 
 */
bool guardar_elemento_en_array(void* elemento, iterador_recorrer_t* iterador) {
  iterador->datos[iterador->posicion] = elemento;
  iterador->posicion++;
  if (iterador->posicion == iterador->tamanio) return false;
  return true;
}

/*
 * Libera de memoria los nodos del arbol que tienen a nodo como raiz y destruye a sus 
 * elementos con la función destruir en caso de que esta no sea null, caso contrario
 * se dejan en memoria.
 */
void abb_destruir_desde_nodo(nodo_abb_t* nodo, void (*destructor)(void*)) {
  if (!nodo) return;
  abb_destruir_desde_nodo(nodo->izquierda, destructor);
  abb_destruir_desde_nodo(nodo->derecha, destructor);
  if (destructor) destructor(nodo->elemento);
  free(nodo);
}

/**
 * Crea un arbol binario de búsqueda. Comparador es una función que se utiliza
 * para comparar dos elementos. La funcion de comparación no puede ser nula.
 *
 * Devuelve un puntero al arbol creado o NULL en caso de error.
 */
abb_t* abb_crear(abb_comparador comparador) {
  if (!comparador) return NULL;
  abb_t* abb_nuevo = calloc(1, sizeof(abb_t));
  if (!abb_nuevo) return NULL;
  abb_nuevo->comparador = comparador;
  return abb_nuevo;
}

/**
 * Inserta un elemento en el arbol.
 * El arbol admite elementos con valores repetidos.
 *
 * Devuelve el arbol en caso de exito o NULL en caso de error.
 */
abb_t* abb_insertar(abb_t* arbol, void* elemento) {
  if (!arbol) return NULL;
  if (abb_vacio(arbol)) {
    arbol->nodo_raiz = nodo_abb_crear(elemento);
    if (abb_vacio(arbol)) return NULL;
    arbol->tamanio++;
    return arbol;
  }

  nodo_abb_t* arbol_con_elemento_insertado = abb_insertar_en_nodo(arbol->nodo_raiz, elemento, arbol->comparador);
  if (!arbol_con_elemento_insertado) return NULL;
  arbol->nodo_raiz = arbol_con_elemento_insertado;
  arbol->tamanio++;
  return arbol;
}

/**
 * Busca en el arbol un elemento igual al provisto (utilizando la funcion de
 * comparación) y si lo encuentra lo quita del arbol y lo devuelve.
 *
 * Devuelve el elemento extraido del árbol o NULL en caso de erro.
 */
void* abb_quitar(abb_t* arbol, void* elemento) {
  if (abb_vacio(arbol)) return NULL;
  void* elemento_borrado = NULL;
  arbol->nodo_raiz = abb_quitar_de_nodo(arbol->nodo_raiz, arbol->comparador, elemento, &elemento_borrado);
  if (elemento_borrado) {
    arbol->tamanio -= 1;
  }
  return elemento_borrado;
}

/**
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación).
 *
 * Devuelve el elemento encontrado o NULL si no lo encuentra.
 */
void* abb_buscar(abb_t* arbol, void* elemento) {
  if (abb_vacio(arbol)) return NULL;
  return abb_buscar_en_nodo(arbol->nodo_raiz, elemento, arbol->comparador);
}

/**
 * Determina si el árbol está vacío.
 *
 * Devuelve true si está vacío o el arbol es NULL, false si el árbol tiene al
 * menos un elemento.
 */
bool abb_vacio(abb_t* arbol) {
  return !arbol || !arbol->nodo_raiz;
}

/**
 * Devuelve la cantidad de elementos almacenados en el arbol o 0 si el arbol es
 * NULL.
 */
size_t abb_tamanio(abb_t* arbol) {
  if (!arbol) return 0;
  return arbol->tamanio;
}

/**
 * Recorre el arbol e invoca la funcion con cada elemento almacenado en el mismo
 * como primer parámetro. El puntero aux se pasa como segundo parámetro a la
 * función. Si la función devuelve false, se finaliza el recorrido aun si quedan
 * elementos por recorrer. Si devuelve true se sigue recorriendo mientras queden
 * elementos.
 *
 * Recorrido especifica el tipo de recorrido a realizar.
 *
 * Devuelve la cantidad de veces que fue invocada la función.
*/
size_t abb_con_cada_elemento(abb_t* arbol, abb_recorrido recorrido, bool (*funcion)(void*, void*), void* aux) {
  if (!arbol || !arbol->nodo_raiz || !funcion) return 0;
  bool continuar = true;
  if (recorrido == INORDEN) {
    return abb_recorrido_inorden(arbol->nodo_raiz, funcion, aux, &continuar);
  } else if (recorrido == PREORDEN) {
    return abb_recorrido_preorden(arbol->nodo_raiz, funcion, aux, &continuar);
  }
  return abb_recorrido_postorden(arbol->nodo_raiz, funcion, aux, &continuar);
}

/**
 * Recorre el arbol según el recorrido especificado y va almacenando los
 * elementos en el array hasta completar el recorrido o quedarse sin espacio en
 * el array.
 *
 * El array tiene un tamaño maximo especificado por tamanio_array.
 *
 * Devuelve la cantidad de elementos que fueron almacenados exitosamente en el
 * array.
 */
size_t abb_recorrer(abb_t* arbol, abb_recorrido recorrido, void** array, size_t tamanio_array) {
  if (abb_vacio(arbol) || !array || tamanio_array == 0) return 0;
  iterador_recorrer_t iterador = {.posicion = 0, .datos = array, .tamanio = tamanio_array};
  return abb_con_cada_elemento(arbol, recorrido, (bool (*)(void*, void*))guardar_elemento_en_array, &iterador);
}

/**
 * Destruye el arbol liberando la memoria reservada por el mismo.
 */
void abb_destruir(abb_t* arbol) {
  abb_destruir_todo(arbol, NULL);
}

/**
 * Destruye el arbol liberando la memoria reservada por el mismo.
 *
 * Adicionalmente invoca el destructor en cada uno de los elementos almacenados
 * en el arbol (si la funcion destructor no es NULL).
 */
void abb_destruir_todo(abb_t* arbol, void (*destructor)(void*)) {
  if (!arbol) return;
  abb_destruir_desde_nodo(arbol->nodo_raiz, destructor);
  free(arbol);
}