#include "simulador.h"

#include <string.h>

#include "cola.h"
#include "estructuras.h"
#include "heap.h"
#include "hospital.h"
#include "lista.h"

struct _simulador_t {
  bool activo;
  hospital_t* hospital;
  cola_t* entrenadores_espera;
  unsigned entrenadores_atendidos;
  heap_t* pokemones_espera;
  pokemon_t* pokemon_en_tratamiento;
  unsigned pokemones_atendidos;
  unsigned intentos;
  unsigned puntos;
  unsigned cantidad_eventos_simulados;
  lista_t* dificultades;
  unsigned dificultad_en_uso;
};

/*
 * Funcion que libera de memoria una dificultad y su nombre.
* Devuelve true y tiene un parametro inutilizado dedido a que
* se la hizo compatible con lista_con_cada_elemento 
*/
bool dificultad_destruir(void* dificultad, void* datos) {
  if (!dificultad) return true;
  free((char*)((DatosDificultad*)dificultad)->nombre);
  free((DatosDificultad*)dificultad);
  return true;
}

/*
 * Inicio Funciones para crear las dificultades con las funcionalidades indicadas 
 * en AgregarDificultad en eventos.txt
*/
unsigned calcular_puntaje_facil(unsigned cantidad_intentos) {
  if (cantidad_intentos > 10) return 50;
  return 100 - cantidad_intentos * 5;
};

unsigned calcular_puntaje_normal(unsigned cantidad_intentos) {
  if (cantidad_intentos > 8) return 20;
  return 100 - cantidad_intentos * 10;
};

unsigned calcular_puntaje_dificil(unsigned cantidad_intentos) {
  return cantidad_intentos > 0 ? 20 : 100;
};

int verificar_nivel(unsigned nivel_adivinado, unsigned nivel_pokemon) {
  return (int)nivel_adivinado - (int)nivel_pokemon;
};

const char* verificacion_a_string_facil(int resultado_verificacion) {
  if (resultado_verificacion == 0)
    return "Correcto";
  else if (resultado_verificacion > 20)
    return "Demasiado alto";
  else if (resultado_verificacion > 0)
    return "Un poco alto";
  else if (resultado_verificacion < -20)
    return "Demasiado bajo";
  return "Un poco bajo";
};

const char* verificacion_a_string_normal(int resultado_verificacion) {
  if (resultado_verificacion == 0) return "Correcto";
  if (resultado_verificacion > 0) return "Demasiado alto";
  return "Demasiado bajo";
};

const char* verificacion_a_string_dificil(int resultado_verificacion) {
  if (resultado_verificacion == 0) return "Correcto";
  return "Incorrecto";
};

/*
 * Crea una dificultad con los parametros recibidos y devuelve su puntero.
 */
DatosDificultad* dificultad_crear(const char* nombre,
                                  unsigned (*calcular_puntaje)(unsigned),
                                  int (*verificar_nivel)(unsigned, unsigned),
                                  const char* (*verificacion_a_string)(int)) {
  DatosDificultad* dificultad = malloc(sizeof(DatosDificultad));
  if (!dificultad) return NULL;

  char* dificultad_nombre = malloc(strlen(nombre) + 1);
  if (!nombre) {
    free(dificultad);
    return NULL;
  }

  strcpy(dificultad_nombre, nombre);
  dificultad->nombre = dificultad_nombre;
  dificultad->calcular_puntaje = calcular_puntaje;
  dificultad->verificar_nivel = verificar_nivel;
  dificultad->verificacion_a_string = verificacion_a_string;
  return dificultad;
};

/*
 * Fin Funciones para crear las dificultades con las funcionalidades indicadas 
 * en AgregarDificultad en eventos.txt
*/

/*
 * crea una lista de dificultades con las tres dificultades iniciales: facil, normal y
 * dificil. Estas utilizan unas funciones de calcular_puntaje, verificar_nivel y
 * verificacion_a_string que se encuentran arriba.
*/
lista_t* dificultades_iniciales_crear() {
  lista_t* dificultades = lista_crear();
  if (!dificultades) {
    return NULL;
  }

  DatosDificultad* dificultad_facil = dificultad_crear(
      "Fácil", calcular_puntaje_facil, verificar_nivel, verificacion_a_string_facil);
  DatosDificultad* dificultad_normal = dificultad_crear(
      "Normal", calcular_puntaje_normal, verificar_nivel, verificacion_a_string_normal);
  DatosDificultad* dificultad_dificil = dificultad_crear(
      "Difícil", calcular_puntaje_dificil, verificar_nivel, verificacion_a_string_dificil);

  if (!dificultad_facil || !dificultad_normal || !dificultad_dificil) {
    dificultad_destruir(dificultad_facil, NULL);
    dificultad_destruir(dificultad_normal, NULL);
    dificultad_destruir(dificultad_dificil, NULL);
    free(dificultades);

    return NULL;
  }

  if (!lista_insertar(dificultades, dificultad_facil) ||
      !lista_insertar(dificultades, dificultad_normal) ||
      !lista_insertar(dificultades, dificultad_dificil)) {
    dificultad_destruir(dificultad_facil, NULL);
    dificultad_destruir(dificultad_normal, NULL);
    dificultad_destruir(dificultad_dificil, NULL);
    free(dificultades);

    return NULL;
  }

  return dificultades;
}

