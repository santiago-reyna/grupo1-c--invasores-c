#include "asteroide.h"

static Asteroide asteroides[MAX_ASTEROIDES];

void InicializarAsteroides(void) {
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        asteroides[i].activo = false;
    }
}

void GenerarYMoverAsteroides(int anchoPantalla, int altoPantalla, float deltaTiempo) {
    // Generar nuevos asteroides si hay espacio
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        if (!asteroides[i].activo && GetRandomValue(0, 100) < 2) {
            asteroides[i].posicion = (Vector2){ (float)GetRandomValue(0, anchoPantalla - 40), -40.0f };
            asteroides[i].tamanio = (Vector2){ 40.0f, 40.0f };
            asteroides[i].velocidad = (float)GetRandomValue(150, 300);
            asteroides[i].activo = true;
            break;
        }
    }

    // Mover asteroides activos
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        if (asteroides[i].activo) {
            asteroides[i].posicion.y += asteroides[i].velocidad * deltaTiempo;

            // Desactivar si salen de pantalla
            if (asteroides[i].posicion.y > altoPantalla) {
                asteroides[i].activo = false;
            }
        }
    }
}

void DibujarAsteroides(void) {
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        if (asteroides[i].activo) {
            DrawRectangleV(asteroides[i].posicion, asteroides[i].tamanio, GRAY);
        }
    }
}

const Asteroide* ObtenerAsteroides(void) {
    return asteroides;
}