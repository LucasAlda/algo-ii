#include "hospital.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abb.h"
#include "split.h"

#define POSICION_ENTRENADOR_ID 0
#define POSICION_ENTRENADOR_NOMBRE 1
#define POSICION_PRIMER_POKEMON 2
#define SEPARADOR_ARCHIVO ';'
#define LECTURA_DEFAULT_LINEA 100

typedef bool (*funcion_a_elemento)(void*, void*);

struct _hospital_pkm_t {
  size_t cantidad_entrenador;
  abb_t* pokemones;
};

struct _entrenador_t {
  size_t id;
  char* nombre;
};

struct _pkm_t {
  char* nombre;
  size_t nivel;
  entrenador_t entrenador;
};

void pokemon_destruir(void* pokemon) {
  free(((pokemon_t*)pokemon)->nombre);
  free(((pokemon_t*)pokemon)->entrenador.nombre);
  free((pokemon_t*)pokemon);
}

int pokemon_comparar(void* pokemon1, void* pokemon2) {
  return strcmp(((pokemon_t*)pokemon1)->nombre, ((pokemon_t*)pokemon2)->nombre);
}

/**
 * Reserva memoria para un hospital y la inicializa.
 *
 * Devuelve NULL en caso de no poder.
 */
hospital_t* hospital_crear() {
  hospital_t* nuevo_hospital = calloc(1, sizeof(hospital_t));
  if (!nuevo_hospital) return NULL;

  nuevo_hospital->pokemones = abb_crear(pokemon_comparar);
  return nuevo_hospital;
}

/**
 * Recibe un archivo del que lee una linea completa y colocandola dinamicamente en la memoria
 * 
 * Devuelve NULL si el archivo no existe, no lee nada o hay algun error reservando memoria,
 * caso contrario devuelve el puntero al string
 */
char* leer_linea_completa(FILE* archivo) {
  if (!archivo) return NULL;
  size_t leido = 0;
  size_t tamanio = LECTURA_DEFAULT_LINEA;
  char* linea_heap = malloc(tamanio);

  if (!linea_heap) return NULL;

  while (fgets(linea_heap + leido, (int)(tamanio - leido), archivo)) {
    size_t nueva_lectura = strlen(linea_heap + leido);
    if (nueva_lectura > 0 && *(linea_heap + leido + nueva_lectura - 1) == '\n') {
      *(linea_heap + leido + nueva_lectura - 1) = '\0';
      return linea_heap;
    } else {
      char* linea_heap_aux = realloc(linea_heap, tamanio * 2);
      if (!linea_heap_aux) {
        free(linea_heap);
        return NULL;
      }
      linea_heap = linea_heap_aux;
      tamanio *= 2;
    }
    leido += nueva_lectura;
  }

  if (leido == 0) {
    free(linea_heap);
    return NULL;
  }

  archivo += leido;

  return linea_heap;
}

/**
 * La funcion recibe el nombre y nivel validos y devuelve un pokemon inicializado 
 */
pokemon_t* crear_pokemon(char* nombre, size_t nivel, char* nombre_entrenador, size_t id_entrenador) {
  pokemon_t* nuevo_pokemon = malloc(sizeof(pokemon_t));
  if (!nuevo_pokemon) return NULL;

  nuevo_pokemon->nombre = nombre;
  nuevo_pokemon->nivel = nivel;
  nuevo_pokemon->entrenador.nombre = malloc(strlen(nombre_entrenador) + 1);
  if (!nuevo_pokemon->entrenador.nombre) {
    free(nuevo_pokemon);
    return NULL;
  }
  strcpy(nuevo_pokemon->entrenador.nombre, nombre_entrenador);
  nuevo_pokemon->entrenador.id = id_entrenador;
  return nuevo_pokemon;
}

/**
 * recibe un vector de pokemones con su cantidad, el vector de strings con los pokemones ingresados y la cantidad de nuevos 
 * pokemones (todos validados previamente)
 * 
 * agranda la reserva de memoria del vector y agrega los nuevos pokemones
 * si la reserva de momemoria falla devuelve NULL, caso contrario devuelve el puntero del vector agrandado
 */
