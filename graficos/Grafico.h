#ifndef GRAFICO_H
#define GRAFICO_H

#include "raylib.h"
#include "../asteroide/asteroide.h"
#include "../movimientos/juego.h"

#define MAX_DISPAROS 15
#define MAX_DISPAROS_BOSS 25 // Aumentamos para soportar proyectiles teledirigidos

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
    bool esTeledirigido; // <-- NUEVO: Identifica si es el asteroide teledirigido
} Disparo;

typedef struct {
    Vector2 posicion;
    Vector2 tamanio;
    int vidas;
    bool activo;
    bool animandoEntrada;
    float temporizadorAtaque;
    float temporizadorTeledirigido; // <-- NUEVO: Reloj para lanzar teledirigidos
    float tiempoEnPantalla;
} Boss;

void InicializarJuego(Nave *nave, Disparo disparos[], int *puntaje, int anchoPantalla, int altoPantalla);
void ActualizarJugador(Nave *nave);
void DibujarJuego(EstadoJuego estado, Nave nave, Disparo disparos[], Boss boss, Disparo disparosBoss[], int puntaje, Texture2D texNave, Texture2D texAsteroide, Texture2D texFondo);

#endif