#include <stddef.h>
#include "raylib.h"
#include "asteroide/asteroide.h"
#include "movimientos/juego.h"
#include "colisiones/colision.h"

typedef enum { 
    ESTADO_MENU, 
    ESTADO_JUGANDO, 
    ESTADO_GAME_OVER 
} EstadoJuego;

int main(void) {
    const int anchoPantalla = 1000;
    const int altoPantalla = 800;

    InitWindow(anchoPantalla, altoPantalla, "Space Dodge - Super Main Modular");
    InitAudioDevice();
    SetTargetFPS(60);

    // Cargar Recursos
    Texture2D texFondo = LoadTexture("graficos/fondo.png");
    if (texFondo.id == 0) texFondo = LoadTexture("fondo.png");

    Texture2D texNave = LoadTexture("graficos/nave.png");
    if (texNave.id == 0) texNave = LoadTexture("nave.png");

    Texture2D texAsteroide = LoadTexture("graficos/asteroide.png");
    if (texAsteroide.id == 0) texAsteroide = LoadTexture("asteroide.png");

    Sound efectoExplosion = LoadSound("graficos/disparolaser.mp3");
    if (efectoExplosion.stream.buffer == NULL) efectoExplosion = LoadSound("disparolaser.mp3");

    Music musicaFondo = LoadMusicStream("graficos/musicaFondo.mp3");
    if (musicaFondo.stream.buffer == NULL) musicaFondo = LoadMusicStream("musicaFondo.mp3");

    if (musicaFondo.stream.buffer != NULL) PlayMusicStream(musicaFondo);

    // Variables de Juego
    EstadoJuego estadoActual = ESTADO_MENU;
    Nave nave;
    int puntaje = 0;

    InicializarAsteroides();
    inicializar_nave(&nave, anchoPantalla, altoPantalla);

    // BUCLE PRINCIPAL
    while (!WindowShouldClose()) {
        if (musicaFondo.stream.buffer != NULL) UpdateMusicStream(musicaFondo);
        float deltaTiempo = GetFrameTime();

        switch (estadoActual) {
            case ESTADO_MENU:
                if (IsKeyPressed(KEY_ENTER)) {
                    InicializarAsteroides();
                    inicializar_nave(&nave, anchoPantalla, altoPantalla);
                    puntaje = 0;
                    estadoActual = ESTADO_JUGANDO;
                }
                break;

            case ESTADO_JUGANDO:
                actualizar_nave(&nave, anchoPantalla);
                GenerarYMoverAsteroides(anchoPantalla, altoPantalla, deltaTiempo);

                if (VerificarColisionNaveAsteroides(nave, ObtenerAsteroides(), MAX_ASTEROIDES)) {
                    if (efectoExplosion.stream.buffer != NULL) PlaySound(efectoExplosion);
                    estadoActual = ESTADO_GAME_OVER;
                }

                puntaje++;
                break;

            case ESTADO_GAME_OVER:
                if (IsKeyPressed(KEY_R)) {
                    InicializarAsteroides();
                    inicializar_nave(&nave, anchoPantalla, altoPantalla);
                    puntaje = 0;
                    estadoActual = ESTADO_JUGANDO;
                }
                break;
        }

        // RENDERIZADO
        BeginDrawing();
            ClearBackground(DARKGRAY);

            if (texFondo.id > 0) {
                Rectangle destFondo = { 0, 0, (float)anchoPantalla, (float)altoPantalla };
                Rectangle srcFondo = { 0, 0, (float)texFondo.width, (float)texFondo.height };
                DrawTexturePro(texFondo, srcFondo, destFondo, (Vector2){0,0}, 0.0f, WHITE);
            }

            switch (estadoActual) {
                case ESTADO_MENU:
                    DrawText("SPACE DODGE", anchoPantalla / 2 - MeasureText("SPACE DODGE", 40) / 2, 250, 40, RAYWHITE);
                    DrawText("Presiona ENTER para jugar", anchoPantalla / 2 - MeasureText("Presiona ENTER para jugar", 20) / 2, 330, 20, LIGHTGRAY);
                    break;

                case ESTADO_JUGANDO:
                    if (texNave.id > 0) {
                        Rectangle destNave = { nave.posicion.x, nave.posicion.y, nave.tamanio.x, nave.tamanio.y };
                        Rectangle srcNave = { 0, 0, (float)texNave.width, (float)texNave.height };
                        DrawTexturePro(texNave, srcNave, destNave, (Vector2){0,0}, 0.0f, WHITE);
                    } else {
                        DrawRectangleV(nave.posicion, nave.tamanio, BLUE);
                    }

                    DibujarAsteroides(texAsteroide);
                    DrawText(TextFormat("Puntaje: %d", puntaje), 20, 20, 20, YELLOW);
                    break;

                case ESTADO_GAME_OVER:
                    DrawText("¡GAME OVER!", anchoPantalla / 2 - MeasureText("¡GAME OVER!", 40) / 2, 230, 40, RED);
                    DrawText(TextFormat("Puntaje Final: %d", puntaje), anchoPantalla / 2 - MeasureText(TextFormat("Puntaje Final: %d", puntaje), 20) / 2, 290, 20, RAYWHITE);
                    DrawText("Presiona 'R' para reiniciar", anchoPantalla / 2 - MeasureText("Presiona 'R' para reiniciar", 20) / 2, 350, 20, LIGHTGRAY);
                    break;
            }

        EndDrawing();
    }

    if (texFondo.id > 0) UnloadTexture(texFondo);
    if (texNave.id > 0) UnloadTexture(texNave);
    if (texAsteroide.id > 0) UnloadTexture(texAsteroide);
    if (efectoExplosion.stream.buffer != NULL) UnloadSound(efectoExplosion);
    if (musicaFondo.stream.buffer != NULL) UnloadMusicStream(musicaFondo);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}