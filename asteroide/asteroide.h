#ifndef ASTEROIDE_H
#define ASTEROIDE_H

#include "raylib.h"

// Cantidad máxima de asteroides en pantalla
#define MAX_ASTEROIDES 10

typedef struct 
{
    Vector2 posicion;   // Posición en pantalla (X, Y)
    Vector2 tamanio;    // Ancho y alto de la piedra
    float velocidad;    // Velocidad de caída
    bool activo;        // Estado: true (en juego) / false (libre/reutilizable)
} Asteroide;

void InicializarAsteroides(void);
void GenerarYMoverAsteroides(int anchoPantalla, int altoPantalla, float deltaTiempo);
void DibujarAsteroides(void);

#endif 