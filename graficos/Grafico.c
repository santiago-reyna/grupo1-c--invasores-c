#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "Grafico.h"

void InicializarJuego(Nave *nave, Disparo disparos[], int *puntaje, int anchoPantalla, int altoPantalla) {
    nave->tamanio = (Vector2){ 100, 90 };
    nave->posicion = (Vector2){ anchoPantalla / 2.0f - nave->tamanio.x / 2.0f, (float)altoPantalla - 100 };
    nave->velocidad = 350.0f;
    nave->vidas = 3;
    nave->nivelDisparo = 1;

    for (int i = 0; i < MAX_DISPAROS; i++) {
        disparos[i].activo = false;
    }

    *puntaje = 0;
}

void ActualizarJugador(Nave *nave) {
    float dt = GetFrameTime();

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) nave->posicion.x -= nave->velocidad * dt;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) nave->posicion.x += nave->velocidad * dt;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) nave->posicion.y -= nave->velocidad * dt;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) nave->posicion.y += nave->velocidad * dt;

    if (nave->posicion.x < 0) nave->posicion.x = 0;
    if (nave->posicion.x > 1000 - nave->tamanio.x) nave->posicion.x = 1000 - nave->tamanio.x;
    if (nave->posicion.y < 0) nave->posicion.y = 0;
    if (nave->posicion.y > 800 - nave->tamanio.y) nave->posicion.y = 800 - nave->tamanio.y;
}

void DibujarJuego(EstadoJuego estado, Nave nave, Disparo disparos[], Boss boss, Disparo disparosBoss[], int puntaje, Texture2D texNave, Texture2D texAsteroide, Texture2D texFondo) {
    BeginDrawing();
    ClearBackground(BLACK);

    // Fondo
    DrawTexturePro(texFondo, (Rectangle){0, 0, (float)texFondo.width, (float)texFondo.height}, (Rectangle){0, 0, 1000, 800}, (Vector2){0, 0}, 0.0f, WHITE);

    switch (estado) {
        case ESTADO_MENU:
            DrawText("INVASORES", 1000 / 2 - MeasureText("INVASORES", 40) / 2, 300, 40, YELLOW);
            DrawText("Presiona ENTER para iniciar", 1000 / 2 - MeasureText("Presiona ENTER para iniciar", 20) / 2, 400, 20, WHITE);
            break;

        case ESTADO_JUGANDO:
            // Dibujar Nave
            DrawTexturePro(texNave, (Rectangle){0, 0, (float)texNave.width, (float)texNave.height}, (Rectangle){nave.posicion.x, nave.posicion.y, nave.tamanio.x, nave.tamanio.y}, (Vector2){0,0}, 0.0f, WHITE);

            // Dibujar Asteroides Comunes desde la Lista Enlazada
            NodoAsteroide *actual = ObtenerListaAsteroides();
            while (actual != NULL) {
                DrawTexturePro(
                    texAsteroide, 
                    (Rectangle){0, 0, (float)texAsteroide.width, (float)texAsteroide.height}, 
                    (Rectangle){actual->posicion.x, actual->posicion.y, actual->tamanio.x, actual->tamanio.y}, 
                    (Vector2){0, 0}, 
                    0.0f, 
                    WHITE
                );
                actual = actual->siguiente;
            }

            // Dibujar Boss si está activo
            if (boss.activo) {
                DrawTexturePro(
                    texAsteroide, 
                    (Rectangle){0, 0, (float)texAsteroide.width, (float)texAsteroide.height}, 
                    (Rectangle){boss.posicion.x, boss.posicion.y, boss.tamanio.x, boss.tamanio.y}, 
                    (Vector2){0, 0}, 
                    0.0f, 
                    RED
                );
                
                // Barra de Vida del Boss (Escalada a 150 de HP)
                DrawRectangle(300, 20, 400, 15, DARKGRAY);
                DrawRectangle(300, 20, (int)(400.0f * ((float)boss.vidas / 150.0f)), 15, RED);
                DrawRectangleLines(300, 20, 400, 15, WHITE);
                DrawText("BOSS FINAL", 1000 / 2 - MeasureText("BOSS FINAL", 16) / 2, 2, 16, RED);
            }

            // Dibujar Disparos del Jugador
            for (int i = 0; i < MAX_DISPAROS; i++) {
                if (disparos[i].activo) {
                    DrawRectangle(disparos[i].posicion.x, disparos[i].posicion.y, 5, 15, RED);
                }
            }

            // Dibujar Proyectiles del Boss (Asteroides comunes o Teledirigidos)
            for (int i = 0; i < MAX_DISPAROS_BOSS; i++) {
                if (disparosBoss[i].activo) {
                    if (disparosBoss[i].esTeledirigido) {
                        // Asteroide Teledirigido (40x40)
                        DrawTexturePro(
                            texAsteroide, 
                            (Rectangle){0, 0, (float)texAsteroide.width, (float)texAsteroide.height}, 
                            (Rectangle){disparosBoss[i].posicion.x, disparosBoss[i].posicion.y, 40, 40}, 
                            (Vector2){0,0}, 
                            0.0f, 
                            WHITE
                        );
                    } else {
                        // Mini-Asteroide disparo estándar (18x18)
                        DrawTexturePro(
                            texAsteroide, 
                            (Rectangle){0, 0, (float)texAsteroide.width, (float)texAsteroide.height}, 
                            (Rectangle){disparosBoss[i].posicion.x, disparosBoss[i].posicion.y, 18, 18}, 
                            (Vector2){0,0}, 
                            0.0f, 
                            ORANGE
                        );
                    }
                }
            }

            // HUD Jugador
            DrawText(TextFormat("Puntaje: %d", puntaje), 20, 20, 20, YELLOW);
            DrawText("Vidas:", 20, 50, 20, WHITE);
            for (int i = 0; i < nave.vidas; i++) {
                Rectangle src = { 0, 0, (float)texNave.width, (float)texNave.height };
                Rectangle dest = { 90 + (i * 30), 50, 40, 40 };
                DrawTexturePro(texNave, src, dest, (Vector2){0,0}, 0.0f, WHITE);
            }
            break;

        case ESTADO_GAME_OVER:
            DrawText("GAME OVER", 1000 / 2 - MeasureText("GAME OVER", 40) / 2, 300, 40, RED);
            DrawText(TextFormat("Puntaje Final: %d", puntaje), 1000 / 2 - MeasureText(TextFormat("Puntaje Final: %d", puntaje), 20) / 2, 360, 20, WHITE);
            DrawText("Presiona R para reiniciar", 1000 / 2 - MeasureText("Presiona R para reiniciar", 20) / 2, 400, 20, GRAY);
            break;

        case ESTADO_VICTORIA:
            DrawText("¡VICTORIA!", 1000 / 2 - MeasureText("¡VICTORIA!", 40) / 2, 300, 40, GREEN);
            DrawText("¡Has derrotado al Boss Final!", 1000 / 2 - MeasureText("¡Has derrotado al Boss Final!", 20) / 2, 350, 20, WHITE);
            DrawText(TextFormat("Puntaje Final: %d", puntaje), 1000 / 2 - MeasureText(TextFormat("Puntaje Final: %d", puntaje), 20) / 2, 390, 20, YELLOW);
            DrawText("Presiona R para reiniciar", 1000 / 2 - MeasureText("Presiona R para reiniciar", 20) / 2, 430, 20, GRAY);
            break;
    }

    EndDrawing();
}