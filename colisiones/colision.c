#include "colision.h"

bool VerificarColisionNaveAsteroides(Nave nave, const Asteroide asteroides[], int cantidadAsteroides) {
    Rectangle recNave = { nave.posicion.x, nave.posicion.y, nave.tamanio.x, nave.tamanio.y };

    for (int i = 0; i < cantidadAsteroides; i++) {
        if (asteroides[i].activo) {
            Rectangle recAsteroide = { 
                asteroides[i].posicion.x, 
                asteroides[i].posicion.y, 
                asteroides[i].tamanio.x, 
                asteroides[i].tamanio.y 
            };

            if (CheckCollisionRecs(recNave, recAsteroide)) {
                return true;
            }
        }
    }

    return false;
}