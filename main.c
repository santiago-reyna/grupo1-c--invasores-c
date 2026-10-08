#include "raylib.h"
#include "grafico.h"

int main (){
    
    const int anchoPantalla = 1000;
    const int altoPantalla = 800;
    initWindow(anchoPantalla, altoPantalla, "Fondo nave");
    initAudioDevice();
    setTargetFPS(60);


    Texture2D texFondo = LoadTexture("fondo.png");
    Texture2D texNave = LoadTexture("nave.png");
    Texture2D texAsteroide = LoadTexture("asteroide.png");
    
    Sound efectoExplosion = LoadSound("disparo.wav");
    Music musicaFondo = LoadMusicStream("musicaFondo.mp3");

    playMusicStream(musicaFondo);

    EstadoJuego estadoActual = ESTADO_MENU;
    Nave nave;
    Asteroide asteroides[MAX_ASTEROIDES];
    int puntaje = 0;

    InicializarJuego(&nave, asteroides, &puntaje);


    while(! windowShouldClose()){
        updateMusicStream(musicaFondo);

        switch(estadoActual){
            case ESTADO_MENU:
                if (IsKeyPressed(KEY_ENTER)){
                    InicializarJuego(&nave, asteroides, &puntaje);
                    estadoActual = ESTADO_JUGANDO;
                }
                break;

            case ESTADO_JUGANDO:

                ActualizarJugador(&nave);

                GenerarYMoverAsteroides(asteroides);

                if(ComprobarColisiones(&nave, asteroides)){
                    PlaySound(efectoExplosion);
                    estadoActual = ESTADO_GAME_OVER;
                }

                puntaje++;
                break;


            case ESTADO_GAME_OVER:
                if (IsKeyPressed(KEY_R)){
                    InicializarJuego(&nave, asteroides, &puntaje);
                    estadoActual = ESTADO_JUGANDO;
                }
                
                break;
            
               
        }

        DibujarJuego(estadoActual, nave, asteroides, puntaje, texNave, texAsteroide, texFondo);

    }
    
    

    UnloadTexture(texFondo);
    UnloadTexture(texNave);
    UnloadTexture(texAsteroide);
    UnloadSound(efectoExplosion);
    UnloadMusicStream(musicaFondo);
    CloseAudioDevice();
    CloseWindow();


    return 0;
}