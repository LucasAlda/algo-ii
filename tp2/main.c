#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "src/hospital.h"
#include "src/simulador.h"

typedef struct {
  simulador_t* simulador;
  bool jugando;
} Juego;

void inicializar_juego(Juego* juego, hospital_t* hospital) {
  juego->simulador = simulador_crear(hospital);
  juego->jugando = true;
}

void destruir_juego(Juego juego) {
  simulador_destruir(juego.simulador);
}

unsigned calcular_puntaje_ambos(unsigned cantidad_intentos) {
  if (cantidad_intentos == 1)
    return 500;
  if (cantidad_intentos < 5)
    return 200;
  else
    return 35;
}

int verificar_nivel_ambos(unsigned nivel_adivinado, unsigned nivel_pokemon) {
  if (nivel_adivinado == nivel_pokemon)
    return 0;
  else
    return (int)(nivel_adivinado - nivel_pokemon);
}

const char* verificacion_a_string_temperatura(int resultado_verificacion) {
  if (resultado_verificacion == 0)
    return "Acertaste";
  else if (resultado_verificacion > -10 && resultado_verificacion < 10)
    return "Esta muy caliente tu intento";
  else if (resultado_verificacion > -25 && resultado_verificacion < 25)
    return "Esta tibio tu intento";
  else
    return "Esta frio tu intento";
}

const char* verificacion_a_string_arriba_abajo(int resultado_verificacion) {
  if (resultado_verificacion == 0)
    return "Acertaste";
  else if (resultado_verificacion > 0)
    return "Mas Bajo";
  else
    return "Mas Alto";
}

char leer_comando() {
  char entrada[100];
  char* leido;
  leido = fgets(entrada, 100, stdin);
  if (!leido)
    return 0;
  while (*leido == ' ')
    leido++;
  return (char)tolower(*leido);
}

int leer_entero() {
  char entrada[100];
  char *leido, *comprobacion;
  leido = fgets(entrada, 100, stdin);
  if (!leido)
    return -1;
  while (*leido == ' ')
    leido++;
  int numero = (int)strtoul(leido, &comprobacion, 0);
  while (comprobacion == leido) {
    printf("Ingrese un numero:\n");
    leido = fgets(entrada, 100, stdin);
    if (!leido)
      return -1;
    while (*leido == ' ')
      leido++;
    numero = (int)strtoul(leido, &comprobacion, 0);
  }
  return numero;
}

//Simula el evento de obtener las estadisticas y luego las imprime por pantalla
void mostrar_estadisticas(Juego* juego) {
  EstadisticasSimulacion e;
  if (simulador_simular_evento(juego->simulador, ObtenerEstadisticas, &e) == ErrorSimulacion) {
    printf("No se pudo obtener las estadisticas\n");
    return;
  }

  printf("Entrenadores antendidos: %u\n", e.entrenadores_atendidos);
  printf("Entrenadores totales: %u\n", e.entrenadores_totales);
  printf("Pokemones atendidos: %u\n", e.pokemon_atendidos);
  printf("Pokemones en espera: %u\n", e.pokemon_en_espera);
  printf("Pokemones totales %u\n", e.pokemon_totales);
  printf("Tus puntos: %u\n", e.puntos);
  printf("Eventos simulados: %u\n", e.cantidad_eventos_simulados);
}

void atender_entrenador(Juego* juego) {
  if (simulador_simular_evento(juego->simulador, AtenderProximoEntrenador, NULL) == ErrorSimulacion) {
    printf("No se pudo atender al proximo entrenador\n");
    return;
  }
  printf("Siguiente Entrenador atendido, sus pokemones a espera de ser atendidos\n");
}

// Simula el evento de mostrar informacion del pokemon en tratamiento y luego muestra la informacion obtenida por pantalla
void mostrar_informacion_pokemon_en_tratamiento(Juego* juego) {
  InformacionPokemon i;
  if (simulador_simular_evento(juego->simulador, ObtenerInformacionPokemonEnTratamiento, &i) == ErrorSimulacion) {
    printf("No se pudo obtener la informacion buscada\n");
    return;
  }

  printf("Nombre del pokemon atendido: %s\n", i.nombre_pokemon);
  printf("Nombre de su entrenador: %s\n", i.nombre_entrenador);
}

//Simula el evento de intentar adivinar el nivel de un pokemon e imprime por pantalla el resultado de la verificacion a string dado el nivel propuesto
void intentar_adivinar(Juego* juego) {
  Intento intento;
  printf("Escriba el nivel que cree que tiene el pokemon:\n");
  intento.nivel_adivinado = (unsigned int)leer_entero();

  if (simulador_simular_evento(juego->simulador, AdivinarNivelPokemon, &intento) == ErrorSimulacion) {
    printf("No se pudo simular adivinar el nivel del pokemon");
    return;
  }

  printf("%s\n", intento.resultado_string);
}