/*
 * Funcion que se pasa a lista_con_cada_elemento que se encarga de guardar los 
 * entrenadores en la cola de espera
*/
bool entrenador_encolar(void* entrenador, void* entrenadores_espera) {
  cola_encolar((cola_t*)entrenadores_espera, (entrenador_t*)entrenador);
  return true;
}

/*
 * crea una cola con los entrenadores de un hospital que sera guardada en el simulador 
*/
cola_t* cargar_entrenadores_espera(lista_t* entrenadores_hospital) {
  cola_t* entrenadores_espera = cola_crear();
  if (!entrenadores_espera) {
    return NULL;
  }

  lista_con_cada_elemento(entrenadores_hospital, entrenador_encolar, entrenadores_espera);

  return entrenadores_espera;
}

/*
 * Funcion que compara dos pokemones por su nivel, devuelve mayor a 0 si el primero es
 * mayor al segundo, menor a 0 si el primero es menor al segundo y 0 si son iguales. 
*/
int pokemones_comparar_nivel(const void* pokemon1, const void* pokemon2) {
  return (int)((pokemon_t*)pokemon1)->nivel - (int)((pokemon_t*)pokemon2)->nivel;
}

/*
 * Funcion creada para abb_con_cada_elemento que inserta un pokemon en los pokemones
 * en espera si es que estos son del entrenador que esta siendo atendido 
*/
bool pokemon_a_espera(void* pokemon, void* simulador) {
  if (((pokemon_t*)pokemon)->entrenador == (entrenador_t*)cola_frente(((simulador_t*)simulador)->entrenadores_espera)) {
    heap_insertar(((simulador_t*)simulador)->pokemones_espera, pokemon);
  }
  return true;
}

/**
 * Crea un simulador para un hospital. El simulador toma control del hospital y
 * el mismo no debe ser modificado ni liberado por fuera del simulador.
 *
 * Devuelve el simulador creado o NULL en caso de error.
 */
simulador_t* simulador_crear(hospital_t* hospital) {
  if (!hospital) return NULL;

  simulador_t* simulador = calloc(1, sizeof(simulador_t));
  if (!simulador) return NULL;

  simulador->activo = true;
  simulador->hospital = hospital;
  simulador->dificultades = dificultades_iniciales_crear();
  simulador->entrenadores_espera = cargar_entrenadores_espera(hospital->entrenadores);
  simulador->pokemones_espera = heap_crear(pokemones_comparar_nivel);
  simulador->dificultad_en_uso = 1;

  if (!simulador->dificultades || !simulador->entrenadores_espera || !simulador->pokemones_espera) {
    free(simulador);
    lista_destruir(simulador->dificultades);
    lista_con_cada_elemento(simulador->dificultades, dificultad_destruir, NULL);
    cola_destruir(simulador->entrenadores_espera);
    heap_destruir(simulador->pokemones_espera);
    return NULL;
  }

  return simulador;
}

/*
 * Se completan las estadisticas con todos los datos de la simulacion actual siguiendo 
 * con lo planteado como ObtenerEstadisticas en eventos.txt
 * 
 * Si el puntero es nulo se devuelve ErrorSimulacion
 * 
*/
ResultadoSimulacion obtener_estadisticas(simulador_t* simulador, EstadisticasSimulacion* estadisticas) {
  if (!estadisticas) return ErrorSimulacion;

  estadisticas->entrenadores_atendidos = simulador->entrenadores_atendidos;
  estadisticas->entrenadores_totales = (unsigned)hospital_cantidad_entrenadores(simulador->hospital);
  estadisticas->pokemon_atendidos = simulador->pokemones_atendidos;
  estadisticas->pokemon_totales = (unsigned)hospital_cantidad_pokemon(simulador->hospital);
  estadisticas->pokemon_en_espera = (unsigned)heap_cantidad(simulador->pokemones_espera);
  estadisticas->puntos = simulador->puntos;
  estadisticas->cantidad_eventos_simulados = simulador->cantidad_eventos_simulados;

  return ExitoSimulacion;
}

