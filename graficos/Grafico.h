#ifndef GRAFICO_H
#define GRAFICO_H

#include "raylib.h"

#define MAX_ASTEROIDES 10
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
} Nave;

typedef struct {
    Vector2 posicion;
    Vector2 velocidad;
    Vector2 tamanio;
    bool activo;
} Asteroide;

typedef struct {
    Vector2 posicion;
    Vector2 velocidad;
    bool activo;
} Disparo;

void InicializarJuego(Nave *nave, Asteroide asteroides[], Disparo disparos[], int *puntaje);
void ActualizarJugador(Nave *nave);
void GenerarYMoverAsteroides(Asteroide asteroides[]);
bool ComprobarColision(Nave nave, Asteroide asteroides[]);
void DibujarJuego(EstadoJuego estado, Nave nave, Asteroide asteroides[], Disparo disparos[], int puntaje, Texture2D texNave, Texture2D texAsteroide, Texture2D texFondo);

#endif