//Simula el evento de elegir dificultad y avisa al usuario si se pudo o no por pantalla
void cambiar_dificultad(Juego* juego) {
  int id;
  printf("Inserte el id de la dificultad que desea:\n");
  id = (int)leer_entero();

  if (simulador_simular_evento(juego->simulador, SeleccionarDificultad, &id) == ErrorSimulacion) {
    printf("No se pudo cambiar de dificultad\n");
    return;
  } else
    printf("Dificultad Cambiada!\n");
}

//Simula el evento de obtener la informacion de la dificultad esa informacion por pantalla
void mostrar_informacion_dificultad_actual(Juego* juego) {
  InformacionDificultad iD;
  printf("Inserte el id de la dificultad de la cual desea obtener informacion:\n");
  iD.id = (int)leer_entero();

  if (simulador_simular_evento(juego->simulador, ObtenerInformacionDificultad, &iD) == ErrorSimulacion) {
    printf("Error obteniendo la informacion de la dificultad buscada\n");
    return;
  }

  printf("Nombre de la dificultad: %s\n", iD.nombre_dificultad);
  printf("Dificultad en uso: %s\n", iD.en_uso ? "Si" : "No");
}

void finalizar_juego(Juego* juego) {
  simulador_simular_evento(juego->simulador, FinalizarSimulacion, NULL);
  juego->jugando = false;
  printf("Se salio del juego\n");
}

//En base a lo que haya ingresado el usuario ejecuta cierto comando que pueden ser los distintos eventos soportados del simulador o uno erroneo
void ejecutar_comando(Juego* juego, char comando) {
  if (juego == NULL) {
    printf("El juego recibido no existe\n");
    return;
  }
  switch (comando) {
    case 'q':
      finalizar_juego(juego);
      break;
    case 'e':
      mostrar_estadisticas(juego);
      break;
    case 'p':
      atender_entrenador(juego);
      break;
    case 'i':
      mostrar_informacion_pokemon_en_tratamiento(juego);
      break;
    case 'a':
      intentar_adivinar(juego);
      break;
    case 'd':
      cambiar_dificultad(juego);
      break;
    case 'o':
      mostrar_informacion_dificultad_actual(juego);
      break;
    default:
      printf("El comando utilizado no existe\n");
  }
}

int main(int argc, char* argv[]) {
  hospital_t* hospital = hospital_crear();
  hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

  Juego juego;
  inicializar_juego(&juego, hospital);

  DatosDificultad* dificultad_temperatura = malloc(sizeof(DatosDificultad));
  if (dificultad_temperatura == NULL) {
    printf("Hubo un error al agregar una dificultad\n");
    destruir_juego(juego);
    return -1;
  }
  dificultad_temperatura->nombre = "Temperatura";
  dificultad_temperatura->calcular_puntaje = calcular_puntaje_ambos;
  dificultad_temperatura->verificar_nivel = verificar_nivel_ambos;
  dificultad_temperatura->verificacion_a_string = verificacion_a_string_temperatura;
  ResultadoSimulacion resultado = simulador_simular_evento(juego.simulador, AgregarDificultad, dificultad_temperatura);
  if (resultado == ErrorSimulacion) {
    printf("Hubo un error al agregar una dificultad\n");
    destruir_juego(juego);
    return -1;
  }

  DatosDificultad* dificultad_arriba_abajo = malloc(sizeof(DatosDificultad));
  if (dificultad_arriba_abajo == NULL) {
    printf("Hubo un error al agregar una dificultad\n");
    destruir_juego(juego);
    return -1;
  }
  dificultad_arriba_abajo->nombre = "Arriba/Abajo";
  dificultad_arriba_abajo->calcular_puntaje = calcular_puntaje_ambos;
  dificultad_arriba_abajo->verificar_nivel = verificar_nivel_ambos;
  dificultad_arriba_abajo->verificacion_a_string = verificacion_a_string_arriba_abajo;
  ResultadoSimulacion resultado2 = simulador_simular_evento(juego.simulador, AgregarDificultad, dificultad_arriba_abajo);
  if (resultado2 == ErrorSimulacion) {
    printf("Hubo un error al agregar una dificultad\n");
    destruir_juego(juego);
    return -1;
  }

  do {
    printf("-----------------------------\n");
    printf("----  LISTA de COMANDOS  ----\n");
    printf("-----------------------------\n");
    printf("q: salir\n");
    printf("e: ver estadisticas del simulador\n");
    printf("p: atender al proximo entrenador\n");
    printf("i: ver la informacion del pokemon en tratamiento\n");
    printf("a: intentar adivinar\n");
    printf("d: cambiar de dificultad\n");
    printf("o: mostrar los detalles de alguna dificultad\n");
    printf("-----------------------------\n");
    char c = leer_comando();
    system("clear");
    ejecutar_comando(&juego, c);
  } while (juego.jugando);

  free(dificultad_temperatura);
  free(dificultad_arriba_abajo);
  destruir_juego(juego);

  return 0;
}
