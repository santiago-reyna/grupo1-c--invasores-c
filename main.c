#include <math.h>
#include <stddef.h>
#include "raylib.h"
#include "asteroide/asteroide.h"
#include "movimientos/juego.h"
#include "graficos/Grafico.h"

int main(void) {
    const int anchoPantalla = 1000;
    const int altoPantalla = 800;
    
    InitWindow(anchoPantalla, altoPantalla, "Space Dodge - Hitbox Fix");
    InitAudioDevice();
    SetTargetFPS(60);

    // Cargar Texturas con validación segura
    Texture2D texFondo = LoadTexture("assets/fondo.png");
    if (texFondo.id == 0) texFondo = LoadTexture("fondo.png");

    Texture2D texNave = LoadTexture("assets/nave.png");
    if (texNave.id == 0) texNave = LoadTexture("nave.png");

    Texture2D texAsteroide = LoadTexture("assets/asteroide.png");
    if (texAsteroide.id == 0) texAsteroide = LoadTexture("asteroide.png");
    
    // Cargar Audio con validación segura
    Music musicaFondo = LoadMusicStream("assets/musicaFondo.wav");
    if (!IsMusicValid(musicaFondo)) musicaFondo = LoadMusicStream("musicaFondo.wav");
    if (IsMusicValid(musicaFondo)) {
        PlayMusicStream(musicaFondo);
        SetMusicVolume(musicaFondo, 0.5f);
    }

    Sound sonidoDisparo = LoadSound("assets/disparo.wav");
    if (!IsSoundValid(sonidoDisparo)) sonidoDisparo = LoadSound("disparo.wav");
    if (IsSoundValid(sonidoDisparo)) SetSoundVolume(sonidoDisparo, 0.5f);

    Sound sonidoExplosion = LoadSound("assets/explosion.wav");
    if (!IsSoundValid(sonidoExplosion)) sonidoExplosion = LoadSound("explosion.wav");
    if (IsSoundValid(sonidoExplosion)) SetSoundVolume(sonidoExplosion, 1.0f);

    Sound sonidoDanio = LoadSound("assets/danio.wav");
    if (!IsSoundValid(sonidoDanio)) sonidoDanio = LoadSound("danio.wav");
    if (IsSoundValid(sonidoDanio)) SetSoundVolume(sonidoDanio, 0.8f);

    EstadoJuego estadoActual = ESTADO_MENU;
    Nave nave;
    Disparo disparos[MAX_DISPAROS];
    Disparo disparosBoss[MAX_DISPAROS_BOSS];
    Boss boss;
    int puntaje = 0;

    // Temporizadores de eventos
    float tiempoPartida = 0.0f;
    bool asteroidesDetenidos = false;

    // Hitbox protegida: no se permitirá que el margen inverta las dimensiones
    const float margenX = 40.0f; 
    const float margenY = 32.0f;

    InicializarAsteroides(anchoPantalla);
    InicializarJuego(&nave, disparos, &puntaje, anchoPantalla, altoPantalla);

    while (!WindowShouldClose()) {
        if (IsMusicValid(musicaFondo)) UpdateMusicStream(musicaFondo);

        switch (estadoActual) {
            case ESTADO_MENU:
                if (IsKeyPressed(KEY_ENTER)) {
                    InicializarAsteroides(anchoPantalla);
                    InicializarJuego(&nave, disparos, &puntaje, anchoPantalla, altoPantalla);
                    
                    // Resetear Boss y Temporizadores de forma controlada
                    tiempoPartida = 0.0f;
                    asteroidesDetenidos = false;
                    boss.activo = false;
                    boss.animandoEntrada = false;
                    boss.vidas = 150;
                    boss.tamanio = (Vector2){ 200.0f, 200.0f };
                    boss.posicion = (Vector2){ ((float)anchoPantalla / 2.0f) - 100.0f, -220.0f };
                    boss.temporizadorAtaque = 0.0f;
                    boss.temporizadorTeledirigido = 0.0f;
                    boss.tiempoEnPantalla = 0.0f;

                    for (int i = 0; i < MAX_DISPAROS_BOSS; i++) {
                        disparosBoss[i].activo = false;
                        disparosBoss[i].esTeledirigido = false;
                    }

                    estadoActual = ESTADO_JUGANDO;
                }
                break;

            case ESTADO_JUGANDO: {
                float dt = GetFrameTime();
                tiempoPartida += dt;

                ActualizarJugador(&nave);

                // --- 1. LÓGICA DE TIEMPO Y APARICIÓN DEL BOSS ---
                if (tiempoPartida >= 20.0f && !asteroidesDetenidos) {
                    asteroidesDetenidos = true;
                }

                if (tiempoPartida >= 27.0f && !boss.activo && asteroidesDetenidos) {
                    boss.activo = true;
                    boss.animandoEntrada = true;
                }

                if (!asteroidesDetenidos) {
                    ActualizarDificultadAsteroides(dt);
                    GenerarYMoverAsteroides(anchoPantalla, altoPantalla, dt);
                } else {
                    NodoAsteroide *actual = ObtenerListaAsteroides();
                    while (actual != NULL) {
                        actual->posicion.y += actual->velocidad * dt;
                        actual = actual->siguiente;
                    }
                }

                // --- 2. ANIMACIÓN Y COMPORTAMIENTO DEL BOSS ---
                if (boss.activo) {
                    if (boss.animandoEntrada) {
                        boss.posicion.y += 100.0f * dt;
                        if (boss.posicion.y >= 80.0f) {
                            boss.posicion.y = 80.0f;
                            boss.animandoEntrada = false;
                        }
                    } else {
                        boss.tiempoEnPantalla += dt;
                        boss.temporizadorAtaque += dt;
                        boss.temporizadorTeledirigido += dt;

                        float cadencia = (boss.tiempoEnPantalla >= 5.0f) ? 0.8f : 1.5f;

                        if (boss.temporizadorAtaque >= cadencia) {
                            boss.temporizadorAtaque = 0.0f;

                            if (boss.tiempoEnPantalla < 5.0f) {
                                for (int i = 0; i < MAX_DISPAROS_BOSS; i++) {
                                    if (!disparosBoss[i].activo) {
                                        disparosBoss[i].posicion = (Vector2){ boss.posicion.x + boss.tamanio.x / 2.0f - 9.0f, boss.posicion.y + boss.tamanio.y };
                                        disparosBoss[i].velocidad = (Vector2){ 0.0f, 6.0f };
                                        disparosBoss[i].esTeledirigido = false;
                                        disparosBoss[i].activo = true;
                                        if (IsSoundValid(sonidoDisparo)) PlaySound(sonidoDisparo);
                                        break;
                                    }
                                }
                            } else {
                                struct { float velX; float velY; } config[3] = {
                                    { 0.0f, 7.0f }, { -3.0f, 6.0f }, { 3.0f, 6.0f }
                                };
                                for (int c = 0; c < 3; c++) {
                                    for (int i = 0; i < MAX_DISPAROS_BOSS; i++) {
                                        if (!disparosBoss[i].activo) {
                                            disparosBoss[i].posicion = (Vector2){ boss.posicion.x + boss.tamanio.x / 2.0f - 9.0f, boss.posicion.y + boss.tamanio.y };
                                            disparosBoss[i].velocidad = (Vector2){ config[c].velX, config[c].velY };
                                            disparosBoss[i].esTeledirigido = false;
                                            disparosBoss[i].activo = true;
                                            break; // Avanza a la siguiente configuración del arreglo
                                        }
                                    }
                                }
                                if (IsSoundValid(sonidoDisparo)) PlaySound(sonidoDisparo);
                            }
                        }

                        // Generar Asteroide Teledirigido
                        if (boss.temporizadorTeledirigido >= 3.0f) {
                            boss.temporizadorTeledirigido = 0.0f;
                            for (int i = 0; i < MAX_DISPAROS_BOSS; i++) {
                                if (!disparosBoss[i].activo) {
                                    disparosBoss[i].posicion = (Vector2){ boss.posicion.x + boss.tamanio.x / 2.0f - 20.0f, boss.posicion.y + boss.tamanio.y };
                                    disparosBoss[i].velocidad = (Vector2){ 0.0f, 0.0f };
                                    disparosBoss[i].esTeledirigido = true;
                                    disparosBoss[i].activo = true;
                                    break;
                                }
                            }
                        }
                    }
                }

                // --- 3. LÓGICA DE DISPARO JUGADOR ---
                if (puntaje >= 2400 && nave.nivelDisparo < 3) nave.nivelDisparo = 3;

                if (IsKeyPressed(KEY_SPACE)) {
                    if (nave.nivelDisparo == 1) {
                        for (int i = 0; i < MAX_DISPAROS; i++) {
                            if (!disparos[i].activo) {
                                disparos[i].posicion = (Vector2){ nave.posicion.x + nave.tamanio.x / 2.0f - 2.5f, nave.posicion.y };
                                disparos[i].velocidad = (Vector2){ 0.0f, -10.0f };
                                disparos[i].activo = true;
                                if (IsSoundValid(sonidoDisparo)) PlaySound(sonidoDisparo);
                                break;
                            }
                        }
                    } else if (nave.nivelDisparo == 2) {
                        int creados = 0;
                        for (int i = 0; i < MAX_DISPAROS && creados < 2; i++) {
                            if (!disparos[i].activo) {
                                float offsetX = (creados == 0) ? 5.0f : nave.tamanio.x - 10.0f;
                                disparos[i].posicion = (Vector2){ nave.posicion.x + offsetX, nave.posicion.y };
                                disparos[i].velocidad = (Vector2){ 0.0f, -10.0f };
                                disparos[i].activo = true;
                                creados++;
                            }
                        }
                        if (creados > 0 && IsSoundValid(sonidoDisparo)) PlaySound(sonidoDisparo);
                    } else if (nave.nivelDisparo >= 3) {
                        int creados = 0;
                        struct { float offX; float velX; float velY; } config[3] = {
                            { nave.tamanio.x / 2.0f - 2.5f,  0.0f, -16.0f },
                            { 0.0f,                        -3.0f, -14.0f },
                            { nave.tamanio.x - 5.0f,        3.0f, -14.0f }
                        };
                        for (int c = 0; c < 3; c++) {
                            for (int i = 0; i < MAX_DISPAROS; i++) {
                                if (!disparos[i].activo) {
                                    disparos[i].posicion = (Vector2){ nave.posicion.x + config[c].offX, nave.posicion.y };
                                    disparos[i].velocidad = (Vector2){ config[c].velX, config[c].velY };
                                    disparos[i].activo = true;
                                    creados++;
                                    break;
                                }
                            }
                        }
                        if (creados > 0 && IsSoundValid(sonidoDisparo)) PlaySound(sonidoDisparo);
                    }
                }

                // Movimiento disparos jugador
                for (int i = 0; i < MAX_DISPAROS; i++) {
                    if (disparos[i].activo) {
                        disparos[i].posicion.x += disparos[i].velocidad.x;
                        disparos[i].posicion.y += disparos[i].velocidad.y;
                        if (disparos[i].posicion.y < 0.0f || disparos[i].posicion.x < -10.0f || disparos[i].posicion.x > (float)anchoPantalla + 10.0f) {
                            disparos[i].activo = false;
                        }
                    }
                }

                // Movimiento proyectiles Boss
                for (int i = 0; i < MAX_DISPAROS_BOSS; i++) {
                    if (disparosBoss[i].activo) {
                        if (disparosBoss[i].esTeledirigido) {
                            Vector2 centroNave = { nave.posicion.x + nave.tamanio.x / 2.0f, nave.posicion.y + nave.tamanio.y / 2.0f };
                            float dirX = centroNave.x - disparosBoss[i].posicion.x;
                            float dirY = centroNave.y - disparosBoss[i].posicion.y;
                            float magnitud = sqrtf(dirX * dirX + dirY * dirY);

                            if (magnitud > 0.0001f) { // Prevención división por cero muy pequeños
                                float velocidadLenta = 150.0f;
                                disparosBoss[i].posicion.x += (dirX / magnitud) * velocidadLenta * dt;
                                disparosBoss[i].posicion.y += (dirY / magnitud) * velocidadLenta * dt;
                            }
                        } else {
                            disparosBoss[i].posicion.x += disparosBoss[i].velocidad.x;
                            disparosBoss[i].posicion.y += disparosBoss[i].velocidad.y;
                        }

                        if (disparosBoss[i].posicion.y > (float)altoPantalla + 20.0f || disparosBoss[i].posicion.y < -50.0f ||
                            disparosBoss[i].posicion.x < -20.0f || disparosBoss[i].posicion.x > (float)anchoPantalla + 20.0f) {
                            disparosBoss[i].activo = false;
                        }
                    }
                }

                // --- 4. COLISIONES ---
                
                // Generación defensiva de la Hitbox
                float anchoCaja = (nave.tamanio.x > margenX * 2.0f) ? nave.tamanio.x - (margenX * 2.0f) : 1.0f;
                float altoCaja = (nave.tamanio.y > margenY * 2.0f) ? nave.tamanio.y - (margenY * 2.0f) : 1.0f;
                
                Rectangle rectNave = { 
                    nave.posicion.x + margenX, 
                    nave.posicion.y + margenY, 
                    anchoCaja, 
                    altoCaja 
                };

                // Colisiones: Disparos del Jugador
                for (int i = 0; i < MAX_DISPAROS; i++) {
                    if (disparos[i].activo) {
                        Rectangle rectDisparo = { disparos[i].posicion.x, disparos[i].posicion.y, 5.0f, 15.0f };
                        
                        // Impacto contra Asteroides Comunes
                        NodoAsteroide *actual = ObtenerListaAsteroides();
                        while (actual != NULL) {
                            NodoAsteroide *siguiente = actual->siguiente;
                            Rectangle rectAsteroide = { actual->posicion.x, actual->posicion.y, actual->tamanio.x, actual->tamanio.y };

                            if (CheckCollisionRecs(rectDisparo, rectAsteroide)) {
                                disparos[i].activo = false;
                                EliminarAsteroide(actual, anchoPantalla, &nave);
                                puntaje += 50;
                                if (IsSoundValid(sonidoExplosion)) PlaySound(sonidoExplosion);
                                break;
                            }
                            actual = siguiente;
                        }

                        // Impacto Disparo Jugador vs Boss
                        if (boss.activo && disparos[i].activo) {
                            Rectangle rectBoss = { boss.posicion.x, boss.posicion.y, boss.tamanio.x, boss.tamanio.y };
                            if (CheckCollisionRecs(rectDisparo, rectBoss)) {
                                disparos[i].activo = false;
                                boss.vidas--;
                                puntaje += 200;
                                if (IsSoundValid(sonidoExplosion)) PlaySound(sonidoExplosion);

                                if (boss.vidas <= 0) {
                                    boss.activo = false;
                                    estadoActual = ESTADO_VICTORIA;
                                }
                            }
                        }

                        // Impacto Disparo Jugador vs Asteroide Teledirigido del Boss
                        if (disparos[i].activo) {
                            for (int k = 0; k < MAX_DISPAROS_BOSS; k++) {
                                if (disparosBoss[k].activo && disparosBoss[k].esTeledirigido) {
                                    Rectangle rectTeledirigido = { disparosBoss[k].posicion.x, disparosBoss[k].posicion.y, 40.0f, 40.0f };
                                    if (CheckCollisionRecs(rectDisparo, rectTeledirigido)) {
                                        disparos[i].activo = false;
                                        disparosBoss[k].activo = false;
                                        puntaje += 25;
                                        if (IsSoundValid(sonidoExplosion)) PlaySound(sonidoExplosion);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }

                // Disparos/Teledirigidos del Boss vs Nave
                for (int i = 0; i < MAX_DISPAROS_BOSS; i++) {
                    if (disparosBoss[i].activo) {
                        Rectangle rectImpacto = disparosBoss[i].esTeledirigido 
                            ? (Rectangle){ disparosBoss[i].posicion.x, disparosBoss[i].posicion.y, 40.0f, 40.0f }
                            : (Rectangle){ disparosBoss[i].posicion.x, disparosBoss[i].posicion.y, 18.0f, 18.0f };

                        if (CheckCollisionRecs(rectNave, rectImpacto)) {
                            disparosBoss[i].activo = false;
                            nave.vidas--;
                            if (nave.vidas <= 0) {
                                if (IsSoundValid(sonidoExplosion)) PlaySound(sonidoExplosion);
                                estadoActual = ESTADO_GAME_OVER;
                            } else {
                                if (IsSoundValid(sonidoDanio)) PlaySound(sonidoDanio);
                            }
                        }
                    }
                }

                // Nave vs Asteroides Comunes
                NodoAsteroide *actualAsteroide = ObtenerListaAsteroides();
                while (actualAsteroide != NULL) {
                    NodoAsteroide *siguiente = actualAsteroide->siguiente;
                    Rectangle rectAsteroide = { actualAsteroide->posicion.x, actualAsteroide->posicion.y, actualAsteroide->tamanio.x, actualAsteroide->tamanio.y };

                    if (CheckCollisionRecs(rectNave, rectAsteroide)) {
                        nave.vidas--;
                        EliminarAsteroide(actualAsteroide, anchoPantalla, &nave);

                        if (nave.vidas <= 0) {
                            if (IsSoundValid(sonidoExplosion)) PlaySound(sonidoExplosion);
                            estadoActual = ESTADO_GAME_OVER;
                        } else {
                            if (IsSoundValid(sonidoDanio)) PlaySound(sonidoDanio);
                        }
                        break;
                    }
                    actualAsteroide = siguiente;
                }

                // Nave vs Boss (Choque directo)
                if (boss.activo) {
                    Rectangle rectBoss = { boss.posicion.x, boss.posicion.y, boss.tamanio.x, boss.tamanio.y };
                    if (CheckCollisionRecs(rectNave, rectBoss)) {
                        nave.vidas = 0;
                        if (IsSoundValid(sonidoExplosion)) PlaySound(sonidoExplosion);
                        estadoActual = ESTADO_GAME_OVER;
                    }
                }

                break;
            }

            case ESTADO_GAME_OVER:
            case ESTADO_VICTORIA:
                if (IsKeyPressed(KEY_R)) {
                    InicializarAsteroides(anchoPantalla);
                    InicializarJuego(&nave, disparos, &puntaje, anchoPantalla, altoPantalla);

                    tiempoPartida = 0.0f;
                    asteroidesDetenidos = false;

                    boss.activo = false;
                    boss.animandoEntrada = false;
                    boss.vidas = 150;
                    boss.tamanio = (Vector2){ 200.0f, 200.0f };
                    boss.posicion = (Vector2){ ((float)anchoPantalla / 2.0f) - 100.0f, -220.0f };
                    boss.temporizadorAtaque = 0.0f;
                    boss.temporizadorTeledirigido = 0.0f;
                    boss.tiempoEnPantalla = 0.0f;

                    for (int i = 0; i < MAX_DISPAROS_BOSS; i++) {
                        disparosBoss[i].activo = false;
                        disparosBoss[i].esTeledirigido = false;
                    }

                    estadoActual = ESTADO_JUGANDO;
                }
                break;
        }

        DibujarJuego(estadoActual, nave, disparos, boss, disparosBoss, puntaje, texNave, texAsteroide, texFondo);
    }

    // Limpieza segura
    if (texFondo.id > 0) UnloadTexture(texFondo);
    if (texNave.id > 0) UnloadTexture(texNave);
    if (texAsteroide.id > 0) UnloadTexture(texAsteroide);
    if (IsMusicValid(musicaFondo)) { StopMusicStream(musicaFondo); UnloadMusicStream(musicaFondo); }
    if (IsSoundValid(sonidoDisparo)) UnloadSound(sonidoDisparo);
    if (IsSoundValid(sonidoExplosion)) UnloadSound(sonidoExplosion);
    if (IsSoundValid(sonidoDanio)) UnloadSound(sonidoDanio);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}