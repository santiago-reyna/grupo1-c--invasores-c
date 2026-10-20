#include "raylib.h"
#include "asteroide/asteroide.h"

int main(void) {
    const int anchoPantalla = 800;
    const int altoPantalla = 600;

    // Inicializar ventana de Raylib
    InitWindow(anchoPantalla, altoPantalla, "Space Dodge - Integrante 2");
    SetTargetFPS(60);

    // Cargar la textura del asteroide (intenta en la carpeta o la raíz)
    Texture2D texAsteroide = LoadTexture("graficos/asteroide.png");
    if (texAsteroide.id == 0) texAsteroide = LoadTexture("asteroide.png");

    // Preparar el estado inicial
    InicializarAsteroides();

    // CICLO PRINCIPAL
    while (!WindowShouldClose()) {
        float deltaTiempo = GetFrameTime(); // Tiempo transcurrido por frame

        // Actualizar datos de generación y caída
        GenerarYMoverAsteroides(anchoPantalla, altoPantalla, deltaTiempo);

        // Dibujar en pantalla
        BeginDrawing();
            ClearBackground(BLACK);

            // Dibujar asteroides pasando la textura requerida por asteroide.h
            DibujarAsteroides(texAsteroide);

            DrawText("Esquiva de Asteroides - Integrante 2", 10, 10, 20, RAYWHITE);
        EndDrawing();
    }

    // Descargar recursos y cerrar ventana
    UnloadTexture(texAsteroide);
    CloseWindow();

    return 0;
}