abb_t* agregar_pokemones(abb_t* pokemones, char** ingreso_entrenador, size_t cant_nuevos_pokemones) {
  if (!pokemones) return NULL;

  char* nombre_entrenador = ingreso_entrenador[POSICION_ENTRENADOR_NOMBRE];
  size_t id_entrenador = (size_t)atoi(ingreso_entrenador[POSICION_ENTRENADOR_ID]);
  size_t ingreso_entrenador_posicion = POSICION_PRIMER_POKEMON;

  for (size_t i = 0; i < cant_nuevos_pokemones; i++) {
    pokemon_t* nuevo_pokemon = crear_pokemon(ingreso_entrenador[ingreso_entrenador_posicion],
                                             (size_t)atoi(ingreso_entrenador[ingreso_entrenador_posicion + 1]),
                                             nombre_entrenador, id_entrenador);
    if (!nuevo_pokemon) return NULL;

    abb_insertar(pokemones, nuevo_pokemon);
    ingreso_entrenador_posicion += 2;
  }

  return pokemones;
}

/**
 * Recibe un hospital valido, un vector de strings del ingreso de pokemones de un entrenador validado con su cantidad de elementos
 * 
 * La funcion se ocupa de interpretar el formato del string y almacenar los datos de sus pokemones en el hospital 
 * devolviendo el hospital actualizado o NULL en caso de error reservando memoria
 */
hospital_t* hospital_agregar_entrenador(hospital_t* hospital, char** ingreso_entrenador, size_t cantidad_elementos_leida) {
  // se le resta dos por el nombre y id del entrenador, el /2 es porque hay dos datos por entrenador
  size_t cant_nuevos_pokemones = (cantidad_elementos_leida - 2) / 2;
  abb_t* pokemones_aux = agregar_pokemones(hospital->pokemones, ingreso_entrenador, cant_nuevos_pokemones);
  if (!pokemones_aux) {
    return NULL;
  }

  hospital->pokemones = pokemones_aux;
  hospital->cantidad_entrenador++;

  return hospital;
}

/**
 * la funcion recibe el vector de strings valido con los datos del ingreso de pokemones por un entrenador leido del archivo al cual
 * va a liberar de memoria, tambien recibe un bool que marca si libera los nombres de la memoria o no (si no hay errores previos
 * se prevee que los punteros de strings se reutilizen en el hospital y no hay que liberarlos en esta instancia)
 */
void eliminar_informacion_ingreso_entrenador(char** ingreso_entrenador, bool liberar_strings) {
  free(ingreso_entrenador[POSICION_ENTRENADOR_ID]);
  free(ingreso_entrenador[POSICION_ENTRENADOR_NOMBRE]);

  size_t i = POSICION_PRIMER_POKEMON;
  while (ingreso_entrenador[i]) {
    if (liberar_strings) free(ingreso_entrenador[i]);
    free(ingreso_entrenador[i + 1]);
    i += 2;
  }
  free(ingreso_entrenador);
}

/**
 * Lee un archivo con entrenadores que hacen tratar a sus pokemon en el hospital y los agrega al mismo.
 *
 * Ver en el enunciado el formato del archivo. Si algo falla, el hospital no es modificado.
 *
 * En caso de error devuelve false. Caso contrario, true.
 */
