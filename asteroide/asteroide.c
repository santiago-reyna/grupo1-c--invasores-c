#include "asteroide.h"
#include <stdlib.h>

static Asteroide asteroides[MAX_ASTEROIDES];
static float temporizadorRespawn = 0.0f;

void InicializarAsteroides(void) {
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        asteroides[i].activo = false;
    }
    temporizadorRespawn = 0.0f;
}

void GenerarYMoverAsteroides(int anchoPantalla, int altoPantalla, float deltaTiempo) {
    // 1. Temporizador constante para generar asteroides inactivos
    temporizadorRespawn += deltaTiempo;

    if (temporizadorRespawn >= 0.5f) {
        temporizadorRespawn = 0.0f;

        for (int i = 0; i < MAX_ASTEROIDES; i++) {
            if (!asteroides[i].activo) {
                asteroides[i].tamanio = (Vector2){ 50.0f, 50.0f };
                float posX = (float)GetRandomValue(0, anchoPantalla - (int)asteroides[i].tamanio.x);
                
                asteroides[i].posicion = (Vector2){ posX, -50.0f };
                asteroides[i].velocidad = (float)GetRandomValue(150, 300);
                asteroides[i].activo = true;
                break; // Reactiva uno y espera al siguiente ciclo
            }
        }
    }

    // 2. Movimiento hacia abajo
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        if (asteroides[i].activo) {
            asteroides[i].posicion.y += asteroides[i].velocidad * deltaTiempo;

            // Desactivar si supera la parte inferior
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

void DesactivarAsteroide(int indice) {
    if (indice >= 0 && indice < MAX_ASTEROIDES) {
        asteroides[indice].activo = false; 
    }
}