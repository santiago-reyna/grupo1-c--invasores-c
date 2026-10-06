#ifndef JUEGO_H
#define JUEGO_H

#include "raylib.h"

typedef struct {
    Vector2 posicion; // Coordenadas (x, y) en pantalla
    Vector2 tamanio;  // Ancho (width) y alto (height)
    float velocidad;  // Píxeles de movimiento por frame
    Color color;      // Color representativo o tint de textura
} Nave;

// Declaración de funciones y variables del juego
void inicializar_nave(Nave *nave, int ancho_pantalla, int alto_pantalla);
void actualizar_nave(Nave *nave, int ancho_pantalla);
void renderizar_nave(Nave nave);

#endif