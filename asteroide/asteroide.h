#ifndef ASTEROIDE_H
#define ASTEROIDE_H

#include "raylib.h"

#define MAX_ASTEROIDES 10

typedef struct 
{
    Vector2 posicion;
    Vector2 tamanio;
    float velocidad;
    bool activo;
} Asteroide;

void InicializarAsteroides(void);
void GenerarYMoverAsteroides(int anchoPantalla, int altoPantalla, float deltaTiempo);
void DibujarAsteroides(void);
const Asteroide* ObtenerAsteroides(void);

#endif