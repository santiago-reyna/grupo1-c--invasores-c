#include "asteroide.h"
#include <stdlib.h> // Para la generación de números aleatorios

// Arreglo global de asteroides y temporizador
static Asteroide asteroides[MAX_ASTEROIDES];
static float tiempoParaSiguienteAsteroide = 0.0f;

// Desactiva todos los asteroides para iniciar la partida
void InicializarAsteroides(void) {
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        asteroides[i].activo = false; // Todos inician apagados
    }
    tiempoParaSiguienteAsteroide = 0.0f;
}

// Controla la generación aleatoria, caída y reutilización
void GenerarYMoverAsteroides(int anchoPantalla, int altoPantalla, float deltaTiempo) {
    // 1. GENERACIÓN ALEATORIA Y REUTILIZACIÓN
    tiempoParaSiguienteAsteroide += deltaTiempo;

    // Intenta activar un asteroide cada 1 segundo
    if (tiempoParaSiguienteAsteroide >= 1.0f) {
        tiempoParaSiguienteAsteroide = 0.0f;

        // Busca en la lista un asteroide inactivo para reutilizarlo
        for (int i = 0; i < MAX_ASTEROIDES; i++)
         {
            if (!asteroides[i].activo)
             {
                asteroides[i].activo = true; // Se reactiva
                asteroides[i].tamanio = (Vector2){ 40.0f, 40.0f };
                
                // Posición X aleatoria dentro de los bordes de la ventana
                float posXAleatoria = GetRandomValue(0, anchoPantalla - (int)asteroides[i].tamanio.x);
                // Inicia arriba, justo fuera de la vista
                asteroides[i].posicion = (Vector2){ posXAleatoria, -asteroides[i].tamanio.y };
                
                // Velocidad aleatoria de caída
                asteroides[i].velocidad = (float)GetRandomValue(150, 300);

                break; // Se activa uno y sale del bucle
            }
        }
    }

    // 2. MOVIMIENTO HACIA ABAJO Y DESACTIVACIÓN
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        if (asteroides[i].activo) {
            // Se incrementa Y para simular la caída constante
            asteroides[i].posicion.y += asteroides[i].velocidad * deltaTiempo;

            // Si sobrepasa la parte inferior de la pantalla, se desactiva
            if (asteroides[i].posicion.y > altoPantalla) {
                asteroides[i].activo = false; // Queda libre para volverse a usar
            }
        }
    }
}

// Dibuja en pantalla los asteroides activos
void DibujarAsteroides(void) {
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        if (asteroides[i].activo) {
            DrawRectangleV(asteroides[i].posicion, asteroides[i].tamanio, GRAY);
        }
    }
}