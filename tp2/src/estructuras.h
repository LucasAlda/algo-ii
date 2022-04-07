#ifndef __ESTRUCTURA_H__
#define __ESTRUCTURA_H__
#include <stdlib.h>

#include "abb.h"
#include "lista.h"

typedef bool (*funcion_a_elemento)(void*, void*);

struct _hospital_pkm_t {
  abb_t* pokemones;
  lista_t* entrenadores;
};

struct _entrenador_t {
  char* nombre;
  size_t id;
};

struct _pkm_t {
  char* nombre;
  size_t nivel;
  struct _entrenador_t* entrenador;
};

#endif /* __ESTRUCTURA_H__ */
