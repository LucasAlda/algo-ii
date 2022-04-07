#include <stdbool.h>

#include "estructuras.h"
#include "pa2mm.h"
#include "src/hospital.h"
#include "src/simulador.h"
#include "string.h"

bool ignorar_pokemon(pokemon_t* p) {
  p = p;
  return true;
}

/* No intenten esto en sus casas */
/* Ya vamos a ver como evitar esto en el TDA Lista */
struct {
  pokemon_t* pokemon[500];
  size_t cantidad;
} acumulados;

void resetear_acumulados() {
  acumulados.cantidad = 0;
}

bool acumular_pokemon(pokemon_t* p) {
  acumulados.pokemon[acumulados.cantidad] = p;
  acumulados.cantidad++;
  return true;
}

bool acumular_pokemon_hasta_miltank(pokemon_t* p) {
  acumulados.pokemon[acumulados.cantidad] = p;
  acumulados.cantidad++;
  return strcmp(pokemon_nombre(p), "miltank");
}

bool acumulados_en_orden_correcto() {
  if (acumulados.cantidad < 2)
    return true;
  pokemon_t* anterior = acumulados.pokemon[0];
  for (int i = 1; i < acumulados.cantidad; i++) {
    pokemon_t* actual = acumulados.pokemon[i];
    if (strcmp(pokemon_nombre(anterior), pokemon_nombre(actual)) > 0)
      return false;
  }
  return true;
}

/* Pruebas */

void puedoCrearYDestruirUnHospital() {
  hospital_t* h = NULL;

  pa2m_afirmar((h = hospital_crear()), "Crear un hospital devuelve un hospital");

  pa2m_afirmar(hospital_cantidad_entrenadores(h) == 0, "Un hospital se crea con cero entrenadores");
  pa2m_afirmar(hospital_cantidad_pokemon(h) == 0, "Un hospital se crea con cero pokemon");

  pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon) == 0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

  hospital_destruir(h);
}

void dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema() {
  hospital_t* h = NULL;

  pa2m_afirmar(hospital_cantidad_entrenadores(h) == 0, "Un hospital NULL tiene cero entrenadores");
  pa2m_afirmar(hospital_cantidad_pokemon(h) == 0, "Un hospital NULL tiene cero pokemon");

  pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon) == 0, "Recorrer los pokemon de un hospital NULL resulta en 0 pokemon recorridos");

  hospital_destruir(h);
}

void dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital() {
  hospital_t* h = hospital_crear();

  pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/archivo_vacio.hospital"), "Puedo leer un archivo vacío");

  pa2m_afirmar(hospital_cantidad_entrenadores(h) == 0, "Un hospital vacío tiene cero entrenadores");
  pa2m_afirmar(hospital_cantidad_pokemon(h) == 0, "Un hospital vacío tiene tiene cero pokemon");

  pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon) == 0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

  hospital_destruir(h);
}

void dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital() {
  hospital_t* h = hospital_crear();

  pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/un_entrenador.hospital"), "Puedo leer un archivo con un entrenador");

  pa2m_afirmar(hospital_cantidad_entrenadores(h) == 1, "El hospital tiene 1 entrenador");
  pa2m_afirmar(hospital_cantidad_pokemon(h) == 3, "El hospital tiene 3 pokemon");

  resetear_acumulados();
  pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon) == 3, "Recorrer los pokemon resulta en 3 pokemon recorridos");
  pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

  hospital_destruir(h);
}

void dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital() {
  hospital_t* h = hospital_crear();

  pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");

  pa2m_afirmar(hospital_cantidad_entrenadores(h) == 5, "El hospital tiene 5 entrenadores");
  pa2m_afirmar(hospital_cantidad_pokemon(h) == 24, "El hospital tiene 24 pokemon");

  resetear_acumulados();
  pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon) == 24, "Recorrer los pokemon resulta en 24 pokemon recorridos");
  pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

  hospital_destruir(h);
}

void dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital() {
  hospital_t* h = hospital_crear();

  pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");
  pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer otro archivo con varios entrenadores");
  pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un tercer archivo con varios entrenadores");

  pa2m_afirmar(hospital_cantidad_entrenadores(h) == 15, "El hospital tiene 15 entrenadores");
  pa2m_afirmar(hospital_cantidad_pokemon(h) == 72, "El hospital tiene 72 pokemon");

  resetear_acumulados();
  pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon) == 72, "Recorrer los pokemon resulta en 72 pokemon recorridos");
  pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

  hospital_destruir(h);
}

void puedoCrearYDestruirUnSimulador() {
  hospital_t* hospital = hospital_crear();
  simulador_t* simulador = NULL;

  pa2m_afirmar((simulador = simulador_crear(hospital)), "Crear un simulador devuelve un simulador");
  pa2m_afirmar((true), "Puedo destruir un simulador (con el hospital incluido) sin perder memoria");

  simulador_destruir(simulador);
}

unsigned calcular_puntaje(unsigned cantidad_intentos) {
  return cantidad_intentos > 0 ? 20 : 100;
};

int verificar_nivel_nuevo(unsigned nivel_adivinado, unsigned nivel_pokemon) {
  return (int)nivel_adivinado - (int)nivel_pokemon;
};

const char* verificacion_a_string(int resultado_verificacion) {
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

void puedoObtenerEstadisticasYDificultades() {
  hospital_t* hospital = hospital_crear();

  EstadisticasSimulacion e;
  simulador_t* simulador = NULL;

  pa2m_afirmar((simulador = simulador_crear(hospital)), "Crear un simulador sin entrenadores");
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &e) == ExitoSimulacion, "Obtener estadisticas");
  pa2m_afirmar((e.entrenadores_atendidos == 0), "Cantidad de entrenadores atendidos es 0");
  pa2m_afirmar((e.entrenadores_totales == 0), "Cantidad de entrenadores totales es 0");
  pa2m_afirmar((e.pokemon_atendidos == 0), "Cantidad de pokemon atendidos es 0");
  pa2m_afirmar((e.pokemon_totales == 0), "Cantidad de pokemon totales es 0");
  pa2m_afirmar((e.pokemon_en_espera == 0), "Cantidad de pokemon en espera es 0");

  printf("\n");

  InformacionDificultad iD;

  iD.id = 0;
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionDificultad, &iD) == ExitoSimulacion, "Obtener Informacion Dificultad Facil es ExitoSimulacion");
  pa2m_afirmar((iD.id == 0 && strcmp(iD.nombre_dificultad, "Fácil") == 0 && iD.en_uso == false), "Informacion Dificultad Facil correcta");
  iD.id = 1;
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionDificultad, &iD) == ExitoSimulacion, "Obtener Informacion Dificultad Normal es ExitoSimulacion");
  pa2m_afirmar((iD.id == 1 && strcmp(iD.nombre_dificultad, "Normal") == 0), "Informacion Dificultad Normal correcta");
  pa2m_afirmar((iD.en_uso == true), "Dificultad Normal en Uso");
  iD.id = 2;
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionDificultad, &iD) == ExitoSimulacion, "Obtener Informacion Dificultad Dificil es ExitoSimulacion");
  pa2m_afirmar((iD.id == 2 && strcmp(iD.nombre_dificultad, "Difícil") == 0 && iD.en_uso == false), "Informacion Dificultad Dificil correcta");

  printf("\n");

  iD.id = 100;
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionDificultad, &iD) == ErrorSimulacion, "Obtener Informacion Dificultad id 100 es ErrorSimulacion");
  pa2m_afirmar((iD.id == -1), "Informacion Dificultad id es -1");
  pa2m_afirmar((iD.nombre_dificultad == NULL), "Informacion Dificultad nombre es NULL");
  pa2m_afirmar((iD.en_uso == false), "Informacion Dificultad en uso es false");

  printf("\n");

  simulador_simular_evento(simulador, ObtenerEstadisticas, &e);
  pa2m_afirmar((e.cantidad_eventos_simulados == 6), "Cantidad de eventos simulados es 6");
  pa2m_afirmar((simulador_simular_evento(simulador, FinalizarSimulacion, NULL) == ExitoSimulacion), "Finalizar simulacion es ExitoSimulacion");

  simulador_destruir(simulador);
}