/*
 * Agrega los pokemones del proximo entrenador por antender a los pokemones en espera
 * y quita al entrenador de los entrenadores en espera siguiendo con el evento de la
 * simulacion planteado como AtenderProximoEntrenador en eventos.txt
 * 
 * Si no hay ningun pokemon en tratamiento, se elige al de menor nivel y se lo atiende.
 * 
 * Si no hay entrenadores en espera, se devuelve ErrorSimulacion
*/
ResultadoSimulacion atender_proximo_entrenador(simulador_t* simulador) {
  if (!cola_frente(simulador->entrenadores_espera)) return ErrorSimulacion;

  abb_con_cada_elemento(simulador->hospital->pokemones, INORDEN, pokemon_a_espera, simulador);

  if (!simulador->pokemon_en_tratamiento) {
    simulador->pokemon_en_tratamiento = heap_extraer_raiz(simulador->pokemones_espera);
  }

  cola_desencolar(simulador->entrenadores_espera);
  simulador->entrenadores_atendidos++;

  return ExitoSimulacion;
}

/*
 * Rellena la informacion del pokemon en tratamiento de un simulador implementando
 * el evento de la simulacion planteado como ObtenerInformacionPokemonEnTratamiento en
 * eventos.txt
 * 
 * Si el no hay ningun pokemon en tratamiento o el puntero es nulo, se devuelve ErrorSimulacion
*/
ResultadoSimulacion obtener_informacion_pokemon_en_tratamiento(simulador_t* simulador,
                                                               InformacionPokemon* informacion) {
  if (!informacion) return ErrorSimulacion;

  if (!simulador->pokemon_en_tratamiento) {
    informacion->nombre_pokemon = NULL;
    informacion->nombre_entrenador = NULL;
    return ErrorSimulacion;
  }

  informacion->nombre_pokemon = simulador->pokemon_en_tratamiento->nombre;
  informacion->nombre_entrenador = pokemon_entrenador(simulador->pokemon_en_tratamiento)->nombre;

  return ExitoSimulacion;
}

/*
 * Verifica mediante las funciones provistas por la dificultad en uso si el intento
 * de adivinar el nivel de pokemon es exitoso implementando el evento de la 
 * simulacion planteado como AdivinarNivelPokemon en eventos.txt.
 * 
 * En caso de que sea exitoso tambien se procede a poner en tratamiento al siguiente
 * pokemon de menor nivel en espera y reiniciar los intentos.
 * 
 * Si el intento es nulo o no hay ningun pokemon en tratamiento, se devuelve ErrorSimulacion
*/
ResultadoSimulacion adivinar_nivel_pokemon(simulador_t* simulador, Intento* intento) {
  if (!intento || !simulador->pokemon_en_tratamiento) return ErrorSimulacion;

  DatosDificultad* dificultad_actual =
      lista_elemento_en_posicion(simulador->dificultades, simulador->dificultad_en_uso);

  int resultado =
      dificultad_actual->verificar_nivel(intento->nivel_adivinado,
                                         (unsigned)simulador->pokemon_en_tratamiento->nivel);

  intento->es_correcto = resultado == 0;
  intento->resultado_string = dificultad_actual->verificacion_a_string(resultado);
  simulador->intentos++;

  if (intento->es_correcto) {
    simulador->puntos += dificultad_actual->calcular_puntaje(simulador->intentos);
    simulador->pokemones_atendidos++;
    simulador->pokemon_en_tratamiento = heap_extraer_raiz(simulador->pokemones_espera);
    simulador->intentos = 0;
  }

  return ExitoSimulacion;
}

/*
 * Agrega  la dificultad recibida al simulador implementando el evento de la 
 * simulacion planteado como AgregarDificultad en eventos.txt
 * 
 * Si hay algun valor no valido o el nombre ya se encuentra utlizado,
 * se devuelve ErrorSimulacion
*/
ResultadoSimulacion agregar_dificultad(simulador_t* simulador, DatosDificultad* dificultad) {
  if (!dificultad || !dificultad->nombre ||
      !dificultad->verificacion_a_string ||
      !dificultad->calcular_puntaje ||
      !dificultad->verificar_nivel) return ErrorSimulacion;

  lista_iterador_t* it;
  bool repetido = false;

  for (it = lista_iterador_crear(simulador->dificultades);
       lista_iterador_tiene_siguiente(it) && !repetido;
       lista_iterador_avanzar(it)) {
    if (strcmp(dificultad->nombre, ((DatosDificultad*)lista_iterador_elemento_actual(it))->nombre) == 0)
      repetido = true;
  }
  lista_iterador_destruir(it);

  if (repetido) return ErrorSimulacion;

  DatosDificultad* dificultad_nueva = dificultad_crear(dificultad->nombre,
                                                       dificultad->calcular_puntaje,
                                                       dificultad->verificar_nivel,
                                                       dificultad->verificacion_a_string);

  if (!lista_insertar(simulador->dificultades, dificultad_nueva)) {
    dificultad_destruir(dificultad_nueva, NULL);
    return ErrorSimulacion;
  }
  return ExitoSimulacion;
}

