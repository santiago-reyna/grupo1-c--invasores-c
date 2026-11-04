#ifndef GRAFICO_H
#define GRAFICO_H

#include "raylib.h"
#include "../asteroide/asteroide.h"
#include "../movimientos/juego.h"

// Aumentamos de 5 a 15 para soportar los disparos triples
#define MAX_DISPAROS 15

typedef enum {
    ESTADO_MENU,
    ESTADO_JUGANDO,
    ESTADO_GAME_OVER,
    ESTADO_VICTORIA
} EstadoJuego;

typedef struct {
    Vector2 posicion;
    Vector2 velocidad;
    bool activo;
} Disparo;

void InicializarJuego(Nave *nave, Disparo disparos[], int *puntaje, int anchoPantalla, int altoPantalla);
void ActualizarJugador(Nave *nave);
void DibujarJuego(EstadoJuego estado, Nave nave, Disparo disparos[], int puntaje, Texture2D texNave, Texture2D texAsteroide, Texture2D texFondo);

#endif