void atenderHospitalUnEntrenador() {
  hospital_t* hospital = hospital_crear();
  hospital_leer_archivo(hospital, "ejemplos/un_entrenador.hospital");

  EstadisticasSimulacion e;
  InformacionPokemon iP;
  simulador_t* simulador = NULL;

  pa2m_afirmar((simulador = simulador_crear(hospital)), "Crear un simulador con un entrenador");
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &e) == ExitoSimulacion, "Obtener estadisticas");
  pa2m_afirmar((e.entrenadores_atendidos == 0), "Cantidad de entrenadores atendidos es 0");
  pa2m_afirmar((e.entrenadores_totales == 1), "Cantidad de entrenadores totales es 1");
  pa2m_afirmar((e.pokemon_atendidos == 0), "Cantidad de pokemon atendidos es 0");
  pa2m_afirmar((e.pokemon_totales == 3), "Cantidad de pokemon totales es 3");
  pa2m_afirmar((e.pokemon_en_espera == 0), "Cantidad de pokemon en espera es 0");

  printf("\n");

  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &iP) == ErrorSimulacion, "Obtener Informacion Pokemon en tratamiento ErrorSimulacion (no hay)");
  pa2m_afirmar((iP.nombre_entrenador == NULL && iP.nombre_pokemon == NULL), "Pokemon en tratamiento valores nulos");

  printf("\n");

  pa2m_afirmar(simulador_simular_evento(simulador, AtenderProximoEntrenador, &e) == ExitoSimulacion, "Atender primer entrenador");
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &e) == ExitoSimulacion, "Obtener estadisticas de nuevo");
  pa2m_afirmar((e.entrenadores_atendidos == 1), "Cantidad de entrenadores atendidos es 1");
  pa2m_afirmar((e.entrenadores_totales == 1), "Cantidad de entrenadores totales es 1");
  pa2m_afirmar((e.pokemon_atendidos == 0), "Cantidad de pokemon atendidos es 0");
  pa2m_afirmar((e.pokemon_totales == 3), "Cantidad de pokemon totales es 3");
  pa2m_afirmar((e.pokemon_en_espera == 2), "Cantidad de pokemon en espera es 2");

  printf("\n");

  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &iP) == ExitoSimulacion, "Obtener Informacion Pokemon en tratamiento ExitoSimulacion");
  pa2m_afirmar((strcmp(iP.nombre_pokemon, "lapras") == 0), "Pokemon en tratamiento nombre lapras");
  pa2m_afirmar((strcmp(iP.nombre_entrenador, "Mariano") == 0), "Pokemon en tratamiento entrenador Mariano");

  printf("\n");

  printf("\n");

  simulador_simular_evento(simulador, ObtenerEstadisticas, &e);
  pa2m_afirmar((e.cantidad_eventos_simulados == 6), "Cantidad de eventos simulados es 6");
  pa2m_afirmar((simulador_simular_evento(simulador, FinalizarSimulacion, NULL) == ExitoSimulacion), "Finalizar simulacion es ExitoSimulacion");

  simulador_destruir(simulador);
}

