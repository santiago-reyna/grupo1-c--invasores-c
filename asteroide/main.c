#include "raylib.h"
#include "asteroide.h"

int main(void) {
    const int anchoPantalla = 800;
    const int altoPantalla = 600;

    // Inicializar ventana de Raylib
    InitWindow(anchoPantalla, altoPantalla, "Space Dodge - Integrante 2");
    SetTargetFPS(60); // 60 fotogramas por segundo

    // Preparar el estado inicial
    InicializarAsteroides();

    // CICLO PRINCIPAL
    while (!WindowShouldClose()) {
        float deltaTiempo = GetFrameTime(); // Tiempo transcurrido por frame

        // Actualizar datos
        GenerarYMoverAsteroides(anchoPantalla, altoPantalla, deltaTiempo);

        // Dibujar en pantalla
        BeginDrawing();
            ClearBackground(BLACK);

            // Dibujar componentes del Integrante 2
            DibujarAsteroides();

            DrawText("Esquiva de Asteroides - Integrante 2", 10, 10, 20, RAYWHITE);
        EndDrawing();
    }

    // Cerrar la ventana al salir
    CloseWindow();

    return 0;
}