#include "colision.h"
#include <stddef.h>

bool VerificarColisionNaveAsteroides(Nave *nave, int anchoPantalla) {
    float margen = 6.0f;

    Rectangle recNave = { 
        nave->posicion.x + margen, 
        nave->posicion.y + margen, 
        nave->tamanio.x - (margen * 2), 
        nave->tamanio.y - (margen * 2) 
    };

    NodoAsteroide *actual = ObtenerListaAsteroides();

    while (actual != NULL) {
        NodoAsteroide *siguiente = actual->siguiente;

        Rectangle recAsteroide = { 
            actual->posicion.x + margen, 
            actual->posicion.y + margen, 
            actual->tamanio.x - (margen * 2), 
            actual->tamanio.y - (margen * 2) 
        };

        if (CheckCollisionRecs(recNave, recAsteroide)) {
            // Pasamos &nave como 3er argumento para actualizar el nivel de disparo al colisionar
            EliminarAsteroide(actual, anchoPantalla, nave);
            return true;
        }

        actual = siguiente;
    }

    return false;
}