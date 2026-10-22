#ifndef ASTEROIDE_H
#define ASTEROIDE_H

#include "raylib.h"
#include <stdlib.h>


typedef struct NodoAsteroide {
    Vector2 posicion;
    Vector2 tamanio;
    float velocidad;
    struct NodoAsteroide* siguiente; 
} NodoAsteroide;


void InicializarAsteroides(int anchoPantalla);

void GenerarYMoverAsteroides(int anchoPantalla, int altoPantalla, float deltaTiempo);

void DibujarAsteroides(Texture2D texAsteroide);

NodoAsteroide* ObtenerListaAsteroides(void);

void EliminarAsteroide(NodoAsteroide* objetivo, int anchoPantalla);

#endif