void atenderHospitalConVariosEntrenadores() {
  hospital_t* hospital = hospital_crear();
  hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

  EstadisticasSimulacion e;
  InformacionPokemon iP;
  simulador_t* simulador = NULL;

  pa2m_afirmar((simulador = simulador_crear(hospital)), "Crear un simulador con varios entrenadores");
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &e) == ExitoSimulacion, "Obtener estadisticas");
  pa2m_afirmar((e.entrenadores_atendidos == 0), "Cantidad de entrenadores atendidos es 0");
  pa2m_afirmar((e.entrenadores_totales == 5), "Cantidad de entrenadores totales es 5");
  pa2m_afirmar((e.pokemon_atendidos == 0), "Cantidad de pokemon atendidos es 0");
  pa2m_afirmar((e.pokemon_totales == 24), "Cantidad de pokemon totales es 24");
  pa2m_afirmar((e.pokemon_en_espera == 0), "Cantidad de pokemon en espera es 0");

  printf("\n");

  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &iP) == ErrorSimulacion, "Obtener Informacion Pokemon en tratamiento ErrorSimulacion (no hay)");
  pa2m_afirmar((iP.nombre_entrenador == NULL && iP.nombre_pokemon == NULL), "Pokemon en tratamiento valores nulos");

  printf("\n");

  pa2m_afirmar(simulador_simular_evento(simulador, AtenderProximoEntrenador, &e) == ExitoSimulacion, "Atender primer entrenador");
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &e) == ExitoSimulacion, "Obtener estadisticas de nuevo");
  pa2m_afirmar((e.entrenadores_atendidos == 1), "Cantidad de entrenadores atendidos es 1");
  pa2m_afirmar((e.entrenadores_totales == 5), "Cantidad de entrenadores totales es 5");
  pa2m_afirmar((e.pokemon_atendidos == 0), "Cantidad de pokemon atendidos es 0");
  pa2m_afirmar((e.pokemon_totales == 24), "Cantidad de pokemon totales es 24");
  pa2m_afirmar((e.pokemon_en_espera == 3), "Cantidad de pokemon en espera es 3");

  printf("\n");

  pa2m_afirmar(simulador_simular_evento(simulador, AtenderProximoEntrenador, &e) == ExitoSimulacion, "Atender segundo entrenador");
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &e) == ExitoSimulacion, "Obtener estadisticas de nuevo");
  pa2m_afirmar((e.entrenadores_atendidos == 2), "Cantidad de entrenadores atendidos es 2");
  pa2m_afirmar((e.entrenadores_totales == 5), "Cantidad de entrenadores totales es 5");
  pa2m_afirmar((e.pokemon_atendidos == 0), "Cantidad de pokemon atendidos es 0");
  pa2m_afirmar((e.pokemon_totales == 24), "Cantidad de pokemon totales es 24");
  pa2m_afirmar((e.pokemon_en_espera == 7), "Cantidad de pokemon en espera es 7");

  printf("\n");

  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &iP) == ExitoSimulacion, "Obtener Informacion Pokemon en tratamiento ExitoSimulacion");
  pa2m_afirmar((strcmp(iP.nombre_pokemon, "rampardos") == 0), "Pokemon en tratamiento nombre rampardos");
  pa2m_afirmar((strcmp(iP.nombre_entrenador, "lucas") == 0), "Pokemon en tratamiento entrenador lucas");

  printf("\n");

  pa2m_afirmar((simulador_simular_evento(simulador, AtenderProximoEntrenador, &e) == ExitoSimulacion && simulador_simular_evento(simulador, AtenderProximoEntrenador, &e) == ExitoSimulacion && simulador_simular_evento(simulador, AtenderProximoEntrenador, &e) == ExitoSimulacion), "Atender entrenadores restantes");
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &e) == ExitoSimulacion, "Obtener estadisticas de nuevo");
  pa2m_afirmar((e.entrenadores_atendidos == 5), "Cantidad de entrenadores atendidos es 5");
  pa2m_afirmar((e.entrenadores_totales == 5), "Cantidad de entrenadores totales es 5");
  pa2m_afirmar((e.pokemon_atendidos == 0), "Cantidad de pokemon atendidos es 0");
  pa2m_afirmar((e.pokemon_totales == 24), "Cantidad de pokemon totales es 24");
  pa2m_afirmar((e.pokemon_en_espera == 23), "Cantidad de pokemon en espera es 23");

  printf("\n");

  pa2m_afirmar(simulador_simular_evento(simulador, AtenderProximoEntrenador, &e) == ErrorSimulacion, "Atender entrenador mas ErrorSimulacion (no hay)");

  printf("\n");

  simulador_simular_evento(simulador, ObtenerEstadisticas, &e);
  pa2m_afirmar((e.cantidad_eventos_simulados == 13), "Cantidad de eventos simulados es 13");
  pa2m_afirmar((simulador_simular_evento(simulador, FinalizarSimulacion, NULL) == ExitoSimulacion), "Finalizar simulacion es ExitoSimulacion");

  simulador_destruir(simulador);
}

