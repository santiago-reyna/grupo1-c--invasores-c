#include "colision.h"
#include <stddef.h>

bool VerificarColisionNaveAsteroides(Nave nave, int anchoPantalla) {
    float margen = 6.0f; // Tolerancia contra bordes transparentes

    Rectangle recNave = { 
        nave.posicion.x + margen, 
        nave.posicion.y + margen, 
        nave.tamanio.x - (margen * 2), 
        nave.tamanio.y - (margen * 2) 
    };

    // Pedimos la cabeza de la lista enlazada
    NodoAsteroide *actual = ObtenerListaAsteroides();

    // Recorremos con un bucle while
    while (actual != NULL) {
        NodoAsteroide *siguiente = actual->siguiente;

        Rectangle recAsteroide = { 
            actual->posicion.x + margen, 
            actual->posicion.y + margen, 
            actual->tamanio.x - (margen * 2), 
            actual->tamanio.y - (margen * 2) 
        };

        if (CheckCollisionRecs(recNave, recAsteroide)) {
            // Si hay colisión, eliminamos el asteroide de la lista y retornamos true
            EliminarAsteroide(actual, anchoPantalla);
            return true;
        }

        actual = siguiente;
    }

    return false;
}