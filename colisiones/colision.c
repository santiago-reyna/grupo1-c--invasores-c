#include "colision.h"

bool VerificarColisionNaveAsteroides(Nave nave, const Asteroide asteroides[], int cantidadAsteroides) {
    // Tolerancia de 6px para evitar colisiones fantasma en bordes transparentes
    float margen = 6.0f; 

    Rectangle recNave = { 
        nave.posicion.x + margen, 
        nave.posicion.y + margen, 
        nave.tamanio.x - (margen * 2), 
        nave.tamanio.y - (margen * 2) 
    };

    for (int i = 0; i < cantidadAsteroides; i++) {
        if (asteroides[i].activo) {
            Rectangle recAsteroide = { 
                asteroides[i].posicion.x + margen, 
                asteroides[i].posicion.y + margen, 
                asteroides[i].tamanio.x - (margen * 2), 
                asteroides[i].tamanio.y - (margen * 2) 
            };

            if (CheckCollisionRecs(recNave, recAsteroide)) {
                return true;
            }
        }
    }

    return false;
}