#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "Grafico.h"

void InicializarJuego(Nave *nave, Disparo disparos[], int *puntaje, int anchoPantalla, int altoPantalla) {
    if (nave == NULL || disparos == NULL || puntaje == NULL) return;

    nave->tamanio = (Vector2){ 100.0f, 90.0f };
    nave->posicion = (Vector2){ (float)anchoPantalla / 2.0f - nave->tamanio.x / 2.0f, (float)altoPantalla - 100.0f };
    nave->velocidad = 350.0f;
    nave->vidas = 3;
    nave->nivelDisparo = 1;

    for (int i = 0; i < MAX_DISPAROS; i++) {
        disparos[i].activo = false;
    }

    *puntaje = 0;
}

void ActualizarJugador(Nave *nave) {
    if (nave == NULL) return;

    float dt = GetFrameTime();

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) nave->posicion.x -= nave->velocidad * dt;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) nave->posicion.x += nave->velocidad * dt;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) nave->posicion.y -= nave->velocidad * dt;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) nave->posicion.y += nave->velocidad * dt;

    // Límites de pantalla seguros (adaptados a la resolución por defecto o dinámicos)
    float anchoVentana = (float)GetScreenWidth();
    float altoVentana = (float)GetScreenHeight();

    if (nave->posicion.x < 0.0f) nave->posicion.x = 0.0f;
    if (nave->posicion.x > anchoVentana - nave->tamanio.x) nave->posicion.x = anchoVentana - nave->tamanio.x;
    if (nave->posicion.y < 0.0f) nave->posicion.y = 0.0f;
    if (nave->posicion.y > altoVentana - nave->tamanio.y) nave->posicion.y = altoVentana - nave->tamanio.y;
}

