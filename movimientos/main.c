#include "raylib.h"
#include "juego.h"

int main() {
    // Inicialización de la ventana
    const int ancho_pantalla = 800;
    const int alto_pantalla = 600;
    InitWindow(ancho_pantalla, alto_pantalla, "Invasores");

    // Inicialización de la nave
    Nave nave;
    inicializar_nave(&nave, ancho_pantalla, alto_pantalla);

    // Bucle principal del juego
    while (!WindowShouldClose()) {
        // Actualización de la nave
        actualizar_nave(&nave, ancho_pantalla);

        // Renderizado
        BeginDrawing();
        ClearBackground(BLACK);
        renderizar_nave(nave);
        EndDrawing();
    }

    // Cierre de la ventana
    CloseWindow();
    return 0;
}