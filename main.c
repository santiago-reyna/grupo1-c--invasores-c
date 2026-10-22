#include "raylib.h"
#include "graficos/Grafico.h"

int main(void) {
    const int anchoPantalla = 1000;
    const int altoPantalla = 800;
    
    InitWindow(anchoPantalla, altoPantalla, "Space Dodge - Versión Funcional");
    InitAudioDevice();
    SetTargetFPS(60);

    // Cargar Texturas
    Texture2D texFondo = LoadTexture("assets/fondo.png");
    if (texFondo.id == 0) texFondo = LoadTexture("fondo.png");

    Texture2D texNave = LoadTexture("assets/nave.png");
    if (texNave.id == 0) texNave = LoadTexture("nave.png");

    Texture2D texAsteroide = LoadTexture("assets/asteroide.png");
    if (texAsteroide.id == 0) texAsteroide = LoadTexture("asteroide.png");
    
    // Cargar Audio
    Music musicaFondo = LoadMusicStream("assets/musicaFondo.wav");
    if (!IsMusicValid(musicaFondo)) musicaFondo = LoadMusicStream("musicaFondo.wav");
    if (IsMusicValid(musicaFondo)) PlayMusicStream(musicaFondo);

    Sound sonidoDisparo = LoadSound("assets/disparo.wav");
    if (!IsSoundValid(sonidoDisparo)) sonidoDisparo = LoadSound("disparo.wav");

    Sound sonidoExplosion = LoadSound("assets/explosion.wav");
    if (!IsSoundValid(sonidoExplosion)) sonidoExplosion = LoadSound("explosion.wav");

    Sound sonidoDanio = LoadSound("assets/danio.wav");
    if (!IsSoundValid(sonidoDanio)) sonidoDanio = LoadSound("danio.wav");

    if (IsMusicValid(musicaFondo)) SetMusicVolume(musicaFondo, 0.5f);
    if (IsSoundValid(sonidoDisparo)) SetSoundVolume(sonidoDisparo, 0.5f);
    if (IsSoundValid(sonidoExplosion)) SetSoundVolume(sonidoExplosion, 1.0f);
    if (IsSoundValid(sonidoDanio)) SetSoundVolume(sonidoDanio, 0.8f);

    EstadoJuego estadoActual = ESTADO_MENU;
    Nave nave;
    Disparo disparos[MAX_DISPAROS];
    int puntaje = 0;

    // Inicializaciones principales
    InicializarAsteroides(anchoPantalla);
    InicializarJuego(&nave, disparos, &puntaje, anchoPantalla, altoPantalla);

    while (!WindowShouldClose()) {
        if (IsMusicValid(musicaFondo)) UpdateMusicStream(musicaFondo);

        switch (estadoActual) {
            case ESTADO_MENU:
                if (IsKeyPressed(KEY_ENTER)) {
                    InicializarAsteroides(anchoPantalla);
                    InicializarJuego(&nave, disparos, &puntaje, anchoPantalla, altoPantalla);
                    estadoActual = ESTADO_JUGANDO;
                }
                break;

            case ESTADO_JUGANDO:
                ActualizarJugador(&nave);
                GenerarYMoverAsteroides(anchoPantalla, altoPantalla, GetFrameTime());

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

                // Movimiento disparos
                for (int i = 0; i < MAX_DISPAROS; i++) {
                    if (disparos[i].activo) {
                        disparos[i].posicion.y += disparos[i].velocidad.y;
                        if (disparos[i].posicion.y < 0) disparos[i].activo = false;
                    }
                }

                // Colisiones: Disparo vs Asteroide (Lista Enlazada)
                for (int i = 0; i < MAX_DISPAROS; i++) {
                    if (disparos[i].activo) {
                        Rectangle rectDisparo = { disparos[i].posicion.x, disparos[i].posicion.y, 5, 15 };
                        NodoAsteroide *actual = ObtenerListaAsteroides();
                        
                        while (actual != NULL) {
                            NodoAsteroide *siguiente = actual->siguiente;
                            Rectangle rectAsteroide = { actual->posicion.x, actual->posicion.y, actual->tamanio.x, actual->tamanio.y };

                            if (CheckCollisionRecs(rectDisparo, rectAsteroide)) {
                                disparos[i].activo = false;
                                EliminarAsteroide(actual, anchoPantalla);
                                puntaje += 50;
                                if (IsSoundValid(sonidoExplosion)) PlaySound(sonidoExplosion);
                                break;
                            }
                            actual = siguiente;
                        }
                    }
                }

                // Colisión: Nave vs Asteroide (Lista Enlazada)
                Rectangle rectNave = { nave.posicion.x, nave.posicion.y, nave.tamanio.x, nave.tamanio.y };
                NodoAsteroide *actualAsteroide = ObtenerListaAsteroides();

                while (actualAsteroide != NULL) {
                    NodoAsteroide *siguiente = actualAsteroide->siguiente;
                    Rectangle rectAsteroide = { actualAsteroide->posicion.x, actualAsteroide->posicion.y, actualAsteroide->tamanio.x, actualAsteroide->tamanio.y };

                    if (CheckCollisionRecs(rectNave, rectAsteroide)) {
                        nave.vidas--;
                        EliminarAsteroide(actualAsteroide, anchoPantalla);

                        if (nave.vidas <= 0) {
                            if (IsSoundValid(sonidoExplosion)) PlaySound(sonidoExplosion);
                            estadoActual = ESTADO_GAME_OVER;
                        } else {
                            if (IsSoundValid(sonidoDanio)) PlaySound(sonidoDanio);
                            if (IsSoundValid(sonidoExplosion)) PlaySound(sonidoExplosion);
                        }
                        break;
                    }
                    actualAsteroide = siguiente;
                }

                puntaje++;
                break;

            case ESTADO_GAME_OVER:
                if (IsKeyPressed(KEY_R)) {
                    InicializarAsteroides(anchoPantalla);
                    InicializarJuego(&nave, disparos, &puntaje, anchoPantalla, altoPantalla);
                    estadoActual = ESTADO_JUGANDO;
                }
                break;
        }

        // Renderizado
        DibujarJuego(estadoActual, nave, disparos, puntaje, texNave, texAsteroide, texFondo);
    }

    // Limpieza de recursos
    if (texFondo.id > 0) UnloadTexture(texFondo);
    if (texNave.id > 0) UnloadTexture(texNave);
    if (texAsteroide.id > 0) UnloadTexture(texAsteroide);
    if (IsMusicValid(musicaFondo)) UnloadMusicStream(musicaFondo);
    if (IsSoundValid(sonidoDisparo)) UnloadSound(sonidoDisparo);
    if (IsSoundValid(sonidoExplosion)) UnloadSound(sonidoExplosion);
    if (IsSoundValid(sonidoDanio)) UnloadSound(sonidoDanio);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}