bool hospital_leer_archivo(hospital_t* hospital, const char* nombre_archivo) {
  FILE* archivo = fopen(nombre_archivo, "r");
  if (!archivo) return false;
  char* linea = leer_linea_completa(archivo);

  while (linea) {
    size_t cantidad_elementos_leida = 0;
    char** ingreso_entrenador = split(linea, SEPARADOR_ARCHIVO);

    free(linea);
    if (!ingreso_entrenador) {
      fclose(archivo);
      return false;
    }

    while (ingreso_entrenador[cantidad_elementos_leida]) cantidad_elementos_leida++;

    // si no hay al menos dos elementos (nombre e id del entrenador) o la cantidad de elementos es
    // par (id y nombre del entrenador o nombre y nivel de pokemones), el archivo esta mal y se devuelve false liberando todo antes
    if (cantidad_elementos_leida < 2 || cantidad_elementos_leida % 2 != 0) {
      eliminar_informacion_ingreso_entrenador(ingreso_entrenador, true);
      fclose(archivo);
      return false;
    }

    hospital_t* hospital_aux = hospital_agregar_entrenador(hospital, ingreso_entrenador, cantidad_elementos_leida);
    if (!hospital_aux) {
      eliminar_informacion_ingreso_entrenador(ingreso_entrenador, true);
      fclose(archivo);
      return false;
    }

    eliminar_informacion_ingreso_entrenador(ingreso_entrenador, false);
    linea = leer_linea_completa(archivo);
  }

  fclose(archivo);

  return true;
}

/**
 * Devuelve la cantidad de pokemon que son atendidos actualmente en el hospital.
 */
size_t hospital_cantidad_pokemon(hospital_t* hospital) {
  if (!hospital) return 0;
  return abb_tamanio(hospital->pokemones);
}

/**
 * Devuelve la cantidad de entrenadores que actualmente hacen atender a sus
 * pokemon en el hospital.
 */
size_t hospital_cantidad_entrenadores(hospital_t* hospital) {
  if (!hospital) return 0;
  return hospital->cantidad_entrenador;
}

/**
 * Recibe un vector de pokemones y su  tope
 * 
 * La funcion se encarga de ordenar el vector alfabeticamente
 *  
 */

void ordenar_pokemones_alfabeticamente(pokemon_t* vector_pokemones, size_t cantidad_pokemon) {
  int i = 0;
  bool cambio = true;
  while (i < (int)cantidad_pokemon - 1 && cambio) {
    cambio = false;
    for (int j = 0; j < (int)cantidad_pokemon - i - 1; j++) {
      if (strcmp(vector_pokemones[j].nombre, vector_pokemones[j + 1].nombre) > 0) {
        pokemon_t aux = vector_pokemones[j];
        vector_pokemones[j] = vector_pokemones[j + 1];
        vector_pokemones[j + 1] = aux;
        cambio = true;
      }
    }
  }
}

/**
 * Aplica una función a cada uno de los pokemon almacenados en el hospital. La
 * función debe aplicarse a cada pokemon en orden alfabético.
 *
 * La función a aplicar recibe el pokemon y devuelve true o false. Si la función
 * devuelve true, se debe seguir aplicando la función a los próximos pokemon si
 * quedan. Si la función devuelve false, no se debe continuar.
 *
 * Devuelve la cantidad de pokemon a los que se les aplicó la función (hayan devuelto true o false).
 */
size_t hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p)) {
  if (!hospital || !funcion) return 0;

  return abb_con_cada_elemento(hospital->pokemones, INORDEN, (funcion_a_elemento)funcion, NULL);
}

/**
 *  Libera el hospital y toda la memoria utilizada por el mismo.
 */
void hospital_destruir(hospital_t* hospital) {
  if (!hospital) return;

  abb_destruir_todo(hospital->pokemones, pokemon_destruir);
  free(hospital);
}

/**
 * Devuelve el nivel de un pokemon o 0 si el pokemon es NULL.
 */
size_t pokemon_nivel(pokemon_t* pokemon) {
  if (!pokemon) return 0;
  return pokemon->nivel;
}

/**
 * Devuelve el nombre de un pokemon o NULL si el pokemon es NULL.
 */
const char* pokemon_nombre(pokemon_t* pokemon) {
  if (!pokemon) return NULL;
  return pokemon->nombre;
}

/**
 * Devuelve el entrenador de un pokemon o NULL si el pokemon es NULL.
 */
entrenador_t* pokemon_entrenador(pokemon_t* pokemon) {
  if (!pokemon) return NULL;
  return &(pokemon->entrenador);
}
