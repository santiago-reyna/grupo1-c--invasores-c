#include "raylib.h"
#include "asteroide/asteroide.h"
#include "movimientos/juego.h"
#include "colisiones/colision.h"

int main(void) {
    const int ancho_pantalla = 800;
    const int alto_pantalla = 600;

    InitWindow(ancho_pantalla, alto_pantalla, "Invasores - Super Main");
    SetTargetFPS(60);

    Nave nave;
    bool colisionDetectada = false;

    // Inicialización
    InicializarAsteroides();
    inicializar_nave(&nave, ancho_pantalla, alto_pantalla);

    // BUCLE PRINCIPAL
    while (!WindowShouldClose()) {
        float deltaTiempo = GetFrameTime();

        if (!colisionDetectada) {
            actualizar_nave(&nave, ancho_pantalla);
            GenerarYMoverAsteroides(ancho_pantalla, alto_pantalla, deltaTiempo);

            colisionDetectada = VerificarColisionNaveAsteroides(nave, ObtenerAsteroides(), MAX_ASTEROIDES);
        } else {
            if (IsKeyPressed(KEY_R)) {
                InicializarAsteroides();
                inicializar_nave(&nave, ancho_pantalla, alto_pantalla);
                colisionDetectada = false;
            }
        }

        // RENDERIZADO
        BeginDrawing();
            ClearBackground(BLACK);

            DibujarAsteroides();
            renderizar_nave(nave);

            if (colisionDetectada) {
                DrawText("¡COLISIÓN DETECTADA!", ancho_pantalla / 2 - 150, alto_pantalla / 2 - 20, 25, RED);
                DrawText("Presiona 'R' para reiniciar", ancho_pantalla / 2 - 140, alto_pantalla / 2 + 20, 20, RAYWHITE);
            } else {
                DrawText("Usa Flechas / AD para moverte", 10, 10, 20, RAYWHITE);
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}