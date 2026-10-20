#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "Grafico.h"

void InicializarJuego(Nave *nave, Asteroide asteroides[], Disparo disparos[], int *puntaje) {
    nave->tamanio = (Vector2){ 60, 60 };
    nave->posicion = (Vector2){ 1000 / 2.0f - nave->tamanio.x / 2.0f, 800 - 100 };
    nave->velocidad = (Vector2){ 7.0f, 7.0f };

    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        asteroides[i].tamanio = (Vector2){ 50, 50 };
        asteroides[i].posicion = (Vector2){ rand() % (1000 - 50), - (rand() % 500) };
        asteroides[i].velocidad = 3.0f + (rand() % 3);
        asteroides[i].activo = true;
    }

    for (int i = 0; i < MAX_DISPAROS; i++) {
        disparos[i].activo = false;
    }

    *puntaje = 0;
}

void ActualizarJugador(Nave *nave) {
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) nave->posicion.x -= nave->velocidad.x;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) nave->posicion.x += nave->velocidad.x;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) nave->posicion.y -= nave->velocidad.y;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) nave->posicion.y += nave->velocidad.y;

    if (nave->posicion.x < 0) nave->posicion.x = 0;
    if (nave->posicion.x > 1000 - nave->tamanio.x) nave->posicion.x = 1000 - nave->tamanio.x;
    if (nave->posicion.y < 0) nave->posicion.y = 0;
    if (nave->posicion.y > 800 - nave->tamanio.y) nave->posicion.y = 800 - nave->tamanio.y;
}

bool ComprobarColision(Nave nave, Asteroide asteroides[]) {
    Rectangle rectNave = { nave.posicion.x, nave.posicion.y, nave.tamanio.x, nave.tamanio.y };
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        if (asteroides[i].activo) {
            Rectangle rectAsteroide = { asteroides[i].posicion.x, asteroides[i].posicion.y, asteroides[i].tamanio.x, asteroides[i].tamanio.y };
            if (CheckCollisionRecs(rectNave, rectAsteroide)) {
                return true;
            }
        }
    }
    return false;
}

void DibujarJuego(EstadoJuego estado, Nave nave, Asteroide asteroides[], Disparo disparos[], int puntaje, Texture2D texNave, Texture2D texAsteroide, Texture2D texFondo) {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawTexturePro(texFondo, (Rectangle){0, 0, (float)texFondo.width, (float)texFondo.height}, (Rectangle){0, 0, 1000, 800}, (Vector2){0, 0}, 0.0f, WHITE);

    switch (estado) {
        case ESTADO_MENU:
            DrawText("INVASORES", 1000 / 2 - MeasureText("INVASORES", 40) / 2, 300, 40, YELLOW);
            DrawText("Presiona ENTER para iniciar", 1000 / 2 - MeasureText("Presiona ENTER para iniciar", 20) / 2, 400, 20, WHITE);
            break;

        case ESTADO_JUGANDO:
            DrawTexturePro(texNave, (Rectangle){0, 0, (float)texNave.width, (float)texNave.height}, (Rectangle){nave.posicion.x, nave.posicion.y, nave.tamanio.x, nave.tamanio.y}, (Vector2){0,0}, 0.0f, WHITE);

            for (int i = 0; i < MAX_ASTEROIDES; i++) {
                if (asteroides[i].activo) {
                    DrawTexturePro(texAsteroide, (Rectangle){0, 0, (float)texAsteroide.width, (float)texAsteroide.height}, (Rectangle){asteroides[i].posicion.x, asteroides[i].posicion.y, asteroides[i].tamanio.x, asteroides[i].tamanio.y}, (Vector2){0,0}, 0.0f, WHITE);
                }
            }

            for (int i = 0; i < MAX_DISPAROS; i++) {
                if (disparos[i].activo) {
                    DrawRectangle(disparos[i].posicion.x, disparos[i].posicion.y, 5, 15, RED);
                }
            }

            DrawText(TextFormat("Puntaje: %d", puntaje), 20, 20, 20, YELLOW);
            break;

        case ESTADO_GAME_OVER:
            DrawText("GAME OVER", 1000 / 2 - MeasureText("GAME OVER", 40) / 2, 300, 40, RED);
            DrawText(TextFormat("Puntaje Final: %d", puntaje), 1000 / 2 - MeasureText(TextFormat("Puntaje Final: %d", puntaje), 20) / 2, 360, 20, WHITE);
            DrawText("Presiona R para reiniciar", 1000 / 2 - MeasureText("Presiona R para reiniciar", 20) / 2, 400, 20, GRAY);
            break;
    }

    EndDrawing();
}