void DibujarJuego(EstadoJuego estado, Nave nave, Disparo disparos[], Boss boss, Disparo disparosBoss[], int puntaje, Texture2D texNave, Texture2D texAsteroide, Texture2D texFondo) {
    BeginDrawing();
    ClearBackground(BLACK);

    int anchoWin = GetScreenWidth();
    int altoWin = GetScreenHeight();

    // Dibujo seguro de Fondo
    if (IsTextureValid(texFondo) && texFondo.id > 0) {
        DrawTexturePro(
            texFondo, 
            (Rectangle){ 0, 0, (float)texFondo.width, (float)texFondo.height }, 
            (Rectangle){ 0, 0, (float)anchoWin, (float)altoWin }, 
            (Vector2){ 0, 0 }, 
            0.0f, 
            WHITE
        );
    }

    switch (estado) {
        case ESTADO_MENU:
            DrawText("INVASORES", anchoWin / 2 - MeasureText("INVASORES", 40) / 2, 300, 40, YELLOW);
            DrawText("Presiona ENTER para iniciar", anchoWin / 2 - MeasureText("Presiona ENTER para iniciar", 20) / 2, 400, 20, WHITE);
            break;

        case ESTADO_JUGANDO:
            // Dibujar Nave (con fallback visual si falla la textura)
            if (IsTextureValid(texNave) && texNave.id > 0) {
                DrawTexturePro(
                    texNave, 
                    (Rectangle){ 0, 0, (float)texNave.width, (float)texNave.height }, 
                    (Rectangle){ nave.posicion.x, nave.posicion.y, nave.tamanio.x, nave.tamanio.y }, 
                    (Vector2){ 0, 0 }, 
                    0.0f, 
                    WHITE
                );
            } else {
                DrawRectangleV(nave.posicion, nave.tamanio, BLUE);
            }

            // Dibujar Asteroides Comunes desde la Lista Enlazada
            NodoAsteroide *actual = ObtenerListaAsteroides();
            while (actual != NULL) {
                if (IsTextureValid(texAsteroide) && texAsteroide.id > 0) {
                    DrawTexturePro(
                        texAsteroide, 
                        (Rectangle){ 0, 0, (float)texAsteroide.width, (float)texAsteroide.height }, 
                        (Rectangle){ actual->posicion.x, actual->posicion.y, actual->tamanio.x, actual->tamanio.y }, 
                        (Vector2){ 0, 0 }, 
                        0.0f, 
                        WHITE
                    );
                } else {
                    DrawRectangleV(actual->posicion, actual->tamanio, RED);
                }
                actual = actual->siguiente;
            }

            // Dibujar Boss si está activo
            if (boss.activo) {
                if (IsTextureValid(texAsteroide) && texAsteroide.id > 0) {
                    DrawTexturePro(
                        texAsteroide, 
                        (Rectangle){ 0, 0, (float)texAsteroide.width, (float)texAsteroide.height }, 
                        (Rectangle){ boss.posicion.x, boss.posicion.y, boss.tamanio.x, boss.tamanio.y }, 
                        (Vector2){ 0, 0 }, 
                        0.0f, 
                        RED
                    );
                } else {
                    DrawRectangleV(boss.posicion, boss.tamanio, PURPLE);
                }
                
                // Barra de Vida del Boss (Escalada a 150 HP con protección contra negativo/división cero)
                int vidasBoss = (boss.vidas < 0) ? 0 : boss.vidas;
                float porcentajeVida = (float)vidasBoss / 150.0f;
                if (porcentajeVida > 1.0f) porcentajeVida = 1.0f;

                DrawRectangle(anchoWin / 2 - 200, 20, 400, 15, DARKGRAY);
                DrawRectangle(anchoWin / 2 - 200, 20, (int)(400.0f * porcentajeVida), 15, RED);
                DrawRectangleLines(anchoWin / 2 - 200, 20, 400, 15, WHITE);
                DrawText("BOSS FINAL", anchoWin / 2 - MeasureText("BOSS FINAL", 16) / 2, 2, 16, RED);
            }

            // Dibujar Disparos del Jugador
            if (disparos != NULL) {
                for (int i = 0; i < MAX_DISPAROS; i++) {
                    if (disparos[i].activo) {
                        DrawRectangle((int)disparos[i].posicion.x, (int)disparos[i].posicion.y, 5, 15, RED);
                    }
                }
            }

            // Dibujar Proyectiles del Boss
            if (disparosBoss != NULL) {
                for (int i = 0; i < MAX_DISPAROS_BOSS; i++) {
                    if (disparosBoss[i].activo) {
                        if (disparosBoss[i].esTeledirigido) {
                            // Asteroide Teledirigido (40x40)
                            if (IsTextureValid(texAsteroide) && texAsteroide.id > 0) {
                                DrawTexturePro(
                                    texAsteroide, 
                                    (Rectangle){ 0, 0, (float)texAsteroide.width, (float)texAsteroide.height }, 
                                    (Rectangle){ disparosBoss[i].posicion.x, disparosBoss[i].posicion.y, 40, 40 }, 
                                    (Vector2){ 0, 0 }, 
                                    0.0f, 
                                    WHITE
                                );
                            } else {
                                DrawRectangle((int)disparosBoss[i].posicion.x, (int)disparosBoss[i].posicion.y, 40, 40, MAROON);
                            }
                        } else {
                            // Mini-Asteroide disparo estándar (18x18)
                            if (IsTextureValid(texAsteroide) && texAsteroide.id > 0) {
                                DrawTexturePro(
                                    texAsteroide, 
                                    (Rectangle){ 0, 0, (float)texAsteroide.width, (float)texAsteroide.height }, 
                                    (Rectangle){ disparosBoss[i].posicion.x, disparosBoss[i].posicion.y, 18, 18 }, 
                                    (Vector2){ 0, 0 }, 
                                    0.0f, 
                                    ORANGE
                                );
                            } else {
                                DrawRectangle((int)disparosBoss[i].posicion.x, (int)disparosBoss[i].posicion.y, 18, 18, ORANGE);
                            }
                        }
                    }
                }
            }

            // HUD Jugador
            DrawText(TextFormat("Puntaje: %d", puntaje), 20, 20, 20, YELLOW);
            DrawText("Vidas:", 20, 50, 20, WHITE);
            for (int i = 0; i < nave.vidas; i++) {
                if (IsTextureValid(texNave) && texNave.id > 0) {
                    Rectangle src = { 0, 0, (float)texNave.width, (float)texNave.height };
                    Rectangle dest = { 90.0f + (i * 30.0f), 50.0f, 40.0f, 40.0f };
                    DrawTexturePro(texNave, src, dest, (Vector2){ 0, 0 }, 0.0f, WHITE);
                } else {
                    DrawRectangle(90 + (i * 30), 50, 25, 25, BLUE);
                }
            }
            break;

        case ESTADO_GAME_OVER:
            DrawText("GAME OVER", anchoWin / 2 - MeasureText("GAME OVER", 40) / 2, 300, 40, RED);
            DrawText(TextFormat("Puntaje Final: %d", puntaje), anchoWin / 2 - MeasureText(TextFormat("Puntaje Final: %d", puntaje), 20) / 2, 360, 20, WHITE);
            DrawText("Presiona R para reiniciar", anchoWin / 2 - MeasureText("Presiona R para reiniciar", 20) / 2, 400, 20, GRAY);
            break;

        case ESTADO_VICTORIA:
            DrawText("¡VICTORIA!", anchoWin / 2 - MeasureText("¡VICTORIA!", 40) / 2, 300, 40, GREEN);
            DrawText("¡Has derrotado al Boss Final!", anchoWin / 2 - MeasureText("¡Has derrotado al Boss Final!", 20) / 2, 350, 20, WHITE);
            DrawText(TextFormat("Puntaje Final: %d", puntaje), anchoWin / 2 - MeasureText(TextFormat("Puntaje Final: %d", puntaje), 20) / 2, 390, 20, YELLOW);
            DrawText("Presiona R para reiniciar", anchoWin / 2 - MeasureText("Presiona R para reiniciar", 20) / 2, 430, 20, GRAY);
            break;
    }

    EndDrawing();
}