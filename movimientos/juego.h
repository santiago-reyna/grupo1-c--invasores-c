#ifndef JUEGO_H
#define JUEGO_H

#include "raylib.h"

// Le asignamos el nombre "struct Nave" para que otros archivos puedan usar Nave* sin redefinir
typedef struct Nave {
    Vector2 posicion;  // Coordenadas (x, y) en pantalla
    Vector2 tamanio;   // Ancho (width) y alto (height)
    float velocidad;   // Píxeles de movimiento por frame
    Color color;       // Color representativo o tint de textura
    int vidas;         // Cantidad de vidas restantes
    int nivelDisparo;  // Nivel de disparo (1 = Simple, 2 = Doble)
} Nave;

// Declaración de funciones del jugador
void inicializar_nave(Nave *nave, int ancho_pantalla, int alto_pantalla);
void actualizar_nave(Nave *nave, int ancho_pantalla);
void renderizar_nave(Nave nave);

#endif
