#include "raylib.h"
#include "Grafico.h"

int main() {
    const int anchoPantalla = 1000;
    const int altoPantalla = 800;
    
    InitWindow(anchoPantalla, altoPantalla, "Fondo nave");
    InitAudioDevice();
    SetTargetFPS(60);

    Texture2D texFondo = LoadTexture("fondo.png");
    Texture2D texNave = LoadTexture("nave.png");
    Texture2D texAsteroide = LoadTexture("asteroide.png");
    
    Music musicaFondo = LoadMusicStream("musicaFondo.wav");
    if (IsMusicValid(musicaFondo)) {
        PlayMusicStream(musicaFondo);
    }

    

    
    Sound sonidoDisparo = LoadSound("disparo.wav");
    Sound sonidoExplosion = LoadSound("explosion.wav");

    
    SetSoundPitch(sonidoDisparo, 1.0f); 

    SetMusicVolume(musicaFondo, 0.5f);     
    SetSoundVolume(sonidoDisparo, 0.5f);    
    SetSoundVolume(sonidoExplosion, 10.0f);  

    EstadoJuego estadoActual = ESTADO_MENU;
    Nave nave;
    Asteroide asteroides[MAX_ASTEROIDES];
    Disparo disparos[MAX_DISPAROS];
    int puntaje = 0;

    InicializarJuego(&nave, asteroides, disparos, &puntaje);

    while (!WindowShouldClose()) {
        if (IsMusicValid(musicaFondo)) {
            UpdateMusicStream(musicaFondo);
        }

        switch (estadoActual) {
            case ESTADO_MENU:
                if (IsKeyPressed(KEY_ENTER)) {
                    InicializarJuego(&nave, asteroides, disparos, &puntaje);
                    estadoActual = ESTADO_JUGANDO;
                }
                break;

            case ESTADO_JUGANDO:
                ActualizarJugador(&nave);
                GenerarYMoverAsteroides(asteroides);

                // Disparar
                if (IsKeyPressed(KEY_SPACE)) {
                    for (int i = 0; i < MAX_DISPAROS; i++) {
                        if (!disparos[i].activo) {
                            disparos[i].posicion = (Vector2){ nave.posicion.x + nave.tamanio.x / 2 - 2.5f, nave.posicion.y };
                            disparos[i].velocidad = (Vector2){ 0, -10.0f };
                            disparos[i].activo = true;
                            
                            if (IsSoundValid(sonidoDisparo)) PlaySound(sonidoDisparo);
                            break;
                        }
                    }
                }

                // Mover disparos
                for (int i = 0; i < MAX_DISPAROS; i++) {
                    if (disparos[i].activo) {
                        disparos[i].posicion.y += disparos[i].velocidad.y;
                        if (disparos[i].posicion.y < 0) {
                            disparos[i].activo = false;
                        }
                    }
                }

                // Colisiones Disparo vs Asteroide
                for (int i = 0; i < MAX_DISPAROS; i++) {
                    if (disparos[i].activo) {
                        for (int j = 0; j < MAX_ASTEROIDES; j++) {
                            if (asteroides[j].activo) {
                                Rectangle rectDisparo = { disparos[i].posicion.x, disparos[i].posicion.y, 5, 15 };
                                Rectangle rectAsteroide = { asteroides[j].posicion.x, asteroides[j].posicion.y, asteroides[j].tamanio.x, asteroides[j].tamanio.y };

                                if (CheckCollisionRecs(rectDisparo, rectAsteroide)) {
                                    disparos[i].activo = false;
                                    asteroides[j].activo = false;
                                    puntaje += 50;

                                    if (IsSoundValid(sonidoExplosion)) PlaySound(sonidoExplosion);
                                }
                            }
                        }
                    }
                }

                // Colisión Nave vs Asteroide
                if (ComprobarColision(nave, asteroides)) {
                    if (IsSoundValid(sonidoExplosion)) PlaySound(sonidoExplosion);
                    estadoActual = ESTADO_GAME_OVER;
                }

                puntaje++;
                break;

            case ESTADO_GAME_OVER:
                if (IsKeyPressed(KEY_R)) {
                    InicializarJuego(&nave, asteroides, disparos, &puntaje);
                    estadoActual = ESTADO_JUGANDO;
                }
                break;
        }

        DibujarJuego(estadoActual, nave, asteroides, disparos, puntaje, texNave, texAsteroide, texFondo);
    }

    UnloadTexture(texFondo);
    UnloadTexture(texNave);
    UnloadTexture(texAsteroide);
    UnloadMusicStream(musicaFondo);
    UnloadSound(sonidoDisparo);
    UnloadSound(sonidoExplosion);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}