#include "colision.h"
#include <stddef.h>

bool VerificarColisionNaveAsteroides(Nave *nave, int anchoPantalla) {
    // Protección defensiva: si el puntero de la nave es NULL, evitamos un colapso del juego
    if (nave == NULL) return false;

    float margen = 6.0f;

    // Aseguramos que la hitbox de la nave sea válida antes de calcular
    float anchoNave = (nave->tamanio.x > (margen * 2.0f)) ? (nave->tamanio.x - (margen * 2.0f)) : 1.0f;
    float altoNave = (nave->tamanio.y > (margen * 2.0f)) ? (nave->tamanio.y - (margen * 2.0f)) : 1.0f;

    Rectangle recNave = { 
        nave->posicion.x + margen, 
        nave->posicion.y + margen, 
        anchoNave, 
        altoNave 
    };

    NodoAsteroide *actual = ObtenerListaAsteroides();

    // Recorrido seguro de la lista de asteroides
    while (actual != NULL) {
        // Guardamos el puntero al siguiente por seguridad en la iteración
        NodoAsteroide *siguiente = actual->siguiente;

        float anchoAsteroide = (actual->tamanio.x > (margen * 2.0f)) ? (actual->tamanio.x - (margen * 2.0f)) : 1.0f;
        float altoAsteroide = (actual->tamanio.y > (margen * 2.0f)) ? (actual->tamanio.y - (margen * 2.0f)) : 1.0f;

        Rectangle recAsteroide = { 
            actual->posicion.x + margen, 
            actual->posicion.y + margen, 
            anchoAsteroide, 
            altoAsteroide 
        };

        if (CheckCollisionRecs(recNave, recAsteroide)) {
            // Elimina el asteroide actual de la lista enlazada y actualiza la nave de forma segura
            EliminarAsteroide(actual, anchoPantalla, nave);
            return true;
        }

        actual = siguiente;
    }

    return false;
}