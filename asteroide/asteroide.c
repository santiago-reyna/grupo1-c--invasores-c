#include "asteroide.h"
#include <stdlib.h>

static Asteroide asteroides[MAX_ASTEROIDES];

void InicializarAsteroides(void) {
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        asteroides[i].tamanio = (Vector2){ 50.0f, 50.0f };
        asteroides[i].posicion = (Vector2){ (float)GetRandomValue(0, 1000 - 50), (float)(-GetRandomValue(50, 400)) };
        asteroides[i].velocidad = (float)GetRandomValue(150, 300);
        asteroides[i].activo = true;
    }
}

void GenerarYMoverAsteroides(int anchoPantalla, int altoPantalla, float deltaTiempo) {
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        if (asteroides[i].activo) {
            // Mover asteroide hacia abajo
            asteroides[i].posicion.y += asteroides[i].velocidad * deltaTiempo;

            // Reaparecer arriba si sale por abajo
            if (asteroides[i].posicion.y > altoPantalla) {
                asteroides[i].posicion.x = (float)GetRandomValue(0, anchoPantalla - (int)asteroides[i].tamanio.x);
                asteroides[i].posicion.y = -50.0f;
                asteroides[i].velocidad = (float)GetRandomValue(150, 300);
            }
        } else {
            // Si fue destruido, respawnear arriba
            asteroides[i].posicion.x = (float)GetRandomValue(0, anchoPantalla - (int)asteroides[i].tamanio.x);
            asteroides[i].posicion.y = (float)(-GetRandomValue(50, 200));
            asteroides[i].velocidad = (float)GetRandomValue(150, 300);
            asteroides[i].activo = true;
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