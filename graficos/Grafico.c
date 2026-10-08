#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "grafico.h"


void inicializarJuego(Nave *nave, Asteroide asteroides[], int *puntaje) {
    nave->tamanio = (Vector2){50, 50};
    nave->posicion = (Vector2){ (float)GetScreenWidth() / 2 - nave->tamanio.x / 2, (float)GetScreenHeight() - 70 };
    nave->velocidad = 7.0f;
    nave->color = BLUE;

    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        asteroides[i].tamanio = (Vector2){40, 40};
        asteroides[i].activo = false;
    }

    *puntaje = 0;
}


void ActualizarJugador(Nave *nave) {
	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
		nave->posicion.x -= nave->velocidad;
	}
	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
		nave->posicion.x += nave->velocidad;
	}
	
	if (nave->posicion.x < 0) nave->posicion.x = 0;
	if (nave->posicion.x + nave->tamanio.x > GetScreenWidth()) {
		nave->posicion.x = GetScreenWidth() - nave->tamanio.x;
	}
}


void GenerarYMoverAsteroides(Asteroide asteroides[]) {
	
	if (GetRandomValue(0, 100) < 5) { 
		for (int i = 0; i < MAX_ASTEROIDES; i++) {
			if (!asteroides[i].activo) {
				asteroides[i].posicion = (Vector2){ (float)GetRandomValue(0, GetScreenWidth() - 40), -40 };
				asteroides[i].velocidad = (float)GetRandomValue(3, 7);
				asteroides[i].activo = true;
				break;
			}
		}
	}

for (int i = 0; i < MAX_ASTEROIDES; i++) {
		if (asteroides[i].activo) {
			asteroides[i].posicion.y += asteroides[i].velocidad;
			
			// Desactivar si salen por abajo para reutilizarlos[cite: 7]
			if (asteroides[i].posicion.y > GetScreenHeight()) {
				asteroides[i].activo = false;
			}
		}
	}
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



void DibujarJuego(EstadoJuego estado, Nave nave, Asteroide asteroides[], int puntaje, Texture2D texNave, Texture2D texAsteroide, Texture2D texFondo) {
	BeginDrawing();
	ClearBackground(BLACK);

DrawTexture(texFondo, 0, 0, WHITE); 

switch(estado){

case ESTADO_MENU:
		DrawText("SPACE DODGE", GetScreenWidth() / 2 - MeasureText("SPACE DODGE", 40) / 2, 180, 40, RAYWHITE);
		DrawText("Presiona ENTER para jugar", GetScreenWidth() / 2 - MeasureText("Presiona ENTER para jugar", 20) / 2, 280, 20, LIGHTGRAY);
		break;    

    case ESTADO_JUGANDO:
        
        DrawTextureV(texNave, nave.posicion, WHITE);
		
		for (int i = 0; i < MAX_ASTEROIDES; i++) {
			if (asteroides[i].activo) {
				DrawTextureV(texAsteroide, asteroides[i].posicion, WHITE);
			}
		}
		
	
		DrawText(TextFormat("Puntaje: %d", puntaje), 15, 15, 20, YELLOW);
		break; 

        case ESTADO_GAME_OVER:
		DrawText("¡GAME OVER!", GetScreenWidth() / 2 - MeasureText("¡GAME OVER!", 40) / 2, 160, 40, RED);
		DrawText(TextFormat("Puntaje Final: %d", puntaje), GetScreenWidth() / 2 - MeasureText(TextFormat("Puntaje Final: %d", puntaje), 20) / 2, 230, 20, RAYWHITE);
		DrawText("Presiona R para reiniciar", GetScreenWidth() / 2 - MeasureText("Presiona R para reiniciar", 20) / 2, 300, 20, LIGHTGRAY);
		break;
	}
	
	EndDrawing();
}









