#include "asteroide.h"
#include <stdlib.h>

static Asteroide asteroides[MAX_ASTEROIDES];

void InicializarAsteroides(void) {
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        asteroides[i].activo = false;
    }
}

void GenerarYMoverAsteroides(int anchoPantalla, int altoPantalla, float deltaTiempo) {
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        if (!asteroides[i].activo && GetRandomValue(0, 100) < 3) {
            asteroides[i].posicion = (Vector2){ (float)GetRandomValue(0, anchoPantalla - 50), -50.0f };
            asteroides[i].tamanio = (Vector2){ 50.0f, 50.0f };
            asteroides[i].velocidad = (float)GetRandomValue(150, 350);
            asteroides[i].activo = true;
            break;
        }
    }

    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        if (asteroides[i].activo) {
            asteroides[i].posicion.y += asteroides[i].velocidad * deltaTiempo;

            if (asteroides[i].posicion.y > altoPantalla) {
                asteroides[i].activo = false;
            }
        }
    }
}

void DibujarAsteroides(Texture2D texAsteroide) {
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        if (asteroides[i].activo) {
            if (texAsteroide.id > 0) {
                Rectangle dest = { 
                    asteroides[i].posicion.x, 
                    asteroides[i].posicion.y, 
                    asteroides[i].tamanio.x, 
                    asteroides[i].tamanio.y 
                };
                Rectangle src = { 0, 0, (float)texAsteroide.width, (float)texAsteroide.height };
                Vector2 origin = { 0, 0 };

                DrawTexturePro(texAsteroide, src, dest, origin, 0.0f, WHITE);
            } else {
                DrawRectangleV(asteroides[i].posicion, asteroides[i].tamanio, RED);
            }
        }
    }
}

const Asteroide* ObtenerAsteroides(void) {
    return asteroides;
}