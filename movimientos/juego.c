#include "juego.h"

void inicializar_nave(Nave *nave, int anchoPantalla, int altoPantalla) {
    nave->tamanio = (Vector2){ 60.0f, 60.0f };
    nave->posicion = (Vector2){ 
        (float)anchoPantalla / 2.0f - nave->tamanio.x / 2.0f, 
        (float)altoPantalla - 110.0f 
    };
    nave->velocidad = 350.0f;
}

void actualizar_nave(Nave *nave, int anchoPantalla) {
    float deltaTiempo = GetFrameTime();

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        nave->posicion.x -= nave->velocidad * deltaTiempo;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        nave->posicion.x += nave->velocidad * deltaTiempo;
    }

    // Límites de pantalla
    if (nave->posicion.x < 0) {
        nave->posicion.x = 0;
    }
    if (nave->posicion.x + nave->tamanio.x > anchoPantalla) {
        nave->posicion.x = (float)anchoPantalla - nave->tamanio.x;
    }
}