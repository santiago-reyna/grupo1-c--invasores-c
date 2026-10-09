#ifndef COLISION_H
#define COLISION_H

#include "raylib.h"
#include "../movimientos/juego.h"
#include "../asteroide/asteroide.h"

bool VerificarColisionNaveAsteroides(Nave nave, const Asteroide asteroides[], int cantidadAsteroides);

#endif