void adivinarPokemon() {
  hospital_t* hospital = hospital_crear();
  hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

  EstadisticasSimulacion e;
  Intento intento;
  InformacionPokemon iP;
  simulador_t* simulador = NULL;

  pa2m_afirmar((simulador = simulador_crear(hospital)), "Crear un simulador con varios entrenadores");
  pa2m_afirmar(simulador_simular_evento(simulador, AtenderProximoEntrenador, &e) == ExitoSimulacion, "Atender primer entrenador");
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &iP) == ExitoSimulacion, "Obtener Informacion Pokemon en tratamiento ExitoSimulacion");
  pa2m_afirmar((strcmp(iP.nombre_pokemon, "rampardos") == 0), "Pokemon en tratamiento nombre rampardos");
  pa2m_afirmar((strcmp(iP.nombre_entrenador, "lucas") == 0), "Pokemon en tratamiento entrenador lucas");
  printf("\n");

  intento.nivel_adivinado = 10;
  pa2m_afirmar((simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento) == ExitoSimulacion), "Adivinar pokemon resultado correcto ExitoSimulacion");
  pa2m_afirmar((intento.es_correcto), "Intento es correcto");
  pa2m_afirmar((strlen(intento.resultado_string) > 0), "Resultado string contiene un mensaje");
  printf("\n");

  simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &iP);
  pa2m_afirmar((strcmp(iP.nombre_pokemon, "charizard") == 0 && strcmp(iP.nombre_entrenador, "lucas") == 0), "Nuevo pokemon en tratamiento es charizard (lucas)");
  printf("\n");

  intento.nivel_adivinado = 15;
  pa2m_afirmar((simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento) == ExitoSimulacion), "Adivinar pokemon resultado bajo ExitoSimulacion");
  pa2m_afirmar((!intento.es_correcto), "Intento no es correcto");
  pa2m_afirmar((strlen(intento.resultado_string) > 0), "Resultado string contiene un mensaje");
  printf("\n");

  intento.nivel_adivinado = 25;
  pa2m_afirmar((simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento) == ExitoSimulacion), "Adivinar pokemon resultado alto ExitoSimulacion");
  pa2m_afirmar((!intento.es_correcto), "Intento no es correcto");
  pa2m_afirmar((strlen(intento.resultado_string) > 0), "Resultado string contiene un mensaje");
  printf("\n");

  intento.nivel_adivinado = 20;
  pa2m_afirmar((simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento) == ExitoSimulacion), "Adivinar pokemon resultado correcto nuevamente ExitoSimulacion");
  pa2m_afirmar((intento.es_correcto), "Intento es correcto");
  pa2m_afirmar((strlen(intento.resultado_string) > 0), "Resultado string contiene un mensaje");
  printf("\n");

  intento.nivel_adivinado = 43;
  pa2m_afirmar((simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento) == ExitoSimulacion), "Adivinar pokemon resultado correcto nuevamente ExitoSimulacion");
  intento.nivel_adivinado = 85;
  pa2m_afirmar((simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento) == ExitoSimulacion), "Adivinar pokemon resultado correcto nuevamente ExitoSimulacion");
  printf("\n");

  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &e) == ExitoSimulacion, "Obtener estadisticas de nuevo");
  pa2m_afirmar((e.entrenadores_atendidos == 1), "Cantidad de entrenadores atendidos es 1");
  pa2m_afirmar((e.entrenadores_totales == 5), "Cantidad de entrenadores totales es 5");
  pa2m_afirmar((e.pokemon_atendidos == 4), "Cantidad de pokemon atendidos es 4");
  pa2m_afirmar((e.pokemon_totales == 24), "Cantidad de pokemon totales es 24");
  pa2m_afirmar((e.pokemon_en_espera == 0), "Cantidad de pokemon en espera es 0");
  printf("\n");

  pa2m_afirmar((simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento) == ErrorSimulacion), "Adivinar pokemon cuando no quedan ErrorSimulacion");
  printf("\n");

  simulador_simular_evento(simulador, ObtenerEstadisticas, &e);
  pa2m_afirmar((e.puntos > 0), "Cantidad de puntos es mayor a 0");
  pa2m_afirmar((e.cantidad_eventos_simulados == 12), "Cantidad de eventos simulados es 12");
  pa2m_afirmar((simulador_simular_evento(simulador, FinalizarSimulacion, NULL) == ExitoSimulacion), "Finalizar simulacion es ExitoSimulacion");

  simulador_destruir(simulador);
}