/*
 * Cambia la dificultad de un simulador al del id recibido implementando
 * el evento de la simulacion planteado como SeleccionarDificultad en
 * eventos.txt
 * 
 * Si el id no corresponde a ninguna dificultad, se devuelve ErrorSimulacion
*/
ResultadoSimulacion seleccionar_dificultad(simulador_t* simulador, int* id) {
  if (!id) return ErrorSimulacion;

  DatosDificultad* dificultad = lista_elemento_en_posicion(simulador->dificultades, (size_t)*id);
  if (!dificultad) return ErrorSimulacion;

  simulador->dificultad_en_uso = (unsigned)*id;

  return ExitoSimulacion;
};

/*
 * Rellena la informacion de la dificultad actual de un simulador implementando
 * el evento de la simulacion planteado como ObtenerInformacionDificultad en
 * eventos.txt
 * 
 * Si el id no corresponde a ninguna dificultad o el puntero sea nulo, se devuelve
 * ErrorSimulacion
*/
ResultadoSimulacion obtener_informacion_dificultad(simulador_t* simulador,
                                                   InformacionDificultad* informacion) {
  if (!informacion) return ErrorSimulacion;

  DatosDificultad* dificultad =
      lista_elemento_en_posicion(simulador->dificultades, (size_t)informacion->id);
  if (!dificultad) {
    informacion->id = -1;
    informacion->nombre_dificultad = NULL;
    informacion->en_uso = false;
    return ErrorSimulacion;
  }

  informacion->nombre_dificultad = dificultad->nombre;
  informacion->en_uso = simulador->dificultad_en_uso == (unsigned)informacion->id;

  return ExitoSimulacion;
}

/*
 * Recibe un simulador y lo configura como inactivo, a partir de ahora
 * no se pueden realizar ninguna operacion sobre el y estas retornaran 
 * ErrorSimulacion.
 * 
*/
ResultadoSimulacion finalizar_simulacion(simulador_t* simulador) {
  simulador->activo = false;
  return ExitoSimulacion;
}

/**
 * Simula un evento. Cada evento puede recibir un puntero a un dato que depende
 * de cada evento. En caso de no necesitarse, se debe pasar NULL.
 *
 * Ejecuta el evento correspondiente (ver la documentación) y en caso de ser
 * necesario modifica la información apuntada por dato.
 *
 * Devuelve ErrorSimulacion si no existe el simulador o existe algún error en el
 * manejo de memoria.
 *
 * Devuelve ExitoSimulacion o ErrorSimulacion según corresponda a cada evento.
 */
ResultadoSimulacion simulador_simular_evento(simulador_t* simulador,
                                             EventoSimulacion evento,
                                             void* datos) {
  if (!simulador || !simulador->activo) return ErrorSimulacion;

  simulador->cantidad_eventos_simulados++;
  ResultadoSimulacion resultado;

  switch (evento) {
    case ObtenerEstadisticas:
      resultado = obtener_estadisticas(simulador, datos);
      break;
    case AtenderProximoEntrenador:
      resultado = atender_proximo_entrenador(simulador);
      break;
    case ObtenerInformacionPokemonEnTratamiento:
      resultado = obtener_informacion_pokemon_en_tratamiento(simulador, datos);
      break;
    case AdivinarNivelPokemon:
      resultado = adivinar_nivel_pokemon(simulador, datos);
      break;
    case AgregarDificultad:
      resultado = agregar_dificultad(simulador, datos);
      break;
    case SeleccionarDificultad:
      resultado = seleccionar_dificultad(simulador, datos);
      break;
    case ObtenerInformacionDificultad:
      resultado = obtener_informacion_dificultad(simulador, datos);
      break;
    case FinalizarSimulacion:
      resultado = finalizar_simulacion(simulador);
      break;
    default:
      resultado = ErrorSimulacion;
  }

  return resultado;
}

/**
 * Destruye el simulador y libera la memoria asociada (incluida la del hospital).
 */
void simulador_destruir(simulador_t* simulador) {
  if (!simulador) return;

  hospital_destruir(simulador->hospital);
  cola_destruir(simulador->entrenadores_espera);
  lista_con_cada_elemento(simulador->dificultades, dificultad_destruir, NULL);
  heap_destruir(simulador->pokemones_espera);
  lista_destruir(simulador->dificultades);
  free(simulador);
}
