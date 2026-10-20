#include "raylib.h"
#include "graficos/Grafico.h"

int main(void) {
    const int anchoPantalla = 1000;
    const int altoPantalla = 800;
    
    InitWindow(anchoPantalla, altoPantalla, "Space Dodge - Versión Funcional");
    InitAudioDevice();
    SetTargetFPS(60);

    // Cargar Texturas
    Texture2D texFondo = LoadTexture("graficos/fondo.png");
    if (texFondo.id == 0) texFondo = LoadTexture("fondo.png");

    Texture2D texNave = LoadTexture("graficos/nave.png");
    if (texNave.id == 0) texNave = LoadTexture("nave.png");

    Texture2D texAsteroide = LoadTexture("graficos/asteroide.png");
    if (texAsteroide.id == 0) texAsteroide = LoadTexture("asteroide.png");
    
    // Cargar Audio
    Music musicaFondo = LoadMusicStream("graficos/musicaFondo.wav");
    if (!IsMusicValid(musicaFondo)) musicaFondo = LoadMusicStream("musicaFondo.wav");
    if (IsMusicValid(musicaFondo)) PlayMusicStream(musicaFondo);

    Sound sonidoDisparo = LoadSound("graficos/disparo.wav");
    if (!IsSoundValid(sonidoDisparo)) sonidoDisparo = LoadSound("disparo.wav");

    Sound sonidoExplosion = LoadSound("graficos/explosion.wav");
    if (!IsSoundValid(sonidoExplosion)) sonidoExplosion = LoadSound("explosion.wav");

    if (IsMusicValid(musicaFondo)) SetMusicVolume(musicaFondo, 0.5f);
    if (IsSoundValid(sonidoDisparo)) SetSoundVolume(sonidoDisparo, 0.5f);
    if (IsSoundValid(sonidoExplosion)) SetSoundVolume(sonidoExplosion, 1.0f);

    EstadoJuego estadoActual = ESTADO_MENU;
    Nave nave;
    Disparo disparos[MAX_DISPAROS];
    int puntaje = 0;

    // Inicializaciones
    InicializarAsteroides();
    nave.tamanio = (Vector2){ 60, 60 };
    nave.posicion = (Vector2){ anchoPantalla / 2.0f - nave.tamanio.x / 2.0f, altoPantalla - 100 };
    nave.velocidad = (Vector2){ 7.0f, 7.0f };

    for (int i = 0; i < MAX_DISPAROS; i++) disparos[i].activo = false;

    while (!WindowShouldClose()) {
        if (IsMusicValid(musicaFondo)) UpdateMusicStream(musicaFondo);

        switch (estadoActual) {
            case ESTADO_MENU:
                if (IsKeyPressed(KEY_ENTER)) {
                    InicializarAsteroides();
                    nave.posicion = (Vector2){ anchoPantalla / 2.0f - nave.tamanio.x / 2.0f, altoPantalla - 100 };
                    for (int i = 0; i < MAX_DISPAROS; i++) disparos[i].activo = false;
                    puntaje = 0;
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

                // Obtener asteroides actualizados
                const Asteroide *asteroidesArray = ObtenerAsteroides();

                // Colisiones: Disparo vs Asteroide
                for (int i = 0; i < MAX_DISPAROS; i++) {
                    if (disparos[i].activo) {
                        for (int j = 0; j < MAX_ASTEROIDES; j++) {
                            if (asteroidesArray[j].activo) {
                                Rectangle rectDisparo = { disparos[i].posicion.x, disparos[i].posicion.y, 5, 15 };
                                Rectangle rectAsteroide = { asteroidesArray[j].posicion.x, asteroidesArray[j].posicion.y, asteroidesArray[j].tamanio.x, asteroidesArray[j].tamanio.y };

                                if (CheckCollisionRecs(rectDisparo, rectAsteroide)) {
                                    disparos[i].activo = false;
                                    DesactivarAsteroide(j);
                                    puntaje += 50;
                                    if (IsSoundValid(sonidoExplosion)) PlaySound(sonidoExplosion);
                                }
                            }
                        }
                    }
                }

                // Colisión: Nave vs Asteroide
                for (int j = 0; j < MAX_ASTEROIDES; j++) {
                    if (asteroidesArray[j].activo) {
                        Rectangle rectNave = { nave.posicion.x, nave.posicion.y, nave.tamanio.x, nave.tamanio.y };
                        Rectangle rectAsteroide = { asteroidesArray[j].posicion.x, asteroidesArray[j].posicion.y, asteroidesArray[j].tamanio.x, asteroidesArray[j].tamanio.y };

                        if (CheckCollisionRecs(rectNave, rectAsteroide)) {
                            if (IsSoundValid(sonidoExplosion)) PlaySound(sonidoExplosion);
                            estadoActual = ESTADO_GAME_OVER;
                        }
                    }
                }

                puntaje++;
                break;

            case ESTADO_GAME_OVER:
                if (IsKeyPressed(KEY_R)) {
                    InicializarAsteroides();
                    nave.posicion = (Vector2){ anchoPantalla / 2.0f - nave.tamanio.x / 2.0f, altoPantalla - 100 };
                    for (int i = 0; i < MAX_DISPAROS; i++) disparos[i].activo = false;
                    puntaje = 0;
                    estadoActual = ESTADO_JUGANDO;
                }
                break;
        }

        // Renderizado usando el arreglo de asteroides en movimiento
        DibujarJuego(estadoActual, nave, (Asteroide*)ObtenerAsteroides(), disparos, puntaje, texNave, texAsteroide, texFondo);
    }

    if (texFondo.id > 0) UnloadTexture(texFondo);
    if (texNave.id > 0) UnloadTexture(texNave);
    if (texAsteroide.id > 0) UnloadTexture(texAsteroide);
    if (IsMusicValid(musicaFondo)) UnloadMusicStream(musicaFondo);
    if (IsSoundValid(sonidoDisparo)) UnloadSound(sonidoDisparo);
    if (IsSoundValid(sonidoExplosion)) UnloadSound(sonidoExplosion);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}