void manejarDificultades() {
  hospital_t* hospital = hospital_crear();
  hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

  InformacionDificultad iD;
  simulador_t* simulador = NULL;

  pa2m_afirmar((simulador = simulador_crear(hospital)), "Crear un simulador");

  iD.id = 0;
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionDificultad, &iD) == ExitoSimulacion, "Obtener Informacion Dificultad Facil");
  pa2m_afirmar((iD.id == 0 && strcmp(iD.nombre_dificultad, "Fácil") == 0 && iD.en_uso == false), "Informacion Dificultad Facil correcta");
  iD.id = 1;
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionDificultad, &iD) == ExitoSimulacion, "Obtener Informacion Dificultad Normal");
  pa2m_afirmar((iD.id == 1 && strcmp(iD.nombre_dificultad, "Normal") == 0 && iD.en_uso == true), "Informacion Dificultad Normal correcta");
  iD.id = 2;
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionDificultad, &iD) == ExitoSimulacion, "Obtener Informacion Dificultad Dificil");
  pa2m_afirmar((iD.id == 2 && strcmp(iD.nombre_dificultad, "Difícil") == 0 && iD.en_uso == false), "Informacion Dificultad Dificil correcta");
  printf("\n");

  int dificultad_a_elegir = 0;
  iD.id = 0;
  pa2m_afirmar(simulador_simular_evento(simulador, SeleccionarDificultad, &dificultad_a_elegir) == ExitoSimulacion, "Seleccionar Dificultad Facil");
  simulador_simular_evento(simulador, ObtenerInformacionDificultad, &iD);
  pa2m_afirmar((iD.id == 0 && strcmp(iD.nombre_dificultad, "Fácil") == 0 && iD.en_uso == true), "Dificultad Facil seleccionada");

  DatosDificultad* dificultad_nueva = malloc(sizeof(DatosDificultad));
  dificultad_nueva->calcular_puntaje = calcular_puntaje;
  dificultad_nueva->verificar_nivel = verificar_nivel_nuevo;
  dificultad_nueva->verificacion_a_string = verificacion_a_string;
  dificultad_nueva->nombre = "Imposible";
  printf("\n");

  iD.id = 3;
  pa2m_afirmar(simulador_simular_evento(simulador, AgregarDificultad, dificultad_nueva) == ExitoSimulacion, "Agregar Dificultad Imposible");
  simulador_simular_evento(simulador, ObtenerInformacionDificultad, &iD);
  pa2m_afirmar((iD.id == 3 && strcmp(iD.nombre_dificultad, "Imposible") == 0 && iD.en_uso == false), "Dificultad Imposible correcta");
  printf("\n");

  dificultad_a_elegir = 100;
  pa2m_afirmar(simulador_simular_evento(simulador, SeleccionarDificultad, &dificultad_a_elegir) == ErrorSimulacion, "Seleccionar Dificultad Inexistente ErrorSimulacion");
  iD.id = 0;
  simulador_simular_evento(simulador, ObtenerInformacionDificultad, &iD);
  pa2m_afirmar((iD.id == 0 && strcmp(iD.nombre_dificultad, "Fácil") == 0 && iD.en_uso == true), "Dificultad Facil continua seleccionada");

  simulador_destruir(simulador);
  free(dificultad_nueva);
}

