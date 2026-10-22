#ifndef GRAFICO_H
#define GRAFICO_H

#include "raylib.h"
#include "../asteroide/asteroide.h"

#define MAX_DISPAROS 5

typedef enum {
    ESTADO_MENU,
    ESTADO_JUGANDO,
    ESTADO_GAME_OVER
} EstadoJuego;

typedef struct {
    Vector2 posicion;
    Vector2 velocidad;
    Vector2 tamanio;
    int vidas;
} Nave;

typedef struct {
    Vector2 posicion;
    Vector2 velocidad;
    bool activo;
} Disparo;

void InicializarJuego(Nave *nave, Disparo disparos[], int *puntaje, int anchoPantalla, int altoPantalla);
void ActualizarJugador(Nave *nave);
void DibujarJuego(EstadoJuego estado, Nave nave, Disparo disparos[], int puntaje, Texture2D texNave, Texture2D texAsteroide, Texture2D texFondo);

#endif