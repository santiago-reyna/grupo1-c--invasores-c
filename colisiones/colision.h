#ifndef COLISION_H
#define COLISION_H

#include "raylib.h"
#include "../movimientos/juego.h"
#include "../asteroide/asteroide.h"

// Función adaptada para recorrer la lista enlazada de asteroides
bool VerificarColisionNaveAsteroides(Nave nave, int anchoPantalla);

#endif