void pruebasNulls() {
  simulador_t* simulador = NULL;
  simulador = simulador_crear(NULL);

  pa2m_afirmar((!simulador), "Crear un simulador sin hospital es NULL");
  pa2m_afirmar(simulador_simular_evento(NULL, ObtenerEstadisticas, NULL) == ErrorSimulacion, "Obtener Estadisticas con NULL simulador ErrorSimulacion");
  pa2m_afirmar(simulador_simular_evento(NULL, FinalizarSimulacion, NULL) == ErrorSimulacion, "Finalizar Simulacion con NULL simulador ErrorSimulacion");
  pa2m_afirmar(simulador_simular_evento(NULL, SeleccionarDificultad, NULL) == ErrorSimulacion, "Seleccionar Dificultad con NULL simulador ErrorSimulacion");
  pa2m_afirmar(simulador_simular_evento(NULL, AgregarDificultad, NULL) == ErrorSimulacion, "Agregar Dificultad con NULL simulador ErrorSimulacion");
  pa2m_afirmar(simulador_simular_evento(NULL, ObtenerInformacionDificultad, NULL) == ErrorSimulacion, "Obtener Informacion Dificultad con NULL simulador ErrorSimulacion");
  pa2m_afirmar(simulador_simular_evento(NULL, AdivinarNivelPokemon, NULL) == ErrorSimulacion, "Adivinar Pokemon con NULL simulador ErrorSimulacion");
  printf("\n");
  simulador = simulador_crear(hospital_crear());
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, NULL) == ErrorSimulacion, "Obtener Estadisticas con EstadisticasSimulacion NULL ErrorSimulacion");
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, NULL) == ErrorSimulacion, "Obtener Info Pokemon con InformacionPokemon NULL ErrorSimulacion");
  pa2m_afirmar(simulador_simular_evento(simulador, AdivinarNivelPokemon, NULL) == ErrorSimulacion, "AdivinarNivelPokemon con Intento NULL ErrorSimulacion");
  pa2m_afirmar(simulador_simular_evento(simulador, AgregarDificultad, NULL) == ErrorSimulacion, "AgregarDificultad con DatosDificultad NULL ErrorSimulacion");
  pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionDificultad, NULL) == ErrorSimulacion, "ObtenerInformacionDificultad con InformacionDificultad NULL ErrorSimulacion");

  simulador_destruir(simulador);
}

int main() {
  pa2m_nuevo_grupo("Pruebas de  creación y destrucción");
  puedoCrearYDestruirUnHospital();

  pa2m_nuevo_grupo("Pruebas con NULL");
  dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema();

  pa2m_nuevo_grupo("Pruebas con un archivo vacío");
  dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital();

  pa2m_nuevo_grupo("Pruebas con un archivo de un entrenador");
  dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital();

  pa2m_nuevo_grupo("Pruebas con un archivo de varios entrenadores");
  dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital();

  pa2m_nuevo_grupo("Pruebas con mas de un archivo");
  dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital();

  pa2m_nuevo_grupo("Pruebas Simulador");

  pa2m_nuevo_grupo("Pruebas de creación y destrucción de un simulador");
  puedoCrearYDestruirUnSimulador();

  pa2m_nuevo_grupo("Pruebas de evento de obtener estadisticas y dificultades");
  puedoObtenerEstadisticasYDificultades();

  pa2m_nuevo_grupo("Pruebas de atender un entrenador");
  atenderHospitalUnEntrenador();

  pa2m_nuevo_grupo("Pruebas de atender con varios entrenadores");
  atenderHospitalConVariosEntrenadores();

  pa2m_nuevo_grupo("Pruebas adivinar Pokemon");
  adivinarPokemon();

  pa2m_nuevo_grupo("Pruebas seleccionar y agregar dificultad");
  manejarDificultades();

  pa2m_nuevo_grupo("Pruebas Nulls");
  pruebasNulls();

  return pa2m_mostrar_reporte();
}
